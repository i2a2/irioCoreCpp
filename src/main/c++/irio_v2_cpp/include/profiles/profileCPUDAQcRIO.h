#pragma once

#include <profiles/profileCPUDAQ.h>
#include <profiles/profileCRIO.h>

namespace iriov2 {

/**
 * Profile with the terminals
 * specific to data acquisition functionality
 * and the ones specific to cRIO devices.
 *
 * @ingroup Profiles
 * @ingroup ProfilesDAQ
 * @ingroup ProfilesCRIO
 * @ingroup cRIO
 */
class ProfileCPUDAQcRIO: public ProfileCPUDAQ, public ProfileCRIO {
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
	ProfileCPUDAQcRIO(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);
};

}  // namespace iriov2

