#include "utils.h"
#include <stdexcept>

void throwIfNotSuccessNiFpga(const NiFpga_Status &status, const std::string &errMsg) {
	if (status != NiFpga_Status_Success) {
		const std::string err = errMsg + std::string("(Code: ")
				+ std::to_string(static_cast<std::int32_t>(status)) + std::string(")");
		throw std::runtime_error(err);
	}
}

template<typename T>
void findAndInsertEnumResources(
		const std::unordered_map<std::string, T> &mapSearch,
		const std::string &resourceName,
		const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert) {
	static_assert(std::is_base_of<bfp::Resource, T>::value, "T must inherit from bfp::Resource");

	for (size_t i = 0; i < maxElem; ++i) {
		const auto it = mapSearch.find(resourceName + std::to_string(i));
		if (it != mapSearch.end()) {
			mapInsert.insert( { i, it->second.address });
		}
	}
}

void findAndInsertEnumRegisters(
		const bfp::BFP &parsedBitfile,
		const std::string &terminalName,
		const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert) {
	findAndInsertEnumResources(parsedBitfile.getRegisters(), terminalName, maxElem, mapInsert);
}

void findAndInsertEnumDMAs(
		const bfp::BFP &parsedBitfile,
		const std::string &dmaName,
		const size_t maxElem,
		std::unordered_map<std::uint32_t, const std::uint32_t> &mapInsert) {
	findAndInsertEnumResources(parsedBitfile.getDMAs(), dmaName, maxElem, mapInsert);
}

std::uint32_t getAddressEnumResource(
		const std::unordered_map<std::uint32_t, const std::uint32_t> &mapResource,
		const std::uint32_t n,
		const std::string &resourceName) {
	auto it = mapResource.find(n);
	if (it == mapResource.end()) {
		throw std::runtime_error(
				std::to_string(n) + " is not a valid " + resourceName + " terminal");
	}

	return it->second;
}
