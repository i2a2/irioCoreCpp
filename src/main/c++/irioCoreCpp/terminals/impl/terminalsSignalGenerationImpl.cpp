#include "terminals/impl/terminalsSignalGenerationImpl.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "utils.h"
#include "errorsIrio.h"

namespace irio {

void findAndCheck(
		ParserManager *parserManager,
		const std::string &terminalName,
		const size_t maxNum,
		const size_t expectedNum,
		std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert) {
	for(size_t i = 0; i < maxNum; ++i) {
		parserManager->findRegisterEnumAddress(terminalName, i,
						GroupResource::SG, mapInsert, true);
	}

	if (mapInsert->size() != expectedNum) {
		const std::string errMsg =
			"Number of " + terminalName + " found (" +
			std::to_string(mapInsert->size()) +
			") does not match the number " + "of SGs expected (" +
			std::to_string(expectedNum) + ")";
		parserManager->logResourceError(terminalName, errMsg,
										GroupResource::DAQ);
	}
}

TerminalsSignalGenerationImpl::TerminalsSignalGenerationImpl(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBaseImpl(session) {
	NiFpga_Status status;

	std::uint32_t addrSGNO;
	if (!parserManager->findRegisterAddress(TERMINAL_SGNO, GroupResource::SG,
			&addrSGNO, true)) {
		// There are no signal generators, go back
		return;
	}
	status = NiFpga_ReadU8(m_session, addrSGNO, &m_numSG);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + std::string(TERMINAL_SGNO));

	// Find SignalType
	findAndCheck(parserManager, TERMINAL_SGSIGNALTYPE,
			platform.maxSG, m_numSG, &m_mapSignalType_addr);

	// Find Amp
	findAndCheck(parserManager, TERMINAL_SGAMP,
			platform.maxSG, m_numSG, &m_mapAmp_addr);

	// Find Freq
	findAndCheck(parserManager, TERMINAL_SGFREQ,
			platform.maxSG, m_numSG, &m_mapFreq_addr);

	// Find Phase
	findAndCheck(parserManager, TERMINAL_SGPHASE,
			platform.maxSG, m_numSG, &m_mapPhase_addr);

	// Find Update Rate
	findAndCheck(parserManager, TERMINAL_SGUPDATERATE,
			platform.maxSG, m_numSG, &m_mapUpdateRate_addr);

	// Read all Fref
	std::unordered_map<std::uint32_t, const std::uint32_t> mapFrefAux;
	findAndCheck(parserManager, TERMINAL_SGFREF, platform.maxSG, m_numSG,
			&mapFrefAux);
	for (const auto pair : mapFrefAux) {
		std::uint32_t aux;
		status = NiFpga_ReadU32(m_session, pair.second, &aux);
		utils::throwIfNotSuccessNiFpga(status,
				"Error reading " + std::string(TERMINAL_SGFREF)
				+ std::to_string(pair.first));

		m_mapFref.insert({pair.first, aux});
	}
}

std::uint8_t TerminalsSignalGenerationImpl::getSGNoImpl() const {
	return m_numSG;
}

std::uint32_t TerminalsSignalGenerationImpl::getSGFrefImpl(
		const std::uint32_t n) const {
	const auto it = m_mapFref.find(n);
	if(it == m_mapFref.end()) {
		throw errors::ResourceNotFoundError(n, TERMINAL_SGFREF);
	}
	return it->second;
}

std::uint8_t TerminalsSignalGenerationImpl::getSGSignalTypeImpl(
		const std::uint32_t n) const {
	auto addr = utils::getAddressEnumResource(m_mapSignalType_addr,
			n, TERMINAL_SGSIGNALTYPE);

	std::uint8_t aux;
	auto status = NiFpga_ReadU8(m_session, addr, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal "
			+ std::string(TERMINAL_SGSIGNALTYPE) + std::to_string(n));

	return aux;
}

std::uint32_t getValue(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	auto addr = utils::getAddressEnumResource(mapTerminals, n, terminalName);

	std::uint32_t aux;
	auto status = NiFpga_ReadU32(session, addr, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + terminalName + std::to_string(n));

	return aux;
}

std::uint32_t TerminalsSignalGenerationImpl::getSGAmpImpl(
		const std::uint32_t n) const {
	return getValue(m_session, n, m_mapAmp_addr, TERMINAL_SGAMP);
}

std::uint32_t TerminalsSignalGenerationImpl::getSGFreqImpl(
		const std::uint32_t n) const {
	return getValue(m_session, n, m_mapFreq_addr, TERMINAL_SGFREQ);
}

std::uint32_t TerminalsSignalGenerationImpl::getSGPhaseImpl(
		const std::uint32_t n) const {
	return getValue(m_session, n, m_mapPhase_addr, TERMINAL_SGPHASE);
}

std::uint32_t TerminalsSignalGenerationImpl::getSGUpdateRateImpl(
		const std::uint32_t n) const {
	return getValue(m_session, n, m_mapUpdateRate_addr, TERMINAL_SGUPDATERATE);
}

void TerminalsSignalGenerationImpl::setSGSignalTypeImpl(
		const std::uint32_t n,
		const std::uint8_t value) const {
	const auto addr = utils::getAddressEnumResource(
			m_mapSignalType_addr, n, TERMINAL_SGSIGNALTYPE);

	auto status = NiFpga_WriteU8(m_session, addr, value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal "
			+ std::string(TERMINAL_SGSIGNALTYPE) + std::to_string(n));
}

void setValue(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::uint32_t value,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	const auto addr = utils::getAddressEnumResource(mapTerminals, n, terminalName);

	auto status = NiFpga_WriteU32(session, addr, value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + terminalName + std::to_string(n));
}

void TerminalsSignalGenerationImpl::setSGAmpImpl(
		const std::uint32_t n, const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapAmp_addr, TERMINAL_SGAMP);
}

void TerminalsSignalGenerationImpl::setSGFreqImpl(
		const std::uint32_t n, const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapFreq_addr, TERMINAL_SGFREQ);
}

void TerminalsSignalGenerationImpl::setSGPhaseImpl(
		const std::uint32_t n, const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapPhase_addr, TERMINAL_SGPHASE);
}

void TerminalsSignalGenerationImpl::setSGUpdateRateImpl(
		const std::uint32_t n,
		const std::uint32_t value) const {
	setValue(m_session, n, value, m_mapUpdateRate_addr, TERMINAL_SGUPDATERATE);
}
}  // namespace iriov2
