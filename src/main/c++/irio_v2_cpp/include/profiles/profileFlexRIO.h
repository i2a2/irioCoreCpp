#pragma once

#include <profiles/profileBase.h>


namespace iriov2 {

class ProfileFlexRIO: virtual public ProfileBase {
public:
	/**
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile
	 * @param session
	 * @param id
	 */
	ProfileFlexRIO(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const ProfileBase::PROFILE_ID &id);

	const std::shared_ptr<const TerminalsFlexRIO> flexRIO() const override;

private:
	std::shared_ptr<const TerminalsFlexRIO> m_flexRIO;
};

}

