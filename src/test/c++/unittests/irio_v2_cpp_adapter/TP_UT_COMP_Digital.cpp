#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "platforms.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioHandlerDigital.h"

using namespace iriov2;


class DigitalTestsAdapter: public BaseTestsAdapter {
public:
	DigitalTestsAdapter():
		BaseTestsAdapter("../../../resources/7854", "Rseries_CPUDAQ_7854")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "V9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
		irio_resetStatus(&status);
	}

	~DigitalTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

///////////////////////////////////////////////////////////////
///// Digital Tests
///////////////////////////////////////////////////////////////
TEST_F(DigitalTestsAdapter, getDI) {
	int32_t value;
	const auto ret = irio_getDI(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DigitalTestsAdapter, getAuxDI) {
	int32_t value;
	const auto ret = irio_getAuxDI(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DigitalTestsAdapter, getDO) {
	int32_t value;
	const auto ret = irio_getDO(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DigitalTestsAdapter, getAuxDO) {
	int32_t value;
	const auto ret = irio_getAuxDO(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DigitalTestsAdapter, setDO) {
	const auto ret = irio_setDO(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DigitalTestsAdapter, setAuxDO) {
	const auto ret = irio_setAuxDO(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}
