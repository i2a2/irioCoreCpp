#include <profiles/profileDAQFlexRIO.h>

namespace iriov2 {

ProfileDAQFlexRIO::ProfileDAQFlexRIO(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform,
		const ProfileBase::PROFILE_ID &id) :
		ProfileDAQ(parsedBitfile, session, platform, id), m_flexRIO(
				new TerminalsFlexRIO(parsedBitfile, session)) {
}

const std::shared_ptr<const TerminalsFlexRIO> ProfileDAQFlexRIO::flexRIO() const {
	return m_flexRIO;
}

}
