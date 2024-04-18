#pragma once

#include "terminals/terminalsBase.h"

namespace irio {
/**
 * Class managing all terminals specific to
 * FlexRIO devices
 *
 * @ingroup Terminals
 * @ingroup FlexRIO
 */
class TerminalsFlexRIO: public TerminalsBase{
 public:
  /**
   * Manages finding the related terminals specific to FlexRIO devices.
   *
   * @param parserManager     Pointer to class managing parsing the bitfile
   *                          and finding its resources
   * @param session		NiFpga_Session to be used in NiFpga related functions
   */
  TerminalsFlexRIO(ParserManager *parserManager, const NiFpga_Session &session);

  /**
   * Returns whether the adapter connected in the FPGA is correct
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return true if no problem found, false otherwise
   */
  bool getRIOAdapterCorrect() const;

  /**
   * Return the ID of the module connected to the FlexRIO device
   * 
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   *
   * @return ID of the module connected to the FlexRIO device
   */
  std::uint32_t getInsertedIOModuleID() const;
};

}  // namespace irio
