#include "terminals/terminalsSignalGeneration.h"
#include "terminals/impl/terminalsSignalGenerationImpl.h"
#include <memory>

namespace iriov2 {

TerminalsSignalGeneration::TerminalsSignalGeneration(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				TerminalsBase(std::make_shared<TerminalsSignalGenerationImpl>(parsedBitfile, session, platform))  {
}

std::uint8_t TerminalsSignalGeneration::getSGNo() const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGNo();
}

std::uint32_t TerminalsSignalGeneration::getSGFref(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGFref(n);
}

std::uint8_t TerminalsSignalGeneration::getSGSignalType(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGSignalType(n);
}

std::uint32_t TerminalsSignalGeneration::getSGAmp(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGAmp(n);
}

std::uint32_t TerminalsSignalGeneration::getSGFreq(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGFreq(n);
}

std::uint32_t TerminalsSignalGeneration::getSGPhase(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGPhase(n);
}

std::uint32_t TerminalsSignalGeneration::getSGUpdateRate(const std::uint32_t n) const {
	return std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->getSGUpdateRate(n);
}

void TerminalsSignalGeneration::setSGSignalType(
		const std::uint32_t n,
		const std::uint8_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->setSGSignalType(n, value);
}

void TerminalsSignalGeneration::setSGAmp(const std::uint32_t n, const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->setSGAmp(n, value);
}

void TerminalsSignalGeneration::setSGFreq(const std::uint32_t n, const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->setSGFreq(n, value);
}

void TerminalsSignalGeneration::setSGPhase(const std::uint32_t n, const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->setSGPhase(n, value);
}

void TerminalsSignalGeneration::setSGUpdateRate(
		const std::uint32_t n,
		const std::uint32_t value) const {
	std::static_pointer_cast<TerminalsSignalGenerationImpl>(m_impl)->setSGUpdateRate(n, value);
}

}
