#pragma once

#include <profiles/profileBase.h>
#include <platforms.h>

namespace iriov2 {

class ProfileDAQ: virtual public ProfileBase {
public:
	/**
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param platform
	 * @param id
	 */
	ProfileDAQ(
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
