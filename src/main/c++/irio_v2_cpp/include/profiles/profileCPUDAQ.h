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
			const PROFILE_ID &id);
};

}  // namespace iriov2
