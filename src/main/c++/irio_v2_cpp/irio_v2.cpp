#include "irio_v2.h"

#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"
#include "profiles/profileDAQ.h"

namespace iriov2 {

/*********************************************
 * PUBLIC METHODS
 *********************************************/

IrioV2::IrioV2(
		const std::string &bitfilePath,
		const std::string &RIODeviceModel,
		const std::string &RIOSerialNumber,
		const std::string &FPGAVIversion,
		const std::string &appCallID,
		const bool verbose) :
		m_bfp(bitfilePath), m_session(0) {
	//TODO: Just for testing purposes, this shouldn't be the resource model...
	m_resourceName = RIODeviceModel;

	if (m_bfp.getBitfileVersion() != FPGAVIversion) {
		throw std::runtime_error(
				"FPGAVIVserion mismatch (" + FPGAVIversion + " != " + m_bfp.getBitfileVersion()
						+ ")");
	}

	initDriver();
	openSession();
	searchPlatform();
	searchDevProfile();
}

IrioV2::~IrioV2() {
	closeDriver();
	finalizeDriver();
}

const std::shared_ptr<const TerminalsAnalog> IrioV2::analog() {
	return m_profile->analog();
}

const std::shared_ptr<const TerminalsDigital> IrioV2::digital() {
	return m_profile->digital();
}

/*********************************************
 * PRIVATE METHODS
 *********************************************/

void IrioV2::finalizeDriver() {
#ifndef CCS_VERSION
	const auto status = NiFpga_Finalize();
	throwIfNotSuccessNiFpga(status, "Error finalizing NiFpga library");
#endif
}

void IrioV2::closeDriver() {
	if (m_session != 0)
		NiFpga_Close(m_session, 0); //TODO: Should it accept different close attributes?
	m_session = 0;
}

void IrioV2::initDriver() {
#ifndef CCS_VERSION
	const auto status = NiFpga_Initialize();
	throwIfNotSuccessNiFpga(status, "Error initializing NiFpga library");
#endif
}

void IrioV2::openSession() {
	const auto status = NiFpga_Open(m_bfp.getBitfilePath().c_str(), m_bfp.getSignature().c_str(),
			m_resourceName.c_str(), NiFpga_OpenAttribute_NoRun, &m_session);
	throwIfNotSuccessNiFpga(status, "Error opening bitfile " + m_bfp.getBitfilePath());
}

void IrioV2::searchPlatform() {
	//Read Platform
	auto platform_addr = m_bfp.getRegister(TERMINAL_PLATFORM).address;
	std::uint8_t platform;
	const auto status = NiFpga_ReadU8(m_session, platform_addr, &platform);
	throwIfNotSuccessNiFpga(status, "Error reading Platform");

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
		throw std::runtime_error("Platform specified is not supported");
	}
}

void IrioV2::searchDevProfile() {
	static const std::unordered_map<std::uint8_t,
			const std::unordered_map<std::uint8_t, std::uint8_t>> validProfileByPlatform =
	{
			{FLEXRIO_PLATFORM_VALUE, {	{ProfileBase::PROFILE_VALUE_DAQ, ProfileBase::FLEXRIO_DAQ},
										{ProfileBase::PROFILE_VALUE_IMAQ, ProfileBase::FLEXRIO_IMAQ},
										{ProfileBase::PROFILE_VALUE_DAQGPU,ProfileBase::FLEXRIO_GPUDAQ},
										{ProfileBase::PROFILE_VALUE_IMAQGPU, ProfileBase::FLEXRIO_GPUIMAQ}
									}},

			{CRIO_PLATFORM_VALUE, {		{ProfileBase::PROFILE_VALUE_DAQ, ProfileBase::CRIO_DAQ},
										{ProfileBase::PROFILE_VALUE_IO, ProfileBase::CRIO_IO}
									}},

			{RSERIES_PLATFORM_VALUE, {	{ProfileBase::PROFILE_VALUE_DAQ, ProfileBase::R_DAQ}}}
	};

	auto profile_addr = m_bfp.getRegister(TERMINAL_DEVPROFILE).address;
	std::uint8_t profile;
	const auto status = NiFpga_ReadU8(m_session, profile_addr, &profile);
	throwIfNotSuccessNiFpga(status, "Error reading DevProfile");

	const std::uint8_t platform = m_platform->platformID;
	const auto validValues = validProfileByPlatform.find(platform)->second;
	const auto it = validValues.find(profile);
	if (it == validValues.end()) {
		throw std::runtime_error(
				"DevProfile " + std::to_string(profile) + " is not valid for the platform "
						+ std::to_string(platform));
	}

	//TODO: Finish
	switch (it->second) {
	case ProfileBase::FLEXRIO_DAQ:
		//TODO: Replace with one with the FlexRIO registers
		m_profile.reset(
				new ProfileDAQ(m_bfp, m_session, *m_platform.get(), ProfileBase::FLEXRIO_DAQ));
		break;
	case ProfileBase::FLEXRIO_IMAQ:
		break;
	case ProfileBase::FLEXRIO_GPUDAQ:
		break;
	case ProfileBase::FLEXRIO_GPUIMAQ:
		break;
	case ProfileBase::CRIO_DAQ:
		break;
	case ProfileBase::CRIO_IO:
		break;
	case ProfileBase::R_DAQ:
		break;
	}
}

}
