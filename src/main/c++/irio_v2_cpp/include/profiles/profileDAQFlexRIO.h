#pragma once

#include <profiles/profileDAQ.h>
#include <profiles/profileFlexRIO.h>

namespace iriov2 {

/**
 * Profile with the terminals
 * specific to data acquisition functionality
 * and the ones specific to FlexRIO devices.
 * See \ref ProfileDAQ and \ref ProfileFlexRIO
 *
 * @ingroup Profiles
 * @ingroup ProfilesDAQ
 * @ingroup ProfilesFlexRIO
 * @ingroup FlexRIO
 */
class ProfileDAQFlexRIO: public ProfileDAQ, ProfileFlexRIO {
public:
	/**
	 * Constructor.
	 *
	 * See \ref ProfileDAQ and \ref ProfileFlexRIO for more
	 * information about the terminals created
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile 	Parsed bitfile
	 * @param session			NiFpga_Session to be used in NiFpga related functions
	 * @param platform			Platform used
	 * @param id				Profile used
	 */
	ProfileDAQFlexRIO(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);
};

}

