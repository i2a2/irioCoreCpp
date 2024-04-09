#pragma once

#include <memory>
#include <string>
#include <utility>

#include "platforms.h"
#include "profilesTypes.h"
#include "terminals/terminals.h"

namespace irio {

class ProfileBase;
class ParserManager;

/**
 * @brief The environment variable name for specifying the parse log path.
 *
 * This constant defines the name of the environment variable that can be used
 * to specify the path where the parse log should be written.
 */
constexpr char PARSE_LOG_PATH_ENV_VAR[] = "PARSE_LOG_IRIOV2";

/**
 * @brief The default path for the parse log.
 *
 * This constant defines the default path for the parse log.
 */
constexpr char DEFAULT_PARSE_LOG_PATH[] = "/tmp/irio_log/";

/**
 * irio-v2 main class.
 * Hides the complexity of using the National Instruments low level driver by
 * abstracting several elements from the user. It parses the specified Bitfile
 * and downloads it to the selected RIO device. It also provides the user
 * function to access the terminals and perform read/write operations.
 *
 * @ingroup IRIO
 */
class Irio {
 public:
	/**
	 * Constructor.
	 * - Parses and extract all the resources in the specified Bitfile (\p bitfilePath)
	 * - Initializes the low level library
	 * - Downloads the Bitfile to the specified device (\p RIOSerialNumber)
	 * - Applies the appropriate profile depending on the values specified in the LabVIEW project. This allows access to some terminals or others.
	 * - Checks that the configured values and resources matches the design rules
	 *
	 * @throw irio::errors::RIODeviceNotFoundError		Unable to found a device with the specified \p RIOSerialNumber
	 * @throw irio::errors::BFPParseBitfileError			Unable to parse \p bitfilePath
	 * @throw irio::errors::ResourceNotFoundError			Some of the necessary resources were not found in the bitfile
	 * @throw irio::errors::FPGAVIVersionMismatchError	Parsed FPGAVIversion does not match the one specified
	 * @throw irio::errors::UnsupportedDevProfileError	The DevProfile read does not match any of the supported profiles
	 * @throw irio::errors::UnsupportedPlatformError		The platform read does not match any of the supported platforms
	 * @throw irio::errors::NiFpgaError					Error occurred in an FPGA operation
	 *
	 * @param bitfilePath		Bitfile to parse and download
	 * @param RIOSerialNumber	RIO Serial Number of the device to use
	 * @param FPGAVIversion		Version of the Bitfile. If it does not match the one parsed and exception will be thrown
	 * @param parseVerbose		Print discovered resources
	 */
  Irio(const std::string &bitfilePath, const std::string &RIOSerialNumber,
		 const std::string &FPGAVIversion, const bool parseVerbose = false);

  /**
   * Destructor.
   * Closes the session if there is one open and finalizes
   * the low level library.
   */
  ~Irio();

  /**
   * Returns unique identifier
   *
   * @return Unique identifier
   */
  std::uint32_t getID() const;

  /**
   * Starts the VI downloaded in the FPGA.
   *
   * @throw irio::errors::InitializationTimeoutError	InitDone is not ready
   * after a timeout
   * @throw irio::errors::ModulesNotOKError		At initialization, the
   * modules are not ready
   * @throw irio::errors::NiFpgaFPGAAlreadyRunning	Specified bitfile is
   * already running in the FPGA
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @param timeoutMs Max time to wait for InitDone to be ready
   */
  void startFPGA(std::uint32_t timeoutMs = 5000) const;

  /**
   * Returns the parsed TERMINAL_FPGAVIVERSION read from the FPGA
   *
   * @return String with the FPGAVIversion as (M.m). First element is the major
   * version, second the minor version
   */
  std::string getFPGAVIversion() const;

  /**
   * Reads the TERMINAL_INITDONE terminal from the FPGA and return its
   * value. This indicates if the FPGA has been initialized.
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return InitDone value. True if FPAG initialized, false otherwise
   */
  bool getInitDone() const;

