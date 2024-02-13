#include <profileDAQ.h>

namespace iriov2{

    ProfileDAQ::ProfileDAQ(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform& platform):
        Profile(parsedBitfile, session)
    {
        m_analog.reset(new TerminalsAnalog(parsedBitfile, session, platform));
    }

}