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

template<typename T>
void findAndInsertEnumResources(
		const std::unordered_map<std::string, T> &mapSearch,
		const std::string &resourceName, const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert) {
	static_assert(std::is_base_of<bfp::Resource, T>::value,
			"T must inherit from bfp::Resource");

	for (size_t i = 0; i < maxElem; ++i) {
		const auto it = mapSearch.find(resourceName + std::to_string(i));
		if (it != mapSearch.end()) {
			mapInsert->insert({ i, it->second.address });
		}
	}
}

void findAndInsertEnumRegisters(const bfp::BFP &parsedBitfile,
		const std::string &terminalName, const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert) {
	findAndInsertEnumResources(parsedBitfile.getRegisters(), terminalName,
			maxElem, mapInsert);
}

void findAndInsertEnumDMAs(const bfp::BFP &parsedBitfile,
		const std::string &dmaName, const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> *mapInsert) {
	findAndInsertEnumResources(parsedBitfile.getDMAs(), dmaName, maxElem,
			mapInsert);
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

template<typename T>
void findArrayRegReadToVector(const bfp::BFP &parsedBitfile,
		const NiFpga_Session &session, const std::string &nameReg,
		std::vector<T> *vec,
		std::function<NiFpga_Status(NiFpga_Session,
				std::uint32_t, T*, size_t)> readFunc) {
	const auto reg = parsedBitfile.getRegister(nameReg);
	vec->resize(reg.numElem);
	const auto status = readFunc(session, reg.address, vec->data(), vec->size());
	utils::throwIfNotSuccessNiFpga(status, "Error reading " + nameReg);
}


template void findArrayRegReadToVector<std::uint8_t>(
		const bfp::BFP &parsedBitfile, const NiFpga_Session &session,
		const std::string &nameReg, std::vector<std::uint8_t> *vec,
		std::function<
				NiFpga_Status(NiFpga_Session, std::uint32_t, std::uint8_t*,
						size_t)> readFunc);

template void findArrayRegReadToVector<std::uint16_t>(
		const bfp::BFP &parsedBitfile, const NiFpga_Session &session,
		const std::string &nameReg, std::vector<std::uint16_t> *vec,
		std::function<
				NiFpga_Status(NiFpga_Session, std::uint32_t, std::uint16_t*,
						size_t)> readFunc);

}  // namespace utils
}  // namespace iriov2
