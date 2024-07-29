#include <dirent.h>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "rioDiscovery.h"
#include "errorsIrio.h"

#ifndef CCS_VERSION
#include <nisyscfg/nisyscfg.h>
#endif

namespace irio {

#ifdef CCS_VERSION
const std::unordered_map<std::string, std::string> modelMap = {
	{"0x76260x7626", "NI 9154"},
	{"0x76270x7627", "NI 9155"},
	{"0x75390x7539", "NI 9157"},
	{"0x753A0x753A", "NI 9159"},
	{"0x73910x7391", "PXI-7842R"},
	{"0x73E10x73E1", "PXI-7854R"},
	{"0xC4C40x74D0", "PXIe-7961R"},
	{"0xC4C40x74E2", "PXIe-7962R"},
	{"0xC4C40x74E3", "PXIe-7965R"},
	{"0xC4C40x75CE", "PXIe-7966R"},
	{"0xC4C40x74F3", "PCIe-5140R"},
	{"0xC4C40x7553", "PCIe-1473R"},
	{"0xC4C40x76FB", "PCIe-1473R-LX110"},
	{"0xC4C40x7570", "PCIe-1474R"},
	{"0xC4C40x7571", "PCIe-1475R"},
	{"0xC4C40x7572", "PCIe-1476R"},
	{"0x77B10x77B1", "NI-7931R"},
	{"0x77B20x77B2", "NI-7932R"},
	{"0x77AC0x77AC", "NI-7935R"},
	{"0xC4C40x76B5", "PXIe-7971R"},
	{"0xC4C40x76B6", "PXIe-7972R"},
	{"0xC4C40x76B7", "PXIe-7975R"},
	{"0xC4C40x7777", "PXIe-7976R"},
};


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

RIODeviceInfo getRIODeviceCCS(const std::string &serialNumber) {
	auto devices = getListDevices();
	auto it = devices.begin();
	std::string auxStr;
	std::string resourceName = "";
	std::string deviceHex = "";
	std::string subDeviceHex = "";

	while (it != devices.end() && resourceName.empty()) {
		std::ifstream snFile(*it + std::string("/nirio_serial_number"));
		if (snFile.is_open() && getline(snFile, auxStr) && auxStr == serialNumber) {
			auto slashPos = it->rfind('/');
			auto boardPos = it->rfind('!');
			resourceName = it->substr(slashPos + 1, boardPos - slashPos - 1);
		} else {
			it++;
			continue;
		}

		std::ifstream devFile(*it + std::string("/device/device"));
		if(devFile.is_open() && getline(devFile, auxStr)) {
			deviceHex = auxStr;
		}

		std::ifstream subDevFile(*it + std::string("/device/subsystem_device"));
		if(subDevFile.is_open() && getline(subDevFile, auxStr)) {
			subDeviceHex = auxStr;
		}
	}

	if (it == devices.end()) {
		throw errors::RIODeviceNotFoundError();
	}

	RIODeviceInfo ret;
	const auto modelIt = modelMap.find(deviceHex+subDeviceHex);
	if(modelIt != modelMap.end()) {
		ret.deviceModel = modelIt->second;
	} else {
		ret.deviceModel = "Unkown Model";
	}
	ret.resourceName = resourceName;
	return ret;
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
	explicit NISysCfg(const std::string &targetName,
						unsigned int timeout = 5000) {
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

	void findDeviceFromSerialNumber(const std::string &serialNumber) {
		createFilter(NISysCfgFilterPropertySerialNumber, serialNumber.c_str());
		findResource();
	}

	std::string getResourceName() const {
		char resourceName[NISYSCFG_SIMPLE_STRING_LENGTH];
		getIndexedProperty(NISysCfgIndexedPropertyExpertResourceName, resourceName);
		return resourceName;
	}

	std::string getDeviceModel() const {
		char deviceModel[NISYSCFG_SIMPLE_STRING_LENGTH];
		getResourceProperty(NISysCfgResourcePropertyProductName, deviceModel);
		return deviceModel;
	}

 private:
	template<typename T>
	void createFilter(NISysCfgFilterProperty property, const T* value) {
		const std::string expertName = "NI-RIO";
		NISysCfgCloseHandle(m_filter);

		auto status = NISysCfgCreateFilter(m_session, &m_filter);
		throwIfNiSysCfgError(status,
								"Unable to create NISysCfg filter");
		status = NISysCfgSetFilterProperty(
			m_filter, NISysCfgFilterPropertyExpertName,
			reinterpret_cast<const void *>(expertName.c_str()));
		throwIfNiSysCfgError(status,
								"Unable to configure NISysCfg filter");
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
	void getIndexedProperty(NISysCfgIndexedProperty property, T* val) const {
		auto status = NISysCfgGetResourceIndexedProperty(m_resource,
				property, 0, reinterpret_cast<void*>(val));
		throwIfNiSysCfgError(status, "");
	}

	template<typename T>
	void getResourceProperty(NISysCfgResourceProperty property, T* val) const {
		auto status = NISysCfgGetResourceProperty(m_resource, property,
				reinterpret_cast<void*>(val));
		throwIfNiSysCfgError(status, "");
	}

	NISysCfgSessionHandle m_session;
	NISysCfgFilterHandle m_filter;
	NISysCfgResourceHandle m_resource;
};


RIODeviceInfo getRIODevicesNI(const std::string &serialNumber) {
	NISysCfg syscfg("localhost");
	syscfg.findDeviceFromSerialNumber(serialNumber);
	RIODeviceInfo ret;
	ret.deviceModel = syscfg.getDeviceModel();
	ret.resourceName = syscfg.getResourceName();
	return ret;
}

#endif

RIODeviceInfo getRIODeviceAux(const std::string &serialNumber) {
#ifdef CCS_VERSION
	return getRIODeviceCCS(serialNumber);
#else
	return getRIODevicesNI(serialNumber);
#endif
}

RIODeviceInfo searchRIODevice(const std::string serialNumber) {
	try {
		return getRIODeviceAux(serialNumber);
	} catch (errors::RIODeviceNotFoundError&) {
		throw errors::RIODeviceNotFoundError(serialNumber);
	}
}

}  // namespace irio

