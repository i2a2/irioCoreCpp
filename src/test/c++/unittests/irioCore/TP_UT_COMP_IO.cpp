#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsIO.h"
#include "terminals/names/namesTerminalsCRIO.h"
#include "platforms.h"
#include "profilesTypes.h"

#include "irioDriver.h"
#include "irioError.h"

using namespace irio;


class IOTestsAdapter: public BaseTestsAdapter {
public:
	IOTestsAdapter():
		BaseTestsAdapter("../../../resources/9159", "cRIO_IO")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::cRIO);
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
					   bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
					   PROFILE_VALUE_IO);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_CRIOMODULESOK).getAddress(),
						1);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_INSERTEDIOMODULESID).getAddress(),
						insertedIOModulesIDFake, sizeof(insertedIOModulesIDFake)/sizeof(uint16_t));
		setValueForReg(ReadFunctions::NiFpga_ReadU16,
						bfp.getRegister(TERMINAL_SAMPLINGRATE+std::to_string(0)).getAddress(),
						fakeDec);
	}

	void SetUp() override {
		irio_initStatus(&status);
	}

	void TearDown() override {
		irio_resetStatus(&status);
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	~IOTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	irioDrv_t p_DrvPvt;
	TStatus status;

	const uint16_t insertedIOModulesIDFake[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	const uint16_t fakeDec = 12;
};

///////////////////////////////////////////////////////////////
/// Common Tests
///////////////////////////////////////////////////////////////
TEST_F(IOTestsAdapter, getNumIOSamplingRate) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	std::int32_t dec;
	ret = irio_getSamplingRate(&p_DrvPvt, 0, &dec, &status);
	EXPECT_EQ(dec, fakeDec) << "Unexpected value read from IO sampling rate";
	EXPECT_EQ(ret, IRIO_success) << "Error getting IO sampling rate";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}

TEST_F(IOTestsAdapter, setSamplingRate) {
	auto ret = irio_initDriver("test", "0", "TestModel",
			projectName.c_str(), "V9.9", false,
			nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);
	ASSERT_EQ(ret, IRIO_success) << "Driver not initialized properly. " << status.msg;

	ret = irio_setSamplingRate(&p_DrvPvt, 0, fakeDec, &status);
	EXPECT_EQ(ret, IRIO_success) << "Error getting IO sampling rate";
	EXPECT_EQ(status.code, IRIO_success);
	EXPECT_EQ(status.detailCode, Success);
}
