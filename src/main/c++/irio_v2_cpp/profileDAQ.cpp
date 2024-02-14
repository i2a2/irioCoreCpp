#include <profileDAQ.h>

namespace iriov2{

    ProfileDAQ::ProfileDAQ(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform& platform):
        Profile(parsedBitfile, session, Profile::PROFILE_ID_DAQ)
    {
        m_analog.reset(new TerminalsAnalog(parsedBitfile, session, platform));

        m_digital.reset(new TerminalsDigital());
    }

}
