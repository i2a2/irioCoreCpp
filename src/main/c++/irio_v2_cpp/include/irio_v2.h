#pragma once

#include <string>
#include <memory>
#include <platforms.h>
#include <profiles.h>
#include <bfp.h>
#include <NiFpga.h>
#include <terminals/terminals.h>

namespace iriov2{

/**
 * irio-v2 main class.
 * Hides the complexity of using the National Instruments low level driver by abstracting several elements from the user.\n
 * It parses the specified bitfile and downloads it to the selected RIO device. It also provides the user function to
 * access the terminals and perform read/write operations.
 */
class IrioV2
{
public:
	/**
	 * Constructor.\n
	 * - Parses and extract all the resources in the specified bitfile
	 * - Initializes the low level library
	 * - Downloads the bitfile to the specified device
	 * - Applies the appropriate profile depending on the values specified in the LabVIEW project. This allows access to some terminals or others.
	 * - Checks that the configured values and resources matches the design rules
	 *
	 * @param bitfilePath		Bitfile to parse and download
	 * @param RIODeviceModel	RIO Device Model name
	 * @param RIOSerialNumber	RIO Serial Number of the device to use
	 * @param FPGAVIversion		Version of the Bitfile. If it does not match the one parsed and exception will be thrown
	 * @param appCallID			Identifier for log messages
	 * @param verbose			Verbose
	 */
    IrioV2(
        const std::string& bitfilePath,
        const std::string& RIODeviceModel,
        const std::string& RIOSerialNumber,
        const std::string& FPGAVIversion,
        const std::string& appCallID = "",
        const bool verbose = false
        );

    ~IrioV2();

    //void startFPGA();

    const std::shared_ptr<const TerminalsAnalog> analog();
    const std::shared_ptr<const TerminalsDigital> digital();

private:
    void initDriver();
    void closeDriver();
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
