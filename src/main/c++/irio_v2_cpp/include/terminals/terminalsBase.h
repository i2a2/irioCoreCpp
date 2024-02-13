#pragma once

#include <NiFpga.h>

namespace iriov2{

class TerminalsBase
{
public:
	TerminalsBase();
	TerminalsBase(const NiFpga_Session &session);

	const bool activeInProfile;

protected:
	const NiFpga_Session m_session;

};

}


