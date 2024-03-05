#include <terminals/terminalsDigital.h>
#include <terminals/impl/terminalsDigitalImpl.h>
#include <memory>

namespace iriov2 {

TerminalsDigital::TerminalsDigital(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session, const Platform &platform) :
		TerminalsBase(
				std::make_shared<TerminalsDigitalImpl>(
						parsedBitfile, session,	platform)) {
}

bool TerminalsDigital::getDI(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsDigitalImpl>(m_impl)->getDI(n);
}

bool TerminalsDigital::getDO(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsDigitalImpl>(m_impl)->getDO(n);
}

size_t TerminalsDigital::getNumDI() const {
	return std::static_pointer_cast<TerminalsDigitalImpl>(m_impl)->getNumDI();
}

size_t TerminalsDigital::getNumDO() const {
	return std::static_pointer_cast<TerminalsDigitalImpl>(m_impl)->getNumDO();
}

void TerminalsDigital::setDO(const std::uint32_t n, const bool value) const {
	std::static_pointer_cast<TerminalsDigitalImpl>(m_impl)->setDO(n, value);
}
}  // namespace iriov2
