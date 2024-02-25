#pragma once

#include <terminals/terminalsDMACommon.h>

namespace iriov2{

class TerminalsDMADAQ: public TerminalsDMACommon{
public:
	TerminalsDMADAQ(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const std::string &nameTermBlockNWords,
			const std::string &nameTermSamplingRate,
			const std::string &nameTermNCh,
			const std::string &nameTermFrameType,
			const std::string &nameTermSampleSize,
			const std::string &nameTermOverflows,
			const std::string &nameTermDMA,
			const std::string &nameTermDMAEnable);

	std::uint16_t getLengthBlock(const std::uint32_t &n) const;

	std::uint16_t getSamplingRate(const std::uint32_t &n) const;
	void setSamplingRate(const std::uint32_t &n, std::uint16_t &samplingRate) const;

private:
	const std::string m_nameTermSamplingRate;

	std::vector<std::uint16_t> m_lengthBlocks;

	std::unordered_map<std::uint32_t, const std::uint32_t> m_samplingRate_addr;
};

}


