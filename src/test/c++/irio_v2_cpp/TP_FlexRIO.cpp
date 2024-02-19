#include <gtest/gtest.h>
#include <iostream>
#include "irio_v2.h"

using namespace iriov2;

class FlexRIOFixture: public ::testing::Test {
private:
	const std::string ENV_SERIAL_NUMBER = "RIOSerial";
protected:
	std::string serialNumber;

	FlexRIOFixture(){
		auto aux = std::getenv(ENV_SERIAL_NUMBER.c_str());
		if(!aux){
			throw std::runtime_error("Environment variable '" + ENV_SERIAL_NUMBER + "' not defined");
		}
		serialNumber = std::string(aux);
	}
};


TEST_F(FlexRIOFixture, ResourcesCPUDAQ){
	std::string bitfile = "../../resources/NiFpga_FlexRIO_CPUDAQ_7966.lvbitx";

	IrioV2 irio(bitfile, serialNumber, "4.0");

	EXPECT_EQ(irio.analog()->getNumAI(), 2) << "Unexpected number of analog inputs";
	EXPECT_EQ(irio.analog()->getNumAO(), 2) << "Unexpected number of analog outputs";
	EXPECT_EQ(irio.digital()->getNumDI(), 2) << "Unexpected number of digital inputs";
	EXPECT_EQ(irio.digital()->getNumDO(), 2) << "Unexpected number of digital outputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAI(), 2) << "Unexpected number of aux analog inputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAO(), 2) << "Unexpected number of aux analog outputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDI(), 2) << "Unexpected number of aux digital inputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDO(), 2) << "Unexpected number of aux digital outputs";
	EXPECT_EQ(irio.signalGeneration()->getSGNo(), 2) << "Unexpected number of signal generators";
}