  /**
   * Returns the reference clock of the FPGA for sampling rate.
   * Its value is specified by the terminal TERMINAL_INITDONE
   *
   * @return Reference clock of the FPGA for sampling rate
   */
  std::uint32_t getFref() const;

  /**
   * Reads the TERMINAL_DEVQUALITYSTATUS terminal.
   * Its value indicates the status of the acquisition.
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return Status of the acquisition
   */
  std::uint8_t getDevQualityStatus() const;

  /**
   * Reads the TERMINAL_DEVTEMP terminal.
   * Its value indicates the temperature of the FPGA
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return Temperature of the FPGA
   */
  std::int16_t getDevTemp() const;

  /**
   * Reads the TERMINAL_DAQSTARTSTOP terminal.
   * Its value indicates whether the data acquisition is running
   * or not
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return True if data acquisition is running, false otherwise
   */
  bool getDAQStartStop() const;

  /**
   * Reads the TERMINAL_DEBUGMODE terminal.
   * Its value indicates whether the acquired data is simulated
   * or not
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return True if data acquired is simulated, false otherwise
   */
  bool getDebugMode() const;

  /**
   * Starts the data acquisition.
   * Writes true to TERMINAL_DAQSTARTSTOP
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   */
  void setDAQStart() const;

  /**
   * Stops the data acquisition.
   * Writes false to TERMINAL_DAQSTARTSTOP
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   */
  void setDAQStop() const;

  /**
   * Allows starting or stopping the data acquisition.
   * Writes to TERMINAL_DAQSTARTSTOP
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @param start True to start, false to stop
   */
  void setDAQStartStop(const bool &start) const;

  /**
   * Enables or disables the debug mode.
   * Writes to TERMINAL_DEBUGMODE
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @param debug
   */
  void setDebugMode(const bool &debug) const;

  /**
   * Returns the minimum valid value for the FPGA sampling rate
   *
   * @return Minimum valid sampling rate
   */
  double getMinSamplingRate() const;

  /**
   * Returns the maximum valid value for the FPGA sampling rate
   *
   * @return Maximum valid sampling rate
   */
  double getMaxSamplingRate() const;

  /**
   * Returns the platform detected
   *
   * The object returned has information about the type of platform
   * along with the max number of resources for such platform
   *
   * @return Platform
   */
  Platform getPlatform() const;

  /**
   * Returns the selected profile's ID
   *
   * @return Profile ID
   */
  PROFILE_ID getProfileID() const;

  /**
   * @brief Sets the attribute used when closing the FPGA session.
   *
   * This function sets the attribute used in the NiFpga_Close function.
   * This occurs at class destruction.
   *
   * @param attribute The attribute value to set.
   */
  void setCloseAttribute(std::uint32_t attribute);

  /**
   * @brief Gets the attribute used when closing the FPGA session.
   *
   * This function retrieves the attribute used in the NiFpga_Close function.
   * This occurs at class destruction.
   *
   * @return The attribute value used when closing the session.
   */
  std::uint32_t getCloseAttribute() const;

  ///////////////////////////////////////////////
  /// Terminals
  ///////////////////////////////////////////////

  /**
   * Access to the analog group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return Analog terminals
   */
  TerminalsAnalog getTerminalsAnalog() const;

  /**
   * Access to the digital group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return Digital terminals
   */
  TerminalsDigital getTerminalsDigital() const;

  /**
   * Access to the aux analog group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return Aux analog terminals
   */
  TerminalsAuxAnalog getTerminalsAuxAnalog() const;

  /**
   * Access to the aux digital group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return Aux digital terminals
   */
  TerminalsAuxDigital getTerminalsAuxDigital() const;

  /**
   * Access to the cRIO group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return cRIO terminals
   */
  TerminalscRIO getTerminalsCRIO() const;

  /**
   * Access to the FlexRIO group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return FlexRIO terminals
   */
  TerminalsFlexRIO getTerminalsFlexRIO() const;

