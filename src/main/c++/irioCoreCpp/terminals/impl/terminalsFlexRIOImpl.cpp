#include "terminals/impl/terminalsFlexRIOImpl.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "utils.h"

namespace irio {
TerminalsFlexRIOImpl::TerminalsFlexRIOImpl(ParserManager *parserManager,
		const NiFpga_Session &session) :
		TerminalsBaseImpl(session) {
	parserManager->findRegisterAddress(TERMINAL_RIOADAPTERCORRECT,
			GroupResource::FlexRIO, &m_rioadaptercorrect_addr, false);
	parserManager->findRegisterAddress(TERMINAL_INSERTEDIOMODULEID,
			GroupResource::FlexRIO, &m_insertediomoduleid_addr, false);
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
}  // namespace irio

