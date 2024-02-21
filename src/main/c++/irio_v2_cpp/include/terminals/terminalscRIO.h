#pragma once

#include "terminals/terminalsBase.h"
#include <bfp.h>
#include <NiFpga.h>

namespace iriov2 {

class TerminalscRIO: public TerminalsBase {
public:
	TerminalscRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	bool getcRIOModulesOk() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	std::vector<std::uint16_t> getInsertedIOModulesID() const;

private:
	std::uint32_t m_criomodulesok_addr;
	std::uint32_t m_insertediomodulesid_addr;
	size_t m_numModules;

};

}

