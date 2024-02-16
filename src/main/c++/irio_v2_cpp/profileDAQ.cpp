#include <profiles/profileDAQ.h>

namespace iriov2{

    ProfileDAQ::ProfileDAQ(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Platform& platform, const ProfileBase::PROFILE_ID& id):
        ProfileBase(parsedBitfile, session, id)
    {
        m_analog.reset(new TerminalsAnalog(parsedBitfile, session, platform));
        m_digital.reset(new TerminalsDigital(parsedBitfile, session, platform));
    }

    const std::shared_ptr<const TerminalsAnalog> ProfileDAQ::analog(){
    	return m_analog;
    }

    const std::shared_ptr<const TerminalsDigital> ProfileDAQ::digital(){
    	return m_digital;
    }
}
