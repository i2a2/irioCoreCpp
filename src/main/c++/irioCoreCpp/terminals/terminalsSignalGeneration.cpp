#include <memory>

#include "terminals/impl/terminalsSignalGenerationImpl.h"
#include "terminals/terminalsSignalGeneration.h"

namespace irio {

TerminalsSignalGeneration::TerminalsSignalGeneration(
		ParserManager *parserManager, const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBase(std::make_shared<TerminalsSignalGenerationImpl>
						(parserManager, session, platform)) {
}

std::uint8_t TerminalsSignalGeneration::getSGNo() const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGNoImpl();
}

std::uint32_t TerminalsSignalGeneration::getSGFref(
		const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGFrefImpl(n);
}

std::vector<std::uint32_t> TerminalsSignalGeneration::getVectorSGFrefs() const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
		->getVectorSGFrefsImpl();
}

std::uint8_t TerminalsSignalGeneration::getSGSignalType(
		const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGSignalTypeImpl(n);
}

std::uint32_t TerminalsSignalGeneration::getSGAmp(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGAmpImpl(n);
}

std::uint32_t TerminalsSignalGeneration::getSGFreq(
		const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGFreqImpl(n);
}

std::uint32_t TerminalsSignalGeneration::getSGPhase(
		const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGPhaseImpl(n);
}

std::uint32_t TerminalsSignalGeneration::getSGUpdateRate(
		const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->getSGUpdateRateImpl(n);
}

void TerminalsSignalGeneration::setSGSignalType(const std::uint32_t n,
		const std::uint8_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->setSGSignalTypeImpl(n, value);
}

void TerminalsSignalGeneration::setSGAmp(const std::uint32_t n,
		const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->setSGAmpImpl(n, value);
}

void TerminalsSignalGeneration::setSGFreqDecimation(const std::uint32_t n,
		const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->setSGFreqDecimationImpl(n, value);
}

void TerminalsSignalGeneration::setSGPhase(const std::uint32_t n,
		const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->setSGPhaseImpl(n, value);
}

void TerminalsSignalGeneration::setSGUpdateRateDecimation(const std::uint32_t n,
		const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)
			->setSGUpdateRateDecimationImpl(n, value);
}

}  // namespace irio
