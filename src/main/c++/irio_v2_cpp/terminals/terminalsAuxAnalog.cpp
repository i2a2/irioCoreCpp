#include <terminals/terminalsAuxAnalog.h>
#include <terminals/impl/terminalsAuxAnalogImpl.h>
#include <memory>

namespace iriov2 {

TerminalsAuxAnalog::TerminalsAuxAnalog(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBase(std::make_shared<TerminalsAuxAnalogImpl>(parsedBitfile, session, platform)) {
}

std::int32_t TerminalsAuxAnalog::getAuxAI(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getAuxAI(n);
}

std::int32_t TerminalsAuxAnalog::getAuxAO(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getAuxAO(n);
}

size_t TerminalsAuxAnalog::getNumAuxAI() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getNumAuxAI();
}

size_t TerminalsAuxAnalog::getNumAuxAO() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getNumAuxAO();
}

std::int64_t TerminalsAuxAnalog::getAuxAI64(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getAuxAI64(n);
}

std::int64_t TerminalsAuxAnalog::getAuxAO64(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getAuxAO64(n);
}

size_t TerminalsAuxAnalog::getNumAuxAI64() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getNumAuxAI64();
}

size_t TerminalsAuxAnalog::getNumAuxAO64() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->getNumAuxAO64();
}

void TerminalsAuxAnalog::setAuxAO(const std::uint32_t n, const std::int32_t value) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->setAuxAO(n, value);
}

void TerminalsAuxAnalog::setAuxAO64(const std::uint32_t n, const std::int64_t value) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)->setAuxAO64(n, value);
}

}
