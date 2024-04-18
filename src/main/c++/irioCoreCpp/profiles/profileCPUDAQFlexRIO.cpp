#include <profiles/profileCPUDAQFlexRIO.h>

namespace irio {

ProfileCPUDAQFlexRIO::ProfileCPUDAQFlexRIO(ParserManager *parserManager,
										   const NiFpga_Session &session,
										   const Platform &platform)
	: ProfileBase(PROFILE_ID::FLEXRIO_CPUDAQ),
	  ProfileCPUDAQ(parserManager, session, platform,
					PROFILE_ID::FLEXRIO_CPUDAQ),
	  ProfileFlexRIO(parserManager, session, PROFILE_ID::FLEXRIO_CPUDAQ) {}
}  // namespace irio
