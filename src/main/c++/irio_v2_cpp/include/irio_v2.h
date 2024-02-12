#pragma once

#include <string>
#include "bfp.h"
#include "platforms.h"
#include <NiFpga.h>
#include <memory>

namespace iriov2{

class IrioV2
{
public:
    IrioV2(
        const std::string& bitfilePath,
        const std::string& RIODeviceModel,
        const std::string& RIOSerialNumber,
        const std::pair<std::uint8_t, std::uint8_t>& FPGAversion,
        const std::string& appCallID = "",
        const bool verbose = false
        );

    IrioV2(
        const std::string& bitfilePath,
        const std::string& RIODeviceModel,
        const std::string& RIOSerialNumber,
        const std::string& FPGAversion,
        const std::string& appCallID = "",
        const bool verbose = false
        );
private:
    void initDriver();
    void openSession();
    void searchPlatform();

    bfp::BFP m_bfp;
    std::unique_ptr<Platform> m_platform;
    NiFpga_Session m_session;
    std::string m_resourceName;
    
};

}
