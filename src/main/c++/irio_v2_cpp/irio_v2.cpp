#include "irio_v2.h"

#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"
#include "profileDAQ.h"
#include <set>

namespace iriov2{


/*********************************************
 * PUBLIC METHODS
 *********************************************/

IrioV2::IrioV2(const std::string &bitfilePath, const std::string &RIODeviceModel, const std::string &RIOSerialNumber, const std::string &FPGAVIversion, const std::string &appCallID, const bool verbose):
    m_bfp(bitfilePath), m_session(0)
{
	m_resourceName = RIODeviceModel; //TODO: Just for testing purposes, this shouldn't be the resource model...

	if(m_bfp.getBitfileVersion() != FPGAVIversion){
		throw std::runtime_error("FPGAVIVserion mismatch ("
				+ FPGAVIversion + " != "
				+ m_bfp.getBitfileVersion() + ")");
	}

    initDriver();
    openSession();
    searchPlatform();
    searchDevProfile();
}

IrioV2::~IrioV2(){
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

void IrioV2::finalizeDriver(){
#ifndef CCS_VERSION
    const auto status = NiFpga_Finalize();
    throwIfNotSuccessNiFpga(status, "Error finalizing NiFpga library");
#endif
}

void IrioV2::closeDriver(){
	if(m_session!=0)
		NiFpga_Close(m_session, 0); //TODO: Should it accept different close attributes?
	m_session = 0;
}

void IrioV2::initDriver(){
#ifndef CCS_VERSION
    const auto status = NiFpga_Initialize();
    throwIfNotSuccessNiFpga(status, "Error initializing NiFpga library");
#endif
}

void IrioV2::openSession(){
    const auto status = NiFpga_Open(m_bfp.getBitfilePath().c_str(), m_bfp.getSignature().c_str(), m_resourceName.c_str(), NiFpga_OpenAttribute_NoRun, &m_session);
    throwIfNotSuccessNiFpga(status, "Error opening bitfile " + m_bfp.getBitfilePath());
}

void IrioV2::searchPlatform(){
    //Read Platform
    auto platform_addr = m_bfp.getRegister(TERMINAL_PLATFORM).address;
    std::uint8_t platform;
    const auto status = NiFpga_ReadU8(m_session, platform_addr, &platform);
    throwIfNotSuccessNiFpga(status, "Error reading Platform");

    switch(platform){
    case FLEXRIO_PLATFORM_VALUE:
    	m_platform.reset(new FlexRIO());
    	break;
    case CRIO_PLATFORM_VALUE:
    	m_platform.reset(new CRIO());
    	break;
    case RSERIES_PLATFORM_VALUE:
    	m_platform.reset(new RSeries());
    	break;
    default:
    	throw std::runtime_error("Platform specified is not supported");
    }
}

void IrioV2::searchDevProfile() {
	static const std::unordered_map<std::uint8_t, const std::unordered_map<std::uint8_t, std::uint8_t>> validProfileByPlatform =
	{
			{FLEXRIO_PLATFORM_VALUE, {	{Profile::PROFILE_VALUE_DAQ, Profile::FLEXRIO_DAQ},
										{Profile::PROFILE_VALUE_IMAQ, Profile::FLEXRIO_IMAQ},
										{Profile::PROFILE_VALUE_DAQGPU,Profile::FLEXRIO_GPUDAQ},
										{Profile::PROFILE_VALUE_IMAQGPU, Profile::FLEXRIO_GPUIMAQ}
									}},

			{CRIO_PLATFORM_VALUE, {		{Profile::PROFILE_VALUE_DAQ, Profile::CRIO_DAQ},
										{Profile::PROFILE_VALUE_IO, Profile::CRIO_IO}
									}},

			{RSERIES_PLATFORM_VALUE, {	{Profile::PROFILE_VALUE_DAQ, Profile::R_DAQ}}}
	};

	auto profile_addr = m_bfp.getRegister(TERMINAL_DEVPROFILE).address;
	std::uint8_t profile;
	const auto status = NiFpga_ReadU8(m_session, profile_addr, &profile);
	throwIfNotSuccessNiFpga(status, "Error reading DevProfile");

	const auto validValues = validProfileByPlatform.find(m_platform->platformID)->second;
	const auto it = validValues.find(profile);
	if(it == validValues.end()){
		throw std::runtime_error("DevProfile "+ std::to_string(profile)
				+ " is not valid for the platform " + std::to_string(m_platform->platformID));
	}

	//TODO: Finish
	switch(it->second){
	case Profile::FLEXRIO_DAQ:
		//TODO: Replace with one with the FlexRIO registers
		m_profile.reset(new ProfileDAQ(m_bfp, m_session, *m_platform.get(), Profile::FLEXRIO_DAQ));
		break;
	case Profile::FLEXRIO_IMAQ:
		break;
	case Profile::FLEXRIO_GPUDAQ:
		break;
	case Profile::FLEXRIO_GPUIMAQ:
		break;
	case Profile::CRIO_DAQ:
		break;
	case Profile::CRIO_IO:
		break;
	case Profile::R_DAQ:
		break;
	}

}





}
