#pragma once

#include "terminals/terminalsBase.h"
#include "bfp.h"
#include "platforms.h"
#include <NiFpga.h>

namespace iriov2{

class TerminalsSignalGeneration: public TerminalsBase
{
public:
	TerminalsSignalGeneration(const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform& platform);

	std::uint8_t getSGNo() const;
	std::uint32_t getSGFref(const std::uint32_t n) const;
	std::uint8_t getSGSignalType(const std::uint32_t n) const;
	std::uint32_t getSGAmp(const std::uint32_t n) const;
	std::uint32_t getSGFreq(const std::uint32_t n) const;
	std::uint32_t getSGPhase(const std::uint32_t n) const;
	std::uint32_t getSGUpdateRate(const std::uint32_t n) const;

	void setSGSignalType(const std::uint32_t n, const std::uint8_t value) const;
	void setSGAmp(const std::uint32_t n, const std::uint32_t value) const;
	void setSGFreq(const std::uint32_t n, const std::uint32_t value) const;
	void setSGPhase(const std::uint32_t n, const std::uint32_t value) const;
	void setSGUpdateRate(const std::uint32_t n, const std::uint32_t value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapSignalType_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAmp_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapFreq_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapPhase_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapUpdateRate_addr;

	std::uint8_t m_numSG = 0;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapFref;


};

}


