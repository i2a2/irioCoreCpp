#pragma once

#include <string>
#include <stdexcept>
#include <gtest/gtest.h>
#include <unordered_map>

std::string getEnv(const std::string& envName);

class IrioFixture : public ::testing::Test {
private:
	const std::string ENV_SERIAL_NUMBER = "RIOSerial";
	const std::string ENV_MODEL = "RIODevice";

	const std::unordered_map<std::string, std::string> mapFamily = {
		{"7966", "FlexRIO"},
		{"7965", "FlexRIO"},
		{"7961", "FlexRIO"},
		{"9159", "cRIO"},
		{"7854", "Rseries"},
	};
public:
	const std::string BASE_PATH_BITFILES = "../../resources/";

	std::string serialNumber;
	std::string model;
	std::string boardType;
	std::string bitfileName;

	IrioFixture(const std::string &typeBitfile);

	std::string getBitfilePath(const std::string &folder="");
};


