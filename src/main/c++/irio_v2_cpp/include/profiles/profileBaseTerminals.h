#pragma once

#include <bfp.h>
#include <NiFpga.h>

namespace iriov2 {
/**
 * Base profile with the common terminals shared with all profiles
 */
class ProfileBaseTerminals {
public:
	/**
	 * Constructor.\n
	 *
	 * Manages finding the common terminals in the parsed bitfile
	 * and reading their values from the FPGA if required
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile	Parsed bitfile with the common terminals
	 * @param session		NiFpga_Session to be used in NiFpga related functions
	 */
	ProfileBaseTerminals(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	/**
	 * Returns the parsed \ref TERMINAL_FPGAVIVERSION read from the FPGA
	 *
	 * @return Pair with the FPGAVIVersion. First element is the major version, second the minor version
	 */
	std::pair<std::uint8_t, std::uint8_t> getFPGAVIversion() const;

	/**
	 * Reads the \ref TERMINAL_INITDONE terminal from the FPGA and return its value.
	 * This indicates if the FPGA has been initialized.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return InitDone value. True if FPAG initialized, false otherwise
	 */
	bool getInitDone() const;

	/**
	 * Returns the reference clock of the FPGA for sampling rate.
	 * Its value is specified by the terminal \ref TERMINAL_INITDONE
	 *
	 * @return Reference clock of the FPGA for sampling rate
	 */
	std::uint32_t getFref() const;

	/**
	 * Reads the \ref TERMINAL_DEVQUALITYSTATUS terminal.
	 * Its value indicates the status of the acquisition.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return Status of the acquisition
	 */
	std::uint8_t getDevQualityStatus() const;

	/**
	 * Reads the \ref TERMINAL_DEVTEMP terminal.
	 * Its value indicates the temperature of the FPGA
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return Temperature of the FPGA
	 */
	std::int16_t getDevTemp() const;

	/**
	 * Reads the \ref TERMINAL_DAQSTARTSTOP terminal.
	 * Its value indicates whether the data acquisition is running
	 * or not
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return True if data acquisition is running, false otherwise
	 */
	bool getDAQStartStop() const;

	/**
	 * Reads the \ref TERMINAL_DEBUGMODE terminal.
	 * Its value indicates whether the acquired data is simulated
	 * or not
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return True if data acquired is simulated, false otherwise
	 */
	bool getDebugMode() const;

	/**
	 * Starts the data acquisition.
	 * Writes true to \ref TERMINAL_DAQSTARTSTOP
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 */
	void setDAQStart() const;

	/**
	 * Stops the data acquisition.
	 * Writes false to \ref TERMINAL_DAQSTARTSTOP
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 */
	void setDAQStop() const;

	/**
	 * Allows starting or stopping the data acquisition.
	 * Writes to \ref TERMINAL_DAQSTARTSTOP
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param start True to start, false to stop
	 */
	void setDAQStartStop(const bool &start) const;

	/**
	 * Enables or disables the debug mode.
	 * Writes to \ref TERMINAL_DEBUGMODE
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param debug
	 */
	void setDebugMode(const bool &debug) const;

private:
	std::pair<std::uint8_t, std::uint8_t> m_fpgaviversion;
	std::uint32_t m_fref;

	std::uint32_t m_initdone_addr;
	std::uint32_t m_devqualitystatus_addr;
	std::uint32_t m_devtemp_addr;
	std::uint32_t m_daqstartstop_addr;
	std::uint32_t m_debugmode_addr;

	NiFpga_Session m_session;
};

}

