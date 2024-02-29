#pragma once

#include <terminals/impl/terminalsBaseImpl.h>

namespace iriov2 {
/**
 * Class implementing the digital functionality
 *
 * @ingroup Terminals
 */
class TerminalsDigitalImpl: public TerminalsBaseImpl {
public:
	TerminalsDigitalImpl(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	bool getDI(const std::uint32_t n) const;

	bool getDO(const std::uint32_t n) const;

	size_t getNumDI() const;

	size_t getNumDO() const;

	void setDO(const std::uint32_t n, const bool value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDO;

};

}
