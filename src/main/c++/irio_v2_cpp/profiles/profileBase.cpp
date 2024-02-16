#include <profiles/profileBase.h>


namespace iriov2
{
	ProfileBase::ProfileBase(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const ProfileBase::PROFILE_ID &id):
		ProfileBaseTerminals(parsedBitfile, session), profileID(id)
    { }
    
    const std::shared_ptr<const TerminalsAnalog> ProfileBase::analog(){
    	throw std::runtime_error("Analog not enabled for this profile");
    }

    const std::shared_ptr<const TerminalsDigital> ProfileBase::digital(){
    	throw std::runtime_error("Digital not enabled for this profile");
    }
}
