#pragma once

#include "profiles/profileFlexRIO.h"
#include "profiles/profileCPUIMAQ.h"

namespace irio {
class ProfileCPUIMAQFlexRIO: public ProfileCPUIMAQ, public ProfileFlexRIO {
 public:
  /**
   * Manages creating the required terminals for CPU image acquisition in
   * FlexRIO boards
   *
   * @throw irio::errors::NiFpgaError    Error occurred in an FPGA operation
   *
   * @param parserManager     Pointer to class managing parsing the bitfile
   *                          and finding its resources
   * @param session           NiFpga_Session to be used in NiFpga related
   * functions
   * @param platform          Platform used
   */
  ProfileCPUIMAQFlexRIO(ParserManager *parserManager,
						const NiFpga_Session &session,
						const Platform &platform);
};
}  // namespace irio