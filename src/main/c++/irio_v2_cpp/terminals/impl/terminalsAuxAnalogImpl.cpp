#include <terminals/impl/terminalsAuxAnalogImpl.h>
#include <terminals/names/namesTerminalsAuxAnalog.h>
#include <utils.h>

namespace iriov2 {

TerminalsAuxAnalogImpl::TerminalsAuxAnalogImpl(ParserManager *parserManager,
		const NiFpga_Session &session, const Platform &platform) :
		TerminalsBaseImpl(session) {
	// Find AuxAI and Aux64AI
	for(size_t i = 0; i < platform.maxAuxAI; ++i) {
		parserManager->findRegisterEnumAddress(TERMINAL_AUXAI, i,
						GroupResource::AuxAI, &m_mapAuxAI, true);
		parserManager->findRegisterEnumAddress(TERMINAL_AUX64AI, i,
						GroupResource::AuxAI, &m_mapAuxAI64, true);
	}

	// Find AuxAO and Aux64AO
	for(size_t i = 0; i < platform.maxAuxAO; ++i) {
		parserManager->findRegisterEnumAddress(TERMINAL_AUXAO, i,
						GroupResource::AuxAO, &m_mapAuxAO, true);
		parserManager->findRegisterEnumAddress(TERMINAL_AUX64AO, i,
						GroupResource::AuxAO, &m_mapAuxAO64, true);
	}
}

std::int32_t TerminalsAuxAnalogImpl::getAuxAIImpl(const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAI, n,
			TERMINAL_AUXAI);
	std::int32_t aux;
	auto status = NiFpga_ReadI32(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUXAI)
					+ std::to_string(n));

	return aux;
}

std::int32_t TerminalsAuxAnalogImpl::getAuxAOImpl(const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO, n,
			TERMINAL_AUXAO);
	std::int32_t aux;
	auto status = NiFpga_ReadI32(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUXAO)
					+ std::to_string(n));

	return aux;
}

size_t TerminalsAuxAnalogImpl::getNumAuxAIImpl() const {
	return m_mapAuxAI.size();
}

size_t TerminalsAuxAnalogImpl::getNumAuxAOImpl() const {
	return m_mapAuxAO.size();
}

std::int64_t TerminalsAuxAnalogImpl::getAuxAI64Impl(
		const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAI64, n,
			TERMINAL_AUX64AO);
	std::int64_t aux;
	auto status = NiFpga_ReadI64(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUX64AI)
					+ std::to_string(n));

	return aux;
}

std::int64_t TerminalsAuxAnalogImpl::getAuxAO64Impl(
		const std::uint32_t n) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO64, n,
			TERMINAL_AUX64AI);
	std::int64_t aux;
	auto status = NiFpga_ReadI64(m_session, add, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUX64AO)
					+ std::to_string(n));

	return aux;
}

size_t TerminalsAuxAnalogImpl::getNumAuxAI64Impl() const {
	return m_mapAuxAI64.size();
}

size_t TerminalsAuxAnalogImpl::getNumAuxAO64Impl() const {
	return m_mapAuxAO64.size();
}

void TerminalsAuxAnalogImpl::setAuxAOImpl(const std::uint32_t n,
		const std::int32_t value) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO, n,
			TERMINAL_AUXAO);
	auto status = NiFpga_WriteI32(m_session, add, value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUXAO)
					+ std::to_string(n));
}

void TerminalsAuxAnalogImpl::setAuxAO64Impl(const std::uint32_t n,
		const std::int64_t value) const {
	const std::uint32_t add = utils::getAddressEnumResource(m_mapAuxAO64, n,
			TERMINAL_AUX64AO);
	auto status = NiFpga_WriteI64(m_session, add, value);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading terminal " + std::string(TERMINAL_AUX64AO)
					+ std::to_string(n));
}

}  // namespace iriov2
