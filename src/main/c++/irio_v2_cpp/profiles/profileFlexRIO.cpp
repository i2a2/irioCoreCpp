#include <profiles/profileFlexRIO.h>

namespace iriov2{

ProfileFlexRIO::ProfileFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const ProfileBase::PROFILE_ID &id):
			ProfileBase(parsedBitfile, session, id),
			m_flexRIO(new TerminalsFlexRIO(parsedBitfile, session)){
}

const std::shared_ptr<const TerminalsFlexRIO> ProfileFlexRIO::flexRIO() const {
	return m_flexRIO;
}

}