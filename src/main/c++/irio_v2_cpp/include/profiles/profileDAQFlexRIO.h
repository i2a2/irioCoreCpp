#pragma once

#include <profiles/profileDAQ.h>
#include <profiles/profileFlexRIO.h>

namespace iriov2 {

class ProfileDAQFlexRIO: public ProfileDAQ, ProfileFlexRIO {
public:
	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param platform
	 * @param id
	 */
	ProfileDAQFlexRIO(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);
};

}

