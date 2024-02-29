#include "terminals/terminalsSignalGeneration.h"

namespace iriov2 {

TerminalsSignalGeneration::TerminalsSignalGeneration(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		m_impl(new TerminalsSignalGenerationImpl(parsedBitfile, session, platform)) {
}

TerminalsSignalGeneration::TerminalsSignalGeneration(const TerminalsSignalGeneration &other){
	m_impl = other.m_impl;
}

std::uint8_t TerminalsSignalGeneration::getSGNo() const {
	return m_impl->getSGNo();
}

std::uint32_t TerminalsSignalGeneration::getSGFref(const std::uint32_t n) const {
	return m_impl->getSGFref(n);
}

std::uint8_t TerminalsSignalGeneration::getSGSignalType(const std::uint32_t n) const {
	return m_impl->getSGSignalType(n);
}

std::uint32_t TerminalsSignalGeneration::getSGAmp(const std::uint32_t n) const {
	return m_impl->getSGAmp(n);
}

std::uint32_t TerminalsSignalGeneration::getSGFreq(const std::uint32_t n) const {
	return m_impl->getSGFreq(n);
}

std::uint32_t TerminalsSignalGeneration::getSGPhase(const std::uint32_t n) const {
	return m_impl->getSGPhase(n);
}

std::uint32_t TerminalsSignalGeneration::getSGUpdateRate(const std::uint32_t n) const {
	return m_impl->getSGUpdateRate(n);
}

void TerminalsSignalGeneration::setSGSignalType(
		const std::uint32_t n,
		const std::uint8_t value) const {
	m_impl->setSGSignalType(n, value);
}

void TerminalsSignalGeneration::setSGAmp(const std::uint32_t n, const std::uint32_t value) const {
	m_impl->setSGAmp(n, value);
}

void TerminalsSignalGeneration::setSGFreq(const std::uint32_t n, const std::uint32_t value) const {
	m_impl->setSGFreq(n, value);
}

void TerminalsSignalGeneration::setSGPhase(const std::uint32_t n, const std::uint32_t value) const {
	m_impl->setSGPhase(n, value);
}

void TerminalsSignalGeneration::setSGUpdateRate(
		const std::uint32_t n,
		const std::uint32_t value) const {
	m_impl->setSGUpdateRate(n, value);
}

}
