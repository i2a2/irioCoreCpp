#pragma once

#include <profiles/profileCPUDAQ.h>

namespace irio {

/**
 * Profile with the terminals
 * specific to data acquisition functionality
 * and the ones specific to cRIO devices.
 *
 * @ingroup Profiles
 * @ingroup ProfilesCRIO
 */
class ProfileCPUDAQcRIO: public ProfileCPUDAQ {
 public:
	/**
	 * Constructor.
	 *
	 * See the parent classes for more
	 * information about the terminals created
	 *
	 * @throw irio::errors::NiFpgaError	Error occurred in an FPGA operation
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

}  // namespace irio

