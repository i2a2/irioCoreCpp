#include <chrono>
#include <iomanip>
#include <sstream>

#include "utils.h"
#include "errorsIrio.h"

namespace irio {

namespace utils {

void throwIfNotSuccessNiFpga(const NiFpga_Status &status,
		const std::string &errMsg) {
	if (NiFpga_IsError(status)) {
		const std::string err = errMsg + std::string("(Code: ")
				+ std::to_string(status) + std::string(")");
		throw irio::errors::NiFpgaError(err);
	}
}

std::uint32_t getAddressEnumResource(
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapResource,
		const std::uint32_t n, const std::string &resourceName) {
	auto it = mapResource.find(n);
	if (it == mapResource.end()) {
		throw irio::errors::ResourceNotFoundError(n, resourceName);
	}

	return it->second;
}

std::string getBaseName(const std::string& path) {
	return path.substr(path.find_last_of("/\\") + 1,
					   path.find_last_of(".") - path.find_last_of("/\\") - 1);
}

std::string getTimestamp() {
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto nanoseconds =
		std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(19) << nanoseconds;
	return oss.str();
}

}  // namespace utils
}  // namespace irio
