#pragma once

#include <vector>

#include "terminals/terminalsBase.h"

namespace irio {
/**
 * Class managing all terminals specific to
 * cRIO devices
 *
 * @ingroup Terminals
 * @ingroup cRIO
 */
class TerminalscRIO: public TerminalsBase {
 public:
  /**
   * Manages finding the related terminals specific to cRIO devices.
   *
   * @param parserManager   Pointer to class managing parsing the bitfile
   *                        and finding its resources
   * @param session			    NiFpga_Session to be used in NiFpga related
   * 						            functions
   */
  TerminalscRIO(ParserManager *parserManager, const NiFpga_Session &session);

  /**
   * Returns whether the modules connected to the cRIO are ok
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return true if no problem found, false otherwise
   */
  bool getcRIOModulesOk() const;

  /**
   * Returns a vector with all the modules in the cRIO device
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return Vector with the ID of the modules in the cRIO device
   */
  std::vector<std::uint16_t> getInsertedIOModulesID() const;
};

}  // namespace irio

