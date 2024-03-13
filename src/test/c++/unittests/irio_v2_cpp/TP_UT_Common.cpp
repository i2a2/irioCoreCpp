#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures.h"
#include "fff_nifpga.h"


#include "irio_v2.h"
#include "rioDiscovery.h"
#include "errorsIrio.h"
#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "modules.h"


using namespace iriov2;


class CommonTests: public BaseTests {
public:
	CommonTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);
	}
};

class ErrorCommonTests: public CommonTests {};


///////////////////////////////////////////////////////////////
///// Common Tests
///////////////////////////////////////////////////////////////
TEST_F(CommonTests, ConstructDestruct) {
	EXPECT_NO_THROW(IrioV2 irio(bitfilePath, "0", "9.9"););
}

TEST_F(CommonTests, Fref) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getFref(), frefFake);
}

TEST_F(CommonTests, DevQualityStatus) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getDevQualityStatus(), devQualityStatusFake);
}

TEST_F(CommonTests, DevTemp) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getDevTemp(), devTempFake);
}

TEST_F(CommonTests, MinSamplingRate) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_DOUBLE_EQ(irio.getMinSamplingRate(),
			1.0 * frefFake / std::numeric_limits<std::uint16_t>::max());
}

TEST_F(CommonTests, MaxSamplingRate) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_DOUBLE_EQ(irio.getMaxSamplingRate(), 1.0 * frefFake);
}

TEST_F(CommonTests, DAQStartStopStatus) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getDAQStartStop(), daqStartStopFake);
}

TEST_F(CommonTests, DAQStart) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.setDAQStart(););
}

TEST_F(CommonTests, DAQStop) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.setDAQStop(););
}

TEST_F(CommonTests, getDebugMode) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getDebugMode(););
}

TEST_F(CommonTests, setDebugMode) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.setDebugMode(true););
}

TEST_F(CommonTests, startFPGA) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.startFPGA(););
}


///////////////////////////////////////////////////////////////
///// Error Common Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorCommonTests, FPGAVIVersionMismatchError) {
	const uint8_t incorrectfpgaviversion[2] = { 0, 0 };
	setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU8,
			bfp.getRegister(TERMINAL_FPGAVIVERSION).address, incorrectfpgaviversion, 2);

	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
		errors::FPGAVIVersionMismatchError);
}

TEST_F(ErrorCommonTests, UnsupportedDevProfileError) {
	const uint8_t invalidProfile = 99;
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_DEVPROFILE).address, invalidProfile);

	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
		errors::UnsupportedDevProfileError);
}

TEST_F(ErrorCommonTests, UnsupportedPlatformError) {
	const uint8_t invalidPlatform= 99;
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_PLATFORM).address, invalidPlatform);

	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
		errors::UnsupportedPlatformError);
}

TEST_F(ErrorCommonTests, InitializationTimeoutError) {
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
			bfp.getRegister(TERMINAL_INITDONE).address, 0);
	IrioV2 irio(bitfilePath, "0", "9.9");

	EXPECT_THROW(irio.startFPGA(100);,
		errors::InitializationTimeoutError);
}

TEST_F(ErrorCommonTests, NiFpgaErrorDownloadingBitfile) {
	NiFpga_Open_fake.custom_fake = [](const char*, const char*, const char*, uint32_t, NiFpga_Session* session){
		return NiFpga_Status_InternalError;
	};
	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
		errors::NiFpgaErrorDownloadingBitfile);
}

TEST_F(ErrorCommonTests, NiFpgaError) {
	NiFpga_ReadU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t*) {
		return NiFpga_Status_InternalError;
	};
	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
		errors::NiFpgaError);
}

