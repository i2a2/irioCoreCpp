#pragma once

#include <terminals/terminalsBase.h>
#include <frameTypes.h>

namespace iriov2{
/**
 * Class managing the terminals common to all other
 * terminals that use DMAs in the RIO device
 *
 * @ingroup DMATerminals
 */
class TerminalsDMACommonImpl: public TerminalsBase{
public:
	/**
	 * Constructor.
	 * Manages finding DMA resources common to all terminals
	 * that use them, DAQ/IMAQ. The name of these terminals
	 * are set by the constructor parameters, to allow reusing
	 * code when terminal functionality is the same but name
	 * changes (such is the case for CPU and GPU)
	 *
	 * @throw std::runtime_error	Size mismatch between \p nameTermDMA
	 * 								and \p nameTermDMAEnable
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile			Parsed bitfile with the terminals
	 * @param session				NiFpga_Session to be used in NiFpga
	 * 								related functions
	 * @param platform				Platform that is using the terminals.
	 * 								Used to know the maximum number of terminals
	 * 								that can be found
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
	TerminalsDMACommonImpl(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const std::string &nameTermNCh,
			const std::string &nameTermFrameType,
			const std::string &nameTermSampleSize,
			const std::string &nameTermOverflows,
			const std::string &nameTermDMA,
			const std::string &nameTermDMAEnable);


	/**
	 * Returns number of channels in specific DMA
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n  Number of DMA group
	 * @return	Number of channels implemented in the specified DMA
	 */
	std::uint16_t getNCh(const std::uint32_t n) const;

	/**
	 * Returns the FPGA DMA Overflow register of a specific DMA
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n Number of DMA group
	 * @return	True if overflows occurred, false if not
	 */
	bool getDMAOverflow(const std::uint16_t n) const;

	/**
	 * Returns the FPGA DMA Overflow register
	 *
	 * The <i>n<sup>th</sup></i> bit of the register corresponds
	 * to the overflow value of the <i>n<sup>th</sup></i> DMA.
	 *
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @return FPGA DMA Overflow register value
	 */
	std::uint16_t getAllDMAOverflows() const;

	/**
	 * Return the type of frame used in a specific DMA.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of DMA group
	 * @return	Type of frame used by the DMA
	 */
	FrameType getFrameType(const std::uint32_t n) const;

	/**
	 * Returns a vector of the frame types used by each DMA in the FPGA
	 * @return Vector of frame types, the position corresponds to the number of DMA
	 */
	std::vector<FrameType> getAllFrameType() const;

	/**
	 * Returns the sample size of each data for a specific DMA
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @return	Sample size in bytes for the specified DMA
	 */
	std::uint8_t getSampleSize(const std::uint32_t n) const;

	/**
	 * Returns a vector of sample sizes used by each DMA in the FPGA
	 *
	 * @return Vector of sample sizes (in bytes), the position corresponds to the number of DMA
	 */
	std::vector<std::uint8_t> getAllSampleSizes() const;

	/*TODO: Ask, should we allow use to configure the fifo? or just
	give a lot of memory to each? as the original lib did*/
//	void configureDMA(const std::uint32_t n, const size_t depth) const;
//	void configureAllDMAs(const size_t depth) const;

	/**
	 * Configure a specified DMA, start it and clean its contents.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n DMA group to configure and start
	 */
	void startDMA(const std::uint32_t n) const;

	/**
	 * Configures and starts all DMAs in the FPGA
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 */
	void startAllDMAs() const;

	/**
	 * Stops the specified DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	DMA to stop
	 */
	void stopDMA(const std::uint32_t n) const;

	/**
	 * Stops all DMAs in the FPGA
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 */
	void stopAllDMAs() const;

	/**
	 * Cleans the contents of a specified DMA group.
	 * Warning: If the DMA is running, it may lead to a situation
	 * where more data is read that is possible to clean, which
	 * may lead to a infinite loop
	 *
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	DMA group to clean
	 */
	void cleanDMA(const std::uint32_t n) const;

