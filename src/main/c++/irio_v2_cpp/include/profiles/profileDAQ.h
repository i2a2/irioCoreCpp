#pragma once

#include <profiles/profileBase.h>
#include <platforms.h>

namespace iriov2 {

class ProfileDAQ: public ProfileBase {
public:
	ProfileDAQ(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);

	const std::shared_ptr<const TerminalsAnalog> analog();
	const std::shared_ptr<const TerminalsDigital> digital();
private:
	std::shared_ptr<const TerminalsAnalog> m_analog;
	std::shared_ptr<const TerminalsDigital> m_digital;
};

}
