#include <terminalsAnalog.h>
#include <terminals/namesTerminalsAnalog.h>
#include <utils.h>

namespace iriov2{

TerminalsAnalog::TerminalsAnalog(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		Platform& platform):
		m_session(session)
{
	//Find AI
	findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AI, platform.maxAI, m_mapAI);

	//Find AO
	findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AO, platform.maxAO, m_mapAO);

	//Find AOEnable
	findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AOENABLE, platform.maxAO, m_mapAOEnable);

	if(m_mapAO.size() != m_mapAOEnable.size()){
		throw std::runtime_error("Mismatch in number of AO and AOEnable terminals");
	}
}

std::int32_t getAnalog(
		const NiFpga_Session &session,
		const std::uint8_t n,
		const std::unordered_map<std::uint8_t, const std::uint32_t> &mapTerminals,
		const std::string& terminalName)
{
	auto it = mapTerminals.find(n);
	if(it == mapTerminals.end()){
		throw std::runtime_error(std::to_string(n) + " is not a valid " + terminalName + " terminal");
	}

	std::int32_t aux;
	auto status = NiFpga_ReadI32(session, it->second, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));

	return aux;
}

std::int32_t TerminalsAnalog::getAI(const std::uint8_t n) const{
	return getAnalog(m_session, n, m_mapAI, TERMINAL_AI);
}

std::int32_t TerminalsAnalog::getAO(const std::uint8_t n) const {
	return getAnalog(m_session, n, m_mapAO, TERMINAL_AO);
}

std::int32_t TerminalsAnalog::getAOEnable(std::uint8_t n) const {
	return getAnalog(m_session, n, m_mapAOEnable, TERMINAL_AOENABLE);
}

void setAnalog(
		const NiFpga_Session &session,
		const std::uint8_t n,
		const std::int32_t value,
		const std::unordered_map<std::uint8_t, const std::uint32_t> &mapTerminals,
		const std::string& terminalName)
{
	auto it = mapTerminals.find(n);
	if(it == mapTerminals.end()){
		throw std::runtime_error(std::to_string(n) + " is not a valid " + terminalName + " terminal");
	}

	auto status = NiFpga_WriteI32(session, it->second, value);
	throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));
}

void TerminalsAnalog::setAO(const std::uint8_t n, const std::int32_t value) const {
	setAnalog(m_session, n, value, m_mapAO, TERMINAL_AO);
}

void TerminalsAnalog::setAOEnable(std::uint8_t n, bool value) const{
	setAnalog(m_session, n, static_cast<std::uint32_t>(value), m_mapAOEnable, TERMINAL_AOENABLE);
}

}
