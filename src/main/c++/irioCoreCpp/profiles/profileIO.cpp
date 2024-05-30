#include "profiles/profileIO.h"

namespace irio {
ProfileIO::ProfileIO(ParserManager *parserManager,
							 const NiFpga_Session &session,
							 const Platform &platform,
                             const PROFILE_ID &id)
	: ProfileBase(parserManager, session, id) {
	addTerminal(TerminalsAnalog(parserManager, session, platform));
	addTerminal(TerminalsDigital(parserManager, session, platform));
	addTerminal(TerminalsAuxAnalog(parserManager, session, platform));
	addTerminal(TerminalsAuxDigital(parserManager, session, platform));
	addTerminal(TerminalsSignalGeneration(parserManager, session, platform));
	addTerminal(TerminalsIO(parserManager, session, platform));
}
}  // namespace irio
