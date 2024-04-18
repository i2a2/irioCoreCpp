#pragma once

#include <profiles/profileBase.h>


namespace irio {
/**
 * Profile with the terminals specific to FlexRIO devices.
 * This class must be a extended by other profile classes,
 * should not be used by itself.
 *
 * @ingroup Profiles
 * @ingroup ProfilesFlexRIO
 * @ingroup FlexRIO
 */
class ProfileFlexRIO: virtual public ProfileBase {
 public:
	/**
	 * Constructor.
	 *
	 * Initializes terminals specific to FlexRIO
	 *
	 * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parserManager     Pointer to class managing parsing the bitfile
	 *                          and finding its resources
	 * @param session			NiFpga_Session to be used in NiFpga related functions
	 * @param id				Profile used
	 */
	ProfileFlexRIO(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const PROFILE_ID &id);
};

}  // namespace irio

