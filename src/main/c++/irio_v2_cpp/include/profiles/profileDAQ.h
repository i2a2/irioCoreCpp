#pragma once

#include <profiles/profileBase.h>
#include <platforms.h>

namespace iriov2 {

class ProfileDAQ: virtual public ProfileBase {
public:
	ProfileDAQ(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);

	const std::shared_ptr<const TerminalsAnalog> analog() const override;
	const std::shared_ptr<const TerminalsDigital> digital() const override;
private:
	std::shared_ptr<const TerminalsAnalog> m_analog;
	std::shared_ptr<const TerminalsDigital> m_digital;
};

}
