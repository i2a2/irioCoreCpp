#include "profiles/profileCPUDAQRSeries.h"
#include "profilesTypes.h"

namespace irio {

ProfileCPUDAQRSeries::ProfileCPUDAQRSeries(ParserManager *parserManager,
                                           const NiFpga_Session &session,
                                           const Platform &platform)
    : ProfileCPUDAQ(parserManager, session, platform, PROFILE_ID::R_DAQ) {}

}  // namespace irio
