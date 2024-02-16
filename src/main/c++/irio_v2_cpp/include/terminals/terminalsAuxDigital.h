#pragma once

#include <terminals/terminalsBase.h>
#include <bfp.h>
#include <platforms.h>

namespace iriov2 {

class TerminalsAuxDigital: public TerminalsBase {
public:
	TerminalsAuxDigital(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	bool getAuxDI(const std::uint32_t n) const;
	bool getAuxD0(const std::uint32_t n) const;

	void setAuxDO(const std::uint32_t n, const bool value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxDI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxDO;

};

}
