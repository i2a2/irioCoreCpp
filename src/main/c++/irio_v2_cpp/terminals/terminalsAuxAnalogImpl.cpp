#include <terminals/terminalsAuxAnalogImpl.h>
#include <terminals/names/namesTerminalsAuxAnalog.h>
#include <utils.h>

namespace iriov2 {

TerminalsAuxAnalogImpl::TerminalsAuxAnalogImpl(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBase(session) {
	//Find AuxAI
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXAI, platform.maxAuxAI, m_mapAuxAI);

	//Find AuxAO
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXAO, platform.maxAuxAO, m_mapAuxAO);

	//Find Aux64AI
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUX64AI, platform.maxAuxAI, m_mapAuxAI64);

	//Find Aux64AO
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUX64AO, platform.maxAuxAO, m_mapAuxAO64);
}

std::int32_t TerminalsAuxAnalogImpl::getAuxAI(const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAI, n, TERMINAL_AUXAI);
	std::int32_t aux;
	auto status = NiFpga_ReadI32(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUXAI) + std::to_string(n));

	return aux;
}

std::int32_t TerminalsAuxAnalogImpl::getAuxAO(const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO, n, TERMINAL_AUXAO);
	std::int32_t aux;
	auto status = NiFpga_ReadI32(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUXAO) + std::to_string(n));

	return aux;
}

size_t TerminalsAuxAnalogImpl::getNumAuxAI() const {
	return m_mapAuxAI.size();
}

size_t TerminalsAuxAnalogImpl::getNumAuxAO() const {
	return m_mapAuxAO.size();
}

std::int64_t TerminalsAuxAnalogImpl::getAuxAI64(const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAI64, n, TERMINAL_AUX64AO);
	std::int64_t aux;
	auto status = NiFpga_ReadI64(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUX64AI) + std::to_string(n));

	return aux;
}

std::int64_t TerminalsAuxAnalogImpl::getAuxAO64(const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO64, n, TERMINAL_AUX64AI);
	std::int64_t aux;
	auto status = NiFpga_ReadI64(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUX64AO) + std::to_string(n));

	return aux;
}

size_t TerminalsAuxAnalogImpl::getNumAuxAI64() const {
	return m_mapAuxAI64.size();
}

size_t TerminalsAuxAnalogImpl::getNumAuxAO64() const {
	return m_mapAuxAO64.size();
}

void TerminalsAuxAnalogImpl::setAuxAO(const std::uint32_t n, const std::int32_t value) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO, n, TERMINAL_AUXAO);
	auto status = NiFpga_WriteI32(m_session, add, value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUXAO) + std::to_string(n));
}

void TerminalsAuxAnalogImpl::setAuxAO64(const std::uint32_t n, const std::int64_t value) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO64, n, TERMINAL_AUX64AO);
	auto status = NiFpga_WriteI64(m_session, add, value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUX64AO) + std::to_string(n));

}

}
