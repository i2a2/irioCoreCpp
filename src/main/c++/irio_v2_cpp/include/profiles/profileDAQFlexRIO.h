#pragma once

#include <profiles/profileDAQ.h>
#include <profiles/profileFlexRIO.h>

namespace iriov2 {

class ProfileDAQFlexRIO: public ProfileDAQ, ProfileFlexRIO {
public:
	ProfileDAQFlexRIO(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);
};

}

