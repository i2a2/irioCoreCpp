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
 * Hides the complexity of using the National Instruments low level driver by abstracting several elements from the user.\n
 * It parses the specified Bitfile and downloads it to the selected RIO device. It also provides the user function to
 * access the terminals and perform read/write operations.
 */
class IrioV2 {
public:
	/**
	 * Constructor.\n
	 * - Parses and extract all the resources in the specified Bitfile
	 * - Initializes the low level library
	 * - Downloads the Bitfile to the specified device
	 * - Applies the appropriate profile depending on the values specified in the LabVIEW project. This allows access to some terminals or others.
	 * - Checks that the configured values and resources matches the design rules
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
	 * Destructor.\n
	 * Closes the session if there is one open and finalizes
	 * the low level library.
	 */
	~IrioV2();

	/**
	 * Starts the VI downloaded in the FPGA.
	 * If an error occurs, throws a std::runtime_error exception
	 */
	void startFPGA();

	/**
	 * Stops the VI running in the FPGA
	 * If an error occurs, throws a std::runtime_error exception
	 */
	void stopFPGA();

	/**
	 * Returns a pointer to the analog terminals.
	 * The user must call it to be able to read/write analog
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the analog terminals
	 */
	const std::shared_ptr<const TerminalsAnalog> analog() const;

	/**
	 * Returns a pointer to the digital terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the digital terminals
	 */
	const std::shared_ptr<const TerminalsDigital> digital() const;

	/**
	 * Returns a pointer to the aux analog terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the aux analog terminals
	 */
	const std::shared_ptr<const TerminalsAuxAnalog> auxAnalog() const;

	/**
	 * Returns a pointer to the aux digital terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the aux digital terminals
	 */
	const std::shared_ptr<const TerminalsAuxDigital> auxDigital() const;

	/**
	 * Returns a pointer to the cRIO specific terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the cRIO specific terminals
	 */
	const std::shared_ptr<const TerminalscRIO> cRIO() const;

	/**
	 * Returns a pointer to the FlexRIO specific terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the FlexRIO specific terminals
	 */
	const std::shared_ptr<const TerminalsFlexRIO> flexRIO() const;

	/**
	 * Returns a pointer to the Signal Generation terminals.
	 * The user must call it to be able to read/write digital
	 * related terminals.\n
	 * If the profile does not have them, an exception will
	 * be thrown
	 *
	 * @return Pointer to the Signal Generation terminals
	 */
	const std::shared_ptr<const TerminalsSignalGeneration> signalGeneration() const;

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
	 */
	void openSession();

	/**
	 * Searches for the @ref TERMINAL_PLATFORM terminal and reads its value.\n
	 * Checks that is a valid value and assigns the equivalent
	 * Platform to a variable.
	 */
	void searchPlatform();

	/**
	 * Searches for the @ref TERMINAL_DEVPROFILE terminals and
	 * reads its value.\n
	 * Checks that is a valid value for the selected platform and assigns
	 * the corresponding profile to a variable.
	 * This determines which terminals can the user access.
	 */
	void searchDevProfile();

	std::unique_ptr<Platform> m_platform;
	std::unique_ptr<ProfileBase> m_profile;

	bfp::BFP m_bfp;

	std::string m_resourceName;

	NiFpga_Session m_session;

};

}
