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

	const std::shared_ptr<const TerminalsAnalog> analog() const override;
	const std::shared_ptr<const TerminalsDigital> digital() const override;

	const std::shared_ptr<const TerminalsAuxAnalog> auxAnalog() const override;
	const std::shared_ptr<const TerminalsAuxDigital> auxDigital() const override;

	const std::shared_ptr<const TerminalsSignalGeneration> signalGeneration() const override;

private:
	std::shared_ptr<const TerminalsAnalog> m_analog;
	std::shared_ptr<const TerminalsDigital> m_digital;

	std::shared_ptr<const TerminalsAuxAnalog> m_auxAnalog;
	std::shared_ptr<const TerminalsAuxDigital> m_auxDigital;

	std::shared_ptr<const TerminalsSignalGeneration> m_signalGeneration;

};

}
