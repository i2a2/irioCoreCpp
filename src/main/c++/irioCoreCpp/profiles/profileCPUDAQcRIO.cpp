#include <profiles/profileCPUDAQcRIO.h>
#include <terminals/terminalsCRIO.h>

namespace irio {

ProfileCPUDAQcRIO::ProfileCPUDAQcRIO(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const Platform &platform) :
				ProfileCPUDAQ(parserManager, session,
						platform, PROFILE_ID::FLEXRIO_CPUDAQ) {
	addTerminal(TerminalscRIO(parserManager, session));
}
}  // namespace irio
