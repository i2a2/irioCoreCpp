#include "irio_v2.h"

#include <unistd.h>
#include <math.h>
#include <limits>

#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"
#include "profiles/profiles.h"
#include "rioDiscovery.h"
#include "errorsIrio.h"

namespace iriov2 {

/*********************************************
 * PUBLIC METHODS
 *********************************************/

IrioV2::IrioV2(const std::string &bitfilePath,
		const std::string &RIOSerialNumber, const std::string &FPGAVIversion) :
		m_bfp(bitfilePath, false), m_session(0), m_closeAttribute(0) {
	m_resourceName = searchRIODevice(RIOSerialNumber);

	initDriver();
	openSession();
	try {
		searchCommonResources();

		if(getFPGAVIversion() != FPGAVIversion) {
			throw errors::FPGAVIVersionMismatchError(getFPGAVIversion(),
							FPGAVIversion);
		}

		searchPlatform();
		searchDevProfile();
	} catch (...) {
		// Must close the session, the destructor will not
		// be called if exception occurs in the constructor
		closeSession();
		finalizeDriver();
		throw;
	}
}

IrioV2::~IrioV2() {
	closeSession();
	finalizeDriver();
}

void IrioV2::startFPGA(std::uint32_t timeoutMs) const {
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

std::string IrioV2::getFPGAVIversion() const {
	return m_fpgaviversion;
}

std::uint32_t IrioV2::getFref() const {
	return m_fref;
}

bool IrioV2::getInitDone() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadBool(m_session, m_initdone_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading InitDone");
	return static_cast<bool>(aux);
}

std::uint8_t IrioV2::getDevQualityStatus() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_devqualitystatus_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevQualityStatus");
	return aux;
}

std::int16_t IrioV2::getDevTemp() const {
	std::int16_t aux;
	auto status = NiFpga_ReadI16(m_session, m_devtemp_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevTemp");
	return aux;
}

bool IrioV2::getDAQStartStop() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_daqstartstop_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DAQStartStop");
	return static_cast<bool>(aux);
}

bool IrioV2::getDebugMode() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_debugmode_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DebugMode");
	return static_cast<bool>(aux);
}

void IrioV2::setDAQStart() const {
	setDAQStartStop(true);
}

void IrioV2::setDAQStop() const {
	setDAQStartStop(false);
}

void IrioV2::setDAQStartStop(const bool &start) const {
	auto status = NiFpga_WriteU8(m_session, m_daqstartstop_addr,
			static_cast<std::uint8_t>(start));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DAQStartStop");
}

void IrioV2::setDebugMode(const bool &debug) const {
	auto status = NiFpga_WriteU8(m_session, m_debugmode_addr,
			static_cast<std::uint8_t>(debug));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DebugMode");
}

double IrioV2::getMinSamplingRate() const {
	return m_minSamplingRate;
}

double IrioV2::getMaxSamplingRate() const {
	return m_maxSamplingRate;
}

Platform IrioV2::getPlatform() const {
	return *m_platform.get();
}

PROFILE_ID IrioV2::getProfileID() const {
	return m_profile->profileID;
}

void IrioV2::setCloseAttribute(std::uint32_t attribute) {
	m_closeAttribute = attribute;
}

std::uint32_t IrioV2::getCloseAttribute() const {
	return m_closeAttribute;
}

///////////////////////////////////////////////
/// Terminals
///////////////////////////////////////////////

TerminalsAnalog IrioV2::getTerminalsAnalog() const {
	return m_profile->getTerminal<TerminalsAnalog>();
}

TerminalsDigital IrioV2::getTerminalsDigital() const {
	return m_profile->getTerminal<TerminalsDigital>();
}

TerminalsAuxAnalog IrioV2::getTerminalsAuxAnalog() const {
	return m_profile->getTerminal<TerminalsAuxAnalog>();
}

TerminalsAuxDigital IrioV2::getTerminalsAuxDigital() const {
	return m_profile->getTerminal<TerminalsAuxDigital>();
}

TerminalscRIO IrioV2::getTerminalsCRIO() const {
	return m_profile->getTerminal<TerminalscRIO>();
}

TerminalsFlexRIO IrioV2::getTerminalsFlexRIO() const {
	return m_profile->getTerminal<TerminalsFlexRIO>();
}

