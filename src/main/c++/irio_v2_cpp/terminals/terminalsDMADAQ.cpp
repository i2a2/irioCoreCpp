#include <terminals/terminalsDMADAQ.h>
#include <utils.h>
#include <errorsIrio.h>

namespace iriov2{

TerminalsDMADAQ::TerminalsDMADAQ(
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
		const std::string &nameTermDMAEnable) :
				TerminalsDMACommon(parsedBitfile, session, platform, nameTermNCh, nameTermFrameType,
						nameTermSampleSize, nameTermOverflows, nameTermDMA, nameTermDMAEnable),
				m_nameTermSamplingRate(nameTermSamplingRate) {

	const size_t numDMAs = countDMAs();

	//Find BlockNWords. Get initial values and store them, no need to read them each time.
	utils::findArrayRegReadToVector<std::uint16_t>(parsedBitfile, m_session, nameTermBlockNWords,
			m_lengthBlocks, &NiFpga_ReadArrayU16);

	if (numDMAs != m_lengthBlocks.size()) {
		throw errors::ResourceNotFoundError(
				"Mismatch in number of " + nameTermDMA + " and " + nameTermBlockNWords
						+ " terminals");
	}

	//Find SamplingRate
	utils::findAndInsertEnumRegisters(parsedBitfile, nameTermSamplingRate, platform.maxDMA,
			m_samplingRate_addr);

	if (numDMAs != m_samplingRate_addr.size()) {
		throw errors::ResourceNotFoundError(
				"Mismatch in number of " + nameTermDMA + " and " + nameTermSamplingRate
						+ " terminals");
	}

}

std::uint16_t TerminalsDMADAQ::getLengthBlock(const std::uint32_t &n) const {
	if (n >= m_lengthBlocks.size()) {
		const std::string err = std::to_string(n) + " is not a valid DMA ID";
		throw errors::ResourceNotFoundError(err);
	}

	return m_lengthBlocks.at(n);
}

std::uint16_t TerminalsDMADAQ::getSamplingRate(const std::uint32_t &n) const {
	const auto addr = utils::getAddressEnumResource(m_samplingRate_addr, n, m_nameTermSamplingRate);

	std::uint16_t value;
	const auto status = NiFpga_ReadU16(m_session, addr, &value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + m_nameTermSamplingRate + std::to_string(n));

	return value;
}

void TerminalsDMADAQ::setSamplingRate(
		const std::uint32_t &n,
		const std::uint16_t &samplingRate) const {

	const auto addr = utils::getAddressEnumResource(m_samplingRate_addr, n, m_nameTermSamplingRate);

	const auto status = NiFpga_WriteU16(m_session, addr, samplingRate);
	utils::throwIfNotSuccessNiFpga(status,
			"Error writing " + m_nameTermSamplingRate + std::to_string(n));
}
}
