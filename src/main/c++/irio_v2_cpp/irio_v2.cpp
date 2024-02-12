#include "irio_v2.h"

#include "terminals/namesTerminalsCommon.h"
#include "utils.h"

namespace iriov2{

IrioV2::IrioV2(const std::string &bitfilePath, const std::string &RIODeviceModel, const std::string &RIOSerialNumber, const std::pair<std::uint8_t, std::uint8_t> &FPGAversion, const std::string &appCallID, const bool verbose):
    m_bfp(bitfilePath)
{
    initDriver();
}

IrioV2::IrioV2(const std::string &bitfilePath, const std::string &RIODeviceModel, const std::string &RIOSerialNumber, const std::string &FPGAversion, const std::string &appCallID, const bool verbose):
    m_bfp(bitfilePath)
{
    initDriver();
}

void IrioV2::initDriver(){
    openSession();
    searchPlatform();
}

void IrioV2::openSession(){
    NiFpga_Status status;

    status = NiFpga_Open(m_bfp.getBitfilePath().c_str(), m_bfp.getSignature().c_str(), m_resourceName.c_str(), NiFpga_OpenAttribute_NoRun, &m_session);
    throwIfNotSuccessNiFpga(status, "Error opening bitfile");
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

}
