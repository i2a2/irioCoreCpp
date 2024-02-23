#include <profiles/profileCPUDAQ.h>

namespace iriov2 {

ProfileCPUDAQ::ProfileCPUDAQ(
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

const std::shared_ptr<const TerminalsAnalog> ProfileCPUDAQ::analog() const {
	return m_analog;
}

const std::shared_ptr<const TerminalsDigital> ProfileCPUDAQ::digital() const {
	return m_digital;
}

const std::shared_ptr<const TerminalsAuxAnalog> ProfileCPUDAQ::auxAnalog() const {
	return m_auxAnalog;
}

const std::shared_ptr<const TerminalsAuxDigital> ProfileCPUDAQ::auxDigital() const {
	return m_auxDigital;
}

const std::shared_ptr<const TerminalsSignalGeneration> ProfileCPUDAQ::signalGeneration() const {
	return m_signalGeneration;
}

}
