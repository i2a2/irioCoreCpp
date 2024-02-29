#include "terminals/terminalsFlexRIO.h"
#include "terminals/impl/terminalsFlexRIOImpl.h"

namespace iriov2 {
TerminalsFlexRIO::TerminalsFlexRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session) :
		TerminalsBase(std::make_shared<TerminalsFlexRIOImpl>(parsedBitfile, session)){

}

bool TerminalsFlexRIO::getRIOAdapterCorrect() const {
	return std::static_pointer_cast<TerminalsFlexRIOImpl>(m_impl)->getRIOAdapterCorrect();
}

std::uint32_t TerminalsFlexRIO::getInsertedIOModuleID() const {
	return std::static_pointer_cast<TerminalsFlexRIOImpl>(m_impl)->getInsertedIOModuleID();
}
}

