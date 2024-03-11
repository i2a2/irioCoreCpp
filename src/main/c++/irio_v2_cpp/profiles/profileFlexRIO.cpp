#include <profiles/profileFlexRIO.h>

namespace iriov2 {

ProfileFlexRIO::ProfileFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id) {
	addTerminal(TerminalsFlexRIO(parsedBitfile, session));
}
}  // namespace iriov2
