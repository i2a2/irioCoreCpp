#include "profiles/profileIOcRIO.h"

namespace irio {
ProfileIOcRIO::ProfileIOcRIO(ParserManager *parserManager,
							 const NiFpga_Session &session,
							 const Platform &platform)
	: ProfileIO(parserManager, session, platform, PROFILE_ID::CRIO_IO) {
	addTerminal(TerminalscRIO(parserManager, session));
}
}  // namespace irio
