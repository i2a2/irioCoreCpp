#include <terminals/terminalsAnalog.h>
#include <terminals/impl/terminalsAnalogImpl.h>
#include <memory>

namespace iriov2{

TerminalsAnalog::TerminalsAnalog(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform):
				TerminalsBase(std::make_shared<TerminalsAnalogImpl>(parsedBitfile, session, platform)){
}

std::int32_t TerminalsAnalog::getAI(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAI(n);
}

std::int32_t TerminalsAnalog::getAO(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAO(n);
}

std::int32_t TerminalsAnalog::getAOEnable(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAOEnable(n);
}

size_t TerminalsAnalog::getNumAI() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getNumAI();
}

size_t TerminalsAnalog::getNumAO() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getNumAO();
}

void TerminalsAnalog::setAO(const std::uint32_t n, const std::int32_t value) const {
	std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->setAO(n, value);
}

void TerminalsAnalog::setAOEnable(const std::uint32_t n, const bool value) const {
	std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->setAOEnable(n, value);
}

ModulesType TerminalsAnalog::getModuleConnected() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getModuleConnected();
}

double TerminalsAnalog::getCVADC() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getCVADC();
}

double TerminalsAnalog::getCVDAC() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getCVDAC();
}

double TerminalsAnalog::getMaxValAO() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getMaxValAO();
}

double TerminalsAnalog::getMinValAO() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getMinValAO();
}

CouplingMode TerminalsAnalog::getAICouplingMode() const {
	return std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->getAICouplingMode();
}

void TerminalsAnalog::setAICouplingMode(const CouplingMode &mode) const {
	std::static_pointer_cast<TerminalsAnalogImpl>(m_impl)->setAICouplingMode(mode);
}

}
