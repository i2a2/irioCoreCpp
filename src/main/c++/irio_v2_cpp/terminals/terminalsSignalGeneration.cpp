#include "terminals/terminalsSignalGeneration.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "utils.h"

namespace iriov2 {

void findAndCheck(
		const std::unordered_map<std::string, bfp::Register> &mapReg,
		const std::string &terminalName,
		const size_t maxNum,
		const size_t expectedNum,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert) {

	findAndInsertEnumResources(mapReg, terminalName, maxNum, mapInsert);
	if (mapInsert.size() != expectedNum) {
		throw std::runtime_error("Mismatch between SGNo and number of found " + terminalName);
	}
}

TerminalsSignalGeneration::TerminalsSignalGeneration(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBase(session) {
	NiFpga_Status status;
	const auto mapReg = parsedBitfile.getRegisters();
	const auto it = mapReg.find(TERMINAL_SGNO);
	if (it == mapReg.end()) {
		//There are no signal generators, go back
		return;
	}
	status = NiFpga_ReadU8(m_session, it->second.address, &m_numSG);
	throwIfNotSuccessNiFpga(status, "Error reading " + std::string(TERMINAL_SGNO));

	//Find SignalType
	findAndCheck(mapReg, TERMINAL_SGSIGNALTYPE, platform.maxSG, m_numSG, m_mapSignalType_addr);

	//Find Amp
	findAndCheck(mapReg, TERMINAL_SGAMP, platform.maxSG, m_numSG, m_mapAmp_addr);

	//Find Freq
	findAndCheck(mapReg, TERMINAL_SGFREQ, platform.maxSG, m_numSG, m_mapFreq_addr);

	//Find Phase
	findAndCheck(mapReg, TERMINAL_SGPHASE, platform.maxSG, m_numSG, m_mapPhase_addr);

	//Find Update Rate
	findAndCheck(mapReg, TERMINAL_SGUPDATERATE, platform.maxSG, m_numSG, m_mapUpdateRate_addr);

	//Read all Fref
	std::unordered_map<std::uint32_t, const std::uint32_t> mapFrefAux;
	findAndCheck(mapReg, TERMINAL_SGFREF, platform.maxSG, m_numSG, mapFrefAux);
	for (const auto pair : mapFrefAux) {
		std::uint32_t aux;
		status = NiFpga_ReadU32(m_session, pair.second, &aux);
		throwIfNotSuccessNiFpga(status,
				"Error reading " + std::string(TERMINAL_SGFREF) + std::to_string(pair.first));

		m_mapFref.insert(pair);
	}
}

std::uint8_t TerminalsSignalGeneration::getSGNo() const {
	return m_numSG;
}

std::uint32_t TerminalsSignalGeneration::getSGFref(const std::uint32_t n) const {
	return m_mapFref.at(n);
}

std::uint8_t TerminalsSignalGeneration::getSGSignalType(const std::uint32_t n) const {
	auto addr = getAddressEnumResource(m_mapSignalType_addr, n, TERMINAL_SGSIGNALTYPE);

	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, addr, &aux);
	throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_SGSIGNALTYPE) + std::to_string(n));

	return aux;
}

std::uint32_t getValue(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	auto addr = getAddressEnumResource(mapTerminals, n, terminalName);

	std::int32_t aux;
	auto status = NiFpga_ReadI32(session, addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));

	return aux;
}

std::uint32_t TerminalsSignalGeneration::getSGAmp(const std::uint32_t n) const {
	return getValue(m_session, n, m_mapAmp_addr, TERMINAL_SGAMP);
}

std::uint32_t TerminalsSignalGeneration::getSGFreq(const std::uint32_t n) const {
	return getValue(m_session, n, m_mapFreq_addr, TERMINAL_SGFREQ);
}

std::uint32_t TerminalsSignalGeneration::getSGPhase(const std::uint32_t n) const {
	return getValue(m_session, n, m_mapPhase_addr, TERMINAL_SGPHASE);
}

std::uint32_t TerminalsSignalGeneration::getSGUpdateRate(const std::uint32_t n) const {
	return getValue(m_session, n, m_mapUpdateRate_addr, TERMINAL_SGUPDATERATE);
}

void TerminalsSignalGeneration::setSGSignalType(
		const std::uint32_t n,
		const std::uint8_t value) const {
	const auto it = m_mapSignalType_addr.find(n);
	if (it == m_mapSignalType_addr.end()) {
		throw std::runtime_error(
				std::to_string(n) + " is not a valid " + std::string(TERMINAL_SGSIGNALTYPE)
						+ " terminal");
	}

	auto status = NiFpga_WriteU8(m_session, it->second, value);
	throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_SGSIGNALTYPE) + std::to_string(n));
}

void setValue(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::int32_t value,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	auto it = mapTerminals.find(n);
	if (it == mapTerminals.end()) {
		throw std::runtime_error(
				std::to_string(n) + " is not a valid " + terminalName + " terminal");
	}

	auto status = NiFpga_WriteI32(session, it->second, value);
	throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));
}

void TerminalsSignalGeneration::setSGAmp(const std::uint32_t n, const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapAmp_addr, TERMINAL_SGAMP);
}

void TerminalsSignalGeneration::setSGFreq(const std::uint32_t n, const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapFreq_addr, TERMINAL_SGFREQ);
}

void TerminalsSignalGeneration::setSGPhase(const std::uint32_t n, const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapPhase_addr, TERMINAL_SGPHASE);
}

void TerminalsSignalGeneration::setSGUpdateRate(
		const std::uint32_t n,
		const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapUpdateRate_addr, TERMINAL_SGUPDATERATE);
}

}
