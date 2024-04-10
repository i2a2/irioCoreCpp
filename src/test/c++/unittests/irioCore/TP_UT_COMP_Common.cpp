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

using namespace irio;


class CommonTestsAdapter: public BaseTestsAdapter {
public:
	CommonTestsAdapter():
		BaseTestsAdapter("../../../resources/7854", "Rseries_CPUDAQ_7854")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::RSeries);
	}

	void SetUp() override {
		irio_initStatus(&status);
	}

	void TearDown() override {
		irio_resetStatus(&status);
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	~CommonTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	irioDrv_t p_DrvPvt;
	TStatus status;
};

class ErrorCommonTestsAdapter: public CommonTestsAdapter {};

///////////////////////////////////////////////////////////////
/// Common Tests
///////////////////////////////////////////////////////////////
TEST_F(CommonTestsAdapter, InitClose) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", true,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);

	ret = irio_closeDriver(&p_DrvPvt, 0, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(CommonTestsAdapter, getVersion) {
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
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_success) << "Error setting FPGAStart";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getFPGAStart) {
	
	int32_t value;
	const auto ret = irio_getFPGAStart(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(CommonTestsAdapter, getFPGAVIVersion) {
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

TEST_F(CommonTestsAdapter, getPlatformType) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	uint8_t value;
	ret = irio_getPlatformType(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
	EXPECT_EQ(value, static_cast<uint8_t>(irio::PLATFORM_ID::RSeries));
}

TEST_F(CommonTestsAdapter, getInitDone) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint8_t initDone;
	ret = irio_getInitDone(&p_DrvPvt, &initDone, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxAOValue) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	float value;
	ret = irio_getMaxAOValue(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMinAOValue) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	float value;
	ret = irio_getMinAOValue(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfAI) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfAI(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfAO) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfAO(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfDI) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfDI(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfDO) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfDO(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfAuxAI) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfAuxAI(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfAuxAO) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfAuxAO(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfAuxDI) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfAuxDI(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfAuxDO) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfAuxDO(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfSG) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfSG(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfDMAs) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfDMAs(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxNumberOfGPUDMAs) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	std::uint16_t value;
	ret = irio_getMaxNumberOfGPUDMAs(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMaxSamplingRate) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	float value;
	ret = irio_getMaxSamplingRate(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getMinSamplingRate) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;
	float value;
	ret = irio_getMinSamplingRate(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(CommonTestsAdapter, getDebugMode) {
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
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", true,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, nullptr);

	EXPECT_EQ(ret, IRIO_error) <<
			"A nullptr was passed as argument to initDriver, but no error was returned";

	irio_closeDriver(&p_DrvPvt, 0, &status);
}

TEST_F(ErrorCommonTestsAdapter, IrioNotInitializedError) {
	int ret;

	ret = irio_closeDriver(&p_DrvPvt, 0, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error when function called before init";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, Generic_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, InvalidBitfile) {
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
	int ret;

	ret = irio_initDriver("test", "0", "TestModel",
				"Rseries_MismatchSG_7854", "V9.9", false,
				nullptr, "../../../resources/failResources/7854", &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, ResourceNotFound_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, FPGAVIVersionMismatchError) {
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
	
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

	EXPECT_EQ(ret, IRIO_error) << "Expected error";
	EXPECT_EQ(status.code, IRIO_error) << "Invalid error code";
	EXPECT_EQ(status.detailCode, Generic_Error) << "Invalid detailed error code";
	EXPECT_NE(status.msg, nullptr) << "No error message included with error";
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartWhenAlreadyStarted) {
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
	auto ret = irio_setFPGAStart(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_error) << "Expected error when calling the function without calling first initDriver";
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, Generic_Error);
}

TEST_F(ErrorCommonTestsAdapter, setFPGAStartNiFpgaFPGAAlreadyRunning) {
	NiFpga_Run_fake.custom_fake = [](NiFpga_Session, uint32_t) {
		return NiFpga_Status_FpgaAlreadyRunning;
	};

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
				bfp.getRegister(TERMINAL_INITDONE).getAddress(), 0);

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
	int32_t value;
	auto ret = irio_getDAQStartStop(&p_DrvPvt, &value, &status);
	EXPECT_EQ(ret, IRIO_error);
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, Generic_Error);
}

TEST_F(ErrorCommonTestsAdapter, getCommonReadNiRIOWarning) {
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
	auto ret = irio_setDAQStartStop(&p_DrvPvt, 1, &status);
	EXPECT_EQ(ret, IRIO_error);
	EXPECT_EQ(status.code, IRIO_error);
	EXPECT_EQ(status.detailCode, Generic_Error);
}

TEST_F(ErrorCommonTestsAdapter, setCommonReadNIRIOWarning) {
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


