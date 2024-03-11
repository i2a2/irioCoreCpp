#include <profiles/profileCPUDAQ.h>
#include <terminals/terminalsDMADAQCPU.h>

namespace iriov2 {

ProfileCPUDAQ::ProfileCPUDAQ(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session, const Platform &platform,
		const PROFILE_ID &id) :
		ProfileBase(id) {
	addTerminal(TerminalsAnalog(parsedBitfile, session, platform));
	addTerminal(TerminalsDigital(parsedBitfile, session, platform));
	addTerminal(TerminalsAuxAnalog(parsedBitfile, session, platform));
	addTerminal(TerminalsAuxDigital(parsedBitfile, session, platform));
	addTerminal(TerminalsSignalGeneration(parsedBitfile, session, platform));
	addTerminal(TerminalsDMADAQCPU(parsedBitfile, session, platform));
}

}  // namespace iriov2
