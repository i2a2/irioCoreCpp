#pragma once

#include <NiFpga.h>
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>
#include <memory>

namespace iriov2{
class TerminalsAnalog
{
public:
	TerminalsAnalog(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, Platform& platform);

	std::int32_t getAI(const std::uint8_t n) const;
	std::int32_t getAO(const std::uint8_t n) const;
	std::int32_t getAOEnable(const std::uint8_t n) const;

	void setAO(const std::uint8_t n, const std::int32_t value) const;
	void setAOEnable(const std::uint8_t n, const bool value) const;

private:
	NiFpga_Session m_session;
	std::unique_ptr<Platform> m_platform;

	std::unordered_map<std::uint8_t, const std::uint32_t> m_mapAI;
	std::unordered_map<std::uint8_t, const std::uint32_t> m_mapAO;
	std::unordered_map<std::uint8_t, const std::uint32_t> m_mapAOEnable;
};
}
