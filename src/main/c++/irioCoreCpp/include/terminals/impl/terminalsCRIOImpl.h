#pragma once

#include <vector>

#include "terminals/impl/terminalsBaseImpl.h"

namespace irio {
/**
 * Class implementing the functionality
 * specific to cRIO
 *
 * @ingroup Terminals
 * @ingroup cRIO
 */
class TerminalscRIOImpl: public TerminalsBaseImpl {
 public:
	TerminalscRIOImpl(ParserManager *parserManager,
			const NiFpga_Session &session);

	bool getcRIOModulesOk() const;

	std::vector<std::uint16_t> getInsertedIOModulesID() const;

 private:
	std::uint32_t m_criomodulesok_addr;
	std::uint32_t m_insertediomodulesid_addr;
	size_t m_numModules;
};
}  // namespace irio

