#include <memory>

#include "terminals/terminalsCommon.h"
#include "terminals/impl/terminalsCommonImpl.h"

namespace irio {

TerminalsCommon::TerminalsCommon(ParserManager *parserManager,
								 const NiFpga_Session &session)
	: TerminalsBase(
		  std::make_shared<TerminalsCommonImpl>(parserManager, session)) {}

std::string TerminalsCommon::getFPGAVIversion() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getFPGAVIversionImpl();
}
std::uint32_t TerminalsCommon::getFref() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)->getFrefImpl();
}
bool TerminalsCommon::getInitDone() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getInitDoneImpl();
}
std::uint8_t TerminalsCommon::getDevQualityStatus() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getDevQualityStatusImpl();
}
std::int16_t TerminalsCommon::getDevTemp() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getDevTempImpl();
}
bool TerminalsCommon::getDAQStartStop() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getDAQStartStopImpl();
}
bool TerminalsCommon::getDebugMode() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getDebugModeImpl();
}
void TerminalsCommon::setDAQStart() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->setDAQStartImpl();
}
void TerminalsCommon::setDAQStop() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->setDAQStopImpl();
}
void TerminalsCommon::setDAQStartStop(const bool &start) const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->setDAQStartStopImpl(start);
}
void TerminalsCommon::setDebugMode(const bool &debug) const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->setDebugModeImpl(debug);
}
double TerminalsCommon::getMinSamplingRate() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getMinSamplingRateImpl();
}
double TerminalsCommon::getMaxSamplingRate() const {
	return std::static_pointer_cast<TerminalsCommonImpl>(m_impl)
		->getMaxSamplingRateImpl();
}

}  // namespace irio
