#include "terminals/terminalsBase.h"

namespace iriov2{

TerminalsBase::TerminalsBase():
		activeInProfile(false), m_session(0) {}

TerminalsBase::TerminalsBase(const NiFpga_Session &session):
		activeInProfile(true), m_session(session) {}

}
