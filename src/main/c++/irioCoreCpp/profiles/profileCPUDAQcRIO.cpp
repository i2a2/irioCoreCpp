#include <profiles/profileCPUDAQcRIO.h>

namespace irio {

ProfileCPUDAQcRIO::ProfileCPUDAQcRIO(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const Platform &platform) :
				ProfileBase(parserManager, session, PROFILE_ID::FLEXRIO_CPUDAQ),
				ProfileCPUDAQ(parserManager, session,
						platform, PROFILE_ID::FLEXRIO_CPUDAQ),
				ProfileCRIO(parserManager, session,
						PROFILE_ID::FLEXRIO_CPUDAQ) {
}
}  // namespace irio
