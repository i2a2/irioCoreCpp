#pragma once

#include "profiles/profileBase.h"

namespace irio {

/**
 * Profile with the terminals
 * specific to IO functionality
 * 
 * @ingroup Profiles
 * 
*/
class ProfileIO: public ProfileBase {
 public:
  /**
   * Profile with the terminals
   * specific to IO functionality
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @param parserManager     Pointer to class managing parsing the bitfile
   *                          and finding its resources
   * @param session			NiFpga_Session to be used in NiFpga related functions
   * @param platform		Platform used
   * @param id                Profile used
   */
  ProfileIO(ParserManager *parserManager, const NiFpga_Session &session,
			const Platform &platform, const PROFILE_ID &id);
};

}  // namespace irio
