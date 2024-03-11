#include <profiles/profileCRIO.h>

namespace iriov2 {

ProfileCRIO::ProfileCRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id),
			m_cRIO(parsedBitfile, session) {
}

TerminalscRIO ProfileCRIO::cRIO() const {
	return m_cRIO;
}

}  // namespace iriov2
