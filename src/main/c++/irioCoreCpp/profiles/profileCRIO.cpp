#include <profiles/profileCRIO.h>

namespace irio {

ProfileCRIO::ProfileCRIO(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id) {
	addTerminal(TerminalscRIO(parserManager, session));
}
}  // namespace irio
