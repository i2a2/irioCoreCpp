#include <profiles/profileFlexRIO.h>

namespace irio {

ProfileFlexRIO::ProfileFlexRIO(
		ParserManager *parserManager,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id) {
	addTerminal(TerminalsFlexRIO(parserManager, session));
}
}  // namespace iriov2
