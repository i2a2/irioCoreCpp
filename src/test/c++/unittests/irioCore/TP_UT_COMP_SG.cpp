#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "platforms.h"
#include "modules.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioHandlerSG.h"

using namespace irio;


class SGTestsAdapter: public BaseTestsAdapter {
public:
	SGTestsAdapter():
		BaseTestsAdapter("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "V9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
	}

	void SetUp() override {
		irio_initStatus(&status);
	}

	void TearDown() override {
		irio_resetStatus(&status);
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	~SGTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};


///////////////////////////////////////////////////////////////
// SG Tests
///////////////////////////////////////////////////////////////
TEST_F(SGTestsAdapter, getNumSG) {
	size_t num;
	const auto ret = irio_getNumSG(&p_DrvPvt, &num, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(num, 2);
}

TEST_F(SGTestsAdapter, getSGSignalType) {
	int32_t value;
	const auto ret = irio_getSGSignalType(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, setSGSignalType) {
	int32_t value = 1;
	const auto ret = irio_setSGSignalType(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGFreq) {
	int32_t value;
	const auto ret = irio_getSGFreq(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, setSGFreq) {
	int32_t value = 1;
	const auto ret = irio_setSGFreq(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGPhase) {
	int32_t value;
	const auto ret = irio_getSGPhase(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, setSGPhase) {
	int32_t value = 1;
	const auto ret = irio_setSGPhase(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGAmp) {
	int32_t value;
	const auto ret = irio_getSGAmp(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, setSGAmp) {
	int32_t value = 1;
	const auto ret = irio_setSGAmp(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGUpdateRate) {
	int32_t value;
	const auto ret = irio_getSGUpdateRate(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, setSGUpdateRate) {
	int32_t value = 1;
	const auto ret = irio_setSGUpdateRate(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGFref) {
	uint32_t value;
	const auto ret = irio_getSGFref(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getFref) {
	int32_t value;
	const auto ret = irio_getFref(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGCVDAC) {
	double value;
	const auto ret = irio_getSGCVDAC(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(SGTestsAdapter, getSGCVADC) {
	double value;
	const auto ret = irio_getSGCVADC(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

