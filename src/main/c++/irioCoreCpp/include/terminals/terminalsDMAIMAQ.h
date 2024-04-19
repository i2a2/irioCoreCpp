#pragma once

#include <memory>
#include <vector>

#include "terminals/terminalsDMACommon.h"
#include "imaqTypes.h"

namespace irio {
class TerminalsDMAIMAQImpl;

/**
 * Class managing the resources used for IMAQ DAQ operations
 * 
 * @ingroup IMAQTerminals
*/
class TerminalsDMAIMAQ: public TerminalsDMACommon {
 public:
    /**
     * Manages finding IMAQ resources
	 * 
	 * @throw irio::errors::NiFpgaError Error reading resources from the FPGA
	 * 
     * @param impl Shared pointer to a class implementing the functionality
     */
    explicit TerminalsDMAIMAQ(std::shared_ptr<TerminalsDMAIMAQImpl> impl);

	/**
	 * Configure CameraLink frame grabber
	 *
	 * Configure CameraLink frame grabber with the camera parameters.
	 * This configuration includes:
	 *  - Set whether the CaneraLink signals FVAL, LVAL, DVAL, and SPARE
	 * 		are active high or active low.
	 *  - Set whether the CameraLink control signals will be driven by
	 * 		the FPGA or not.
	 *  - Set the scanning mode between regular and line scan.
	 *  - Set the signal mapping when using extended configuration mode.
	 *  - Set the configuration mode.
	 *
	 * This configuration must be done before running the FPGA.
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param fvalHigh			0=FVAL active low 1=FVAL active high
	 * @param lvalHigh			0=LVAL active low 1=LVAL active high
	 * @param dvalHigh			0=DVAL active low 1=DVAL active high
	 * @param spareHigh			0=SPARE active low 1=SPARE active high
	 * @param controlEnable		0=High impedance on conrol signals 1=Control
	 * 							signals driver by the FPGA.
	 * @param linescan			0=Regular scan (complete image) 1=Line scan
	 * @param signalMapping		Signal map to be used.
	 * @param mode				Configuration to be used.
	 */
	void configCameraLink(
		const std::int32_t fvalHigh, const std::int32_t lvalHigh,
		const std::int32_t dvalHigh, const std::int32_t spareHigh,
		const std::int32_t controlEnable, const std::int32_t linescan,
		const CLSignalMapping &signalMapping, const CLMode &mode) const;

	/**
	 * Tries to read an specifeid number of pixels.
	 * If there are less pixels than requested available, nothing is read.
	 * 
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 * 
	 * @param n 				Number of DMA group where image is stored
	 * @param imagePixelSize 	Size of the image in pixels
	 * @param imageRead 		Buffer to write the read data.
	 * 							Allocation and deallocation of data is 
	 * 							user responsibility
	 * @return	Number of elements read. 0 if they were not enough,
	 * 			the number specified in \p elementsToRead it they were.
	 */
	size_t readImageNonBlocking(const std::uint32_t n,
								const size_t imagePixelSize,
								std::uint64_t *imageRead) const;

	/**
	 * Waits to read an specified number of pixels from a DMA group.
	 * 
	 * The read operation will block until the requested number of elements
	 * are available or a timeout expires if specified.
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::DMAReadTimeout 	If reading is in blocking mode,
	 * 											and the timeout expires waiting
	 * 											for enough data to be read
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n					Number of DMA group
	 * @param imagePixelSize	Number of pixels to read from the DMA
	 * @param imageRead			Buffer to write the read image. Allocation and
	 * 							deallocation of this buffer is user responsibility
	 * @param timeout			Max time in milliseconds to wait for the \p
	 * 							elementsToRead to be available, 
	 * 							0 to wait indefinitely.
	 * @return 	Unless the timeout expires, this function will always
	 * 			return the specified \p elementsToRead
	 */
	size_t readImageBlocking(const std::uint32_t n, const size_t imagePixelSize,
							 std::uint64_t *imageRead,
							 const std::uint32_t timeout = 0) const;

