#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "register.h"
#include "dma.h"

namespace bfp{
class BFP{
public:
	BFP(const std::string& bitfile);

	std::string getBitfilePath() const;

	std::string getSignature() const;
	std::uint32_t getBaseAddress() const;

	std::unordered_map<std::string, Register> getRegisters() const;
	Register getRegister(const std::string& registerName) const;
	std::unordered_map<std::string, DMA> getDMAs() const;
	DMA getDMA(const std::string& dmaName) const;

private:
	const std::string m_bitfilePath;

	std::string m_signature;
	std::uint32_t m_baseAddress;

	std::unordered_map<std::string, Register> m_regMap;
	std::unordered_map<std::string, DMA> m_dmaMap;
};
}
