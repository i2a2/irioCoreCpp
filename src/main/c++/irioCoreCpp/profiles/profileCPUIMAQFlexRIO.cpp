#include "profiles/profileCPUIMAQFlexRIO.h"

namespace irio {

ProfileCPUIMAQFlexRIO::ProfileCPUIMAQFlexRIO(ParserManager *parserManager,
											 const NiFpga_Session &session,
											 const Platform &platform)
	: ProfileCPUIMAQ(parserManager, session, platform,
					 PROFILE_ID::FLEXRIO_CPUIMAQ) {
	addTerminal(TerminalsFlexRIO(parserManager, session));
}
}  // namespace irio
