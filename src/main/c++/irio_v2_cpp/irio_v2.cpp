#include "irio_v2.h"

#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"
#include "profiles/profiles.h"
#include "rioDiscovery.h"
#include "errorsIrio.h"
#include <unistd.h>
#include <math.h>
#include <limits>

namespace iriov2 {

/*********************************************
 * PUBLIC METHODS
 *********************************************/

IrioV2::IrioV2(
		const std::string &bitfilePath,
		const std::string &RIOSerialNumber,
		const std::string &FPGAVIversion) :
		m_bfp(bitfilePath, false) {
	m_resourceName = RIODiscovery::searchRIODevice(RIOSerialNumber);

	if (m_bfp.getBitfileVersion() != FPGAVIversion) {
		throw errors::FPGAVIVersionMismatchError(m_bfp.getBitfileVersion(), FPGAVIversion);
	}

	m_session = 0;
	initDriver();
	openSession();
	try{
		searchCommonResources();
		searchPlatform();
		searchDevProfile();
	}catch(...){
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
	const timespec ts{0, SLEEP_INTERVAL_NS};

	const auto maxTries = static_cast<std::uint32_t>(std::ceil(
			(timeoutMs * 1e6) / SLEEP_INTERVAL_NS));
	auto status = NiFpga_Run(m_session, 0);
	utils::throwIfNotSuccessNiFpga(status, "Error starting the VI");

	unsigned int tries = 0;
	while (!getInitDone() && tries < maxTries) {
		nanosleep(&ts, nullptr);
		tries++;
	}

	if(!getInitDone()){
		throw errors::InitializationTimeoutError();
	}

	switch (m_platform->platformID) {
	case FLEXRIO_PLATFORM_VALUE:
		if (m_profile->flexRIO().getInsertedIOModuleID() != 0
				&& !m_profile->flexRIO().getRIOAdapterCorrect()) {
			throw errors::ModulesNotOKError("FlexRIO IO Module check failed");
		}
		break;
	case CRIO_PLATFORM_VALUE:
		if (!m_profile->cRIO().getcRIOModulesOk()) {
			throw errors::ModulesNotOKError("cRIO IO Module check failed");
		}
		break;
	default:
		break;
	}

	setDAQStop();
}

std::pair<std::uint8_t, std::uint8_t> IrioV2::getFPGAVIversion() const {
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
	auto status = NiFpga_WriteBool(m_session, m_daqstartstop_addr,
			static_cast<std::uint8_t>(start));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DAQStartStop");
}

void IrioV2::setDebugMode(const bool &debug) const {
	auto status = NiFpga_WriteBool(m_session, m_debugmode_addr, static_cast<std::uint8_t>(debug));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DebugMode");
}

double IrioV2::getMinSamplingRate() const {
	return m_minSamplingRate;
}

double IrioV2::getMaxSamplingRate() const {
	return m_maxSamplingRate;
}


///////////////////////////////////////////////
/// Terminals
///////////////////////////////////////////////
TerminalsAnalog IrioV2::analog() const {
	return m_profile->analog();
}

TerminalsDigital IrioV2::digital() const {
	return m_profile->digital();
}

TerminalsAuxAnalog IrioV2::auxAnalog() const {
	return m_profile->auxAnalog();
}

TerminalsAuxDigital IrioV2::auxDigital() const {
	return m_profile->auxDigital();
}

TerminalscRIO IrioV2::cRIO() const {
	return m_profile->cRIO();
}

TerminalsFlexRIO IrioV2::flexRIO() const {
	return m_profile->flexRIO();
}

TerminalsSignalGeneration IrioV2::signalGeneration() const {
	return m_profile->signalGeneration();
}

TerminalsDMADAQ IrioV2::daq() const {
	return m_profile->daq();
}

/*********************************************
 * PRIVATE METHODS
 *********************************************/

void IrioV2::finalizeDriver() const noexcept{
#ifndef CCS_VERSION
	NiFpga_Finalize();
#endif
}

void IrioV2::closeSession() noexcept {
	if (m_session != 0)
		NiFpga_Close(m_session, 0); //TODO: Should it accept different close attributes?
	m_session = 0;
}

void IrioV2::initDriver() const {
#ifndef CCS_VERSION
	const auto status = NiFpga_Initialize();
	utils::throwIfNotSuccessNiFpga(status, "Error initializing NiFpga library");
#endif
}

void IrioV2::openSession() {
	const auto status = NiFpga_Open(m_bfp.getBitfilePath().c_str(), m_bfp.getSignature().c_str(),
			m_resourceName.c_str(), NiFpga_OpenAttribute_NoRun, &m_session);
	utils::throwIfNotSuccessNiFpga(status, "Error opening bitfile " + m_bfp.getBitfilePath());
}

void IrioV2::searchCommonResources(){
	NiFpga_Status status;

	//Read FPGAVIversion
	auto fpgaviversion_addr = m_bfp.getRegister(TERMINAL_FPGAVIVERSION).address;
	std::array<std::uint8_t, 2> fpgaviversion;
	status = NiFpga_ReadArrayU8(m_session, fpgaviversion_addr, fpgaviversion.data(), 2);
	utils::throwIfNotSuccessNiFpga(status, "Error reading FPGAVIversion");
	m_fpgaviversion = std::make_pair(fpgaviversion[0], fpgaviversion[1]);

	//Read Fref
	auto fref_addr = m_bfp.getRegister(TERMINAL_FREF).address;
	status = NiFpga_ReadU32(m_session, fref_addr, &m_fref);
	utils::throwIfNotSuccessNiFpga(status, "Error reading Fref");

	m_initdone_addr = m_bfp.getRegister(TERMINAL_INITDONE).address;
	m_devqualitystatus_addr = m_bfp.getRegister(TERMINAL_DEVQUALITYSTATUS).address;
	m_devtemp_addr = m_bfp.getRegister(TERMINAL_DEVTEMP).address;
	m_daqstartstop_addr = m_bfp.getRegister(TERMINAL_DAQSTARTSTOP).address;
	m_debugmode_addr = m_bfp.getRegister(TERMINAL_DEBUGMODE).address;

	m_minSamplingRate = 1.0 * m_fref / std::numeric_limits<std::uint16_t>::max();
	m_maxSamplingRate = m_fref;
}

void IrioV2::searchPlatform() {
	//Read Platform
	auto platform_addr = m_bfp.getRegister(TERMINAL_PLATFORM).address;
	std::uint8_t platform;
	const auto status = NiFpga_ReadU8(m_session, platform_addr, &platform);
	utils::throwIfNotSuccessNiFpga(status, "Error reading Platform");

	switch (platform) {
	case FLEXRIO_PLATFORM_VALUE:
		m_platform.reset(new PlatformFlexRIO());
		break;
	case CRIO_PLATFORM_VALUE:
		m_platform.reset(new PlatformCRIO());
		break;
	case RSERIES_PLATFORM_VALUE:
		m_platform.reset(new PlatformRSeries());
		break;
	default:
		throw errors::UnsupportedPlatformError(platform);
	}
}

void IrioV2::searchDevProfile() {
	static const std::unordered_map<std::uint8_t,
			const std::unordered_map<std::uint8_t, ProfileBase::PROFILE_ID>> validProfileByPlatform = { {
			FLEXRIO_PLATFORM_VALUE, { { ProfileBase::PROFILE_VALUE_DAQ, ProfileBase::PROFILE_ID::FLEXRIO_CPUDAQ },
					{ ProfileBase::PROFILE_VALUE_IMAQ, ProfileBase::PROFILE_ID::FLEXRIO_CPUIMAQ }, {
							ProfileBase::PROFILE_VALUE_DAQGPU, ProfileBase::PROFILE_ID::FLEXRIO_GPUDAQ }, {
							ProfileBase::PROFILE_VALUE_IMAQGPU, ProfileBase::PROFILE_ID::FLEXRIO_GPUIMAQ } } },

	{ CRIO_PLATFORM_VALUE, { { ProfileBase::PROFILE_VALUE_DAQ, ProfileBase::PROFILE_ID::CRIO_DAQ }, {
			ProfileBase::PROFILE_VALUE_IO, ProfileBase::PROFILE_ID::CRIO_IO } } },

	{ RSERIES_PLATFORM_VALUE, { { ProfileBase::PROFILE_VALUE_DAQ, ProfileBase::PROFILE_ID::R_DAQ } } } };

	auto profile_addr = m_bfp.getRegister(TERMINAL_DEVPROFILE).address;
	std::uint8_t profile;
	const auto status = NiFpga_ReadU8(m_session, profile_addr, &profile);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevProfile");

	const std::uint8_t platform = m_platform->platformID;
	const auto validValues = validProfileByPlatform.find(platform)->second;
	const auto it = validValues.find(profile);
	if (it == validValues.end()) {
		throw errors::UnsupportedDevProfileError(profile, platform);
	}
	const auto profileEnum = static_cast<ProfileBase::PROFILE_ID>(profile);

	//TODO: Finish
	switch (profileEnum) {
	case ProfileBase::PROFILE_ID::FLEXRIO_CPUDAQ:
		m_profile.reset(
				new ProfileCPUDAQFlexRIO(m_bfp, m_session, *m_platform));
		break;
	case ProfileBase::PROFILE_ID::FLEXRIO_CPUIMAQ:
		throw std::runtime_error("Profile not implemented");
	case ProfileBase::PROFILE_ID::FLEXRIO_GPUDAQ:
		throw std::runtime_error("Profile not implemented");
	case ProfileBase::PROFILE_ID::FLEXRIO_GPUIMAQ:
		throw std::runtime_error("Profile not implemented");
	case ProfileBase::PROFILE_ID::CRIO_DAQ:
		throw std::runtime_error("Profile not implemented");
	case ProfileBase::PROFILE_ID::CRIO_IO:
		throw std::runtime_error("Profile not implemented");
	case ProfileBase::PROFILE_ID::R_DAQ:
		m_profile.reset(new ProfileCPUDAQ(m_bfp, m_session, *m_platform, ProfileBase::PROFILE_ID::R_DAQ));
		break;
	}
}

}
