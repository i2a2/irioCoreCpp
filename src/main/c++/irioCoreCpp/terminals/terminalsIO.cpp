#include <memory>

#include "terminals/terminalsIO.h"
#include "terminals/impl/terminalsIOImpl.h"

namespace irio {

TerminalsIO::TerminalsIO(ParserManager *parserManager,
		const NiFpga_Session &session, const Platform &platform) :
		TerminalsBase(
				std::make_shared<TerminalsIOImpl>(parserManager, session, platform)) {
}

void TerminalsIO::setSamplingRateDecimation(const std::uint32_t &n,
											const std::uint16_t dec) const {
	std::static_pointer_cast<TerminalsIOImpl>(m_impl)
		->setSamplingRateDecimationImpl(n, dec);
}

std::uint16_t TerminalsIO::getSamplingRateDecimation(
	const std::uint32_t &n) const {
	return std::static_pointer_cast<TerminalsIOImpl>(m_impl)
		->getSamplingRateDecimationImpl(n);
}

size_t TerminalsIO::getNumIOSamplingRate() const {
    return std::static_pointer_cast<TerminalsIOImpl>(m_impl)
		->getNumIOSamplingRateImpl();
}

}  // namespace irio
