#pragma once
#include <string>

namespace irio {
/**
 * @ingroup IrioCoreCpp
 *
 * Searches for RIO devices and returns its name if any matches the
 * specified serial number
 *
 * @throw irio::errors::RIODeviceNotFoundError	Not found device with specified serial number
 * @throw irio::errors::RIODiscoveryError			Error while discovering devices
 *
 * @param serialNumber	Serial number to search
 * @return	Name of the RIO device
 */
std::string searchRIODevice(const std::string serialNumber);
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


