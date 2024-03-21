#pragma once

#include "terminals/terminalsBase.h"

namespace iriov2 {
/**
 * Class managing all terminals related to the
 * digital functionality of the RIO device
 *
 * @ingroup Terminals
 */
class TerminalsDigital: public TerminalsBase{
 public:
	/**
	 * Constructor.
	 * Manages finding the related digital terminals in the parsed bitfile
	 * and reading their values from the FPGA if required
	 *
	 * @throw std::runtime_error	The combined number of found DI and
	 * DO terminals are greater than the number of digital
	 * terminals supported by the platform
	 *
	 * @param parsedBitfile	Parsed bitfile with the analog terminals
	 * @param session		NiFpga_Session to be used in NiFpga related functions
	 * @param platform		Platform that is using the terminals. Used to know the maximum number of terminals that can be found.
	 */
	TerminalsDigital(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);

	/**
	 * Returns the value of an DI terminal.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the DI terminal to read
	 * @return	Value read from the DI terminal
	 */
	bool getDI(const std::uint32_t n) const;

	/**
	 * Returns the value of an DO terminal.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the DO terminal to read
	 * @return	Value read from the DO terminal
	 */
	bool getDO(const std::uint32_t n) const;

	/**
	 * Returns number of DI terminals found
	 *
	 * @return Number of DI terminals found
	 */
	size_t getNumDI() const;

	/**
	 * Returns number of DO terminals found
	 *
	 * @return Number of DO terminals found
	 */
	size_t getNumDO() const;

	/**
	 * Writes to a specific DO terminal a value
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the DO terminal to write
	 * @param value	Value to write to the terminal
	 */
	void setDO(const std::uint32_t n, const bool value) const;
};

}  // namespace iriov2
