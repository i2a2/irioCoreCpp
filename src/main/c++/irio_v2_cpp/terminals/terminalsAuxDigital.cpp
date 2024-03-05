#include <terminals/terminalsAuxDigital.h>
#include <terminals/impl/terminalsAuxDigitalImpl.h>
#include <memory>

namespace iriov2 {

TerminalsAuxDigital::TerminalsAuxDigital(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session, const Platform &platform) :
		TerminalsBase(
				std::make_shared<TerminalsAuxDigitalImpl>(parsedBitfile,
						session, platform)) {
}

bool TerminalsAuxDigital::getAuxDI(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxDigitalImpl>(m_impl)
			->getAuxDI(n);
}

bool TerminalsAuxDigital::getAuxDO(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxDigitalImpl>(m_impl)
			->getAuxDO(n);
}

size_t TerminalsAuxDigital::getNumAuxDI() const {
	return std::static_pointer_cast<TerminalsAuxDigitalImpl>(m_impl)
			->getNumAuxDI();
}

size_t TerminalsAuxDigital::getNumAuxDO() const {
	return std::static_pointer_cast<TerminalsAuxDigitalImpl>(m_impl)
			->getNumAuxDO();
}

void TerminalsAuxDigital::setAuxDO(const std::uint32_t n,
		const bool value) const {
	return std::static_pointer_cast<TerminalsAuxDigitalImpl>(m_impl)
			->setAuxDO(n, value);
}
}  // namespace iriov2
