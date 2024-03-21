#include "terminals/impl/terminalsFlexRIOImpl.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "utils.h"

namespace iriov2 {
TerminalsFlexRIOImpl::TerminalsFlexRIOImpl(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session) :
		TerminalsBaseImpl(session) {
	m_rioadaptercorrect_addr = parsedBitfile.getRegister(
			TERMINAL_RIOADAPTERCORRECT).getAddress();
	m_insertediomoduleid_addr = parsedBitfile.getRegister(
			TERMINAL_INSERTEDIOMODULEID).getAddress();
}

bool TerminalsFlexRIOImpl::getRIOAdapterCorrect() const {
	NiFpga_Bool aux;
	auto status = NiFpga_ReadBool(m_session, m_rioadaptercorrect_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + std::string(TERMINAL_RIOADAPTERCORRECT));
	return static_cast<bool>(aux);
}

std::uint32_t TerminalsFlexRIOImpl::getInsertedIOModuleID() const {
	std::uint32_t aux;
	auto status = NiFpga_ReadU32(m_session, m_insertediomoduleid_addr, &aux);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + std::string(TERMINAL_INSERTEDIOMODULEID));
	return aux;
}
}  // namespace iriov2

