#pragma once
#include <string>

namespace iriov2 {
/**
 * @ingroup IrioV2
 *
 * Searches for RIO devices and returns its name if any matches the
 * specified serial number
 *
 * @throw iriov2::errors::RIODeviceNotFoundError	Not found device with specified serial number
 * @throw iriov2::errors::RIODiscoveryError			Error while discovering devices
 *
 * @param serialNumber	Serial number to search
 * @return	Name of the RIO device
 */
std::string searchRIODevice(const std::string &serialNumber);

}  // namespace iriov2


