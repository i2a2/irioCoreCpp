#include <terminals/terminalsDigital.h>
#include <terminals/names/namesTerminalsDigital.h>
#include <utils.h>

namespace iriov2 {

TerminalsDigital::TerminalsDigital(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBase(session) {
	//Find DI
	findAndInsertEnumRegisters(parsedBitfile, TERMINAL_DI, platform.maxDigital, m_mapDI);

	//Find DO
	findAndInsertEnumRegisters(parsedBitfile, TERMINAL_DO, platform.maxDigital, m_mapDO);

	if ((m_mapDI.size() + m_mapDO.size()) > platform.maxDigital) {
		throw std::runtime_error("More Digital terminals than supported");
	}
}

bool getDigital(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	const auto addr = getAddressEnumResource(mapTerminals, n, terminalName);

	std::uint8_t aux;
	auto status = NiFpga_ReadBool(session, addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));

	return static_cast<bool>(aux);
}

bool TerminalsDigital::getDI(const std::uint32_t n) const {
	return getDigital(m_session, n, m_mapDI, TERMINAL_DI);
}

bool TerminalsDigital::getD0(const std::uint32_t n) const {
	return getDigital(m_session, n, m_mapDO, TERMINAL_DO);
}

void TerminalsDigital::setDO(const std::uint32_t n, const bool value) const {
	const auto addr = getAddressEnumResource(m_mapDO, n, TERMINAL_DO);

	auto status = NiFpga_WriteBool(m_session, addr, static_cast<NiFpga_Bool>(value));
	throwIfNotSuccessNiFpga(status,
			"Error writing terminal " + std::string(TERMINAL_DO) + std::to_string(n));
}
}
