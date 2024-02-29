#include <terminals/terminalsDMACommon.h>
#include <errorsIrio.h>
#include <utils.h>
#include <memory>

namespace iriov2{

TerminalsDMACommon::TerminalsDMACommon(std::shared_ptr<TerminalsDMACommonImpl> impl) :
		m_impl(impl) {
}

std::uint16_t TerminalsDMACommon::getNCh(const std::uint32_t n) const {
	return m_impl->getNCh(n);
}

void TerminalsDMACommon::startDMA(const std::uint32_t n) const {
	m_impl->startDMA(n);
}

void TerminalsDMACommon::startAllDMAs() const {
	m_impl->startAllDMAs();
}

void TerminalsDMACommon::stopDMA(const std::uint32_t n) const {
	m_impl->stopDMA(n);
}

void TerminalsDMACommon::stopAllDMAs() const {
	m_impl->stopAllDMAs();
}

size_t TerminalsDMACommon::countDMAs() const {
	return m_impl->countDMAs();
}

void TerminalsDMACommon::cleanDMA(const std::uint32_t n) const {
	m_impl->cleanDMA(n);
}

void TerminalsDMACommon::cleanAllDMAs() const {
	m_impl->cleanAllDMAs();
}

bool TerminalsDMACommon::isDMAEnable(const std::uint32_t n) const {
	return m_impl->isDMAEnable(n);
}

void TerminalsDMACommon::enableDMA(const std::uint32_t n) const {
	m_impl->enableDMA(n);
}

void TerminalsDMACommon::disableDMA(const std::uint32_t n) const {
	m_impl->disableDMA(n);
}

void TerminalsDMACommon::enaDisDMA(const std::uint32_t n, bool enaDis) const {
	m_impl->enaDisDMA(n, enaDis);
}

bool TerminalsDMACommon::getDMAOverflow(const std::uint16_t n) const {
	return m_impl->getDMAOverflow(n);
}

std::uint16_t TerminalsDMACommon::getAllDMAOverflows() const {
	return m_impl->getAllDMAOverflows();
}

FrameType TerminalsDMACommon::getFrameType(const std::uint32_t n) const {
	return m_impl->getFrameType(n);
}

std::vector<FrameType> TerminalsDMACommon::getAllFrameType() const {
	return m_impl->getAllFrameType();
}

std::uint8_t TerminalsDMACommon::getSampleSize(const std::uint32_t n) const {
	return m_impl->getSampleSize(n);
}

std::vector<std::uint8_t> TerminalsDMACommon::getAllSampleSizes() const {
	return m_impl->getAllSampleSizes();
}

size_t TerminalsDMACommon::readDataNonBlocking(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data) {
	return m_impl->readDataNonBlocking(n, elementsToRead, data);
}

size_t TerminalsDMACommon::readDataBlocking(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data,
		std::uint32_t timeout) {
	return m_impl->readDataBlocking(n, elementsToRead, data, timeout);
}

size_t TerminalsDMACommon::readData(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data,
		bool blockRead,
		std::uint32_t timeout) const {
	return m_impl->readData(n, elementsToRead, data, blockRead, timeout);
}

}
