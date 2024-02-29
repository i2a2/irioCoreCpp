#include <terminals/terminalsAuxDigital.h>

namespace iriov2 {

TerminalsAuxDigital::TerminalsAuxDigital(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				m_impl(new TerminalsAuxDigitalImpl(parsedBitfile, session, platform)) {
}

TerminalsAuxDigital::TerminalsAuxDigital(const TerminalsAuxDigital &other){
	m_impl = other.m_impl;
}

bool TerminalsAuxDigital::getAuxDI(const std::uint32_t n) const {
	return m_impl->getAuxDI(n);
}

bool TerminalsAuxDigital::getAuxDO(const std::uint32_t n) const {
	return m_impl->getAuxDO(n);
}

size_t TerminalsAuxDigital::getNumAuxDI() const {
	return m_impl->getNumAuxDI();
}

size_t TerminalsAuxDigital::getNumAuxDO() const {
	return m_impl->getNumAuxDO();
}

void TerminalsAuxDigital::setAuxDO(const std::uint32_t n, const bool value) const {
	return m_impl->setAuxDO(n, value);
}
}
