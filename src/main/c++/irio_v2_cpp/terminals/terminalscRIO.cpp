#include "terminals/terminalscRIO.h"

namespace iriov2 {
TerminalscRIO::TerminalscRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session) :
		m_impl(new TerminalscRIOImpl(parsedBitfile, session)) {
}

TerminalscRIO::TerminalscRIO(const TerminalscRIO &other){
	m_impl = other.m_impl;
}

bool TerminalscRIO::getcRIOModulesOk() const {
	return m_impl->getcRIOModulesOk();
}

std::vector<std::uint16_t> TerminalscRIO::getInsertedIOModulesID() const {
	return m_impl->getInsertedIOModulesID();
}
}
