#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "register.h"
#include "dma.h"

namespace iriov2 {
namespace bfp {
/**
 * BitFile Parser.
 *
 * Manages parsing a bitfile and extracting the Registers and DMAs on it.
 * It also extracts the signature and Bitfile version.
 *
 * @ingroup BFP
 */
class BFP {
 public:
	/**
	 * Parse the specified bitfile
	 *
	 * @throw iriov2::errors::BFPParseBitfileError Unable to parse \p bitfile
	 *
	 * @param bitfile			Bitfile to parse
	 * @param warnUnsupported	If true, a message will be printed by std::cerr
	 * 							informing of the registers found with an unsupported type
	 */
	explicit BFP(const std::string &bitfile, const bool warnUnsupported = true);

	/**
	 * Return the path of the parsed Bitfile
	 * @return	Path of the parsed Bitfile
	 */
	std::string getBitfilePath() const;

	/**
	 * Returns the Bitfile version
	 * @return String with the Bitfile version
	 */
	std::string getBitfileVersion() const;

	/**
	 * Return the Bitfile' signature
	 * @return Bitfile' signature
	 */
	std::string getSignature() const;

	/**
	 * Get map with all the registers parsed, the key is the register name (without spaces)
	 * @return	Map with the registers
	 */
	std::unordered_map<std::string, Register> getRegisters() const;

	/**
	 * Get specific register. Throws a runtime exception if not found.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError	Specified \p registerName not found
	 *
	 * @param registerName	Register name to get
	 * @return	Register found
	 */
	Register getRegister(const std::string &registerName) const;

	/**
	 * Get map with all the DMAs parsed, the key if the DMA name (without spaces)
	 *
	 * @return Map with the DMAs
	 */
	std::unordered_map<std::string, DMA> getDMAs() const;

	/**
	 * Get specific DMA. Throws a runtime exception if not found.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError	Specified \p dmaName not found
	 *
	 * @param dmaName	DMA name to get
	 * @return	DMA found
	 */
	DMA getDMA(const std::string &dmaName) const;

 private:
	const std::string m_bitfilePath;

	std::string m_signature;
	std::uint32_t m_baseAddress;

	std::string m_bitfileVersion;

	std::unordered_map<std::string, Register> m_regMap;
	std::unordered_map<std::string, DMA> m_dmaMap;
};
}  // namespace bfp
}  // namespace iriov2
