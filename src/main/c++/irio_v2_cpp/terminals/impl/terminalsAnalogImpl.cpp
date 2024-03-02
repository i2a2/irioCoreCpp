#include <terminals/impl/terminalsAnalogImpl.h>
#include <terminals/names/namesTerminalsAnalog.h>
#include <utils.h>
#include <errorsIrio.h>
#include <niflexrio.h>

namespace iriov2 {

TerminalsAnalogImpl::TerminalsAnalogImpl(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBaseImpl(session) {
	//Find AI
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AI, platform.maxAI, m_mapAI);

	//Find AO
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AO, platform.maxAO, m_mapAO);

	//Find AOEnable
	utils::findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AOENABLE, platform.maxAO, m_mapAOEnable);

	if (m_mapAO.size() != m_mapAOEnable.size()) {
		throw errors::ResourceNotFoundError("Mismatch in number of AO and AOEnable terminals");
	}

	numAI = m_mapAI.size();
	numAO = m_mapAO.size();

	searchModule(platform);
}

std::int32_t getAnalog(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {

	auto addr = utils::getAddressEnumResource(mapTerminals, n, terminalName);

	std::int32_t aux;
	auto status = NiFpga_ReadI32(session, addr, &aux);
	utils::throwIfNotSuccessNiFpga(status, "Error reading terminal " + terminalName + std::to_string(n));

	return aux;
}

std::int32_t TerminalsAnalogImpl::getAIImpl(const std::uint32_t n) const {
	return getAnalog(m_session, n, m_mapAI, TERMINAL_AI);
}

std::int32_t TerminalsAnalogImpl::getAOImpl(const std::uint32_t n) const {
	return getAnalog(m_session, n, m_mapAO, TERMINAL_AO);
}

std::int32_t TerminalsAnalogImpl::getAOEnableImpl(std::uint32_t n) const {
	return getAnalog(m_session, n, m_mapAOEnable, TERMINAL_AOENABLE);
}

size_t TerminalsAnalogImpl::getNumAIImpl() const {
	return numAI;
}

size_t TerminalsAnalogImpl::getNumAOImpl() const {
	return numAO;
}

void setAnalog(
		const NiFpga_Session &session,
		const std::uint32_t n,
		const std::int32_t value,
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
		const std::string &terminalName) {

	auto addr = utils::getAddressEnumResource(mapTerminals, n, terminalName);

	auto status = NiFpga_WriteI32(session, addr, value);
	utils::throwIfNotSuccessNiFpga(status, "Error writing terminal " + terminalName + std::to_string(n));
}

void TerminalsAnalogImpl::setAOImpl(const std::uint32_t n, const std::int32_t value) const {
	setAnalog(m_session, n, value, m_mapAO, TERMINAL_AO);
}

void TerminalsAnalogImpl::setAOEnableImpl(std::uint32_t n, bool value) const {
	setAnalog(m_session, n, static_cast<std::uint32_t>(value), m_mapAOEnable, TERMINAL_AOENABLE);
}

ModulesType TerminalsAnalogImpl::getModuleConnectedImpl() const {
	return m_module->moduleID;
}

double TerminalsAnalogImpl::getCVADCImpl() const {
	return m_module->getCVADC();
}

double TerminalsAnalogImpl::getCVDACImpl() const {
	return m_module->getCVDAC();
}

double TerminalsAnalogImpl::getMaxValAOImpl() const {
	return m_module->getMaxValAO();
}

double TerminalsAnalogImpl::getMinValAOImpl() const {
	return m_module->getMinValAO();
}

CouplingMode TerminalsAnalogImpl::getAICouplingModeImpl() const {
	return m_module->getCouplingMode();
}

void TerminalsAnalogImpl::setAICouplingModeImpl(const CouplingMode &mode) const {
	m_module->setCouplingMode(mode);
}

void TerminalsAnalogImpl::searchModule(const Platform& platform) {
	switch(platform.platformID){
	case FLEXRIO_PLATFORM_VALUE:
		searchFlexRIOModule();
		break;
	case CRIO_PLATFORM_VALUE:
		//TODO: How to support more modules in cRIO?
		m_module.reset(new ModuleNI92xx());
		break;
	case RSERIES_PLATFORM_VALUE:
		//TODO: Constants depend on the board model; make list
		break;
	default:
		m_module.reset(new Module());
	}
}

void TerminalsAnalogImpl::searchFlexRIOModule() {
	std::uint32_t moduleID;
	NiFlexRio_GetAttribute(m_session, NIFLEXRIO_Attr_InsertedFamID,
			NIFLEXRIO_ValueType_U32, &moduleID);
	switch(moduleID){
	case utils::enum2underlying(ModulesType::FlexRIO_NI5761):
		m_module.reset(new ModuleNI5761());
		break;
	case utils::enum2underlying(ModulesType::FlexRIO_NI5781):
		m_module.reset(new ModuleNI5781());
		break;
	case utils::enum2underlying(ModulesType::FlexRIO_NI6581):
		m_module.reset(new ModuleNI6581());
		break;
	case utils::enum2underlying(ModulesType::FlexRIO_NI5734):
		m_module.reset(new ModuleNI5734());
		break;
	default:
		m_module.reset(new Module());
	}
}

}
