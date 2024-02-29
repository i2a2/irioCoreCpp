#pragma once

#include <terminals/terminalsDMACommonImpl.h>

namespace iriov2{

/**
 * Class managing the terminals used for DMA DAQ operations
 *
 * @ingroup DMATerminals
 */
class TerminalsDMADAQImpl: public TerminalsDMACommonImpl{
public:
	/**
	 * Constructor.
	 * Manages finding DMA DAQ resources.
	 * The name of these terminals are set by the constructor
	 * parameters, to allow reusing code when terminal
	 * functionality is the same but name changes
	 * (such is the case for CPU and GPU)
	 *
	 * @throw std::runtime_error	Size mismatch between found resources
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile			Parsed bitfile with the terminals
	 * @param session				NiFpga_Session to be used in NiFpga
	 * 								related functions
	 * @param platform				Platform that is using the terminals.
	 * 								Used to know the maximum number of terminals
	 * 								that can be found
	 * @param nameTermBlockNWords	Name of the terminal that indicates the length
	 * 								of the block used for each DMA
	 * @param nameTermSamplingRate	Name of the terminals that sets the sampling rate
	 * 								for each DMA
	 * @param nameTermNCh			Name of the terminal that describes the number
	 * 								of DMAs implemented in the FPGA
	 * @param nameTermFrameType		Name of the terminal that describes the
	 * 								frame type used in the DMA frame
	 * @param nameTermSampleSize	Name of the terminal with the size in bytes
	 * 								for the channel sample
	 * @param nameTermOverflows		Name of the terminal with the status of the
	 * 								different DMA FIFO
	 * @param nameTermDMA			Name of the DMAs
	 * @param nameTermDMAEnable		Name of the terminals that enable or disable
	 * 								write to DMA
	 */
	TerminalsDMADAQImpl(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const std::string &nameTermBlockNWords,
			const std::string &nameTermSamplingRate,
			const std::string &nameTermNCh,
			const std::string &nameTermFrameType,
			const std::string &nameTermSampleSize,
			const std::string &nameTermOverflows,
			const std::string &nameTermDMA,
			const std::string &nameTermDMAEnable);

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

private:
	const std::string m_nameTermSamplingRate;

	std::vector<std::uint16_t> m_lengthBlocks;

	std::unordered_map<std::uint32_t, const std::uint32_t> m_samplingRate_addr;
};

}


