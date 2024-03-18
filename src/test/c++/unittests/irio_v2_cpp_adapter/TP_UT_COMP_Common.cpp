#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "platforms.h"

#include "irioDriver.h"
#include "irioError.h"

using namespace iriov2;


class CommonTestsAdapter: public BaseTestsAdapter {
public:
	CommonTestsAdapter():
		BaseTestsAdapter("../../../resources/7854", "Rseries_CPUDAQ_7854")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);
	}

	~CommonTestsAdapter() {
		TStatus status;
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	irioDrv_t p_DrvPvt;
};

class ErrorCommonTestsAdapter: public CommonTestsAdapter {};

///////////////////////////////////////////////////////////////
///// Common Tests
///////////////////////////////////////////////////////////////
TEST_F(CommonTestsAdapter, InitClose) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);

	ret = irio_closeDriver(&p_DrvPvt, 0, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(CommonTestsAdapter, getVersion) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	std::unique_ptr<char> irioVersion(new char[256]);
	ret = irio_getVersion(irioVersion.get(), &status);
	EXPECT_EQ(ret, IRIO_success) << "Error getting IRIO Driver Version";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, setFPGAStart) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_success) << "Error setting FPGAStart";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getFPGAVIVersion) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	char version[10];
	size_t valueLength;

	ret = irio_getFPGAVIVersion(&p_DrvPvt, version, sizeof(version), &valueLength, &status);
	EXPECT_EQ(ret, IRIO_success) << "Error getting FPGAVIVersion";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);

	EXPECT_EQ(valueLength, 4);
	EXPECT_EQ(std::string(version), "V9.9");
}

TEST_F(CommonTestsAdapter, getDevQualityStatus) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	int32_t value;
	ret = irio_getDevQualityStatus(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getDevTemp) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	int32_t value;
	ret = irio_getDevTemp(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getProfile) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	int32_t value;
	ret = irio_getDevProfile(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getDebugMode) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	int32_t value;
	ret = irio_getDebugMode(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getDAQStartStop) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	int32_t value;
	ret = irio_getDAQStartStop(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, setDebugMode) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setDebugMode(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, setDAQStartStop) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setDAQStartStop(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, mergeStatusSuccess) {
	TStatus status;
	const std::string testString = "Test okay";

	const auto ret = irio_mergeStatus(&status, Success, 0, testString.c_str());
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
	EXPECT_EQ(std::string(status.msg), testString);
}

TEST_F(CommonTestsAdapter, getErrorString) {
	char* str;
	irio_getErrorString(Success, &str);
	EXPECT_EQ(std::string(str), "Device status is OK");
	free(str);
}

/////////////////////////////////////////////////////////////////
/////// Error Common Tests
/////////////////////////////////////////////////////////////////
TEST_F(ErrorCommonTestsAdapter, InitStatusNullptr) {
	TStatus status;
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", true,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, nullptr);

	EXPECT_EQ(ret, IRIO_error) <<
			"A nullptr was passed as argument to initDriver, but no error was returned";

	irio_closeDriver(&p_DrvPvt, 0, &status);
}

TEST_F(ErrorCommonTestsAdapter, IrioV2NotInitializedError) {
	TStatus status;
	int ret;

	ret = irio_closeDriver(&p_DrvPvt, 0, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error when function called before init";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, Generic_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, InvalidBitfile) {
	TStatus status;
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
				"InvalidProjectName", "V9.9", true,
				nullptr, "InvalidBitfileDir", &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, BitfileNotFound_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, InitResourceNotFoundError) {
	TStatus status;
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
				"Rseries_MismatchSG_7854", "V9.9", true,
				nullptr, "../../../resources/failResources/7854", &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, ResourceNotFound_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, FPGAVIVersionMismatchError) {
	TStatus status;
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "0.0", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, NIRIO_API_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, BitfileDownloadError) {
	NiFpga_Open_fake.custom_fake = [](const char*, const char*, const char*, uint32_t, NiFpga_Session* session){
		return NiFpga_Status_InternalError;
	};
	TStatus status;
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, BitfileDownload_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, GenericError) {
	NiFpga_ReadU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t*) {
		return NiFpga_Status_InternalError;
	};

	TStatus status;
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, Generic_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartWhenAlreadyStarted) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_success) << "Error setting FPGAStart";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_warning) << "Expected error when starting the FPGA twice";
	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, FPGAAlreadyRunning_Warning);
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartDriverNoInit) {
	TStatus status;

	auto ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_error) << "Expected error when calling the function without calling first initDriver";
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, Generic_Error);
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartNiFpgaFPGAAlreadyRunning) {
	NiFpga_Run_fake.custom_fake = [](NiFpga_Session, uint32_t) {
		return NiFpga_Status_FpgaAlreadyRunning;
	};

	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_warning) << "Expected error when starting FPGA";
	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, FPGAAlreadyRunning_Warning);
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartNiFpgaError) {
	NiFpga_Run_fake.custom_fake = [](NiFpga_Session, uint32_t) {
		return NiFpga_Status_SoftwareFault;
	};

	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_error) << "Expected error when starting FPGA";
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, NIRIO_API_Error);
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartInitDoneError) {
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
				bfp.getRegister(TERMINAL_INITDONE).address, 0);

	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_error) << "Expected error when starting FPGA";
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, InitDone_Error);
}

TEST_F(ErrorCommonTestsAdapter, getFPGAVIVersionReadResourceWarning) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	char name[1];
	size_t valueLength;

	ret = irio_getFPGAVIVersion(&p_DrvPvt, name, sizeof(name), &valueLength, &status);
	EXPECT_EQ(ret, IRIO_warning) << "Not error when getting FPGAVIVersion";
	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, Read_Resource_Warning);

	EXPECT_EQ(valueLength, 1);
}

TEST_F(ErrorCommonTestsAdapter, getCommonNotInitError) {
	TStatus status;
	int32_t value;
	auto ret = irio_getDAQStartStop(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_error);
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, Generic_Error);
}

TEST_F(ErrorCommonTestsAdapter, getCommonReadNiRIOWarning) {
	TStatus status;
	int32_t value;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	NiFpga_ReadU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t*) {
		return NiFpga_Status_InternalError;
	};

	ret = irio_getDAQStartStop(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_warning);
	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, Read_NIRIO_Warning);
}

TEST_F(ErrorCommonTestsAdapter, setCommonNotInitError) {
	TStatus status;
	auto ret = irio_setDAQStartStop(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_error);
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, Generic_Error);
}

TEST_F(ErrorCommonTestsAdapter, setCommonReadNIRIOWarning) {
	TStatus status;

	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	NiFpga_WriteU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t) {
		return NiFpga_Status_InternalError;
	};

	ret = irio_setDAQStartStop(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_warning);
	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, Read_NIRIO_Warning);
}

TEST_F(ErrorCommonTestsAdapter, getErrorStringInvalidErrorCode) {
	char* str;
	irio_getErrorString(static_cast<TErrorDetailCode>(99999), &str);
	EXPECT_EQ(std::string(str), "Error code not defined");
	free(str);
}


