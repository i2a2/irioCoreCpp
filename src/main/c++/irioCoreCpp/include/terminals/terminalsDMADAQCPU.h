#pragma once

#include "terminals/terminalsDMADAQ.h"

namespace irio {

/**
 * Class managing the terminals used for DMA CPU DAQ operations
 *
 * @ingroup DMACPUTerminals
 */
class TerminalsDMADAQCPU: public TerminalsDMADAQ{
 public:
	/**
	 * Constructor.
	 * Manages finding all the required
	 * DMA CPU DAQ resources.
	 *
	 * @throw irio::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param parserManager     Pointer to class managing parsing the bitfile
	 *                          and finding its resources
	 * @param session			NiFpga_Session to be used in NiFpga
	 * 							related functions
	 * @param platform			Platform that is using the terminals.
	 * 							Used to know the maximum number of terminals
	 * 							that can be found
	 */
	TerminalsDMADAQCPU(ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);
};

}  // namespace irio


