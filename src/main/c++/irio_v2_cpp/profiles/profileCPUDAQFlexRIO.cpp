#include <profiles/profileCPUDAQFlexRIO.h>

namespace iriov2 {

ProfileCPUDAQFlexRIO::ProfileCPUDAQFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
				ProfileBase(parsedBitfile, session, ProfileBase::FLEXRIO_CPUDAQ),
				ProfileCPUDAQ(parsedBitfile, session, platform, ProfileBase::FLEXRIO_CPUDAQ),
				ProfileFlexRIO(parsedBitfile, session, ProfileBase::FLEXRIO_CPUDAQ){
}

}
