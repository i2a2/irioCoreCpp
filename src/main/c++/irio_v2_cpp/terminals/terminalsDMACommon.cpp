#include <terminals/terminalsDMACommon.h>
#include <terminals/impl/terminalsDMACommonImpl.h>
#include <errorsIrio.h>
#include <utils.h>
#include <memory>

namespace iriov2{

TerminalsDMACommon::TerminalsDMACommon(std::shared_ptr<TerminalsDMACommonImpl> impl) :
		TerminalsBase(impl) {
}

std::uint16_t TerminalsDMACommon::getNCh(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getNChImpl(n);
}

void TerminalsDMACommon::startDMA(const std::uint32_t n) const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->startDMAImpl(n);
}

void TerminalsDMACommon::startAllDMAs() const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->startAllDMAsImpl();
}

void TerminalsDMACommon::stopDMA(const std::uint32_t n) const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->stopDMAImpl(n);
}

void TerminalsDMACommon::stopAllDMAs() const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->stopAllDMAsImpl();
}

size_t TerminalsDMACommon::countDMAs() const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->countDMAsImpl();
}

void TerminalsDMACommon::cleanDMA(const std::uint32_t n) const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->cleanDMAImpl(n);
}

void TerminalsDMACommon::cleanAllDMAs() const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->cleanAllDMAsImpl();
}

bool TerminalsDMACommon::isDMAEnable(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->isDMAEnableImpl(n);
}

void TerminalsDMACommon::enableDMA(const std::uint32_t n) const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->enableDMAImpl(n);
}

void TerminalsDMACommon::disableDMA(const std::uint32_t n) const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->disableDMAImpl(n);
}

void TerminalsDMACommon::enaDisDMA(const std::uint32_t n, bool enaDis) const {
	std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->enaDisDMAImpl(n, enaDis);
}

bool TerminalsDMACommon::getDMAOverflow(const std::uint16_t n) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getDMAOverflowImpl(n);
}

std::uint16_t TerminalsDMACommon::getAllDMAOverflows() const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getAllDMAOverflowsImpl();
}

FrameType TerminalsDMACommon::getFrameType(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getFrameTypeImpl(n);
}

std::vector<FrameType> TerminalsDMACommon::getAllFrameType() const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getAllFrameTypeImpl();
}

std::uint8_t TerminalsDMACommon::getSampleSize(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getSampleSizeImpl(n);
}

std::vector<std::uint8_t> TerminalsDMACommon::getAllSampleSizes() const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->getAllSampleSizesImpl();
}

size_t TerminalsDMACommon::readDataNonBlocking(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->readDataNonBlockingImpl(n, elementsToRead, data);
}

size_t TerminalsDMACommon::readDataBlocking(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data,
		std::uint32_t timeout) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->readDataBlockingImpl(n, elementsToRead, data, timeout);
}

size_t TerminalsDMACommon::readData(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data,
		bool blockRead,
		std::uint32_t timeout) const {
	return std::static_pointer_cast<TerminalsDMACommonImpl>(m_impl)->readDataImpl(n, elementsToRead, data, blockRead, timeout);
}

}
