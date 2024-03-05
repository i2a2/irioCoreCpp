#include <terminals/terminalsAnalog.h>
#include <terminals/impl/terminalsAnalogImpl.h>
#include <memory>

namespace iriov2 {

TerminalsAnalog::TerminalsAnalog(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session, const Platform &platform) :
		TerminalsBase(
				std::make_shared<TerminalsAnalogImpl>(parsedBitfile, session,
						platform)) {
}

std::int32_t TerminalsAnalog::getAI(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAIImpl(n);
}

std::int32_t TerminalsAnalog::getAO(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAOImpl(n);
}

std::int32_t TerminalsAnalog::getAOEnable(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAOEnableImpl(
			n);
}

size_t TerminalsAnalog::getNumAI() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getNumAIImpl();
}

size_t TerminalsAnalog::getNumAO() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getNumAOImpl();
}

void TerminalsAnalog::setAO(const std::uint32_t n,
		const std::int32_t value) const {
	std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->setAOImpl(n, value);
}

void TerminalsAnalog::setAOEnable(const std::uint32_t n,
		const bool value) const {
	std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->setAOEnableImpl(n,
			value);
}

ModulesType TerminalsAnalog::getModuleConnected() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)
			->getModuleConnectedImpl();
}

double TerminalsAnalog::getCVADC() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getCVADCImpl();
}

double TerminalsAnalog::getCVDAC() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getCVDACImpl();
}

double TerminalsAnalog::getMaxValAO() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)
			->getMaxValAOImpl();
}

double TerminalsAnalog::getMinValAO() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)
			->getMinValAOImpl();
}

CouplingMode TerminalsAnalog::getAICouplingMode() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)
			->getAICouplingModeImpl();
}

void TerminalsAnalog::setAICouplingMode(const CouplingMode &mode) const {
	std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->setAICouplingModeImpl(
			mode);
}

}  // namespace iriov2
