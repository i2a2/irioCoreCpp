#include <profileBase.h>


namespace iriov2
{
	ProfileBase::ProfileBase(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const ProfileBase::PROFILE_ID &id):
		ProfileBaseTerminals(parsedBitfile, session), profileID(id)
    { }
    
    const std::shared_ptr<const TerminalsAnalog> ProfileBase::analog(){
        if(!m_analog->activeInProfile){
            throw std::runtime_error("Analog not enabled for this profile");
        }
        return m_analog;
    }

    const std::shared_ptr<const TerminalsDigital> ProfileBase::digital(){
        if(!m_digital->activeInProfile){
            throw std::runtime_error("Digital not enabled for this profile");
        }
        return m_digital;
    }
}
