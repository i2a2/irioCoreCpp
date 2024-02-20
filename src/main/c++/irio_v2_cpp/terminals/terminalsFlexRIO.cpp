#include "terminals/terminalsFlexRIO.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "utils.h"

namespace iriov2 {
TerminalsFlexRIO::TerminalsFlexRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session) :
		TerminalsBase(session) {
	m_rioadaptercorrect_addr = parsedBitfile.getRegister(TERMINAL_RIOADAPTERCORRECT).address;
	m_insertediomoduleid_addr = parsedBitfile.getRegister(TERMINAL_INSERTEDIOMODULEID).address;
}

bool TerminalsFlexRIO::getRIOAdapterCorrect() const {
	NiFpga_Bool aux;
	auto status = NiFpga_ReadBool(m_session, m_rioadaptercorrect_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading " + std::string(TERMINAL_RIOADAPTERCORRECT));
	return static_cast<bool>(aux);
}

std::uint32_t TerminalsFlexRIO::getInsertedIOModuleID() const {
	std::uint32_t aux;
	auto status = NiFpga_ReadU32(m_session, m_insertediomoduleid_addr, &aux);
	throwIfNotSuccessNiFpga(status, "Error reading " + std::string(TERMINAL_INSERTEDIOMODULEID));
	return aux;
}
}
