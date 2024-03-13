#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_compatibility.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "platforms.h"

#include "irioDriver.h"

using namespace iriov2;


class CommonTests: public BaseTestsCompatibility {
public:
	CommonTests():
		BaseTestsCompatibility("../../../resources/7854", "Rseries_CPUDAQ_7854")
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
TEST_F(CommonTests, InitClose) {
	TStatus status;
	irioDrv_t p_DrvPvt;
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);

	ret = irio_closeDriver(&p_DrvPvt, 0, &status);

	EXPECT_EQ(status.code, Success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

//TEST_F(CommonTests, Fref) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_EQ(irio.getFref(), frefFake);
//}
//
//TEST_F(CommonTests, DevQualityStatus) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_EQ(irio.getDevQualityStatus(), devQualityStatusFake);
//}
//
//TEST_F(CommonTests, DevTemp) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_EQ(irio.getDevTemp(), devTempFake);
//}
//
//TEST_F(CommonTests, MinSamplingRate) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_DOUBLE_EQ(irio.getMinSamplingRate(),
//			1.0 * frefFake / std::numeric_limits<std::uint16_t>::max());
//}
//
//TEST_F(CommonTests, MaxSamplingRate) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_DOUBLE_EQ(irio.getMaxSamplingRate(), 1.0 * frefFake);
//}
//
//TEST_F(CommonTests, DAQStartStopStatus) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_EQ(irio.getDAQStartStop(), daqStartStopFake);
//}
//
//TEST_F(CommonTests, DAQStart) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_NO_THROW(irio.setDAQStart(););
//}
//
//TEST_F(CommonTests, DAQStop) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_NO_THROW(irio.setDAQStop(););
//}
//
//TEST_F(CommonTests, getDebugMode) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_NO_THROW(irio.getDebugMode(););
//}
//
//TEST_F(CommonTests, setDebugMode) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_NO_THROW(irio.setDebugMode(true););
//}
//
//TEST_F(CommonTests, startFPGA) {
//	IrioV2 irio(bitfilePath, "0", "9.9");
//	EXPECT_NO_THROW(irio.startFPGA(););
//}
//
//
/////////////////////////////////////////////////////////////////
/////// Error Common Tests
/////////////////////////////////////////////////////////////////
TEST_F(ErrorCommonTests, InitStatusNullptr) {
	irioDrv_t p_DrvPvt;
	TStatus status;
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "9.9", true,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, nullptr);

	EXPECT_EQ(ret, IRIO_error) <<
			"A nullptr was passed as argument to initDriver, but no error was returned";

	irio_closeDriver(&p_DrvPvt, 0, &status);
}

TEST_F(ErrorCommonTests, CloseWhenNoInit) {
	irioDrv_t p_DrvPvt;
	TStatus status;
	int ret;

	ret = irio_closeDriver(&p_DrvPvt, 0, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error when close called before init";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, Generic_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTests, InvalidBitfile) {
	irioDrv_t p_DrvPvt;
	TStatus status;
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
				"InvalidProjectName", "9.9", true,
				nullptr, "InvalidBitfileDir", &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, BitfileNotFound_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTests, InitResourceNotFoundError) {
	irioDrv_t p_DrvPvt;
	TStatus status;
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
				"Rseries_MismatchSG_7854", "9.9", true,
				nullptr, "../../../resources/failResources/7854", &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, ResourceNotFound_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTests, FPGAVIVersionMismatchError) {
	TStatus status;
	irioDrv_t p_DrvPvt;
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "0.0", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, NIRIO_API_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTests, NiFpgaError) {
	NiFpga_Open_fake.custom_fake = [](const char*, const char*, const char*, uint32_t, NiFpga_Session* session){
		*session = 42;
		return NiFpga_Status_InternalError;
	};
	TStatus status;
	irioDrv_t p_DrvPvt;
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, Generic_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

//
//TEST_F(ErrorCommonTests, UnsupportedDevProfileError) {
//	const uint8_t invalidProfile = 99;
//	setValueForReg(ReadFunctions::NiFpga_ReadU8,
//			bfp.getRegister(TERMINAL_DEVPROFILE).address, invalidProfile);
//
//	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
//		errors::UnsupportedDevProfileError);
//}
//
//TEST_F(ErrorCommonTests, UnsupportedPlatformError) {
//	const uint8_t invalidPlatform= 99;
//	setValueForReg(ReadFunctions::NiFpga_ReadU8,
//			bfp.getRegister(TERMINAL_PLATFORM).address, invalidPlatform);
//
//	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
//		errors::UnsupportedPlatformError);
//}
//
//TEST_F(ErrorCommonTests, InitializationTimeoutError) {
//	setValueForReg(ReadFunctions::NiFpga_ReadBool,
//			bfp.getRegister(TERMINAL_INITDONE).address, 0);
//	IrioV2 irio(bitfilePath, "0", "9.9");
//
//	EXPECT_THROW(irio.startFPGA(100);,
//		errors::InitializationTimeoutError);
//}
//
//TEST_F(ErrorCommonTests, NiFpgaError) {
//	NiFpga_Open_fake.custom_fake = [](const char*, const char*, const char*, uint32_t, NiFpga_Session* session){
//		*session = 42;
//		return NiFpga_Status_InternalError;
//	};
//	EXPECT_THROW(IrioV2 irio(bitfilePath, "0", "9.9");,
//		errors::NiFpgaError);
//}
