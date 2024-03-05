#pragma once
#include <string>

namespace iriov2 {

/**
 * Manages searching for RIO devices
 *
 * @ingroup IrioV2
 */
class RIODiscovery{
 public:
	/**
	 * Searches for RIO devices and returns its name if any matches the
	 * specified serial number
	 *
	 * @throw iriov2::errors::RIODeviceNotFoundError	Not found device with specified serial number
	 * @throw iriov2::errors::RIODiscoveryError			Error while discovering devices
	 *
	 * @param serialNumber	Serial number to search
	 * @return	Name of the RIO device
	 */
	static std::string searchRIODevice(const std::string &serialNumber);
};

}  // namespace iriov2


