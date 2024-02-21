#pragma once

#include "terminals/terminalsBase.h"
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>

namespace iriov2 {

class TerminalsAuxAnalog: public TerminalsBase {
public:
	TerminalsAuxAnalog(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::int32_t getAuxAI(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::int16_t getAuxAO(const std::uint32_t n) const;

	size_t getNumAuxAI() const;
	size_t getNumAuxAO() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setAuxAO(const std::uint32_t n, const std::int16_t value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAO;
};

}