TerminalsSignalGeneration IrioV2::getTerminalsSignalGeneration() const {
	return m_profile->getTerminal<TerminalsSignalGeneration>();
}

TerminalsDMADAQ IrioV2::getTerminalsDAQ() const {
	return m_profile->getTerminal<TerminalsDMADAQ>();
}

/*********************************************
 * PRIVATE METHODS
 *********************************************/

void IrioV2::finalizeDriver() const noexcept {
#ifndef CCS_VERSION
	NiFpga_Finalize();
#endif
}

void IrioV2::closeSession() noexcept {
	if (m_session != 0)
		NiFpga_Close(m_session, m_closeAttribute);
	m_session = 0;
}

void IrioV2::initDriver() const {
#ifndef CCS_VERSION
	const auto status = NiFpga_Initialize();
	utils::throwIfNotSuccessNiFpga(status, "Error initializing NiFpga library");
#endif
}

void IrioV2::openSession() {
	const auto status = NiFpga_Open(m_bfp.getBitfilePath().c_str(),
			m_bfp.getSignature().c_str(), m_resourceName.c_str(),
			NiFpga_OpenAttribute_NoRun, &m_session);

	if (NiFpga_IsError(status)) {
		const std::string err = "Error downloading bitfile to FPGA. "
				+ std::string("(Code: ") + std::to_string(status) + std::string(")");

		throw iriov2::errors::NiFpgaErrorDownloadingBitfile(err);
	}
}

void IrioV2::searchCommonResources() {
	NiFpga_Status status;

	// Read FPGAVIversion
	auto fpgaviversion_addr = m_bfp.getRegister(TERMINAL_FPGAVIVERSION).address;
	std::array<std::uint8_t, 2> fpgaviversion;
	status = NiFpga_ReadArrayU8(m_session, fpgaviversion_addr,
			fpgaviversion.data(), 2);
	utils::throwIfNotSuccessNiFpga(status, "Error reading FPGAVIversion");
	m_fpgaviversion = std::to_string(fpgaviversion[0])
			+"."+std::to_string(fpgaviversion[1]);

	// Read Fref
	auto fref_addr = m_bfp.getRegister(TERMINAL_FREF).address;
	status = NiFpga_ReadU32(m_session, fref_addr, &m_fref);
	utils::throwIfNotSuccessNiFpga(status, "Error reading Fref");

	m_initdone_addr = m_bfp.getRegister(TERMINAL_INITDONE).address;
	m_devqualitystatus_addr =
			m_bfp.getRegister(TERMINAL_DEVQUALITYSTATUS).address;
	m_devtemp_addr = m_bfp.getRegister(TERMINAL_DEVTEMP).address;
	m_daqstartstop_addr = m_bfp.getRegister(TERMINAL_DAQSTARTSTOP).address;
	m_debugmode_addr = m_bfp.getRegister(TERMINAL_DEBUGMODE).address;

	m_minSamplingRate = 1.0 * m_fref
			/ std::numeric_limits<std::uint16_t>::max();
	m_maxSamplingRate = m_fref;
}

void IrioV2::searchPlatform() {
	// Read Platform
	auto platform_addr = m_bfp.getRegister(TERMINAL_PLATFORM).address;
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

void IrioV2::searchDevProfile() {
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

	auto profile_addr = m_bfp.getRegister(TERMINAL_DEVPROFILE).address;
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
				new ProfileCPUDAQFlexRIO(m_bfp, m_session, *m_platform));
		break;
	case PROFILE_ID::FLEXRIO_CPUIMAQ:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::FLEXRIO_GPUDAQ:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::FLEXRIO_GPUIMAQ:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::CRIO_DAQ:
		m_profile.reset(
				new ProfileCPUDAQcRIO(m_bfp, m_session, *m_platform));
		break;
	case PROFILE_ID::CRIO_IO:
		throw std::runtime_error("Profile not implemented");
	case PROFILE_ID::R_DAQ:
		m_profile.reset(
				new ProfileCPUDAQ(m_bfp, m_session, *m_platform,
						PROFILE_ID::R_DAQ));
		break;
	}
}

}  // namespace iriov2
