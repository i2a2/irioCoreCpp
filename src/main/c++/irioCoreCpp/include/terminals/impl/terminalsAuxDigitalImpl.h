#pragma once

#include <unordered_map>

#include "terminals/impl/terminalsBaseImpl.h"

namespace irio {
/**
 * Class implementing the auxiliary digital
 * functionality
 *
 * @ingroup Terminals
 */
class TerminalsAuxDigitalImpl: public TerminalsBaseImpl {
 public:
	TerminalsAuxDigitalImpl(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);

	bool getAuxDI(const std::uint32_t n) const;

	bool getAuxDO(const std::uint32_t n) const;

	size_t getNumAuxDI() const;

	size_t getNumAuxDO() const;

	void setAuxDO(const std::uint32_t n, const bool value) const;

 private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxDI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxDO;
};

}  // namespace irio
