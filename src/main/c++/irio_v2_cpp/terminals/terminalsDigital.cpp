#include <terminals/terminalsDigital.h>

namespace iriov2 {

TerminalsDigital::TerminalsDigital(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
			m_impl(new TerminalsDigitalImpl(parsedBitfile, session, platform)){
}

bool TerminalsDigital::getDI(const std::uint32_t n) const {
	return m_impl->getDI(n);
}

bool TerminalsDigital::getDO(const std::uint32_t n) const {
	return m_impl->getDO(n);
}

size_t TerminalsDigital::getNumDI() const {
	return m_impl->getNumDI();
}

size_t TerminalsDigital::getNumDO() const {
	return m_impl->getNumDO();
}

void TerminalsDigital::setDO(const std::uint32_t n, const bool value) const {
	m_impl->setDO(n, value);
}
}
