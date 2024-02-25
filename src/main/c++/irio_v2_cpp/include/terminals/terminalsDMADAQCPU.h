#pragma once

#include <terminals/terminalsDMADAQ.h>

namespace iriov2{

class TerminalsDMADAQCPU: public TerminalsDMADAQ{
public:
	TerminalsDMADAQCPU(const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform);
};

}


