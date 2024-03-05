#include "terminals/impl/terminalscRIOImpl.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "utils.h"

namespace iriov2 {
TerminalscRIOImpl::TerminalscRIOImpl(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session) :
		TerminalsBaseImpl(session) {
	m_criomodulesok_addr =
			parsedBitfile.getRegister(TERMINAL_CRIOMODULESOK).address;
	const auto regModulesID = parsedBitfile.getRegister(
			TERMINAL_INSERTEDIOMODULESID);
	m_insertediomodulesid_addr = regModulesID.address;
	m_numModules = regModulesID.numElem;
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
