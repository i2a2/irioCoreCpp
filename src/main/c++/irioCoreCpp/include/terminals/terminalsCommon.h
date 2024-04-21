#pragma once

#include <string>

#include "terminals/terminalsBase.h"

namespace irio {

/**
 * Class managing terminals common to all profiles
 * 
 * @ingroup Terminals 
 */
class TerminalsCommon: public TerminalsBase {
 public:
  /**
   * Manages finding the related common terminals
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * 
   * @param parserManager   Pointer to class managing parsing the bitfile
   *                        and finding its resources
   * @param session			NiFpga_Session to be used in NiFpga related
   * 						        functions
   */
  TerminalsCommon(ParserManager *parserManager, const NiFpga_Session &session);

  /**
   * Returns the parsed TERMINAL_FPGAVIVERSION read from the FPGA
   *
   * @return String with the FPGAVIversion as (M.m). First element is the major
   * version, second the minor version
   */
  std::string getFPGAVIversion() const;

  /**
   * Returns the reference clock of the FPGA for sampling rate.
   * Its value is specified by the terminal TERMINAL_INITDONE
   *
   * @return Reference clock of the FPGA for sampling rate
   */
  std::uint32_t getFref() const;

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
};

}  // namespace irio
