#pragma once

#include "terminals/terminalsBase.h"
#include <bfp.h>
#include <NiFpga.h>

namespace iriov2 {

class TerminalsFlexRIO: public TerminalsBase {
public:
	TerminalsFlexRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	bool getRIOAdapterCorrect() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return
	 */
	std::uint32_t getInsertedIOModuleID() const;

private:
	std::uint32_t m_rioadaptercorrect_addr;
	std::uint32_t m_insertediomoduleid_addr;

};

}
