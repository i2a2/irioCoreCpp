#pragma once

#include <NiFpga.h>

namespace iriov2{

class TerminalsBase
{
public:
	TerminalsBase(const NiFpga_Session &session);

protected:
	const NiFpga_Session m_session;

};

}


