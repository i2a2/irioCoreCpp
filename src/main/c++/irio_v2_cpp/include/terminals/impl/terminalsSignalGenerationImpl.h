#pragma once

#include "terminals/impl/terminalsBaseImpl.h"

namespace iriov2 {
/**
 * Class managing all terminals related to the
 * signal generation functionality of the RIO device
 *
 * @ingroup Terminals
 */
class TerminalsSignalGenerationImpl: public TerminalsBaseImpl {
public:
	/**
	 * Constructor.
	 * Manages finding the related signal generation terminals
	 * in the parsed bitfile and reading their values
	 * from the FPGA if required
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw std::runtime_error	Number of found terminals do not match the value in SGNo
	 *
	 * @param parsedBitfile	Parsed bitfile with the analog terminals
	 * @param session		NiFpga_Session to be used in NiFpga related functions
	 * @param platform		Platform that is using the terminals. Used to know the maximum number of terminals that can be found.
	 */
	TerminalsSignalGenerationImpl(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	/**
	 * Returns the number of signal generators in the FPGA implementation
	 * @return Number of signal generators implemented
	 */
	std::uint8_t getSGNo() const;

	/**
	 * Returns the reference clock for a specified signal generator.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError The specified signal generator does not exists
	 *
	 * @param n	Number of the signal generator
	 * @return	Reference clock for sampling rate for the specified signal generator
	 */
	std::uint32_t getSGFref(const std::uint32_t n) const;

	/**
	 * Returns the signal type generated for a specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of the signal generator
	 * @return	Type of the signal for the specified signal generator
	 */
	std::uint8_t getSGSignalType(const std::uint32_t n) const;

	/**
	 * Returns the amplitude of the signal for a
	 * specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of the signal generator
	 * @return	Amplitude of the signal for the specified signal generator
	 */
	std::uint32_t getSGAmp(const std::uint32_t n) const;

	/**
	 * Returns the frequency of the signal for a
	 * specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of the signal generator
	 * @return	Frequency of the signal for the specified signal generator
	 */
	std::uint32_t getSGFreq(const std::uint32_t n) const;

	/**
	 * Returns the phase of the signal for a
	 * specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of the signal generator
	 * @return	Phase of the signal for the specified signal generator
	 */
	std::uint32_t getSGPhase(const std::uint32_t n) const;

	/**
	 * Returns the update rate of a
	 * specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n Number of the signal generator
	 * @return	Update rate of the specified signal generator
	 */
	std::uint32_t getSGUpdateRate(const std::uint32_t n) const;

	/**
	 * Configures the signal type for a specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n			Number of the signal generator
	 * @param value		Signal type to configure
	 */
	void setSGSignalType(const std::uint32_t n, const std::uint8_t value) const;

	/**
	 * Configures the amplitude for a specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n			Number of the signal generator
	 * @param value		Amplitude to configure
	 */
	void setSGAmp(const std::uint32_t n, const std::uint32_t value) const;

	/**
	 * Configures the frequency for a specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n			Number of the signal generator
	 * @param value		Frequency to configure
	 */
	void setSGFreq(const std::uint32_t n, const std::uint32_t value) const;

	/**
	 * Configures the phase for a specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n			Number of the signal generator
	 * @param value		Phase to configure
	 */
	void setSGPhase(const std::uint32_t n, const std::uint32_t value) const;

	/**
	 * Configures the update rate for a specific signal generator
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n			Number of the signal generator
	 * @param value		Update rate to configure
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

