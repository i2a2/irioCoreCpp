#pragma once

#include <profiles/profileBase.h>
#include <platforms.h>

namespace iriov2 {
/**
 * Profile with the terminals
 * specific to data acquisition functionality.
 *
 * @ingroup Profiles
 * @ingroup ProfilesDAQ
 */
class ProfileCPUDAQ: virtual public ProfileBase {
public:
	/**
	 * Constructor.
	 *
	 * Manages creating the required terminals for data acquisition functionality, which are:
	 * -\ref TerminalsAnalog
	 * -\ref TerminalsDigital
	 * -\ref TerminalsAuxAnalog
	 * -\ref TerminalsAuxDigital
	 * -\ref TerminalsSignalGeneration
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile 	Parsed bitfile
	 * @param session			NiFpga_Session to be used in NiFpga related functions
	 * @param platform			Platform used
	 * @param id				Profile used
	 */
	ProfileCPUDAQ(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const ProfileBase::PROFILE_ID &id);

	TerminalsAnalog analog() const override;
	TerminalsDigital digital() const override;

	TerminalsAuxAnalog auxAnalog() const override;
	TerminalsAuxDigital auxDigital() const override;

	TerminalsSignalGeneration signalGeneration() const override;

	TerminalsDMADAQ daq() const override;

private:
	TerminalsAnalog m_analog;
	TerminalsDigital m_digital;

	TerminalsAuxAnalog m_auxAnalog;
	TerminalsAuxDigital m_auxDigital;

	TerminalsSignalGeneration m_signalGeneration;

	TerminalsDMADAQCPU m_daq;

};

}
