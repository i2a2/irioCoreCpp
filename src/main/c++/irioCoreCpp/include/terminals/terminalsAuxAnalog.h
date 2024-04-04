#pragma once

#include "terminals/terminalsBase.h"

namespace irio {

/**
 * Class managing all terminals related to the
 * auxiliary analog functionality of the RIO device
 *
 * @ingroup Terminals
 */
class TerminalsAuxAnalog: public TerminalsBase {
 public:
  /**
   * Constructor.
   * Manages finding the related auxiliary analog terminals
   * in the parsed bitfile and reading their values
   * from the FPGA if required
   *
   * @param parserManager     Pointer to class managing parsing the bitfile
   *                          and finding its resources
   * @param session			NiFpga_Session to be used in NiFpga related
   * 							functions
   * @param platform			Platform that is using the terminals. Used to
   * 							know the maximum number of terminals that can
   * 							be found.
   */
  TerminalsAuxAnalog(ParserManager *parserManager,
					 const NiFpga_Session &session, const Platform &platform);

  /**
   * Returns the value of an auxAI terminal.
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n	Number of the auxAI terminal to read
   * @return	Value read from the auxAI terminal
   */
  std::int32_t getAuxAI(const std::uint32_t n) const;

  /**
   * Returns the value of an auxAO terminal.
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n	Number of the auxAO terminal to read
   * @return	Value read from the auxAO terminal
   */
  std::int32_t getAuxAO(const std::uint32_t n) const;

  /**
   * Returns number of auxAI terminals found
   *
   * @return Number of auxAI terminals found
   */
  size_t getNumAuxAI() const;

  /**
   * Returns number of auxAO terminals found
   *
   * @return Number of auxAO terminals found
   */
  size_t getNumAuxAO() const;

  /**
   * Writes to a specific auxAO terminal a value
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n	Number of the auxAO terminal to write
   * @param value	Value to write to the terminal
   */
  void setAuxAO(const std::uint32_t n, const std::int32_t value) const;

  /**
   * Returns the value of an auxAI64 terminal.
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n	Number of the auxAI64 terminal to read
   * @return	Value read from the auxAI64 terminal
   */
  std::int64_t getAuxAI64(const std::uint32_t n) const;

  /**
   * Returns the value of an auxAO64 terminal.
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n	Number of the auxAO64 terminal to read
   * @return	Value read from the auxAO64 terminal
   */
  std::int64_t getAuxAO64(const std::uint32_t n) const;

  /**
   * Returns number of auxAI64 terminals found
   *
   * @return Number of auxAI64 terminals found
   */
  size_t getNumAuxAI64() const;

  /**
   * Returns number of auxAO64 terminals found
   *
   * @return Number of auxAO64 terminals found
   */
  size_t getNumAuxAO64() const;

  /**
   * Writes to a specific auxAO64 terminal a value
   *
   * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
   * @throw irio::errors::ResourceNotFoundError Resource specified not found
   *
   * @param n	Number of the auxAO64 terminal to write
   * @param value	Value to write to the terminal
   */
  void setAuxAO64(const std::uint32_t n, const std::int64_t value) const;
};

}  // namespace irio