	/**
	 * Reads an specified number of pixels from a DMA group.
	 *
	 * It can be configure to wait until there are the specified
	 * number of pixels in the DMA or read nothing if
	 * there are not enough.
	 * If configured to wait, a timeout can be specified.
	 *
	 * @throw irio::errors::ResourceNotFoundError Resource specified not found
	 * @throw irio::errors::DMAReadTimeout 	If reading is in blocking mode,
	 * 											and the timeout expires waiting
	 * 											for enough data to be read
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param n	Number of DMA group
	 * @param imagePixelSize	Number of pixels to read from the DMA
	 * @param imageRead	Buffer to write the read image. Allocation and
	 * 					deallocation of this buffer is user responsibility
	 * @param blockRead	 Whether to wait until the requested number of pixels
	 * 					( \p imagePixelSize) are available or not
	 * @param timeout	If \p blockRead is true. Max time in milliseconds to
	 * 					wait for the \p imagePixelSize to be available,
	 * 					0 means wait indefinitely. If \p blockRead is false,
	 * 					this parameter is ignored.
	 *
	 * @return	Number of elements read. 0 if they were not enough,
	 * 			the number specified in \p imagePixelSize it they were.
	 */
	size_t readImage(const std::uint32_t n, const size_t imagePixelSize,
					 std::uint64_t *imageRead, const bool blockRead,
					 const std::uint32_t timeout = 0) const;

	/**
	 * Sends an UART message to the CameraLink system
	 *
	 * Streams the given vector to the CameraLink camera connected to the frame
	 * grabber.
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 * @throw irio::errors::CLUARTTimeout Timeout waiting for
	 * 										\p TERMINAL_UARTTXREADY to be ready
	 *
	 * @param msg 		Message to send
	 * @param timeout 	Time in ms to wait for line to be ready to send message.
	 * 					(0 to wait indefinetly)
	 */
	void sendUARTMsg(const std::vector<std::uint8_t> &msg,
					 const std::uint32_t timeout = 0) const;

	/**
	 * Reads an UART message from the CameraLink system
	 * 
	 * Do not use bytesToRecv = 0 and timeout = 0 at the same time!
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param bytesToRecv	Number of bytes to read. If it is 0,
	 * 						reads everything until timeout
	 * @param timeout		Max time (ms) to wait between bytes. (0 to wait
	 * 						indefinetly)
	 * @return 	Message read
	 */
	std::vector<std::uint8_t> recvUARTMsg(
		const size_t bytesToRecv = 0, const std::uint32_t timeout = 1000) const;

	/**
	 * Sets UART baud rate
	 * 
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 * @throw irio::errors::CLUARTTimeout Timeout waiting for 
	 * 										\p TERMINAL_UARTSETBAUDRATE to 
	 * 										be ready
	 * 
	 * @param baudRate 	Baud rate to configure.
	 * @param timeout 	Time in ms to wait for the line to be ready
	 * 					(0 to wait indefinetly)
	 */
	void setUARTBaudRate(const UARTBaudRates &baudRate,
						 const std::uint32_t timeout = 0) const;

	/**
	 * Read UART baud rate
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 * @throw irio::errors::CLUARTInvalidBaudRate 	The baud rate read from the
	 * 												FPGA does nto match any of the expected values
	 *
	 * @return Baud rate read
	 */
	UARTBaudRates getUARTBaudRate() const;

	/**
	 * Read UART Break Indicator
	 *
	 * Reads the value from CameraLink UART break indicator port. A non-zero
	 * value returned indicates an interruption in the UART communication.
	 * 
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 * 
	 * @return Current value of the UART Break Indicator
	 */
	std::uint16_t getUARTBreakIndicator() const;

	/**
	 * Read UART Framing Error
	 *
	 * Reads the value from CameraLink UART framing error port. A non-zero value
	 * returned indicates problems while receiving UART data.
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @return Current value of the UART Framing Error
	 */
	std::uint16_t getUARTFramingError() const;

	/**
	 * Read UART Overrun Error
	 *
	 * Reads the value from CameraLink UART overrun error port. A non-zero value
	 * returned indicates an overrun in a UART buffer, causing data loss.
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @return Current value of the UART Overrun Error
	 */
	std::uint16_t getUARTOverrunError() const;
};

}  // namespace irio
