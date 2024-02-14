#include <profileDAQ.h>

namespace iriov2{

    ProfileDAQ::ProfileDAQ(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform& platform, const Profile::PROFILE_ID& id):
        Profile(parsedBitfile, session, id)
    {
        m_analog.reset(new TerminalsAnalog(parsedBitfile, session, platform));
        m_digital.reset(new TerminalsDigital(parsedBitfile, session, platform));
    }

}
