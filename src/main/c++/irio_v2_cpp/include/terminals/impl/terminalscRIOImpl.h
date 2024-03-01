#pragma once

#include "terminals/impl/terminalsBaseImpl.h"

namespace iriov2 {
/**
 * Class implementing the functionality
 * specific to cRIO
 *
 * @ingroup Terminals
 * @ingroup cRIO
 */
class TerminalscRIOImpl: public TerminalsBaseImpl {
public:
	TerminalscRIOImpl(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	bool getcRIOModulesOk() const;

	std::vector<std::uint16_t> getInsertedIOModulesID() const;

private:
	std::uint32_t m_criomodulesok_addr;
	std::uint32_t m_insertediomodulesid_addr;
	size_t m_numModules;
};

}

