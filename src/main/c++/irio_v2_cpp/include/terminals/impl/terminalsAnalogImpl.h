#pragma once

#include <terminals/terminalsBase.h>
#include <modules.h>

namespace iriov2 {
/**
 * Class managing all terminals related to the
 * analog functionality of the RIO device
 *
 * @ingroup Terminals
 */
class TerminalsAnalogImpl: public TerminalsBase {
public:
	/**
	 * Constructor.
	 * Manages finding the related analog terminals in the parsed bitfile
	 * and reading their values from the FPGA if required
	 *
	 * @throw std::runtime_error Size mismatch between number of AO and AOEnable terminals found
	 *
	 * @param parsedBitfile	Parsed bitfile with the analog terminals
	 * @param session		NiFpga_Session to be used in NiFpga related functions
	 * @param platform		Platform that is using the terminals. Used to know the maximum number of terminals that can be found.
	 */
	TerminalsAnalogImpl(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);

	/**
	 * Returns the value of an AI terminal.
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the AI terminal to read
	 * @return	Value read from the AI terminal
	 */
	std::int32_t getAI(const std::uint32_t n) const;

	/**
	 * Returns the value of an AO terminal
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the AO terminal to read
	 * @return	Value read from the AO terminal
	 */
	std::int32_t getAO(const std::uint32_t n) const;

	/**
	 * Returns the value of an AOEnable terminal
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the AOEnable terminal to read
	 * @return	Value read from the AOEnable terminal
	 */
	std::int32_t getAOEnable(const std::uint32_t n) const;

	/**
	 * Returns number of AI terminals found
	 *
	 * @return Number of AI terminals found
	 */
	size_t getNumAI() const;

	/**
	 * Returns number of AO terminals found
	 *
	 * @return Number of AO terminals found
	 */
	size_t getNumAO() const;

	/**
	 * Writes to a specific AO terminal a value
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the AO terminal to write
	 * @param value	Value to write to the terminal
	 */
	void setAO(const std::uint32_t n, const std::int32_t value) const;

	/**
	 * Enables or disables a specific AO terminal
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 *
	 * @param n	Number of the AOEnable terminal to enable/disable
	 * @param value	true for enabling the equivalent AO terminal, false to disable
	 */
	void setAOEnable(const std::uint32_t n, const bool value) const;

	ModulesType getModuleConnected() const;

	double getCVADC() const;

	double getCVDAC() const;

	double getMaxValAO() const;

	double getMinValAO() const;

	CouplingMode getAICouplingMode() const;

	void setAICouplingMode(const CouplingMode &mode) const;

private:

	void searchModule(const Platform &platform);

	void searchFlexRIOModule();

	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAI;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAO;
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAOEnable;

	size_t numAI = 0;
	size_t numAO = 0;

	std::unique_ptr<Module> m_module;
};
}
