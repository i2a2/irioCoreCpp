#include <rioDiscovery.h>
#include <vector>
#include <dirent.h>
#include <fstream>

namespace iriov2{


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

std::string getRIODeviceAux(const std::string &serialNumber) {
#ifdef CCS_VERSION
	return getRIODeviceCCS(serialNumber);
#else
	//TODO
	return "";
#endif
}


std::string RIODiscovery::searchRIODevice(const std::string &serialNumber) {
	std::string sn = getRIODeviceAux(serialNumber);
	if(sn.empty()){
		throw std::runtime_error("No RIO device with serial number " + serialNumber);
	}
	return sn;
}

}

