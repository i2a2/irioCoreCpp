#include <vector>

#include "utils.h"
#include "errorsIrio.h"

namespace iriov2 {

namespace utils {

void throwIfNotSuccessNiFpga(const NiFpga_Status &status,
		const std::string &errMsg) {
	if (NiFpga_IsError(status)) {
		const std::string err = errMsg + std::string("(Code: ")
				+ std::to_string(status) + std::string(")");
		throw iriov2::errors::NiFpgaError(err);
	}
}

std::uint32_t getAddressEnumResource(
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapResource,
		const std::uint32_t n, const std::string &resourceName) {
	auto it = mapResource.find(n);
	if (it == mapResource.end()) {
		throw iriov2::errors::ResourceNotFoundError(n, resourceName);
	}

	return it->second;
}


}  // namespace utils
}  // namespace iriov2
