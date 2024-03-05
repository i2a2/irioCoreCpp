#include "terminals/terminalscRIO.h"
#include <terminals/impl/terminalscRIOImpl.h>
#include <memory>

namespace iriov2 {
TerminalscRIO::TerminalscRIO(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session) :
		TerminalsBase(
				std::make_shared<TerminalscRIOImpl>(parsedBitfile, session)) {
}

bool TerminalscRIO::getcRIOModulesOk() const {
	return std::static_pointer_cast<TerminalscRIOImpl>(m_impl)->getcRIOModulesOk();
}

std::vector<std::uint16_t> TerminalscRIO::getInsertedIOModulesID() const {
	return std::static_pointer_cast<TerminalscRIOImpl>(m_impl)
			->getInsertedIOModulesID();
}
}  // namespace iriov2
