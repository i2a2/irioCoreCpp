#include <profiles/profileCPUDAQFlexRIO.h>
#include <terminals/terminalsFlexRIO.h>


namespace irio {

ProfileCPUDAQFlexRIO::ProfileCPUDAQFlexRIO(ParserManager *parserManager,
										   const NiFpga_Session &session,
										   const Platform &platform)
	: ProfileCPUDAQ(parserManager, session, platform,
					PROFILE_ID::FLEXRIO_CPUDAQ) {
	addTerminal(TerminalsFlexRIO(parserManager, session));
}
}  // namespace irio
