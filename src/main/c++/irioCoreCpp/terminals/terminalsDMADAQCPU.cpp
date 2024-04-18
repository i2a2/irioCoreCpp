#include "terminals/terminalsDMADAQCPU.h"
#include "terminals/names/namesTerminalsDMACPUCommon.h"
#include "terminals/names/namesTerminalsDMADAQCPU.h"
#include "terminals/impl/terminalsDMADAQImpl.h"

namespace irio {

TerminalsDMADAQCPU::TerminalsDMADAQCPU(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsDMADAQ(
				std::make_shared<TerminalsDMADAQImpl>(parserManager, session,
						platform, TERMINAL_DMATTOHOSTBLOCKNWORDS,
						TERMINAL_DMATTOHOSTSAMPLINGRATE, TERMINAL_DMATTOHOSTNCH,
						TERMINAL_DMATTOHOSTFRAMETYPE,
						TERMINAL_DMATTOHOSTSAMPLESIZE,
						TERMINAL_DMATTOHOSTOVERFLOWS, TERMINAL_DMATTOHOST,
						TERMINAL_DMATTOHOSTENABLE)) {
}
}  // namespace irio
