#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "terminals/impl/terminalsDMACommonImpl.h"
#include "imaqTypes.h"

namespace irio {

/**
 * Class implementing IMAQ functionality
 * 
 * @ingroup IMAQTerminals
*/
class TerminalsDMAIMAQImpl : public TerminalsDMACommonImpl {
 public:
	/**
	 * Manages finding DMA IMAQ resources
	 *
	 * The name of the terminals
	 * are set by the constructor parameters, to allow reusing
	 * code when terminal functionality is the same but name
	 * changes (such is the case for CPU and GPU)
	 *
	 * @throw std::runtime_error	Size mismatch between \p nameTermDMA
	 * 								and \p nameTermDMAEnable
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param parserManager     	Pointer to class managing parsing the
	 * 								bitfile and finding its resources
	 * @param session				NiFpga_Session to be used in NiFpga
	 * 								related functions
	 * @param platform				Platform that is using the terminals.
	 * 								Used to know the maximum number of terminals
	 * 								that can be found
	 * @param nameTermNCh			Name of the terminal that describes the
	 * 								number of DMAs implemented in the FPGA
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
	TerminalsDMAIMAQImpl(ParserManager *parserManager,
						 const NiFpga_Session &session,
						 const Platform &platform,
						 const std::string &nameTermNCh,
						 const std::string &nameTermFrameType,
						 const std::string &nameTermSampleSize,
						 const std::string &nameTermOverflows,
						 const std::string &nameTermDMA,
						 const std::string &nameTermDMAEnable);

	void configCameraLinkImpl(
		const std::int32_t fvalHigh, const std::int32_t lvalHigh,
		const std::int32_t dvalHigh, const std::int32_t spareHigh,
		const std::int32_t controlEnable, const std::int32_t linescan,
		const CLSignalMapping &signalMapping, const CLMode &mode);

	size_t readImageNonBlockingImpl(const std::uint32_t n,
									const size_t imagePixelSize,
									std::uint64_t *imageRead) const;

	size_t readImageBlockingImpl(const std::uint32_t n,
								 const size_t imagePixelSize,
								 std::uint64_t *imageRead,
								 const std::uint32_t timeout = 0) const;

	size_t readImageImpl(const std::uint32_t n, const size_t imagePixelSize,
						 std::uint64_t *imageRead, const bool blockRead,
						 const std::uint32_t timeout = 0) const;

	void sendUARTMsgImpl(const std::vector<std::uint8_t> &msg,
						 const std::uint32_t timeout = 0) const;

	std::vector<std::uint8_t> recvUARTMsgImpl(const size_t bytesToRecv,
								const std::uint32_t timeout = 0) const;

	void setUARTBaudRateImpl(const UARTBaudRates &baudRate,
							 const std::uint32_t timeout = 0) const;

	UARTBaudRates getUARTBaudRateImpl() const;

	std::uint16_t getUARTBreakIndicatorImpl() const;

	std::uint16_t getUARTFramingErrorImpl() const;

	std::uint16_t getUARTOverrunErrorImpl() const;

 private:
	void findUART(irio::ParserManager *parserManager);

	void findCLConfig(irio::ParserManager *parserManager);

	std::uint32_t m_baudRate_addr;
	std::uint32_t m_setBaudRate_addr;
	std::uint32_t m_txReady_addr;
	std::uint32_t m_txByte_addr;
	std::uint32_t m_transmit_addr;
	std::uint32_t m_rxReady_addr;
	std::uint32_t m_rxByte_addr;
	std::uint32_t m_receive_addr;
	std::uint32_t m_breakIndicator_addr;
	std::uint32_t m_framingError_addr;
	std::uint32_t m_overrunError_addr;

	std::uint32_t m_fvalHigh_addr;
	std::uint32_t m_lvalHigh_addr;
	std::uint32_t m_dvalHigh_addr;
	std::uint32_t m_spareHigh_addr;
	std::uint32_t m_controlEnable_addr;
	std::uint32_t m_signalMapping_addr;
	std::uint32_t m_configuration_addr;
	std::uint32_t m_lineScan_addr;
};

}  // namespace irio
