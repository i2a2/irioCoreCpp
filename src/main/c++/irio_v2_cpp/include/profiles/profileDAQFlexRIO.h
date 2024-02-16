#pragma once

#include <profiles/profileDAQ.h>

namespace iriov2 {

class ProfileDAQFlexRIO: public ProfileDAQ {
public:
	ProfileDAQFlexRIO(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);

	const std::shared_ptr<const TerminalsFlexRIO> flexRIO() const override;

private:
	std::shared_ptr<const TerminalsFlexRIO> m_flexRIO;
};

}

