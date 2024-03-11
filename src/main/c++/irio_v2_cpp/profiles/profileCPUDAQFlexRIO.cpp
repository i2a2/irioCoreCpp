#include <profiles/profileCPUDAQFlexRIO.h>

namespace iriov2 {

ProfileCPUDAQFlexRIO::ProfileCPUDAQFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				ProfileBase(PROFILE_ID::FLEXRIO_CPUDAQ),
				ProfileCPUDAQ(parsedBitfile, session,
						platform, PROFILE_ID::FLEXRIO_CPUDAQ),
				ProfileFlexRIO(parsedBitfile, session,
						PROFILE_ID::FLEXRIO_CPUDAQ) {
}
}  // namespace iriov2
