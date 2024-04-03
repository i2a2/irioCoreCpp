#include <math.h>
#include <limits>
#include <NiFpga.h>

#include "bfp.h"

#include "irioCoreCpp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"
#include "profiles/profiles.h"
#include "rioDiscovery.h"
#include "errorsIrio.h"
#include "parserManager.h"

namespace irio {

/*********************************************
 * PUBLIC METHODS
 *********************************************/

Irio::Irio(const std::string &bitfilePath,
			   const std::string &RIOSerialNumber,
			   const std::string &FPGAVIversion,
			   const bool parseVerbose)
	: m_session(0), m_closeAttribute(0) {
	m_resourceName = searchRIODevice(RIOSerialNumber);
	bfp::BFP bfp(bitfilePath, false);

	initDriver();
	openSession(bfp.getBitfilePath(), bfp.getSignature());

	ParserManager parserManager(bfp);
	try {
		searchCommonResources(&parserManager);

		if(getFPGAVIversion() != FPGAVIversion) {
			throw errors::FPGAVIVersionMismatchError(getFPGAVIversion(),
							FPGAVIversion);
		}

		searchPlatform(&parserManager);
		searchDevProfile(&parserManager);

		if(parseVerbose) {
			parserManager.printInfo();
		}

		if(parserManager.hasErrorOccurred()) {
			throw errors::ResourceNotFoundError();
		}
	} catch(errors::ResourceNotFoundError&) {
		std::cerr << "[ERROR] Error searching resources in the bitfile "
				  << bitfilePath << std::endl;
		parserManager.printInfoError();

		std::string baseFilename = utils::getBaseName(bitfilePath);
		const char *envVar = std::getenv(PARSE_LOG_PATH_ENV_VAR);
		const std::string logPath = envVar ? envVar : DEFAULT_PARSE_LOG_PATH;
		const std::string timestamp = utils::getTimestamp();

		std::string logFilePath =
			logPath + "/" + baseFilename + "_parse_log_" + timestamp + ".xml";

		parserManager.printInfoXML(logFilePath);

		closeSession();
		finalizeDriver();

		throw;
	} catch (...) {
		// Must close the session, the destructor will not
		// be called if exception occurs in the constructor
		closeSession();
		finalizeDriver();
		throw;
	}
}

Irio::~Irio() {
	closeSession();
	finalizeDriver();
}

std::uint32_t Irio::getID() const {
	return m_session;
}

void Irio::startFPGA(std::uint32_t timeoutMs) const {
	const unsigned int SLEEP_INTERVAL_NS = 1e8;
	const timespec ts { 0, SLEEP_INTERVAL_NS };

	const auto maxTries = static_cast<std::uint32_t>(std::ceil(
			(timeoutMs * 1e6) / SLEEP_INTERVAL_NS));
	auto status = NiFpga_Run(m_session, 0);
	if(status == NiFpga_Status_FpgaAlreadyRunning) {
		throw errors::NiFpgaFPGAAlreadyRunning(
				"Bitfile is already running in the FPGA");
	} else {
		utils::throwIfNotSuccessNiFpga(status, "Error starting the VI");
	}

	unsigned int tries = 0;
	while (!getInitDone() && tries < maxTries) {
		nanosleep(&ts, nullptr);
		tries++;
	}

	if (!getInitDone()) {
		throw errors::InitializationTimeoutError();
	}

	switch (m_platform->platformID) {
	case PLATFORM_ID::FlexRIO:
		if (!getTerminalsFlexRIO().getRIOAdapterCorrect()) {
			throw errors::ModulesNotOKError("FlexRIO IO Module check failed");
		}
		break;
	case PLATFORM_ID::cRIO:
		if (!getTerminalsCRIO().getcRIOModulesOk()) {
			throw errors::ModulesNotOKError("cRIO IO Module check failed");
		}
		break;
	default:
		break;
	}

	setDAQStop();
}

std::string Irio::getFPGAVIversion() const {
	return m_fpgaviversion;
}

std::uint32_t Irio::getFref() const {
	return m_fref;
}

bool Irio::getInitDone() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadBool(m_session, m_initdone_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading InitDone");
	return static_cast<bool>(aux);
}

std::uint8_t Irio::getDevQualityStatus() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_devqualitystatus_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevQualityStatus");
	return aux;
}

std::int16_t Irio::getDevTemp() const {
	std::int16_t aux;
	auto status = NiFpga_ReadI16(m_session, m_devtemp_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevTemp");
	return aux;
}

bool Irio::getDAQStartStop() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_daqstartstop_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DAQStartStop");
	return static_cast<bool>(aux);
}

