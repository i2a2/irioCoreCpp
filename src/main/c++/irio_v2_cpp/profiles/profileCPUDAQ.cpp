#include <profiles/profileCPUDAQ.h>
#include <terminals/terminalsDMADAQCPU.h>

namespace iriov2 {

ProfileCPUDAQ::ProfileCPUDAQ(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session, const Platform &platform,
		const PROFILE_ID &id) :
		ProfileBase(id), m_analog(parsedBitfile, session, platform), m_digital(
				parsedBitfile, session, platform), m_auxAnalog(parsedBitfile,
				session, platform), m_auxDigital(parsedBitfile, session,
				platform), m_signalGeneration(parsedBitfile, session, platform),
				m_daq(parsedBitfile, session, platform) {
}

TerminalsAnalog ProfileCPUDAQ::analog() const {
	return m_analog;
}

TerminalsDigital ProfileCPUDAQ::digital() const {
	return m_digital;
}

TerminalsAuxAnalog ProfileCPUDAQ::auxAnalog() const {
	return m_auxAnalog;
}

TerminalsAuxDigital ProfileCPUDAQ::auxDigital() const {
	return m_auxDigital;
}

TerminalsSignalGeneration ProfileCPUDAQ::signalGeneration() const {
	return m_signalGeneration;
}

TerminalsDMADAQ ProfileCPUDAQ::daq() const {
	return m_daq;
}

}  // namespace iriov2
