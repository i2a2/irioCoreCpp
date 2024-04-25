#include "irioFixture.h"

std::string getEnv(const std::string& envName){
	const auto aux = std::getenv(envName.c_str());
	if(!aux){
		throw std::runtime_error("Environment variable '" + envName + "' not defined");
	}
	return std::string(aux);
}

IrioFixture::IrioFixture(const std::string &typeBitfile) {
	model = getEnv(ENV_MODEL);
	serialNumber = getEnv(ENV_SERIAL_NUMBER);
	boardType = mapFamily.at(model);
	bitfileName =
		"NiFpga_" + boardType + "_" + typeBitfile + "_" + model + ".lvbitx";
}

std::string IrioFixture::getBitfilePath(const std::string &folder) {
	const std::string auxFolder = folder.empty() ? folder : folder + "/";
	return BASE_PATH_BITFILES + auxFolder + model + "/" + bitfileName;
}