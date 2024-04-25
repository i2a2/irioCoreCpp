#pragma once
#include <string>
#include <stdexcept>
#include <gtest/gtest.h>
#include <unordered_map>

std::string getEnv(const std::string& envName){
	const auto aux = std::getenv(envName.c_str());
	if(!aux){
		throw std::runtime_error("Environment variable '" + envName + "' not defined");
	}
	return std::string(aux);
}

class IrioFixture : public ::testing::Test {
private:
	const std::string ENV_SERIAL_NUMBER = "RIOSerial";
	const std::string ENV_MODEL = "RIODevice";

	const std::unordered_map<std::string, std::string> mapFamily = {
		{"7966", "FlexRIO"},
		{"7965", "FlexRIO"},
		{"7961", "FlexRIO"},
		{"9159", "cRIO"},
	};
public:
	const std::string BASE_PATH_BITFILES = "../../resources/";

	std::string serialNumber;
	std::string model;
	std::string boardType;
	std::string bitfileName;

	IrioFixture(const std::string &typeBitfile){
		model = getEnv(ENV_MODEL);
		serialNumber = getEnv(ENV_SERIAL_NUMBER);
		boardType = mapFamily.at(model);
		bitfileName = "NiFpga_" + boardType + "_" + typeBitfile + "_" + model + ".lvbitx";
	}

	std::string getBitfilePath(const std::string &folder=""){
		const std::string auxFolder = folder.empty()?folder:folder+"/";
		return BASE_PATH_BITFILES + auxFolder + model + "/" + bitfileName;
	}
};


