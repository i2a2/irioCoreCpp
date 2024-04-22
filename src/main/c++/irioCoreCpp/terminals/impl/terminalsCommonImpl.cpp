#include <limits>

#include "terminals/impl/terminalsCommonImpl.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"

namespace irio {

TerminalsCommonImpl::TerminalsCommonImpl(ParserManager *parserManager,
										 const NiFpga_Session &session)
	: TerminalsBaseImpl(session) {
    NiFpga_Status status;

	// Read FPGAVIversion
	std::uint32_t fpgaviversion_addr;
	if (parserManager->findRegisterAddress(TERMINAL_FPGAVIVERSION,
			GroupResource::Common, &fpgaviversion_addr)) {
		std::array<std::uint8_t, 2> fpgaviversion;
		status = NiFpga_ReadArrayU8(m_session, fpgaviversion_addr,
				fpgaviversion.data(), 2);
		utils::throwIfNotSuccessNiFpga(status, "Error reading FPGAVIversion");
		m_fpgaviversion = "V" + std::to_string(fpgaviversion[0])
				+ "." + std::to_string(fpgaviversion[1]);
	}

    // Read Fref
	std::uint32_t fref_addr;
	if (parserManager->findRegisterAddress(TERMINAL_FREF,
				GroupResource::Common, &fref_addr)) {
		status = NiFpga_ReadU32(m_session, fref_addr, &m_fref);
		utils::throwIfNotSuccessNiFpga(status, "Error reading Fref");
	}

    parserManager->findRegisterAddress(TERMINAL_INITDONE,
			GroupResource::Common, &m_initdone_addr);
	parserManager->findRegisterAddress(TERMINAL_DEVQUALITYSTATUS,
			GroupResource::Common, &m_devqualitystatus_addr);
	parserManager->findRegisterAddress(TERMINAL_DEVTEMP, GroupResource::Common,
			&m_devtemp_addr);
	parserManager->findRegisterAddress(TERMINAL_DAQSTARTSTOP,
			GroupResource::Common, &m_daqstartstop_addr);
	parserManager->findRegisterAddress(TERMINAL_DEBUGMODE,
			GroupResource::Common, &m_debugmode_addr);

	m_minSamplingRate = 1.0 * m_fref
			/ std::numeric_limits<std::uint16_t>::max();
	m_maxSamplingRate = m_fref;
}


std::string TerminalsCommonImpl::getFPGAVIversionImpl() const {
	return m_fpgaviversion;
}

std::uint32_t TerminalsCommonImpl::getFrefImpl() const {
	return m_fref;
}

bool TerminalsCommonImpl::getInitDoneImpl() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadBool(m_session, m_initdone_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading InitDone");
	return static_cast<bool>(aux);
}

std::uint8_t TerminalsCommonImpl::getDevQualityStatusImpl() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_devqualitystatus_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevQualityStatus");
	return aux;
}

std::int16_t TerminalsCommonImpl::getDevTempImpl() const {
	std::int16_t aux;
	auto status = NiFpga_ReadI16(m_session, m_devtemp_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DevTemp");
	return aux;
}

bool TerminalsCommonImpl::getDAQStartStopImpl() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_daqstartstop_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DAQStartStop");
	return static_cast<bool>(aux);
}

bool TerminalsCommonImpl::getDebugModeImpl() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_debugmode_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading DebugMode");
	return static_cast<bool>(aux);
}

void TerminalsCommonImpl::setDAQStartImpl() const {
	setDAQStartStopImpl(true);
}

void TerminalsCommonImpl::setDAQStopImpl() const {
	setDAQStartStopImpl(false);
}

void TerminalsCommonImpl::setDAQStartStopImpl(const bool &start) const {
	auto status = NiFpga_WriteU8(m_session, m_daqstartstop_addr,
			static_cast<std::uint8_t>(start));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DAQStartStop");
}

void TerminalsCommonImpl::setDebugModeImpl(const bool &debug) const {
	auto status = NiFpga_WriteU8(m_session, m_debugmode_addr,
			static_cast<std::uint8_t>(debug));
	utils::throwIfNotSuccessNiFpga(status, "Error writing DebugMode");
}

double TerminalsCommonImpl::getMinSamplingRateImpl() const {
	return m_minSamplingRate;
}

double TerminalsCommonImpl::getMaxSamplingRateImpl() const {
	return m_maxSamplingRate;
}

}  // namespace irio
