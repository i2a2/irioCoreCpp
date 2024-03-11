#include <profiles/profileFlexRIO.h>

namespace iriov2 {

ProfileFlexRIO::ProfileFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const PROFILE_ID &id):
			ProfileBase(id),
			m_flexRIO(parsedBitfile, session) {
}

TerminalsFlexRIO ProfileFlexRIO::flexRIO() const {
	return m_flexRIO;
}

}  // namespace iriov2
