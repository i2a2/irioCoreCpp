#include <terminals/terminalsAuxAnalog.h>
#include <terminals/names/namesTerminalsAuxAnalog.h>
#include <utils.h>

namespace iriov2 {

TerminalsAuxAnalog::TerminalsAuxAnalog(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				m_impl(new TerminalsAuxAnalogImpl(parsedBitfile, session, platform)){
}

TerminalsAuxAnalog::TerminalsAuxAnalog(const TerminalsAuxAnalog &other){
	m_impl = other.m_impl;
}

std::int32_t TerminalsAuxAnalog::getAuxAI(const std::uint32_t n) const {
	return m_impl->getAuxAI(n);
}

std::int32_t TerminalsAuxAnalog::getAuxAO(const std::uint32_t n) const {
	return m_impl->getAuxAO(n);
}

size_t TerminalsAuxAnalog::getNumAuxAI() const {
	return m_impl->getNumAuxAI();
}

size_t TerminalsAuxAnalog::getNumAuxAO() const {
	return m_impl->getNumAuxAO();
}

std::int64_t TerminalsAuxAnalog::getAuxAI64(const std::uint32_t n) const {
	return m_impl->getAuxAI64(n);
}

std::int64_t TerminalsAuxAnalog::getAuxAO64(const std::uint32_t n) const {
	return m_impl->getAuxAO64(n);
}

size_t TerminalsAuxAnalog::getNumAuxAI64() const {
	return m_impl->getNumAuxAI64();
}

size_t TerminalsAuxAnalog::getNumAuxAO64() const {
	return m_impl->getNumAuxAO64();
}

void TerminalsAuxAnalog::setAuxAO(const std::uint32_t n, const std::int32_t value) const {
	return m_impl->setAuxAO(n, value);
}

void TerminalsAuxAnalog::setAuxAO64(const std::uint32_t n, const std::int64_t value) const {
	return m_impl->setAuxAO64(n, value);
}

}