bool Irio::getDebugMode() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_debugmode_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DebugMode");
	return static_cast<bool>(aux);
}

void Irio::setDAQStart() const {
	setDAQStartStop(true);
}

void Irio::setDAQStop() const {
	setDAQStartStop(false);
}

void Irio::setDAQStartStop(const bool &start) const {
	auto status = NiFpga_WriteU8(m_session, m_daqstartstop_addr,
			static_cast<std::uint8_t>(start));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DAQStartStop");
}

void Irio::setDebugMode(const bool &debug) const {
	auto status = NiFpga_WriteU8(m_session, m_debugmode_addr,
			static_cast<std::uint8_t>(debug));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DebugMode");
}

double Irio::getMinSamplingRate() const {
	return m_minSamplingRate;
}

double Irio::getMaxSamplingRate() const {
	return m_maxSamplingRate;
}

Platform Irio::getPlatform() const {
	return *m_platform.get();
}

PROFILE_ID Irio::getProfileID() const {
	return m_profile->profileID;
}

void Irio::setCloseAttribute(std::uint32_t attribute) {
	m_closeAttribute = attribute;
}

std::uint32_t Irio::getCloseAttribute() const {
	return m_closeAttribute;
}

///////////////////////////////////////////////
/// Terminals
///////////////////////////////////////////////

TerminalsAnalog Irio::getTerminalsAnalog() const {
	return m_profile->getTerminal<TerminalsAnalog>();
}

TerminalsDigital Irio::getTerminalsDigital() const {
	return m_profile->getTerminal<TerminalsDigital>();
}

TerminalsAuxAnalog Irio::getTerminalsAuxAnalog() const {
	return m_profile->getTerminal<TerminalsAuxAnalog>();
}

TerminalsAuxDigital Irio::getTerminalsAuxDigital() const {
	return m_profile->getTerminal<TerminalsAuxDigital>();
}

TerminalscRIO Irio::getTerminalsCRIO() const {
	return m_profile->getTerminal<TerminalscRIO>();
}

TerminalsFlexRIO Irio::getTerminalsFlexRIO() const {
	return m_profile->getTerminal<TerminalsFlexRIO>();
}

TerminalsSignalGeneration Irio::getTerminalsSignalGeneration() const {
	return m_profile->getTerminal<TerminalsSignalGeneration>();
}

TerminalsDMADAQ Irio::getTerminalsDAQ() const {
	return m_profile->getTerminal<TerminalsDMADAQ>();
}

/*********************************************
 * PRIVATE METHODS
 *********************************************/

void Irio::finalizeDriver() const noexcept {
#ifndef CCS_VERSION
	NiFpga_Finalize();
#endif
}

void Irio::closeSession() noexcept {
	if (m_session != 0)
		NiFpga_Close(m_session, m_closeAttribute);
	m_session = 0;
}

void Irio::initDriver() const {
#ifndef CCS_VERSION
	const auto status = NiFpga_Initialize();
	utils::throwIfNotSuccessNiFpga(status, "Error initializing NiFpga library");
#endif
}

void Irio::openSession(const std::string &bitfilePath,
					 const std::string &signature) {
	const auto status = NiFpga_Open(bitfilePath.c_str(),
			signature.c_str(), m_resourceName.c_str(),
			NiFpga_OpenAttribute_NoRun, &m_session);

	if (NiFpga_IsError(status)) {
		const std::string err = "Error downloading bitfile to FPGA. " +
								std::string("(Code: ") +
								std::to_string(status) + std::string(")");

		throw irio::errors::NiFpgaErrorDownloadingBitfile(err);
	}
}

void Irio::searchCommonResources(ParserManager *parserManager) {
	NiFpga_Status status;

	// Read FPGAVIversion
	std::uint32_t fpgaviversion_addr;
	if (parserManager->findRegisterAddress(TERMINAL_FPGAVIVERSION,
			GroupResource::Common, &fpgaviversion_addr)) {
		std::array<std::uint8_t, 2> fpgaviversion;
		status = NiFpga_ReadArrayU8(m_session, fpgaviversion_addr,
				fpgaviversion.data(), 2);
		utils::throwIfNotSuccessNiFpga(status, "Error reading FPGAVIversion");
		m_fpgaviversion = "V" + std::to_string(fpgaviversion[0])
				+ "." + std::to_string(fpgaviversion[1]);
	}

	// Read Fref
	std::uint32_t fref_addr;
	if (parserManager->findRegisterAddress(TERMINAL_FREF,
				GroupResource::Common, &fref_addr)) {
		status = NiFpga_ReadU32(m_session, fref_addr, &m_fref);
		utils::throwIfNotSuccessNiFpga(status, "Error reading Fref");
	}

	parserManager->findRegisterAddress(TERMINAL_INITDONE,
			GroupResource::Common, &m_initdone_addr);
	parserManager->findRegisterAddress(TERMINAL_DEVQUALITYSTATUS,
			GroupResource::Common, &m_devqualitystatus_addr);
	parserManager->findRegisterAddress(TERMINAL_DEVTEMP, GroupResource::Common,
			&m_devtemp_addr);
	parserManager->findRegisterAddress(TERMINAL_DAQSTARTSTOP,
			GroupResource::Common, &m_daqstartstop_addr);
	parserManager->findRegisterAddress(TERMINAL_DEBUGMODE,
			GroupResource::Common, &m_debugmode_addr);

	m_minSamplingRate = 1.0 * m_fref
			/ std::numeric_limits<std::uint16_t>::max();
	m_maxSamplingRate = m_fref;
}

