#include "profiles.h"


namespace iriov2
{
    Profile::Profile(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const Profile::PROFILE_ID &id):
        TerminalsCommon(parsedBitfile, session), profileID(id)
    { }
    
    const std::shared_ptr<const TerminalsAnalog> Profile::analog(){
        if(!m_analog->activeInProfile){
            throw std::runtime_error("Analog not enabled for this profile");
        }
        return m_analog;
    }

    const std::shared_ptr<const TerminalsDigital> Profile::digital(){
        if(!m_digital->activeInProfile){
            throw std::runtime_error("Digital not enabled for this profile");
        }
        return m_digital;
    }
}
