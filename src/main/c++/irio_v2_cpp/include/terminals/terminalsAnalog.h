#pragma once

#include <terminals/terminalsBase.h>
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>

namespace iriov2 {
class TerminalsAnalog: public TerminalsBase {
public:
	/**
	 *
	 * @throw std::runtime_error Size mismatch between number of AO and AOEnable terminals found
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param platform
	 */
	TerminalsAnalog(
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
	std::int32_t getAI(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::int32_t getAO(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::int32_t getAOEnable(const std::uint32_t n) const;

	size_t getNumAI() const;
	size_t getNumAO() const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setAO(const std::uint32_t n, const std::int32_t value) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setAOEnable(const std::uint32_t n, const bool value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAO;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAOEnable;

	size_t numAI = 0;
	size_t numAO = 0;
};
}