  /**
   * Access to the signal generation group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return Signal generation terminals
   */
  TerminalsSignalGeneration getTerminalsSignalGeneration() const;

  /**
   * Access to the DAQ group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return DAQ terminals
   */
  TerminalsDMADAQ getTerminalsDAQ() const;

  /**
   * Access to the IMAQ group terminals
   *
   * The user must call it to be able to read/write the
   * related terminals.
   *
   * @throw irio::errors::TerminalNotImplementedError The selected profile
   * does not have the terminals
   *
   * @return IMAQ terminals
   */
  TerminalsDMAIMAQ getTerminalsIMAQ() const;

 private:
	/**
	 * Initializes the low level library. If system is CCS, does nothing.
	 */
	void initDriver() const;

	/**
	 * Closes the session if it has been opened.
	 */
	void closeSession() noexcept;

	/**
	 * Finalizes the low level library. If system is CCS, does nothing.
	 */
	void finalizeDriver() const noexcept;

    /**
     * Opens a session with the specified bitfile and signature.
     *  
     * Opens a session to the FPGA, downloading the bitfile if necessary.
	 * It does not run the VI, until @ref startFPGA has been called
     * 
     * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
     * 
     * @param bitfilePath The path to the bitfile.
     * @param signature The signature for the session.
     */
	void openSession(const std::string &bitfilePath,
					 const std::string &signature);

	/**
	 * Searches for the TERMINAL_PLATFORM terminal and reads its value.
     * 
	 * Checks that is a valid value and assigns the equivalent
	 * Platform to a variable.
	 *
	 * @throw irio::errors::UnsupportedPlatformError	The platform read does not match any of the supported platforms
	 * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
	 */
	void searchPlatform(ParserManager *parserManager);

	/**
	 * Searches for the TERMINAL_DEVPROFILE terminals and reads its value.
     * 
	 * Checks that is a valid value for the selected platform and assigns
	 * the corresponding profile to a variable.
	 * This determines which terminals can the user access.
	 *
	 * @throw irio::errors::ResourceNotFoundError			Some of the necessary resources were not found in the bitfile
	 * @throw irio::errors::UnsupportedDevProfileError	The DevProfile read does not match any of the supported profiles
	 * @throw irio::errors::NiFpgaError					Error occurred in an FPGA operation
	 */
	void searchDevProfile(ParserManager *parserManager);

	/**
	 * Searches for the common resources used in all profiles.
     * 
	 * It reads some of the resources with their initials values,
	 * so it must be called after a NiFpga_session has been obtained.
	 */
	void searchCommonResources(ParserManager *parserManager);

	/// Platform of the RIO device
	std::unique_ptr<Platform> m_platform;

	/// Profile specified in the bitfile
	std::unique_ptr<ProfileBase> m_profile;

	/// Name of the RIO device used. Obtained through the serialNumber specified.
	std::string m_resourceName;

    /// Session obtained when opening a session using the NiFpga library
	NiFpga_Session m_session;

	/// FpgaVIVersion read from the RIO device
	std::string m_fpgaviversion;
	/// Reference frequency read from the RIO device
	std::uint32_t m_fref;

    /// Address of the initdone resource
	std::uint32_t m_initdone_addr;
    /// Address of the devqualitystatus resource
	std::uint32_t m_devqualitystatus_addr;
    /// Address of the devtemp resource
	std::uint32_t m_devtemp_addr;
    /// Address of the daqstartstop resource
	std::uint32_t m_daqstartstop_addr;
	/// Address of the debugmode resource
	std::uint32_t m_debugmode_addr;

	/// Minimum sampling rate supported by the RIO device
	double m_minSamplingRate;
	/// Maximum sampling rate supported by the RIO device
	double m_maxSamplingRate;

	/// Attribute to use when closing the session with the RIO device. By
	/// default is 0
	std::uint32_t m_closeAttribute;
};

}  // namespace irio
