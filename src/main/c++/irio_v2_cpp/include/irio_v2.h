#pragma once

#include <string>
#include <memory>
#include <platforms.h>
#include <profiles/profileBase.h>
#include <bfp.h>
#include <NiFpga.h>
#include <terminals/terminals.h>

namespace iriov2 {

/**
 * irio-v2 main class.
 * Hides the complexity of using the National Instruments low level driver by abstracting several elements from the user.
 * It parses the specified Bitfile and downloads it to the selected RIO device. It also provides the user function to
 * access the terminals and perform read/write operations.
 *
 * @ingroup IrioV2
 */
class IrioV2 {
public:
	/**
	 * Constructor.
	 * - Parses and extract all the resources in the specified Bitfile (\p bitfilePath)
	 * - Initializes the low level library
	 * - Downloads the Bitfile to the specified device (\p RIOSerialNumber)
	 * - Applies the appropriate profile depending on the values specified in the LabVIEW project. This allows access to some terminals or others.
	 * - Checks that the configured values and resources matches the design rules
	 *
	 * @throw iriov2::errors::BFPParseBitfileError			Unable to parse \p bitfilePath
	 * @throw iriov2::errors::ResourceNotFoundError			Some of the necessary resources were not found in the bitfile
	 * @throw iriov2::errors::FPGAVIVersionMismatchError	Parsed FPGAVIversion does not match the one specified
	 * @throw iriov2::errors::UnsupportedDevProfileError	The DevProfile read does not match any of the supported profiles
	 * @throw iriov2::errors::UnsupportedPlatformError		The platform read does not match any of the supported platforms
	 * @throw iriov2::errors::NiFpgaError					Error occurred in an FPGA operation
	 *
	 * @param bitfilePath		Bitfile to parse and download
	 * @param RIOSerialNumber	RIO Serial Number of the device to use
	 * @param FPGAVIversion		Version of the Bitfile. If it does not match the one parsed and exception will be thrown
	 */
	IrioV2(
			const std::string &bitfilePath,
			const std::string &RIOSerialNumber,
			const std::string &FPGAVIversion);

	/**
	 * Destructor.
	 * Closes the session if there is one open and finalizes
	 * the low level library.
	 */
	~IrioV2();

	/**
	 * Starts the VI downloaded in the FPGA.
	 *
	 * @throw iriov2::errors::InitializationTimeoutError	InitDone is not ready after a timeout
	 * @throw iriov2::errors::ModulesNotOKError		At initialization, the modules are not ready
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param timeoutMs Max time to wait for InitDone to be ready
	 */
	void startFPGA(std::uint32_t timeoutMs=5000);

//	/**
//	 * Stops the VI running in the FPGA
//	 *
//	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
//	 */
//	void stopFPGA();

	/**
	 * Returns a pointer to the analog terminals.
	 * The user must call it to be able to read/write analog
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the analog terminals
	 */
	const std::shared_ptr<const TerminalsAnalog> analog() const;

	/**
	 * Returns a pointer to the digital terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the digital terminals
	 */
	const std::shared_ptr<const TerminalsDigital> digital() const;

	/**
	 * Returns a pointer to the aux analog terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the aux analog terminals
	 */
	const std::shared_ptr<const TerminalsAuxAnalog> auxAnalog() const;

	/**
	 * Returns a pointer to the aux digital terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the aux digital terminals
	 */
	const std::shared_ptr<const TerminalsAuxDigital> auxDigital() const;

	/**
	 * Returns a pointer to the cRIO specific terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the cRIO specific terminals
	 */
	const std::shared_ptr<const TerminalscRIO> cRIO() const;

	/**
	 * Returns a pointer to the FlexRIO specific terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the FlexRIO specific terminals
	 */
	const std::shared_ptr<const TerminalsFlexRIO> flexRIO() const;

	/**
	 * Returns a pointer to the Signal Generation terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the Signal Generation terminals
	 */
	const std::shared_ptr<const TerminalsSignalGeneration> signalGeneration() const;

	/**
	 * Returns a pointer to the DAQ terminals.
	 * This can be for CPU or GPU DAQ functionality depending
	 * on the profile selected.
	 * The user must call it to be able to read/write digital
	 * related terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError The selected profile does not have the terminals
	 *
	 * @return Pointer to the DAQ terminals
	 */
	const std::shared_ptr<const TerminalsDMADAQ> daq() const;

private:
	/**
	 * Initializes the low level library. If system is CCS, does nothing.
	 */
	void initDriver();

	/**
	 * Closes the session if it has been opened.
	 */
	void closeSession();

	/**
	 * Finalizes the low level library. If system is CCS, does nothing.
	 */
	void finalizeDriver();

	/**
	 * Opens a session to the FPGA, downloading the bitfile if necessary.
	 * It does not run the VI, until @ref startFPGA has been called
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 */
	void openSession();

	/**
	 * Searches for the @ref TERMINAL_PLATFORM terminal and reads its value.
	 * Checks that is a valid value and assigns the equivalent
	 * Platform to a variable.
	 *
	 * @throw iriov2::errors::UnsupportedPlatformError	The platform read does not match any of the supported platforms
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 */
	void searchPlatform();

	/**
	 * Searches for the @ref TERMINAL_DEVPROFILE terminals and
	 * reads its value.
	 * Checks that is a valid value for the selected platform and assigns
	 * the corresponding profile to a variable.
	 * This determines which terminals can the user access.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError			Some of the necessary resources were not found in the bitfile
	 * @throw iriov2::errors::UnsupportedDevProfileError	The DevProfile read does not match any of the supported profiles
	 * @throw iriov2::errors::NiFpgaError					Error occurred in an FPGA operation
	 */
	void searchDevProfile();

	std::unique_ptr<Platform> m_platform;
	std::unique_ptr<ProfileBase> m_profile;

	bfp::BFP m_bfp;

	std::string m_resourceName;

	NiFpga_Session m_session;

};

}
