#pragma once

#include <profiles/profileCPUDAQ.h>
#include <profiles/profileFlexRIO.h>

namespace iriov2 {

/**
 * Profile with the terminals
 * specific to data acquisition functionality
 * and the ones specific to FlexRIO devices.
 *
 * @ingroup Profiles
 * @ingroup ProfilesDAQ
 * @ingroup ProfilesFlexRIO
 * @ingroup FlexRIO
 */
class ProfileCPUDAQFlexRIO: public ProfileCPUDAQ, public ProfileFlexRIO {
 public:
	/**
	 * Constructor.
	 *
	 * See the parent classes for more
	 * information about the terminals created
	 *
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parserManager     Pointer to class managing parsing the bitfile
	 *                          and finding its resources
	 * @param session			NiFpga_Session to be used in NiFpga related functions
	 * @param platform			Platform used
	 */
	ProfileCPUDAQFlexRIO(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);
};

}  // namespace iriov2

