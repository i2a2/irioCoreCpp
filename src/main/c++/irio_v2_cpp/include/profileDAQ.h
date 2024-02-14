#pragma once

#include <profiles.h>
#include <platforms.h>

namespace iriov2{

class ProfileDAQ: public Profile
{
public:
    ProfileDAQ(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform &platform, const Profile::PROFILE_ID& id);
};

}
