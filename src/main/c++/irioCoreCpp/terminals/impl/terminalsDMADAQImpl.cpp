#include <terminals/impl/terminalsDMADAQImpl.h>
#include <utils.h>
#include <errorsIrio.h>

namespace irio {

TerminalsDMADAQImpl::TerminalsDMADAQImpl(ParserManager *parserManager,
		const NiFpga_Session &session, const Platform &platform,
		const std::string &nameTermBlockNWords,
		const std::string &nameTermSamplingRate, const std::string &nameTermNCh,
		const std::string &nameTermFrameType,
		const std::string &nameTermSampleSize,
		const std::string &nameTermOverflows, const std::string &nameTermDMA,
		const std::string &nameTermDMAEnable) :
		TerminalsDMACommonImpl(parserManager, session, platform, nameTermNCh,
				nameTermFrameType, nameTermSampleSize, nameTermOverflows,
				nameTermDMA, nameTermDMAEnable), m_nameTermSamplingRate(
				nameTermSamplingRate) {
	const size_t numDMAs = countDMAsImpl();

	// Find BlockNWords
	findArrayRegReadToVector<std::uint16_t>(parserManager,
			GroupResource::DAQ, false, m_session,
			nameTermBlockNWords, &m_lengthBlocks, &NiFpga_ReadArrayU16);

	if (numDMAs != m_lengthBlocks.size()) {
		const std::string errMsg = "Array size of " +
								    nameTermBlockNWords + " ("
								   + std::to_string(m_lengthBlocks.size())
								   + ") does not match the number "
								   + "of DMAs found ("
								   + std::to_string(numDMAs) + ")";
		parserManager->logResourceError(nameTermBlockNWords, errMsg,
										GroupResource::DAQ);
	}

	// Find SamplingRate
	for(uint32_t i = 0; i < platform.maxDMA; ++i) {
		parserManager->findRegisterEnumAddress(nameTermSamplingRate, i,
				GroupResource::DAQ, &m_samplingRate_addr, true);
	}

	parserManager->compareResourcesMap(m_samplingRate_addr,
									   nameTermSamplingRate, getDMAMap(),
									   nameTermDMA, GroupResource::DAQ);
}

std::uint16_t TerminalsDMADAQImpl::getLengthBlock(
		const std::uint32_t &n) const {
	if (n >= m_lengthBlocks.size()) {
		const std::string err = std::to_string(n) + " is not a valid DMA ID";
		throw errors::ResourceNotFoundError(err);
	}

	return m_lengthBlocks.at(n);
}

std::uint16_t TerminalsDMADAQImpl::getSamplingRateDecimation(
		const std::uint32_t &n) const {
	const auto addr = utils::getAddressEnumResource(m_samplingRate_addr, n,
			m_nameTermSamplingRate);

	std::uint16_t value;
	const auto status = NiFpga_ReadU16(m_session, addr, &value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + m_nameTermSamplingRate + std::to_string(n));

	return value;
}

void TerminalsDMADAQImpl::setSamplingRateDecimation(const std::uint32_t &n,
		const std::uint16_t &decimation) const {
	const auto addr = utils::getAddressEnumResource(m_samplingRate_addr, n,
			m_nameTermSamplingRate);

	const auto status = NiFpga_WriteU16(m_session, addr, decimation);
	utils::throwIfNotSuccessNiFpga(status,
			"Error writing " + m_nameTermSamplingRate + std::to_string(n));
}
}  // namespace irio
