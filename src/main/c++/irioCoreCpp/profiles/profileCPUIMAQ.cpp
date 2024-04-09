#include "profiles/profileCPUIMAQ.h"
#include "terminals/terminalsDMAIMAQCPU.h"

namespace irio {

ProfileCPUIMAQ::ProfileCPUIMAQ(ParserManager *parserManager,
							   const NiFpga_Session &session,
							   const Platform &platform)
	: ProfileBase(PROFILE_ID::FLEXRIO_CPUIMAQ) {
    addTerminal(TerminalsDigital(parserManager, session, platform));
    addTerminal(TerminalsAuxDigital(parserManager, session, platform));
    addTerminal(TerminalsAuxAnalog(parserManager, session, platform));
    addTerminal(TerminalsDMAIMAQCPU(parserManager, session, platform));
}

}  // namespace irio
