#include <gtest/gtest.h>
#include <iostream>
#include "irioFixture.h"
#include "irio_v2.h"

using namespace iriov2;

class FlexRIOFixture: public IrioFixture {
public:
	FlexRIOFixture(const std::string &typeBitfile): IrioFixture("FlexRIO", typeBitfile){ }
};

class FlexRIOCPUDAQ: public ::testing::Test, public FlexRIOFixture {
public:
	FlexRIOCPUDAQ(): FlexRIOFixture("CPUDAQ"){};
};

class FlexRIOOnlyResources: public ::testing::Test, public FlexRIOFixture {
public:
	FlexRIOOnlyResources(): FlexRIOFixture("OnlyResources"){};
};


/////////////////////////////////////////////////////////////
/// OnlyResources Tests
////////////////////////////////////////////////////////////

TEST_F(FlexRIOOnlyResources, InitClose){
	try{
		IrioV2 irio(bitfilePath, serialNumber, "4.0");
	}catch(std::exception &e){
		FAIL() << "Error at IrioV2's constructor (" + std::string(e.what()) + ")";
	}
}

TEST_F(FlexRIOOnlyResources, ResourcesMAXIO){
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	EXPECT_EQ(irio.daq()->countDMAs(), 1) << "Unexpected number of DMAs";
	EXPECT_EQ(irio.analog()->getNumAI(), 0) << "Unexpected number of analog inputs";
	EXPECT_EQ(irio.analog()->getNumAO(), 2) << "Unexpected number of analog outputs";
	EXPECT_EQ(irio.digital()->getNumDI(), 54) << "Unexpected number of digital inputs";
	EXPECT_EQ(irio.digital()->getNumDO(), 54) << "Unexpected number of digital outputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAI(), 16) << "Unexpected number of aux analog inputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAO(), 16) << "Unexpected number of aux analog outputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDI(), 16) << "Unexpected number of aux digital inputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDO(), 16) << "Unexpected number of aux digital outputs";
	EXPECT_EQ(irio.signalGeneration()->getSGNo(), 2) << "Unexpected number of signal generators";
}


/////////////////////////////////////////////////////////////
/// FlexRIOCPUDAQ Tests
////////////////////////////////////////////////////////////

/**
 * This test the cover IrioV2 constructor, which:
 * - Initializes low level driver
 * - Parse resources
 * - Download bitfile to FPGA and open session
 * - Find platform and profile
 */
TEST_F(FlexRIOCPUDAQ, InitClose){
	try{
		IrioV2 irio(bitfilePath, serialNumber, "4.0");
	}catch(std::exception &e){
		FAIL() << "Error at IrioV2's constructor (" + std::string(e.what()) + ")";
	}
}

/**
 * Test checks that all terminals are identified correctly
 * at driver initialization
 */
TEST_F(FlexRIOCPUDAQ, Resources){
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	EXPECT_EQ(irio.daq()->countDMAs(), 2) << "Unexpected number of DMAs";
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



