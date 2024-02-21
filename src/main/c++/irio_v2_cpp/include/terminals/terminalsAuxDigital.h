#pragma once

#include <terminals/terminalsBase.h>
#include <bfp.h>
#include <platforms.h>

namespace iriov2 {

class TerminalsAuxDigital: public TerminalsBase {
public:
	/**
	 *
	 *
	 * @throw std::runtime_error	The combined number of found auxDI and
	 * auxDO terminals are greater than the number of auxiliary
	 * digital terminals supported by the platform
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param platform
	 */
	TerminalsAuxDigital(
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
	bool getAuxDI(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	bool getAuxD0(const std::uint32_t n) const;

	size_t getNumAuxDI() const;
	size_t getNumAuxDO() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setAuxDO(const std::uint32_t n, const bool value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxDI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxDO;

};

}
