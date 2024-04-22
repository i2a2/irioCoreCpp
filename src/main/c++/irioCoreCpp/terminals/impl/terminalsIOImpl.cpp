#include "terminals/impl/terminalsIOImpl.h"
#include "terminals/names/namesTerminalsIO.h"
#include "utils.h"

namespace irio {

TerminalsIOImpl::TerminalsIOImpl(ParserManager* parserManager,
								 const NiFpga_Session& session,
								 const Platform& platform)
	: TerminalsBaseImpl(session) {
	// Find IO Sampling Rate
	for (uint32_t i = 0; i < platform.maxModules; ++i) {
		parserManager->findRegisterEnumAddress(TERMINAL_SAMPLINGRATE, i,
											   GroupResource::IO,
											   &m_mapSamplingRate, true);
	}
}

void TerminalsIOImpl::setSamplingRateDecimationImpl(
	const std::uint32_t& n, const std::uint16_t dec) const {
	auto addr = utils::getAddressEnumResource(m_mapSamplingRate, n,
											  TERMINAL_SAMPLINGRATE);
    auto status = NiFpga_WriteU16(m_session, addr, dec);
	utils::throwIfNotSuccessNiFpga(
		status, "Error writing terminal " +
					std::string(TERMINAL_SAMPLINGRATE) + std::to_string(n));
}

std::uint16_t TerminalsIOImpl::getSamplingRateDecimationImpl(
	const std::uint32_t& n) const {
	auto addr = utils::getAddressEnumResource(m_mapSamplingRate, n,
											  TERMINAL_SAMPLINGRATE);
    std::uint16_t dec;
    auto status = NiFpga_ReadU16(m_session, addr, &dec);
    utils::throwIfNotSuccessNiFpga(
		status, "Error reading terminal " +
					std::string(TERMINAL_SAMPLINGRATE) + std::to_string(n));
    return dec;
}

size_t TerminalsIOImpl::getNumIOSamplingRateImpl() const {
	return m_mapSamplingRate.size();
}

}  // namespace irio
