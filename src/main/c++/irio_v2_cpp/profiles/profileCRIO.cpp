#include <profiles/profileCRIO.h>

namespace iriov2 {

ProfileCRIO::ProfileCRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id) {
	addTerminal(TerminalscRIO(parsedBitfile, session));
}
}  // namespace iriov2
