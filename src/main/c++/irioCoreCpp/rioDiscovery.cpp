#include <dirent.h>
#include <vector>
#include <fstream>

#include "rioDiscovery.h"
#include "errorsIrio.h"

#ifndef CCS_VERSION
#include <nisyscfg/nisyscfg.h>
#endif

namespace irio {

#ifdef CCS_VERSION
std::vector<std::string> getListDevices() {
	static const std::string interfacePath = "/sys/class/nirio";

	DIR *dir = opendir(interfacePath.c_str());
	if (!dir) {
		throw errors::RIODiscoveryError(
				"Cannot discover resources because kernel module "
				"is not loaded, run 'modproble nirio' and try again");
	}
	std::vector<std::string> devices;

	try {
		const struct dirent *entry = readdir(dir);

		while (entry) {
			std::string name = entry->d_name;
			if (name.find("board") != std::string::npos) {
				devices.push_back(interfacePath + "/" + name);
			}
			entry = readdir(dir);
		}
	} catch (std::exception&) {
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
		if (snFile.is_open() && getline(snFile, sn) && sn == serialNumber) {
			auto slashPos = it->rfind('/');
			auto boardPos = it->rfind('!');
			name = it->substr(slashPos + 1, boardPos - slashPos - 1);
		} else {
			it++;
		}
	}

	if (it == devices.end()) {
		throw errors::RIODeviceNotFoundError();
	}

	return name;
}
#else

void throwIfNiSysCfgError(const NISysCfgStatus &status,
		const std::string &errMsg) {
	if(status != NISysCfg_OK) {
		const std::string err = errMsg + std::string("(Code: ")
								+ std::to_string(static_cast<std::int32_t>(status)) + std::string(")");
		throw errors::RIODiscoveryError(err);
	}
}


class NISysCfg{
 public:
	explicit NISysCfg(const std::string &targetName, unsigned int timeout = 5000):
			m_filter(), m_resource() {
		auto status = NISysCfgInitializeSession(targetName.c_str(), nullptr,
				nullptr, NISysCfgLocaleEnglish, NISysCfgBoolTrue,
				timeout, nullptr, &m_session);
		throwIfNiSysCfgError(status, "Unable to initialize NISysCfg");
	}

	~NISysCfg() {
		NISysCfgCloseHandle(m_filter);
		NISysCfgCloseHandle(m_resource);
		NISysCfgCloseHandle(m_session);
	}

	NISysCfg(const NISysCfg&) = delete;
	NISysCfg& operator=(const NISysCfg&) = delete;

	std::string getDeviceFromSerialNumber(const std::string &serialNumber) {
		char resourceName[NISYSCFG_SIMPLE_STRING_LENGTH];

		createFilter(NISysCfgFilterPropertySerialNumber, serialNumber.c_str());
		findResource();
		getIndexedProperty(NISysCfgIndexedPropertyExpertResourceName, resourceName);

		return resourceName;
	}

 private:
	template<typename T>
	void createFilter(NISysCfgFilterProperty property, const T* value) {
		NISysCfgCloseHandle(m_filter);

		auto status = NISysCfgCreateFilter(m_session, &m_filter);
		throwIfNiSysCfgError(status,
								"Unable to create NISysCfg filter");
		status = NISysCfgSetFilterProperty(
			m_filter, property, reinterpret_cast<const void *>(value));
		throwIfNiSysCfgError(status,
								"Unable to configure NISysCfg filter");
	}

	void findResource() {
		NISysCfgCloseHandle(m_resource);

		NISysCfgEnumResourceHandle resourceEnum;
		auto status = NISysCfgFindHardware(m_session,
				NISysCfgFilterModeMatchValuesAll,
				m_filter, nullptr, &resourceEnum);
		throwIfNiSysCfgError(status, "Unable to find NISysCfg hardware");

		NISysCfgIsPresentType isPresent = NISysCfgIsPresentTypeNotPresent;
		while (isPresent != NISysCfgIsPresentTypePresent
				&& NISysCfgNextResource(m_session, resourceEnum,
						&m_resource) == NISysCfg_OK) {
			NISysCfgGetResourceProperty(m_resource,
					NISysCfgResourcePropertyIsPresent, &isPresent);
			if(isPresent != NISysCfgIsPresentTypePresent) {
				NISysCfgCloseHandle(m_resource);
			}
		}

		if(isPresent != NISysCfgIsPresentTypePresent) {
			throw errors::RIODeviceNotFoundError();
		}
	}

	template<typename T>
	void getIndexedProperty(NISysCfgIndexedProperty property, T* val) {
		auto status = NISysCfgGetResourceIndexedProperty(m_resource,
				property, 0, reinterpret_cast<void*>(val));
		throwIfNiSysCfgError(status, "");
	}

	NISysCfgSessionHandle m_session;
	NISysCfgFilterHandle m_filter;
	NISysCfgResourceHandle m_resource;
};


std::string getRIODevicesNI(const std::string &serialNumber) {
	NISysCfg syscfg("localhost");
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

std::string searchRIODevice(const std::string serialNumber) {
	try {
		std::string deviceName = getRIODeviceAux(serialNumber);
		return deviceName;
	} catch (errors::RIODeviceNotFoundError&) {
		throw errors::RIODeviceNotFoundError(serialNumber);
	}
}

}  // namespace irio

