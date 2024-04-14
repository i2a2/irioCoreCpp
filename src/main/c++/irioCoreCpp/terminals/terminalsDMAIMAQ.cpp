#include "terminals/terminalsDMAIMAQ.h"

#include "terminals/impl/terminalsDMAIMAQImpl.h"

namespace irio {

TerminalsDMAIMAQ::TerminalsDMAIMAQ(std::shared_ptr<TerminalsDMAIMAQImpl> impl)
	: TerminalsDMACommon(
		  std::static_pointer_cast<TerminalsDMACommonImpl>(impl)) {}

void TerminalsDMAIMAQ::configCameraLink(
    const std::int32_t fvalHigh, const std::int32_t lvalHigh,
    const std::int32_t dvalHigh, const std::int32_t spareHigh,
    const std::int32_t controlEnable, const std::int32_t linescan,
    const CLSignalMapping &signalMapping, const CLMode &mode) {
	std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->configCameraLinkImpl(fvalHigh, lvalHigh, dvalHigh, spareHigh,
							   controlEnable, linescan, signalMapping, mode);
}

size_t TerminalsDMAIMAQ::readImageNonBlocking(const std::uint32_t n,
											  const size_t imagePixelSize,
											  std::uint64_t *imageRead) const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->readImageNonBlockingImpl(n, imagePixelSize, imageRead);
}

size_t TerminalsDMAIMAQ::readImageBlocking(
	const std::uint32_t n, const size_t imagePixelSize,
	std::uint64_t *imageRead, const std::uint32_t timeout) const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->readImageBlockingImpl(n, imagePixelSize, imageRead, timeout);
}

size_t TerminalsDMAIMAQ::readImage(const std::uint32_t n,
								   const size_t imagePixelSize,
								   std::uint64_t *imageRead,
								   const bool blockRead,
								   const std::uint32_t timeout) const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->readImageImpl(n, imagePixelSize, imageRead, blockRead, timeout);
}

void TerminalsDMAIMAQ::sendUARTMsg(const std::vector<std::uint8_t> &msg,
								   const std::uint32_t timeout) const {
	std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)->sendUARTMsgImpl(
		msg, timeout);
}

std::vector<std::uint8_t> TerminalsDMAIMAQ::recvUARTMsg(
	const size_t bytesToRecv, const std::uint32_t timeout) const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->recvUARTMsgImpl(bytesToRecv, timeout);
}

void TerminalsDMAIMAQ::setUARTBaudRate(const UARTBaudRates &baudRate,
									   const std::uint32_t timeout) const {
	std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)->setUARTBaudRateImpl(
		baudRate, timeout);
}

UARTBaudRates TerminalsDMAIMAQ::getUARTBaudRate() const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->getUARTBaudRateImpl();
}

std::uint16_t TerminalsDMAIMAQ::getUARTBreakIndicator() const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->getUARTBreakIndicatorImpl();
}

std::uint16_t TerminalsDMAIMAQ::getUARTFramingError() const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->getUARTFramingErrorImpl();
}

std::uint16_t TerminalsDMAIMAQ::getUARTOverrunError() const {
	return std::static_pointer_cast<TerminalsDMAIMAQImpl>(m_impl)
		->getUARTOverrunErrorImpl();
}
}  // namespace irio
