#pragma once

#include <memory>

#include "terminals/terminalsDMACommon.h"

namespace irio {

class TerminalsDMADAQImpl;

/**
 * Class managing the terminals used for DMA DAQ operations
 *
 * @ingroup DMATerminals
 */
class TerminalsDMADAQ: public TerminalsDMACommon{
 public:
	/**
	 * Manages finding DMA DAQ resources.
	 * 
	 * @throw irio::errors::NiFpgaError Error reading resources from the FPGA
	 *
	 * @param impl Shared pointer to the class that implements the functionality
	 */
	explicit TerminalsDMADAQ(
				std::shared_ptr<TerminalsDMADAQImpl> impl);

	/**
	 * Returns the block length of a specific DMA group
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of DMA group
	 * @return	Length of the block used in the specified DMA group
	 */
	std::uint16_t getLengthBlock(const std::uint32_t &n) const;

	/**
	 * Returns the decimation of a specific DMA group
	 *
	 * The sampling rate configured can be calculated as Fref/decimation
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @return	Decimation used in the specified DMA group
	 */
	std::uint16_t getSamplingRateDecimation(const std::uint32_t &n) const;

	/**
	 * Configures the sampling rate for a specific DMA group
	 *
	 * The sampling rate is configured by a decimation factor.
	 * The sampling rate configured is calculated as Fref/decimation
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @param decimation	Decimation to apply to the Fref
	 */
	void setSamplingRateDecimation(const std::uint32_t &n,
			const std::uint16_t &decimation) const;
};

}  // namespace irio