	/**
	 * Cleans the contents of all DMAs in the FPGA
	 * Warning: If the DMA is running, it may lead to a situation
	 * where more data is read that is possible to clean, which
	 * may lead to a infinite loop
	 *
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 */
	void cleanAllDMAs() const;

	/**
	 * Returns if a DMA group is enabled or not
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n Number of DMA group
	 * @return True if enabled, false if not
	 */
	bool isDMAEnable(const std::uint32_t n) const;

	/**
	 * Enables a DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n Number of DMA group
	 */
	void enableDMA(const std::uint32_t n) const;

	/**
	 * Disables a DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n Number of DMA group
	 */
	void disableDMA(const std::uint32_t n) const;

	/**
	 * Enables or disables a DMA group
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n Number of DMA group
	 * @param enaDis True to enable, false to disable
	 */
	void enaDisDMA(const std::uint32_t n, bool enaDis) const;

	/**
	 * Tries to read an specified number of elements from a DMA group.
	 * If there are less elements than requested nothing is read.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @param elementsToRead	Number of elements to read from the DMA
	 * @param data	Buffer to write the read data. Allocation and deallocation of data is user responsibility
	 *
	 * @return	Number of elements read. 0 if they were not enough,
	 * 			the number specified in \p elementsToRead it they were.
	 */
	size_t readDataNonBlocking(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data);

	/**
	 * Waits to read an specified number of elements from a DMA group.
	 * The read operation will block until the requested number of elements
	 * are available or a timeout expires if specified.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::DMAReadTimeout 	If reading is in blocking mode,
	 * 											and the timeout expires waiting for enough data to be read
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @param elementsToRead	NUmber of elements to read from the DMA
	 * @param data	Buffer to write the read data. Allocation and deallocation
	 * 				of data is user responsibility
	 * @param timeout	Max time in milliseconds to wait for the \p elementsToRead to
	 * 					be available, 0 means wait indefinitely.
	 * @return 	Unless the timeout expires, this function will always
	 * 			return the specified \p elementsToRead
	 */
	size_t readDataBlocking(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data,
			std::uint32_t timeout = 0);

	/**
	 * Reads an specified number of elements from a DMA group.
	 * It can be configure to wait until there are the specified
	 * number of elements in the DMA or read nothing if
	 * there are not enough.
	 * If configured to wait, a timeout can be specified.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::DMAReadTimeout 	If reading is in blocking mode,
	 * 											and the timeout expires waiting for enough data to be read
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @param elementsToRead	Number of elements to read from the DMA
	 * @param data	Buffer to write the read data. Allocation and deallocation of data is user responsibility
	 * @param blockRead	 Whether to wait until the requested number of elements (\p elementsToRead) are available or not
	 * @param timeout	If \p blockRead is true. Max time in milliseconds to wait for the \p elementsToRead to
	 * 					be available, 0 means wait indefinitely. If \p blockRead is false, this parameter is ignored.
	 *
	 * @return	Number of elements read. 0 if they were not enough,
	 * 			the number specified in \p elementsToRead it they were.
	 */
	size_t readData(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data,
			bool blockRead,
			std::uint32_t timeout = 0) const;

	/**
	 * Returns the number of DMAs found
	 *
	 * @return Number of found DMAs
	 */
	size_t countDMAs() const;

protected:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDMA;

private:
	static const size_t SIZE_HOST_DMAS = 2048000; //TODO: Why this number?

	void startDMAImpl(const std::uint32_t &dma) const;
	void cleanDMAImpl(const std::uint32_t &dma) const;

	std::uint32_t m_overflowsAddr;

	std::vector<std::uint16_t> m_nCh;
	std::vector<FrameType> m_frameType;
	std::vector<std::uint8_t> m_sampleSize;

	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapEnable;

	const std::string m_nameTermOverflows;
	const std::string m_nameTermDMA;
	const std::string m_nameTermDMAEnable;

};
}
