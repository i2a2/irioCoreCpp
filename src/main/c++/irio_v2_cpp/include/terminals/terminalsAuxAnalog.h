#pragma once

#include "terminals/terminalsBase.h"
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>

namespace iriov2 {

class TerminalsAuxAnalog: public TerminalsBase {
public:
	TerminalsAuxAnalog(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	std::int32_t getAuxAI(const std::uint32_t n) const;
	std::int16_t getAuxAO(const std::uint32_t n) const;

	size_t getNumAuxAI() const;
	size_t getNumAuxAO() const;

	void setAuxAO(const std::uint32_t n, const std::int16_t value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAO;
};

}
