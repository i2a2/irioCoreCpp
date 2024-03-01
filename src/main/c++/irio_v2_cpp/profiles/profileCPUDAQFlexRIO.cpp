#include <profiles/profileCPUDAQFlexRIO.h>

namespace iriov2 {

ProfileCPUDAQFlexRIO::ProfileCPUDAQFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				ProfileBase(ProfileBase::PROFILE_ID::FLEXRIO_CPUDAQ),
				ProfileCPUDAQ(parsedBitfile, session, platform, ProfileBase::PROFILE_ID::FLEXRIO_CPUDAQ),
				ProfileFlexRIO(parsedBitfile, session, ProfileBase::PROFILE_ID::FLEXRIO_CPUDAQ){
}

}
