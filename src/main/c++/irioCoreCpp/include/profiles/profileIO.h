#pragma once

#include "profiles/profileCRIO.h"

namespace irio {

/**
 * Profile with the terminals
 * specific to cRIO IO functionality
 * 
 * @ingroup Profiles
 * @ingroup ProfilesIO
 * @ingroup ProfilesCRIO
 * @ingroup cRIO
 * 
*/
class ProfileIO: public ProfileCRIO {
 public:
  /**
   * Profile with the terminals
   * specific to cRIO IO functionality
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @param parserManager     Pointer to class managing parsing the bitfile
   *                          and finding its resources
   * @param session			NiFpga_Session to be used in NiFpga related functions
   * @param platform		Platform used
   */
  ProfileIO(ParserManager *parserManager, const NiFpga_Session &session,
			const Platform &platform);
};

}  // namespace irio
