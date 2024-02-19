#pragma once
#include <string>

namespace iriov2{

class RIODiscovery{
public:
	/**
	 * Searches for RIO devices and returns its name if any matches the
	 * specified serial number
	 * @param serialNumber	Serial number to search
	 * @return	Name of the RIO device
	 */
	static std::string searchRIODevice(const std::string &serialNumber);
};

}


