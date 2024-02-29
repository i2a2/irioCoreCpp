#pragma once

#include <terminals/terminalsDMACommon.h>
#include <terminals/impl/terminalsDMADAQImpl.h>

namespace iriov2{

/**
 * Class managing the terminals used for DMA DAQ operations
 *
 * @ingroup DMATerminals
 */
class TerminalsDMADAQ: public TerminalsDMACommon{
public:
	/**
	 * Constructor.
	 * Manages finding DMA DAQ resources.
	 *
	 * @param impl Shared pointer to the class that implements the functionality
	 */
	TerminalsDMADAQ(
			std::shared_ptr<TerminalsDMADAQImpl> impl);

	/**
	 * Returns the block length of a specific DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of DMA group
	 * @return	Length of the block used in the specified DMA group
	 */
	std::uint16_t getLengthBlock(const std::uint32_t &n) const;

	/**
	 * Returns the sampling rate of a specific DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @return	Sampling rate used in the specified DMA group
	 */
	std::uint16_t getSamplingRate(const std::uint32_t &n) const;

	/**
	 * Configures the sampling rate for a specific DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @param samplingRate	Sampling rate to configure
	 */
	void setSamplingRate(const std::uint32_t &n, const std::uint16_t &samplingRate) const;
};

}


