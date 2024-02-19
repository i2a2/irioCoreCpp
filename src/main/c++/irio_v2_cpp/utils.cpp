#include "utils.h"
#include <stdexcept>
#include <dirent.h>
#include <fstream>

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


//TODO: Make this cleaner. Maybe separate each version into separate headers, and just make conditional the include?
#ifdef CCS_VERSION
std::vector<std::string> getListDevices(){
	static const std::string interfacePath = "/sys/class/nirio";

	DIR* dir = opendir(interfacePath.c_str());
	if(!dir){
		throw std::runtime_error("Cannot discover resources because kernel module is not loaded, run 'modproble nirio' and try again");
	}
	std::vector<std::string> devices;

	try{
		struct dirent* entry = readdir(dir);

		while(entry){
			std::string name = entry->d_name;
			if(name.find("board") != std::string::npos){
				devices.push_back(interfacePath + "/" + name);
			}
			entry = readdir(dir);
		}
	}catch(std::exception& ){
		closedir(dir);
		throw;
	}
	closedir(dir);

	return devices;
}


std::string getRIODeviceCCS(const std::string &serialNumber) {
	auto devices = getListDevices();
	auto it = devices.begin();
	std::string sn;
	std::string name = "";

	while (it != devices.end() && name.empty()) {
		std::ifstream snFile(*it + std::string("/nirio_serial_number"));
		if(snFile.is_open() && getline(snFile, sn) && sn == serialNumber){
			auto slashPos = it->rfind('/');
			auto boardPos = it->rfind('!');
			name = it->substr(slashPos+1, boardPos-slashPos-1);
		}
		it++;
	}

	return name;
}
#endif

std::string getRIODeviceAux(const std::string &serualNumber) {
#ifdef CCS_VERSION
	return getRIODeviceCCS(serualNumber);
#else
	//TODO
	return "";
#endif
}

std::string getRIODevice(const std::string &serialNumber) {
	std::string sn = getRIODeviceAux(serialNumber);
	if(sn.empty()){
		throw std::runtime_error("Not device with serial number " + serialNumber);
	}
	return sn;
}
