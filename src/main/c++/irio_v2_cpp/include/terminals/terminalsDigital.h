#pragma once

#include <terminals/terminalsBase.h>
#include <bfp.h>
#include <platforms.h>

namespace iriov2 {

class TerminalsDigital: public TerminalsBase {
public:
	/**
	 *
	 * @throw std::runtime_error	The combined number of found DI and
	 * DO terminals are greater than the number of digital
	 * terminals supported by the platform
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param platform
	 */
	TerminalsDigital(
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
	bool getDI(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	bool getD0(const std::uint32_t n) const;

	size_t getNumDI() const;
	size_t getNumDO() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setDO(const std::uint32_t n, const bool value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDO;

};

}