void Irio::searchPlatform(ParserManager *parserManager) {
	// Read Platform
	std::uint32_t platform_addr;
	if (!parserManager->findRegisterAddress(TERMINAL_PLATFORM,
					GroupResource::Common, &platform_addr)) {
		throw errors::ResourceNotFoundError();
	}

	std::uint8_t platform;
	const auto status = NiFpga_ReadU8(m_session, platform_addr, &platform);
	utils::throwIfNotSuccessNiFpga(status, "Error reading Platform");

	switch (platform) {
	case static_cast<std::uint8_t>(PLATFORM_ID::FlexRIO):
		m_platform.reset(new PlatformFlexRIO());
		break;
	case static_cast<std::uint8_t>(PLATFORM_ID::cRIO):
		m_platform.reset(new PlatformCRIO());
		break;
	case static_cast<std::uint8_t>(PLATFORM_ID::RSeries):
		m_platform.reset(new PlatformRSeries());
		break;
	default:
		throw errors::UnsupportedPlatformError(platform);
	}
}

void Irio::searchDevProfile(ParserManager *parserManager) {
	static const std::unordered_map<PLATFORM_ID,
			const std::unordered_map<std::uint8_t,
				PROFILE_ID>> validProfileByPlatform =
				{ { PLATFORM_ID::FlexRIO, { { PROFILE_VALUE_DAQ,
						PROFILE_ID::FLEXRIO_CPUDAQ }, {
						PROFILE_VALUE_IMAQ,
						PROFILE_ID::FLEXRIO_CPUIMAQ }, {
						PROFILE_VALUE_DAQGPU,
						PROFILE_ID::FLEXRIO_GPUDAQ }, {
						PROFILE_VALUE_IMAQGPU,
						PROFILE_ID::FLEXRIO_GPUIMAQ } } },

				{ PLATFORM_ID::cRIO, { { PROFILE_VALUE_DAQ,
						PROFILE_ID::CRIO_DAQ }, {
						PROFILE_VALUE_IO,
						PROFILE_ID::CRIO_IO } } },

				{ PLATFORM_ID::RSeries, { { PROFILE_VALUE_DAQ,
						PROFILE_ID::R_DAQ } } } };

	std::uint32_t profile_addr;
	if (!parserManager->findRegisterAddress(TERMINAL_DEVPROFILE,
					GroupResource::Common, &profile_addr)) {
		throw errors::ResourceNotFoundError();
	}

	std::uint8_t profile;
	const auto status = NiFpga_ReadU8(m_session, profile_addr, &profile);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevProfile");

	const PLATFORM_ID platform = m_platform->platformID;
	const auto validValues = validProfileByPlatform.find(platform)->second;
	const auto it = validValues.find(profile);
	if (it == validValues.end()) {
		throw errors::UnsupportedDevProfileError
			(profile, static_cast<std::uint8_t>(platform));
	}

	// TODO: Finish
	switch (it->second) {
	case PROFILE_ID::FLEXRIO_CPUDAQ:
		m_profile.reset(
			new ProfileCPUDAQFlexRIO(parserManager, m_session, *m_platform));
		break;
	case PROFILE_ID::FLEXRIO_CPUIMAQ:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::FLEXRIO_GPUDAQ:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::FLEXRIO_GPUIMAQ:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::CRIO_DAQ:
		m_profile.reset(
			new ProfileCPUDAQcRIO(parserManager, m_session, *m_platform));
		break;
	case PROFILE_ID::CRIO_IO:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::R_DAQ:
		m_profile.reset(new ProfileCPUDAQ(parserManager, m_session, *m_platform,
										  PROFILE_ID::R_DAQ));
		break;
	}
}

}  // namespace irio
