#pragma once

#include "terminals/terminalsBase.h"
#include "bfp.h"
#include "platforms.h"
#include <NiFpga.h>

namespace iriov2 {

class TerminalsSignalGeneration: public TerminalsBase {
public:
	/**
	 *
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw std::runtime_error	Number of found terminals do not match the value in SGNo
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param platform
	 */
	TerminalsSignalGeneration(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	std::uint8_t getSGNo() const;

	/**
	 * Gets the reference clock for a specified signal generator.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError The specified signal generator does not exists
	 *
	 * @param n	Number of signal generator
	 * @return	Reference clock for sampling rate for the specified signal generator
	 */
	std::uint32_t getSGFref(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::uint8_t getSGSignalType(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::uint32_t getSGAmp(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::uint32_t getSGFreq(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::uint32_t getSGPhase(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @return
	 */
	std::uint32_t getSGUpdateRate(const std::uint32_t n) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setSGSignalType(const std::uint32_t n, const std::uint8_t value) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setSGAmp(const std::uint32_t n, const std::uint32_t value) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setSGFreq(const std::uint32_t n, const std::uint32_t value) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setSGPhase(const std::uint32_t n, const std::uint32_t value) const;

	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n
	 * @param value
	 */
	void setSGUpdateRate(const std::uint32_t n, const std::uint32_t value) const;

private:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapSignalType_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAmp_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapFreq_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapPhase_addr;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapUpdateRate_addr;

	std::uint8_t m_numSG = 0;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapFref;

};

}

