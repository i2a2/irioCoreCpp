#pragma once
#include <string>

namespace irio {

/**
 * Holds the found resource name and device model
 */
struct RIODeviceInfo {
	/// Resource name. Needed to open session with FPGA
	std::string resourceName;
	/// Device Model of RIO board
	std::string deviceModel;
};

/**
 * @ingroup IrioCoreCpp
 *
 * Searches for a RIO device with the specified serial number.
 * Returns its resource name and the device model
 *
 * @throw irio::errors::RIODeviceNotFoundError	Not found device with specified serial number
 * @throw irio::errors::RIODiscoveryError			Error while discovering devices
 *
 * @param serialNumber	Serial number to search
 * @return	RIODeviceInfo with the resource name and the device model
 */
RIODeviceInfo searchRIODevice(const std::string serialNumber);
/* 
 * serialNumber should be a reference, but due to the mocking
 * framework the project uses (fff) which is really a C framework,
 * when using reference breaks. So as this function is only called at
 * the beginning when performance is not critical, I'm leaving it
 * as is. fff works great for mocking NiFpga_* calls (as they are C functions),
 * so if any finds a mocking framework that could be used easily for these
 * cases, suggest it please.
 */

}  // namespace irio


