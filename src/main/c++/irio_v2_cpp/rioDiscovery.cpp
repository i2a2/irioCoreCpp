#include <rioDiscovery.h>
#include <vector>
#include <dirent.h>
#include <fstream>

#ifndef CCS_VERSION
#include <nisyscfg/nisyscfg.h>
#endif

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
#else

void throwIfNiSysCfgError(const NISysCfgStatus &status, const std::string &errMsg){
	if(status != NISysCfg_OK){
		const std::string err = errMsg + std::string("(Code: ")
						+ std::to_string(static_cast<std::int32_t>(status)) + std::string(")");
		throw std::runtime_error(err);
	}
}


class NISysCfg{
public:
	NISysCfg(const std::string &targetName, unsigned int timeout=5000):
			m_filter(), m_resource() {
		auto status = NISysCfgInitializeSession(targetName.c_str(), NULL, NULL, NISysCfgLocaleEnglish, NISysCfgBoolTrue,
				timeout, NULL, &m_session);
		throwIfNiSysCfgError(status, "Unable to initialize NISysCfg");
	}

	~NISysCfg(){
		NISysCfgCloseHandle(m_filter);
		NISysCfgCloseHandle(m_resource);
		NISysCfgCloseHandle(m_session);
	}

	std::string getDeviceFromSerialNumber(const std::string &serialNumber){
		char resourceName[NISYSCFG_SIMPLE_STRING_LENGTH];

		createFilter(NISysCfgFilterPropertySerialNumber, serialNumber.c_str());
		findResource();
		getIndexedProperty(NISysCfgIndexedPropertyExpertResourceName, resourceName);

		return resourceName;
	}

private:
	void createFilter(NISysCfgFilterProperty property, const void* value){
		NISysCfgCloseHandle(m_filter);

		auto status = NISysCfgCreateFilter(m_session, &m_filter);
		throwIfNiSysCfgError(status, "Unable to create NISysCfg filter");
		status = NISysCfgSetFilterProperty(m_filter, property, value);
		throwIfNiSysCfgError(status, "Unable to configure NISysCfg filter");
	}

	void findResource(){
		NISysCfgCloseHandle(m_resource);

		NISysCfgEnumResourceHandle resourceEnum;
		auto status = NISysCfgFindHardware(m_session, NISysCfgFilterModeMatchValuesAll,
				m_filter, NULL, &resourceEnum);
		throwIfNiSysCfgError(status, "Unable to find NISysCfg hardware");

		NISysCfgIsPresentType isPresent = NISysCfgIsPresentTypeNotPresent;
		while (isPresent != NISysCfgIsPresentTypePresent
				&& NISysCfgNextResource(m_session, resourceEnum, &m_resource) == NISysCfg_OK) {
			NISysCfgGetResourceProperty(m_resource, NISysCfgResourcePropertyIsPresent, &isPresent);
			if(isPresent != NISysCfgIsPresentTypePresent){
				NISysCfgCloseHandle(m_resource);
			}
		}

		if(isPresent != NISysCfgIsPresentTypePresent){
			throw std::runtime_error("Resource not found");
		}
	}

	void getProperty(NISysCfgResourceProperty property, void* val){
		auto status = NISysCfgGetResourceProperty(m_resource, property, val);
		throwIfNiSysCfgError(status, "Unable to get NISysCfg property " + std::to_string(property));
	}

	void getIndexedProperty(NISysCfgIndexedProperty property, void* val){
		auto status = NISysCfgGetResourceIndexedProperty(m_resource, property, 0, val);
		throwIfNiSysCfgError(status, "");
	}

	NISysCfgSessionHandle m_session;
	NISysCfgFilterHandle m_filter;
	NISysCfgResourceHandle m_resource;
};


std::string getRIODevicesNI(const std::string &serialNumber) {
	auto syscfg = NISysCfg("localhost");
	return syscfg.getDeviceFromSerialNumber(serialNumber);
}

#endif

std::string getRIODeviceAux(const std::string &serialNumber) {
#ifdef CCS_VERSION
	return getRIODeviceCCS(serialNumber);
#else
	return getRIODevicesNI(serialNumber);
#endif
}


std::string RIODiscovery::searchRIODevice(const std::string &serialNumber) {
	std::string deviceName = getRIODeviceAux(serialNumber);
	if(deviceName.empty()){
		throw std::runtime_error("No RIO device with serial number " + serialNumber);
	}
	return deviceName;
}

}

