#include <terminals/terminalsAuxAnalog.h>
#include <terminals/impl/terminalsAuxAnalogImpl.h>
#include <memory>

namespace irio {

TerminalsAuxAnalog::TerminalsAuxAnalog(ParserManager *parserManager,
		const NiFpga_Session &session, const Platform &platform) :
		TerminalsBase(std::make_shared<TerminalsAuxAnalogImpl>
						(parserManager, session, platform)) {
}

std::int32_t TerminalsAuxAnalog::getAuxAI(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getAuxAIImpl(n);
}

std::int32_t TerminalsAuxAnalog::getAuxAO(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getAuxAOImpl(n);
}

size_t TerminalsAuxAnalog::getNumAuxAI() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getNumAuxAIImpl();
}

size_t TerminalsAuxAnalog::getNumAuxAO() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getNumAuxAOImpl();
}

std::int64_t TerminalsAuxAnalog::getAuxAI64(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getAuxAI64Impl(n);
}

std::int64_t TerminalsAuxAnalog::getAuxAO64(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getAuxAO64Impl(n);
}

size_t TerminalsAuxAnalog::getNumAuxAI64() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getNumAuxAI64Impl();
}

size_t TerminalsAuxAnalog::getNumAuxAO64() const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->getNumAuxAO64Impl();
}

void TerminalsAuxAnalog::setAuxAO(const std::uint32_t n,
		const std::int32_t value) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->setAuxAOImpl(n, value);
}

void TerminalsAuxAnalog::setAuxAO64(const std::uint32_t n,
		const std::int64_t value) const {
	return std::static_pointer_cast<TerminalsAuxAnalogImpl>(m_impl)
			->setAuxAO64Impl(n, value);
}

}  // namespace iriov2
