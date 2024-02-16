#include <profiles/profileBaseTerminals.h>
#include "terminals/names/namesTerminalsCommon.h"
#include "utils.h"

namespace iriov2 {

ProfileBaseTerminals::ProfileBaseTerminals(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session) :
		m_session(session) {
	NiFpga_Status status;

	//Read FPGAVIversion
	auto fpgaviversion_addr = parsedBitfile.getRegister(TERMINAL_FPGAVIVERSION).address;
	std::uint8_t fpgaviversion[2];
	status = NiFpga_ReadArrayU8(session, fpgaviversion_addr, fpgaviversion, 2);
	throwIfNotSuccessNiFpga(status, "Error reading FPGAVIversion");
	m_fpgaviversion = std::make_pair(fpgaviversion[0], fpgaviversion[1]);

	//Read Fref
	auto fref_addr = parsedBitfile.getRegister(TERMINAL_FREF).address;
	status = NiFpga_ReadU32(session, fref_addr, &m_fref);
	throwIfNotSuccessNiFpga(status, "Error reading Fref");

	m_initdone_addr = parsedBitfile.getRegister(TERMINAL_INITDONE).address;
	m_devqualitystatus_addr = parsedBitfile.getRegister(TERMINAL_DEVQUALITYSTATUS).address;
	m_devtemp_addr = parsedBitfile.getRegister(TERMINAL_DEVTEMP).address;
	m_daqstartstop_addr = parsedBitfile.getRegister(TERMINAL_DAQSTARTSTOP).address;
	m_debugmode_addr = parsedBitfile.getRegister(TERMINAL_DEBUGMODE).address;
}

std::pair<std::uint8_t, std::uint8_t> ProfileBaseTerminals::getFPGAVIversion() const {
	return m_fpgaviversion;
}

std::uint32_t ProfileBaseTerminals::getFref() const {
	return m_fref;
}

bool ProfileBaseTerminals::getInitDone() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadBool(m_session, m_initdone_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading InitDone");
	return static_cast<bool>(aux);
}

std::uint8_t ProfileBaseTerminals::getDevQualityStatus() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_devqualitystatus_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading DevQualityStatus");
	return aux;
}

std::int16_t ProfileBaseTerminals::getDevTemp() const {
	std::int16_t aux;
	auto status = NiFpga_ReadI16(m_session, m_devtemp_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading DevTemp");
	return aux;
}

bool ProfileBaseTerminals::getDAQStartStop() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_daqstartstop_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading DAQStartStop");
	return static_cast<bool>(aux);
}

bool ProfileBaseTerminals::getDebugMode() const {
	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, m_debugmode_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading DebugMode");
	return static_cast<bool>(aux);
}

void ProfileBaseTerminals::setDAQStart() const {
	setDAQStartStop(true);
}

void ProfileBaseTerminals::setDAQStop() const {
	setDAQStartStop(false);
}

void ProfileBaseTerminals::setDAQStartStop(const bool &start) const {
	auto status = NiFpga_WriteBool(m_session, m_daqstartstop_addr,
			static_cast<std::uint8_t>(start));
	throwIfNotSuccessNiFpga(status, "Error writing DAQStartStop");
}

void ProfileBaseTerminals::setDebugMode(const bool &debug) const {
	auto status = NiFpga_WriteBool(m_session, m_debugmode_addr, static_cast<std::uint8_t>(debug));
	throwIfNotSuccessNiFpga(status, "Error writing DebugMode");
}

}
