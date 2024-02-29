#include <terminals/terminalsAnalog.h>

namespace iriov2{

TerminalsAnalog::TerminalsAnalog(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform):
			m_impl(new TerminalsAnalogImpl(parsedBitfile, session, platform)){
}

TerminalsAnalog::TerminalsAnalog(const TerminalsAnalog &other) {
	m_impl = other.m_impl;
}

std::int32_t TerminalsAnalog::getAI(const std::uint32_t n) const {
	return m_impl->getAI(n);
}

std::int32_t TerminalsAnalog::getAO(const std::uint32_t n) const {
	return m_impl->getAO(n);
}

std::int32_t TerminalsAnalog::getAOEnable(const std::uint32_t n) const {
	return m_impl->getAOEnable(n);
}

size_t TerminalsAnalog::getNumAI() const {
	return m_impl->getNumAI();
}

size_t TerminalsAnalog::getNumAO() const {
	return m_impl->getNumAO();
}

void TerminalsAnalog::setAO(const std::uint32_t n, const std::int32_t value) const {
	m_impl->setAO(n, value);
}

void TerminalsAnalog::setAOEnable(const std::uint32_t n, const bool value) const {
	m_impl->setAOEnable(n, value);
}

ModulesType TerminalsAnalog::getModuleConnected() const {
	return m_impl->getModuleConnected();
}

double TerminalsAnalog::getCVADC() const {
	return m_impl->getCVADC();
}

double TerminalsAnalog::getCVDAC() const {
	return m_impl->getCVDAC();
}

double TerminalsAnalog::getMaxValAO() const {
	return m_impl->getMaxValAO();
}

double TerminalsAnalog::getMinValAO() const {
	return m_impl->getMinValAO();
}

CouplingMode TerminalsAnalog::getAICouplingMode() const {
	return m_impl->getAICouplingMode();
}

void TerminalsAnalog::setAICouplingMode(const CouplingMode &mode) const {
	m_impl->setAICouplingMode(mode);
}

}
