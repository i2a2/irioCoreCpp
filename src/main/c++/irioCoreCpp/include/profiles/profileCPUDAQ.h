#pragma once

#include <profiles/profileBase.h>
#include <platforms.h>

namespace irio {
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
	 * - TerminalsAnalog
	 * - TerminalsDigital
	 * - TerminalsAuxAnalog
	 * - TerminalsAuxDigital
	 * - TerminalsSignalGeneration
	 *
	 * @throw irio::errors::NiFpgaError    Error occurred in an FPGA operation
	 *
	 * @param parserManager     Pointer to class managing parsing the bitfile
	 *                          and finding its resources
	 * @param session           NiFpga_Session to be used in NiFpga related functions
	 * @param platform          Platform used
	 * @param id                Profile used
	 */
	ProfileCPUDAQ(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform,
			const PROFILE_ID &id);
};

}  // namespace irio
