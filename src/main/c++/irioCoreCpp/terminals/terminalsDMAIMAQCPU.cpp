#include "terminals/terminalsDMAIMAQCPU.h"
#include "terminals/names/namesTerminalsDMACPUCommon.h"
#include "terminals/names/namesTerminalsDMADAQCPU.h"
#include "terminals/impl/terminalsDMAIMAQImpl.h"

namespace irio {

TerminalsDMAIMAQCPU::TerminalsDMAIMAQCPU(ParserManager *parserManager,
										 const NiFpga_Session &session,
										 const Platform &platform)
	: TerminalsDMAIMAQ(std::make_shared<TerminalsDMAIMAQImpl>(
		  parserManager, session, platform, TERMINAL_DMATTOHOSTNCH,
		  TERMINAL_DMATTOHOSTFRAMETYPE, TERMINAL_DMATTOHOSTSAMPLESIZE,
		  TERMINAL_DMATTOHOSTOVERFLOWS, TERMINAL_DMATTOHOST,
		  TERMINAL_DMATTOHOSTENABLE)) {}
}  // namespace irio
