#include "terminals/terminalscRIO.h"
#include <terminals/impl/terminalscRIOImpl.h>
#include <memory>

namespace irio {
TerminalscRIO::TerminalscRIO(ParserManager *parserManager,
		const NiFpga_Session &session) :
		TerminalsBase(
				std::make_shared<TerminalscRIOImpl>(parserManager, session)) {
}

bool TerminalscRIO::getcRIOModulesOk() const {
	return std::static_pointer_cast<TerminalscRIOImpl>(m_impl)->getcRIOModulesOk();
}

std::vector<std::uint16_t> TerminalscRIO::getInsertedIOModulesID() const {
	return std::static_pointer_cast<TerminalscRIOImpl>(m_impl)
			->getInsertedIOModulesID();
}
}  // namespace iriov2
