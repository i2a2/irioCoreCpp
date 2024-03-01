#include <modules.h>

namespace iriov2{

Module::Module(const ModulesType &id) :
		moduleID(id) {
}

void Module::addConfig(const CouplingMode& mode, const ConfigParams& config){
	m_availableConfigs.emplace(mode, config);
}

void Module::setCouplingMode(const CouplingMode &mode) {
	const auto it = m_availableConfigs.find(mode);
	if (it == m_availableConfigs.end()) {
		throw errors::UnsupportedAICouplingForModule();
	}
	m_couplingMode = mode;
	m_currentConfig = it->second;
}

CouplingMode Module::getCouplingMode() const {
	return m_couplingMode;
}

double Module::getCVADC() const {
	return m_currentConfig.CVADC;
}

double Module::getCVDAC() const {
	return m_currentConfig.CVDAC;
}

double Module::getMaxValAO() const {
	return m_currentConfig.maxValAO;
}

double Module::getMinValAO() const {
	return m_currentConfig.minValAO;
}

ModuleNI5761::ModuleNI5761() :
		Module(ModulesType::FlexRIO_NI5761) {
	addConfig(CouplingMode::AC, m_configAC);
	addConfig(CouplingMode::DC, m_configDC);
	setCouplingMode(CouplingMode::AC);
}

ModuleNI6581::ModuleNI6581() :
		Module(ModulesType::FlexRIO_NI6581) {
	addConfig(CouplingMode::None, m_configNone);
	setCouplingMode(CouplingMode::None);
}

ModuleNI5734::ModuleNI5734() :
		Module(ModulesType::FlexRIO_NI5734) {
	addConfig(CouplingMode::None, m_configNone);
	setCouplingMode(CouplingMode::None);
}

ModuleNI5781::ModuleNI5781() :
		Module(ModulesType::FlexRIO_NI5781) {
	addConfig(CouplingMode::DC, m_configDC);
	setCouplingMode(CouplingMode::DC);
}

ModuleNI92xx::ModuleNI92xx() :
		Module(ModulesType::cRIO_NI92xx) {
	addConfig(CouplingMode::DC, m_configDC);
	setCouplingMode(CouplingMode::DC);
}

}
