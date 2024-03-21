#include "terminals/impl/terminalscRIOImpl.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "utils.h"

namespace iriov2 {
TerminalscRIOImpl::TerminalscRIOImpl(ParserManager *parserManager,
		const NiFpga_Session &session) :
		TerminalsBaseImpl(session) {
	parserManager->findRegisterAddress(TERMINAL_CRIOMODULESOK,
			GroupResource::CRIO, &m_criomodulesok_addr, false);

	bfp::Register regModulesID;
	if(parserManager->findRegister(TERMINAL_INSERTEDIOMODULESID,
			GroupResource::CRIO, &regModulesID, false)) {
		m_insertediomodulesid_addr = regModulesID.getAddress();
		m_numModules = regModulesID.getNumElem();
	}
}

bool TerminalscRIOImpl::getcRIOModulesOk() const {
	NiFpga_Bool aux;
	auto status = NiFpga_ReadBool(m_session, m_criomodulesok_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + std::string(TERMINAL_CRIOMODULESOK));
	return static_cast<bool>(aux);
}

std::vector<std::uint16_t> TerminalscRIOImpl::getInsertedIOModulesID() const {
	static std::vector<std::uint16_t> ret(m_numModules);
	auto status = NiFpga_ReadArrayU16(m_session, m_insertediomodulesid_addr,
			ret.data(), m_numModules);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + std::string(TERMINAL_INSERTEDIOMODULESID));
	return ret;
}
}  // namespace iriov2
