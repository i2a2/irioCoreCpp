#include "profiles/profileCPUIMAQFlexRIO.h"

namespace irio {

ProfileCPUIMAQFlexRIO::ProfileCPUIMAQFlexRIO(ParserManager *parserManager,
										   const NiFpga_Session &session,
										   const Platform &platform)
	: ProfileBase(parserManager, session, PROFILE_ID::FLEXRIO_CPUIMAQ),
	  ProfileCPUIMAQ(parserManager, session, platform,
					PROFILE_ID::FLEXRIO_CPUIMAQ),
	  ProfileFlexRIO(parserManager, session, PROFILE_ID::FLEXRIO_CPUIMAQ) {}
}  // namespace irio
