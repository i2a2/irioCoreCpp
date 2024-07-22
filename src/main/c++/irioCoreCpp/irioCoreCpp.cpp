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
			   const bool parseVerbose):
	m_rioSerialNumber(RIOSerialNumber) {
	m_resourceName = searchRIODevice(RIOSerialNumber);
	bfp::BFP bfp(bitfilePath, false);

	initDriver();
	openSession(bfp.getBitfilePath(), bfp.getSignature());

	ParserManager parserManager(bfp);
	try {
		searchPlatform(&parserManager);
		selectDevProfile(&parserManager);

		const auto fpgaVer =
			m_profile->getTerminal<TerminalsCommon>().getFPGAVIversion();
		if (fpgaVer != FPGAVIversion) {
			throw errors::FPGAVIVersionMismatchError(fpgaVer, FPGAVIversion);
		}

		if(parseVerbose) {
			std::cout << "Resources found: " << std::endl;
			parserManager.printInfo();
		}

		if(parserManager.hasErrorOccurred()) {
			throw errors::ResourceNotFoundError();
		}
	} catch(errors::ResourceNotFoundError&) {
		std::cerr << "[ERROR] Error searching resources in the bitfile "
				  << bitfilePath << std::endl;
		std::cerr << "[ERROR] The following resources were not found:"
				  << std::endl;
		parserManager.printInfoError();

		std::string baseFilename = utils::getBaseName(bitfilePath);
		const char *envVar = std::getenv(PARSE_LOG_PATH_ENV_VAR);
		const std::string logPath = envVar ? envVar : DEFAULT_PARSE_LOG_PATH;
		const std::string timestamp = utils::getTimestamp();

		std::string logFilePath =
			logPath + "/irioCore_" + baseFilename + "_parse_log_" + timestamp + ".xml";

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

std::string Irio::getRIOSerial() const {
	return m_rioSerialNumber;
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

	const auto commonTerm = getTerminalsCommon();
	unsigned int tries = 0;
	while (!commonTerm.getInitDone() && tries < maxTries) {
		nanosleep(&ts, nullptr);
		tries++;
	}

	if (!commonTerm.getInitDone()) {
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

	commonTerm.setDAQStop();
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

TerminalsCommon Irio::getTerminalsCommon() const {
	return m_profile->getTerminal<TerminalsCommon>();
}

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

TerminalsDMAIMAQ Irio::getTerminalsIMAQ() const {
	return m_profile->getTerminal<TerminalsDMAIMAQ>();
}

TerminalsIO Irio::getTerminalsIO() const {
	return m_profile->getTerminal<TerminalsIO>();
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

void Irio::selectDevProfile(ParserManager *parserManager) {
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

	switch (it->second) {
	case PROFILE_ID::FLEXRIO_CPUDAQ:
		m_profile.reset(
			new ProfileCPUDAQFlexRIO(parserManager, m_session, *m_platform));
		break;
	case PROFILE_ID::FLEXRIO_CPUIMAQ:
		m_profile.reset(
			new ProfileCPUIMAQFlexRIO(parserManager, m_session, *m_platform));
		break;
	case PROFILE_ID::FLEXRIO_GPUDAQ:
		throw errors::ProfileNotImplementedError();
	case PROFILE_ID::FLEXRIO_GPUIMAQ:
		throw errors::ProfileNotImplementedError();
	case PROFILE_ID::CRIO_DAQ:
		m_profile.reset(
			new ProfileCPUDAQcRIO(parserManager, m_session, *m_platform));
		break;
	case PROFILE_ID::CRIO_IO:
		m_profile.reset(new ProfileIOcRIO(parserManager, m_session, *m_platform));
		break;
	case PROFILE_ID::R_DAQ:
          m_profile.reset(
              new ProfileCPUDAQRSeries(parserManager, m_session, *m_platform));
          break;
	}
}

}  // namespace irio
