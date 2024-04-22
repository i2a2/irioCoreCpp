#include "profiles/profileBase.h"
#include "errorsIrio.h"

namespace irio {

ProfileBase::ProfileBase(ParserManager *parserManager,
		const NiFpga_Session &session, const PROFILE_ID &id) :
		profileID(id) {
	addTerminal(TerminalsCommon(parserManager, session));
}

template<typename T>
T ProfileBase::getTerminal() const {
	const auto it = m_mapTerminals.find(std::type_index(typeid(T)));
	if(it == m_mapTerminals.end()) {
		throw errors::TerminalNotImplementedError(
				"Terminal not available for the current profile");
	}

	return *static_cast<T*>(it->second.get());
}

template TerminalsAnalog ProfileBase::getTerminal() const;
template TerminalsAuxAnalog ProfileBase::getTerminal() const;
template TerminalsDigital ProfileBase::getTerminal() const;
template TerminalsAuxDigital ProfileBase::getTerminal() const;
template TerminalsDMADAQ ProfileBase::getTerminal() const;
template TerminalsFlexRIO ProfileBase::getTerminal() const;
template TerminalscRIO ProfileBase::getTerminal() const;
template TerminalsSignalGeneration ProfileBase::getTerminal() const;
template TerminalsDMAIMAQ ProfileBase::getTerminal() const;
template TerminalsCommon ProfileBase::getTerminal() const;
template TerminalsIO ProfileBase::getTerminal() const;


template<typename T>
void ProfileBase::addTerminal(T terminal) {
	m_mapTerminals.emplace(std::type_index(typeid(T)),
			std::unique_ptr<TerminalsBase>(new T(terminal)));
}

template<>
void ProfileBase::addTerminal(TerminalsDMADAQCPU terminal) {
	m_mapTerminals.emplace(std::type_index(typeid(TerminalsDMADAQ)),
				std::unique_ptr<TerminalsBase>(new TerminalsDMADAQCPU(terminal)));
}

template <>
void ProfileBase::addTerminal(TerminalsDMAIMAQCPU terminal) {
	m_mapTerminals.emplace(std::type_index(typeid(TerminalsDMAIMAQ)),
				std::unique_ptr<TerminalsBase>(new TerminalsDMAIMAQCPU(terminal)));
}

template void ProfileBase::addTerminal(TerminalsAnalog terminal);
template void ProfileBase::addTerminal(TerminalsAuxAnalog terminal);
template void ProfileBase::addTerminal(TerminalsDigital terminal);
template void ProfileBase::addTerminal(TerminalsAuxDigital terminal);
template void ProfileBase::addTerminal(TerminalsFlexRIO terminal);
template void ProfileBase::addTerminal(TerminalscRIO terminal);
template void ProfileBase::addTerminal(TerminalsSignalGeneration terminal);
template void ProfileBase::addTerminal(TerminalsCommon terminal);
template void ProfileBase::addTerminal(TerminalsIO terminal);

}  // namespace irio
