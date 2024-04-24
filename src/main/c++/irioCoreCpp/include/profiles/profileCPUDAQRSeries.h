#pragma once

#include "profiles/profileCPUDAQ.h"

namespace irio {
/**
 * Profile with the terminals
 * specific to data acquisition functionality
 * and to R Series boards.
 *
 * @ingroup Profiles
 * @ingroup ProfilesRSeries
 */
class ProfileCPUDAQRSeries: public ProfileCPUDAQ {
 public:
	/**
	 * Manages creating the required terminals for data acquisition functionality
	 * and R Series boards.
	 *
	 * @throw irio::errors::NiFpgaError    Error occurred in an FPGA operation
	 *
	 * @param parserManager     Pointer to class managing parsing the bitfile
	 *                          and finding its resources
	 * @param session           NiFpga_Session to be used in NiFpga related functions
	 * @param platform          Platform used
	 */
	ProfileCPUDAQRSeries(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform);
};

}  // namespace irio
