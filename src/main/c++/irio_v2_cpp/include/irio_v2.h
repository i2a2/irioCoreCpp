#pragma once

#include <string>
#include <memory>
#include <platforms.h>
#include <profiles.h>
#include <bfp.h>
#include <NiFpga.h>

namespace iriov2{

class IrioV2
{
public:
    IrioV2(
        const std::string& bitfilePath,
        const std::string& RIODeviceModel,
        const std::string& RIOSerialNumber,
        const std::string& FPGAversion,
        const std::string& appCallID = "",
        const bool verbose = false
        );

    ~IrioV2();
private:
    void initDriver();
    void finalizeDriver();
    void openSession();
    void searchPlatform();
    void searchDevProfile();

    std::unique_ptr<Platform> m_platform;
    std::unique_ptr<Profile> m_profile;

    bfp::BFP m_bfp;

    std::string m_resourceName;

    NiFpga_Session m_session;

};

}
