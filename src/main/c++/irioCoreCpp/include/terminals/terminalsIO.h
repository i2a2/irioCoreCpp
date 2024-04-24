#pragma once

#include "terminals/terminalsBase.h"

namespace irio {

/**
 * Class managing all terminals related to
 * the cRIO IO functionality.
 *
 * @ingroup Terminals
 */
class TerminalsIO: public TerminalsBase {
 public:
  /**
   * Manages finding the related IO terminals
   * 
   * @param parserManager   Pointer to class managing parsing the bitfile
   *                        and finding its resources
   * @param session			NiFpga_Session to be used in NiFpga related
   * 						        functions
   * @param platform		Platform that is using the terminals. Used to
   * 						        know the maximum number of terminals that can be
   * 						        found.
   */
  TerminalsIO(ParserManager *parserManager, const NiFpga_Session &session,
			  const Platform &platform);

  /**
	 * Configures the sampling rate for a specific cRIO module
	 *
	 * The sampling rate is configured by a decimation factor.
	 * The sampling rate configured is calculated as Fref/decimation
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n		Number of cRIO module
	 * @param dec	Decimation to apply to the Fref
	 */
  void setSamplingRateDecimation(const std::uint32_t &n,
								 const std::uint16_t dec) const;

  /**
	 * Returns the decimation of a sampling rate specific to a module
	 *
	 * The sampling rate configured can be calculated as Fref/decimation
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of cRIO module
	 * @return	Decimation used in the specified module
	 */
  std::uint16_t getSamplingRateDecimation(const std::uint32_t &n) const;

  /**
   * Returns number of IO Sampling Rate terminals found
   *
   * @return Number of IO Sampling Rate terminals found
   */
  size_t getNumIOSamplingRate() const;
};

}  // namespace irio
