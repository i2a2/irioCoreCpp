#include <profiles/profileCPUDAQFlexRIO.h>

namespace iriov2 {

ProfileCPUDAQFlexRIO::ProfileCPUDAQFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform,
		const ProfileBase::PROFILE_ID &id) :
				ProfileBase(parsedBitfile, session, id),
				ProfileCPUDAQ(parsedBitfile, session, platform, id),
				ProfileFlexRIO(parsedBitfile, session, id){
}

}
