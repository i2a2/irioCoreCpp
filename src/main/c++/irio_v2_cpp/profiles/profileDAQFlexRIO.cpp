#include <profiles/profileDAQFlexRIO.h>

namespace iriov2 {

ProfileDAQFlexRIO::ProfileDAQFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform,
		const ProfileBase::PROFILE_ID &id) :
				ProfileBase(parsedBitfile, session, id),
				ProfileDAQ(parsedBitfile, session, platform, id),
				ProfileFlexRIO(parsedBitfile, session, id){
}

}
