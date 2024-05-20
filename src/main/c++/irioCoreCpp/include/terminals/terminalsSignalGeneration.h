#pragma once

#include <vector>

#include "terminals/terminalsBase.h"

namespace irio {
/**
 * Class managing all terminals related to the
 * signal generation functionality of the RIO device
 *
 * @ingroup Terminals
 */
class TerminalsSignalGeneration: public TerminalsBase {
 public:
  /**
   * Manages finding the related signal generation terminals
   * in the parsed bitfile and reading their values from the FPGA if required
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @param parserManager   Pointer to class managing parsing the bitfile
   *                       	and finding its resources
   * @param session			NiFpga_Session to be used in NiFpga related functions
   * @param platform		Platform that is using the terminals. Used to know
   * 						        the maximum number of terminals that can be found.
   */
  TerminalsSignalGeneration(ParserManager *parserManager,
							const NiFpga_Session &session,
							const Platform &platform);

  /**
   * Returns the number of signal generators in the FPGA implementation
   * 
   * @return Number of signal generators implemented
   */
  std::uint8_t getSGNo() const;

  /**
   * Returns the reference clock for a specified signal generator.
   *
   * @throw irio::errors::ResourceNotFoundError The specified signal generator
   * does not exists
   *
   * @param n	Number of the signal generator
   * @return	Reference clock for sampling rate for the specified signal
   * generator
   */
  std::uint32_t getSGFref(const std::uint32_t n) const;

  /**
   * Returns a vector with the reference clocks for all the signal generators
   * in the system
   * 
   * @return Vector with reference clocks in ascending order of terminal number
   */
  std::vector<std::uint32_t> getVectorSGFrefs() const;

  /**
   * Returns the signal type generated for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n Number of the signal generator
   * @return	Type of the signal for the specified signal generator
   */
  std::uint8_t getSGSignalType(const std::uint32_t n) const;

  /**
   * Returns the amplitude of the signal for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n Number of the signal generator
   * @return	Amplitude of the signal for the specified signal generator
   */
  std::uint32_t getSGAmp(const std::uint32_t n) const;

  /**
   * Returns the frequency of the signal for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n Number of the signal generator
   * @return	Frequency of the signal for the specified signal generator
   */
  std::uint32_t getSGFreq(const std::uint32_t n) const;

  /**
   * Returns the phase of the signal for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n Number of the signal generator
   * @return	Phase of the signal for the specified signal generator
   */
  std::uint32_t getSGPhase(const std::uint32_t n) const;

  /**
   * Returns the update rate of a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n Number of the signal generator
   * @return	Update rate of the specified signal generator
   */
  std::uint32_t getSGUpdateRate(const std::uint32_t n) const;

  /**
   * Configures the signal type for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n			Number of the signal generator
   * @param value		Signal type to configure
   */
  void setSGSignalType(const std::uint32_t n, const std::uint8_t value) const;

  /**
   * Configures the amplitude for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n			Number of the signal generator
   * @param value		Amplitude to configure
   */
  void setSGAmp(const std::uint32_t n, const std::uint32_t value) const;

  /**
   * Configures the decimation to apply to a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n			Number of the signal generator
   * @param value		Decimation to configure
   */
  void setSGFreqDecimation(const std::uint32_t n,
						   const std::uint32_t value) const;

  /**
   * Configures the phase for a specific signal generator
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n			Number of the signal generator
   * @param value		Phase to configure
   */
  void setSGPhase(const std::uint32_t n, const std::uint32_t value) const;

  /**
   * Configures the update rate decimation for a specific signal generator
   * 
   * The update rate configured can be calculated as Fref/value
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n			  Number of the signal generator
   * @param value		Decimation to configure
   */
  void setSGUpdateRateDecimation(const std::uint32_t n,
								 const std::uint32_t value) const;
};

}  // namespace irio

