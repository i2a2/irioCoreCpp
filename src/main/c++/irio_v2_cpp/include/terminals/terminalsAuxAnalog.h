#pragma once

#include "terminals/terminalsBase.h"
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>

namespace iriov2 {

/**
 * Class managing all terminals related to the
 * auxiliary analog functionality of the RIO device
 */
class TerminalsAuxAnalog: public TerminalsBase {
public:
	/**
	 * Constructor.\n
	 * Manages finding the related auxiliary analog terminals
	 * in the parsed bitfile and reading their values
	 * from the FPGA if required
	 *
	 * @param parsedBitfile	Parsed bitfile with the auxiliary analog terminals
	 * @param session		NiFpga_Session to be used in NiFpga related functions
	 * @param platform		Platform that is using the terminals. Used to know the maximum number of terminals that can be found.
	 */
	TerminalsAuxAnalog(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	/**
	 * Returns the value of an auxAI terminal.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the auxAI terminal to read
	 * @return	Value read from the auxAI terminal
	 */
	std::int32_t getAuxAI(const std::uint32_t n) const;

	/**
	 * Returns the value of an auxAO terminal.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the auxAO terminal to read
	 * @return	Value read from the auxAO terminal
	 */
	std::int16_t getAuxAO(const std::uint32_t n) const;

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
	 * Writes to an specific auxAO terminal a value
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the auxAO terminal to write
	 * @param value	Value to write to the terminal
	 */
	void setAuxAO(const std::uint32_t n, const std::int16_t value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAO;
};

}
