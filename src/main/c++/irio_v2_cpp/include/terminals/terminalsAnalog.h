#pragma once

#include <terminals/terminalsBase.h>
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>

namespace iriov2{
class TerminalsAnalog: public TerminalsBase
{
public:
	TerminalsAnalog() = default;
	TerminalsAnalog(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform& platform);

	std::int32_t getAI(const std::uint32_t n) const;
	std::int32_t getAO(const std::uint32_t n) const;
	std::int32_t getAOEnable(const std::uint32_t n) const;

	void setAO(const std::uint32_t n, const std::int32_t value) const;
	void setAOEnable(const std::uint32_t n, const bool value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAO;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAOEnable;
};
}
