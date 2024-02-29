#include "terminals/terminalsFlexRIO.h"

namespace iriov2 {
TerminalsFlexRIO::TerminalsFlexRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session) :
		m_impl(new TerminalsFlexRIOImpl(parsedBitfile, session)){

}

TerminalsFlexRIO::TerminalsFlexRIO(const TerminalsFlexRIO &other){
	m_impl = other.m_impl;
}

bool TerminalsFlexRIO::getRIOAdapterCorrect() const {
	return m_impl->getRIOAdapterCorrect();
}

std::uint32_t TerminalsFlexRIO::getInsertedIOModuleID() const {
	return m_impl->getInsertedIOModuleID();
}
}

