#include "profiles/profileCPUDAQRSeries.h"
#include "profilesTypes.h"

namespace irio {

ProfileCPUDAQRSeries::ProfileCPUDAQRSeries(ParserManager *parserManager,
                                           const NiFpga_Session &session,
                                           const Platform &platform)
    : ProfileBase(parserManager, session, PROFILE_ID::R_DAQ),
      ProfileCPUDAQ(parserManager, session, platform, PROFILE_ID::R_DAQ) {}

}  // namespace irio
