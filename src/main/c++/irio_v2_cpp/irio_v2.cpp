#include "irio_v2.h"

#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"
#include "profileDAQ.h"

namespace iriov2{

IrioV2::IrioV2(const std::string &bitfilePath, const std::string &RIODeviceModel, const std::string &RIOSerialNumber, const std::string &FPGAversion, const std::string &appCallID, const bool verbose):
    m_bfp(bitfilePath)
{
	m_resourceName = RIODeviceModel; //TODO: Just for testing purposes, this shouldn't be the resource model...

    initDriver();
    openSession();
    searchPlatform();

    m_profile.reset(new ProfileDAQ(m_bfp, m_session, *m_platform.get()));
}

IrioV2::~IrioV2(){
    finalizeDriver();
}

void IrioV2::finalizeDriver(){
    const auto status = NiFpga_Finalize();
    throwIfNotSuccessNiFpga(status, "Error finalizing NiFpga library");
}

void IrioV2::initDriver(){
    const auto status = NiFpga_Initialize();
    throwIfNotSuccessNiFpga(status, "Error initializing NiFpga library");
}

void IrioV2::openSession(){
    const auto status = NiFpga_Open(m_bfp.getBitfilePath().c_str(), m_bfp.getSignature().c_str(), m_resourceName.c_str(), NiFpga_OpenAttribute_NoRun, &m_session);
    throwIfNotSuccessNiFpga(status, "Error opening bitfile " + m_bfp.getBitfilePath());
}

void IrioV2::searchPlatform(){
    //Read Platform
    auto platform_addr = m_bfp.getRegister(TERMINAL_PLATFORM).address;
    std::uint8_t platform;
    NiFpga_Status status = NiFpga_ReadU8(m_session, platform_addr, &platform);
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
	//TODO
}


const std::shared_ptr<const TerminalsAnalog> IrioV2::analog() {
	return m_profile->analog();
}

const std::shared_ptr<const TerminalsDigital> IrioV2::digital() {
	return m_profile->digital();
}

}
