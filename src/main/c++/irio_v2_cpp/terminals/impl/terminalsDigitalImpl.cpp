#include <terminals/impl/terminalsDigitalImpl.h>
#include <terminals/names/namesTerminalsDigital.h>
#include <utils.h>

namespace iriov2 {

TerminalsDigitalImpl::TerminalsDigitalImpl(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBaseImpl(session) {
	// Find DI
	utils::findAndInsertEnumRegisters(
			parsedBitfile, TERMINAL_DI, platform.maxDigital, &m_mapDI);

	// Find DO
	utils::findAndInsertEnumRegisters(
			parsedBitfile, TERMINAL_DO, platform.maxDigital, &m_mapDO);
}

bool getDigital(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {
	const auto addr = utils::getAddressEnumResource(mapTerminals, n, terminalName);

	std::uint8_t aux;
	auto status = NiFpga_ReadBool(session, addr, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + terminalName + std::to_string(n));

	return static_cast<bool>(aux);
}

bool TerminalsDigitalImpl::getDI(const std::uint32_t n) const {
	return getDigital(m_session, n, m_mapDI, TERMINAL_DI);
}

bool TerminalsDigitalImpl::getDO(const std::uint32_t n) const {
	return getDigital(m_session, n, m_mapDO, TERMINAL_DO);
}

size_t TerminalsDigitalImpl::getNumDI() const {
	return m_mapDI.size();
}

size_t TerminalsDigitalImpl::getNumDO() const {
	return m_mapDO.size();
}

void TerminalsDigitalImpl::setDO(
		const std::uint32_t n, const bool value) const {
	const auto addr = utils::getAddressEnumResource(m_mapDO, n, TERMINAL_DO);

	auto status = NiFpga_WriteBool(
			m_session, addr, static_cast<NiFpga_Bool>(value));
	utils::throwIfNotSuccessNiFpga(status,
			"Error writing terminal " + std::string(TERMINAL_DO) + std::to_string(n));
}
}  // namespace iriov2
