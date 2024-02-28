#include <terminals/terminalsAnalog.h>
#include <terminals/names/namesTerminalsAnalog.h>
#include <utils.h>
#include <errorsIrio.h>
#include <niflexrio.h>

namespace iriov2 {

TerminalsAnalog::TerminalsAnalog(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform) :
		TerminalsBase(session) {
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

std::int32_t TerminalsAnalog::getAI(const std::uint32_t n) const {
	return getAnalog(m_session, n, m_mapAI, TERMINAL_AI);
}

std::int32_t TerminalsAnalog::getAO(const std::uint32_t n) const {
	return getAnalog(m_session, n, m_mapAO, TERMINAL_AO);
}

std::int32_t TerminalsAnalog::getAOEnable(std::uint32_t n) const {
	return getAnalog(m_session, n, m_mapAOEnable, TERMINAL_AOENABLE);
}

size_t TerminalsAnalog::getNumAI() const {
	return numAI;
}

size_t TerminalsAnalog::getNumAO() const {
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

void TerminalsAnalog::setAO(const std::uint32_t n, const std::int32_t value) const {
	setAnalog(m_session, n, value, m_mapAO, TERMINAL_AO);
}

void TerminalsAnalog::setAOEnable(std::uint32_t n, bool value) const {
	setAnalog(m_session, n, static_cast<std::uint32_t>(value), m_mapAOEnable, TERMINAL_AOENABLE);
}

ModulesType TerminalsAnalog::getModuleConnected() const {
	return m_module->moduleID;
}

double TerminalsAnalog::getCVADC() const {
	return m_module->getCVADC();
}

double TerminalsAnalog::getCVDAC() const {
	return m_module->getCVDAC();
}

double TerminalsAnalog::getMaxValAO() const {
	return m_module->getMaxValAO();
}

double TerminalsAnalog::getMinValAO() const {
	return m_module->getMinValAO();
}

void TerminalsAnalog::setAICouplingMode(const CouplingMode &mode) {
	m_module->setCouplingMode(mode);
}

void TerminalsAnalog::searchModule(const Platform& platform) {
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
		break;
	}
}

void TerminalsAnalog::searchFlexRIOModule() {
	std::uint32_t module;
	NiFlexRio_GetAttribute(m_session, NIFLEXRIO_Attr_InsertedFamID,
			NIFLEXRIO_ValueType_U32, &module);
	switch(module){
	case FlexRIO_NI5761:
		m_module.reset(new ModuleNI5761());
		break;
	case FlexRIO_NI5781:
		m_module.reset(new ModuleNI5781());
		break;
	case FlexRIO_NI6581:
		m_module.reset(new ModuleNI6581());
		break;
	case FlexRIO_NI5734:
		m_module.reset(new ModuleNI5734());
		break;
	default:
		m_module.reset(new Module());
		break;
	}
}

}
