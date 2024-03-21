#include <profiles/profileCRIO.h>

namespace iriov2 {

ProfileCRIO::ProfileCRIO(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id) {
	addTerminal(TerminalscRIO(parserManager, session));
}
}  // namespace iriov2
