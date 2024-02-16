#pragma once

#include "terminals/terminalsBase.h"
#include <bfp.h>
#include <NiFpga.h>

namespace iriov2 {

class TerminalscRIO: public TerminalsBase {
public:
	TerminalscRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	bool getcRIOModulesOk() const;
	std::vector<std::uint16_t> getInsertedIOModulesID() const;

private:
	std::uint32_t m_criomodulesok_addr;
	std::uint32_t m_insertediomodulesid_addr;
	size_t m_numModules;

};

}

