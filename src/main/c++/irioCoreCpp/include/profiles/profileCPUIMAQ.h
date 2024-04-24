#pragma once

#include "profiles/profileBase.h"
#include "platforms.h"

namespace irio {

/**
 * Profile with the terminals
 * specific to image acquisition functionality
 * 
 * @ingroup Profiles
 */
class ProfileCPUIMAQ: virtual public ProfileBase {
 public:
    /**
     * Manages creating the required terminals for CPU image acquisition
     * 
     * @throw irio::errors::NiFpgaError    Error occurred in an FPGA operation
     * 
     * @param parserManager     Pointer to class managing parsing the bitfile
	 *                          and finding its resources
     * @param session           NiFpga_Session to be used in NiFpga related functions
	 * @param platform          Platform used
     * @param id                Profile used
     */
    ProfileCPUIMAQ(
        ParserManager *parserManager,
        const NiFpga_Session &session,
        const Platform &platform,
        const PROFILE_ID &id);
};
}  // namespace irio
