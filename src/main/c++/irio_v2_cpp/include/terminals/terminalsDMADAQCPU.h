#pragma once

#include "terminals/terminalsDMADAQ.h"

namespace iriov2 {

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
	 * @throw std::runtime_error	Size mismatch between found resources
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile			Parsed bitfile with the terminals
	 * @param session				NiFpga_Session to be used in NiFpga
	 * 								related functions
	 * @param platform				Platform that is using the terminals.
	 * 								Used to know the maximum number of terminals
	 * 								that can be found
	 */
	TerminalsDMADAQCPU(ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);
};

}  // namespace iriov2


