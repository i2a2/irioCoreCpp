#include <profiles/profileDAQ.h>

namespace iriov2 {

ProfileDAQ::ProfileDAQ(
		const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session,
		const Platform &platform,
		const ProfileBase::PROFILE_ID &id) :
		ProfileBase(parsedBitfile, session, id) {
	m_analog.reset(new TerminalsAnalog(parsedBitfile, session, platform));
	m_digital.reset(new TerminalsDigital(parsedBitfile, session, platform));

	m_auxAnalog.reset(new TerminalsAuxAnalog(parsedBitfile, session, platform));
	m_auxDigital.reset(new TerminalsAuxDigital(parsedBitfile, session, platform));

	m_signalGeneration.reset(new TerminalsSignalGeneration(parsedBitfile, session, platform));
}

const std::shared_ptr<const TerminalsAnalog> ProfileDAQ::analog() const {
	return m_analog;
}

const std::shared_ptr<const TerminalsDigital> ProfileDAQ::digital() const {
	return m_digital;
}

const std::shared_ptr<const TerminalsAuxAnalog> ProfileDAQ::auxAnalog() const {
	return m_auxAnalog;
}

const std::shared_ptr<const TerminalsAuxDigital> ProfileDAQ::auxDigital() const {
	return m_auxDigital;
}

const std::shared_ptr<const TerminalsSignalGeneration> ProfileDAQ::signalGeneration() const {
	return m_signalGeneration;
}

}
