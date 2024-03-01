#include <terminals/impl/terminalsAuxDigitalImpl.h>
#include <terminals/names/namesTerminalsAuxDigital.h>
#include <utils.h>

namespace iriov2 {

TerminalsAuxDigitalImpl::TerminalsAuxDigitalImpl(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBaseImpl(session) {
	//Find DI
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXDI, platform.maxDigital, m_mapAuxDI);

	//Find DO
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXDO, platform.maxDigital, m_mapAuxDO);

//	if ((m_mapAuxDI.size() + m_mapAuxDO.size()) > platform.maxDigital) {
//		throw std::runtime_error("More Aux Digital terminals than supported");
//	}
}

bool getAuxDigital(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	const auto addr = utils::getAddressEnumResource(mapTerminals, n, terminalName);

	std::uint8_t aux;
	auto status = NiFpga_ReadBool(session, addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));

	return static_cast<bool>(aux);
}

bool TerminalsAuxDigitalImpl::getAuxDI(const std::uint32_t n) const {
	return getAuxDigital(m_session, n, m_mapAuxDI, TERMINAL_AUXDI);
}

bool TerminalsAuxDigitalImpl::getAuxDO(const std::uint32_t n) const {
	return getAuxDigital(m_session, n, m_mapAuxDO, TERMINAL_AUXDO);
}

size_t TerminalsAuxDigitalImpl::getNumAuxDI() const {
	return m_mapAuxDI.size();
}

size_t TerminalsAuxDigitalImpl::getNumAuxDO() const {
	return m_mapAuxDO.size();
}

void TerminalsAuxDigitalImpl::setAuxDO(const std::uint32_t n, const bool value) const {
	const auto addr = utils::getAddressEnumResource(m_mapAuxDO, n, TERMINAL_AUXDO);

	auto status = NiFpga_WriteBool(m_session, addr, static_cast<NiFpga_Bool>(value));
	utils::throwIfNotSuccessNiFpga(status,
			"Error writing terminal " + std::string(TERMINAL_AUXDO) + std::to_string(n));
}
}
