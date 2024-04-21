#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures.h"
#include "fff_nifpga.h"


#include "irioCoreCpp.h"
#include "rioDiscovery.h"
#include "errorsIrio.h"
#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "terminals/names/namesTerminalsCRIO.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "modules.h"


using namespace irio;


class CommonTests: public BaseTests {
public:
	CommonTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::RSeries);
	}
};

class ErrorCommonTests: public CommonTests {};


///////////////////////////////////////////////////////////////
///// Common Tests
///////////////////////////////////////////////////////////////
TEST_F(CommonTests, ConstructDestruct) {
	EXPECT_NO_THROW(Irio irio(bitfilePath, "0", "V9.9"););
}

TEST_F(CommonTests, Fref) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsCommon().getFref(), frefFake);
}

TEST_F(CommonTests, DevQualityStatus) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsCommon().getDevQualityStatus(),
			  devQualityStatusFake);
}

TEST_F(CommonTests, DevTemp) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsCommon().getDevTemp(), devTempFake);
}

TEST_F(CommonTests, MinSamplingRate) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_DOUBLE_EQ(irio.getTerminalsCommon().getMinSamplingRate(),
			1.0 * frefFake / std::numeric_limits<std::uint16_t>::max());
}

TEST_F(CommonTests, MaxSamplingRate) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_DOUBLE_EQ(irio.getTerminalsCommon().getMaxSamplingRate(),
					 1.0 * frefFake);
}

TEST_F(CommonTests, DAQStartStopStatus) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsCommon().getDAQStartStop(), daqStartStopFake);
}

TEST_F(CommonTests, DAQStart) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsCommon().setDAQStart(););
}

TEST_F(CommonTests, DAQStop) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsCommon().setDAQStop(););
}

TEST_F(CommonTests, getDebugMode) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsCommon().getDebugMode(););
}

TEST_F(CommonTests, setDebugMode) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsCommon().setDebugMode(true););
}

TEST_F(CommonTests, startFPGA) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.startFPGA(););
}

TEST_F(CommonTests, getPlatform) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getPlatform(););
}

TEST_F(CommonTests, getProfileID) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getProfileID(););
}

TEST_F(CommonTests, closeAttribute) {
	EXPECT_NO_THROW(
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getCloseAttribute(), 0);
	irio.setCloseAttribute(1);
	EXPECT_EQ(irio.getCloseAttribute(), 1);
	);
}

///////////////////////////////////////////////////////////////
///// Error Common Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorCommonTests, FPGAVIVersionMismatchError) {
	const uint8_t incorrectfpgaviversion[2] = { 0, 0 };
	setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU8,
			bfp.getRegister(TERMINAL_FPGAVIVERSION).getAddress(), incorrectfpgaviversion, 2);

	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9");,
		errors::FPGAVIVersionMismatchError);
}

TEST_F(ErrorCommonTests, UnsupportedDevProfileError) {
	const uint8_t invalidProfile = 99;
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(), invalidProfile);

	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9");,
		errors::UnsupportedDevProfileError);
}

TEST_F(ErrorCommonTests, UnsupportedPlatformError) {
	const uint8_t invalidPlatform= 99;
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_PLATFORM).getAddress(), invalidPlatform);

	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9");,
		errors::UnsupportedPlatformError);
}

TEST_F(ErrorCommonTests, InitializationTimeoutError) {
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
			bfp.getRegister(TERMINAL_INITDONE).getAddress(), 0);
	Irio irio(bitfilePath, "0", "V9.9");

	EXPECT_THROW(irio.startFPGA(100);,
		errors::InitializationTimeoutError);
}

TEST_F(ErrorCommonTests, NiFpgaError) {
	NiFpga_ReadU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t*) {
		return NiFpga_Status_InternalError;
	};
	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9");,
		errors::NiFpgaError);
}

TEST_F(ErrorCommonTests, NiFpgaErrorDownloadingBitfile) {
	NiFpga_Open_fake.custom_fake = [](const char*, const char*, const char*, uint32_t, NiFpga_Session* session){
		return NiFpga_Status_InternalError;
	};
	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9");,
		errors::NiFpgaErrorDownloadingBitfile);
}

TEST_F(ErrorCommonTests, NiFpgaFPGAAlreadyRunning) {
	NiFpga_Run_fake.custom_fake = [](NiFpga_Session, uint32_t) {
		return NiFpga_Status_FpgaAlreadyRunning;
	};

	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.startFPGA();,
		errors::NiFpgaFPGAAlreadyRunning);
}

