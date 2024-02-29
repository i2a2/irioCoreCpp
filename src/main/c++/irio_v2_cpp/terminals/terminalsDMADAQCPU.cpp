#include <terminals/terminalsDMADAQCPU.h>
#include <terminals/names/namesTerminalsDMACPUCommon.h>
#include <terminals/names/namesTerminalsDMADAQCPU.h>

namespace iriov2{

TerminalsDMADAQCPU::TerminalsDMADAQCPU(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				TerminalsDMADAQ(
						std::shared_ptr<TerminalsDMADAQImpl>(
								new TerminalsDMADAQImpl(parsedBitfile, session, platform,
								TERMINAL_DMATTOHOSTBLOCKNWORDS,
								TERMINAL_DMATTOHOSTSAMPLINGRATE, TERMINAL_DMATTOHOSTNCH,
								TERMINAL_DMATTOHOSTFRAMETYPE,
								TERMINAL_DMATTOHOSTSAMPLESIZE, TERMINAL_DMATTOHOSTOVERFLOWS,
								TERMINAL_DMATTOHOST, TERMINAL_DMATTOHOSTENABLE))) {
}
}
