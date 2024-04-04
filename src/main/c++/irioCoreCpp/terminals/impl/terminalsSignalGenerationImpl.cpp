#include "terminals/impl/terminalsSignalGenerationImpl.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "utils.h"
#include "errorsIrio.h"

namespace irio {

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

	std::unordered_map<std::uint32_t, const std::uint32_t> mapFrefAux;
	std::unordered_map<std::string,
		std::unordered_map<std::uint32_t, const std::uint32_t>*>
			auxTerminalInsertMap = {
				{TERMINAL_SGSIGNALTYPE, &m_mapSignalType_addr},
				{TERMINAL_SGAMP, &m_mapAmp_addr},
				{TERMINAL_SGFREQ, &m_mapFreq_addr},
				{TERMINAL_SGPHASE, &m_mapPhase_addr},
				{TERMINAL_SGUPDATERATE, &m_mapUpdateRate_addr},
				{TERMINAL_SGFREF, &mapFrefAux},
			};
	std::uint8_t sgFound = 0;
	std::uint8_t idxSG = 0;
	while(sgFound < m_numSG && idxSG < platform.maxSG) {
		std::unordered_set<std::string> notFoundRegSet;
		bool anyFound = false;
		for(auto termMap : auxTerminalInsertMap) {
			bool regFound = parserManager->findRegisterEnumAddress(
				termMap.first, idxSG, GroupResource::SG,
				termMap.second, true);
			anyFound |= regFound;
			if(!regFound) {
				notFoundRegSet.insert(termMap.first);
			}
		}
		if(anyFound) {
			sgFound++;
			for (const auto &notFoundReg : notFoundRegSet) {
				std::string resourceName = notFoundReg + std::to_string(idxSG);
				parserManager->logResourceNotFound(resourceName,
												   GroupResource::SG);
			}
		}
		idxSG++;
	}

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
}  // namespace irio
