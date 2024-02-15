#pragma once

#include <profileBase.h>
#include <platforms.h>

namespace iriov2{

class ProfileDAQ: public ProfileBase
{
public:
    ProfileDAQ(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform &platform, const ProfileBase::PROFILE_ID& id);
};

}
