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
#include "irioHandlerAnalog.h"

using namespace iriov2;

template<ModulesType M>
void setFlexRIOConnectedModule(){
	NiFlexRio_GetAttribute_fake.custom_fake =
			[](NiFpga_Session, int32_t, int32_t, void *value) {
		*reinterpret_cast<uint32_t*>(value) =
				static_cast<std::uint32_t>(M);
		return NiFpga_Status_Success;
	};
}


class AnalogTestsAdapter: public BaseTestsAdapter {
public:
	AnalogTestsAdapter():
		BaseTestsAdapter("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::FlexRIO);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "V9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
		irio_resetStatus(&status);
	}

	~AnalogTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

class ErrorAnalogTestsAdapter: public AnalogTestsAdapter {};

class Analog64TestsAdapter: public BaseTestsAdapter {
public:
	Analog64TestsAdapter():
		BaseTestsAdapter("../../../resources/7854", "Rseries_OnlyResources_7854")
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

	~Analog64TestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

class AnalogCouplingTestsAdapter: public BaseTestsAdapter {
public:
	AnalogCouplingTestsAdapter():
		BaseTestsAdapter("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::FlexRIO);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "V9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
		irio_resetStatus(&status);
	}

	~AnalogCouplingTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

///////////////////////////////////////////////////////////////
///// Analog Tests
///////////////////////////////////////////////////////////////
TEST_F(AnalogTestsAdapter, getAI) {
	int32_t value;
	const auto ret = irio_getAI(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, getAuxAI) {
	int32_t value;
	const auto ret = irio_getAuxAI(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(Analog64TestsAdapter, getAuxAI_64) {
	int64_t value;
	const auto ret = irio_getAuxAI_64(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, getAO) {
	int32_t value;
	const auto ret = irio_getAO(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, getAuxAO) {
	int32_t value;
	const auto ret = irio_getAuxAO(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(Analog64TestsAdapter, getAuxAO_64) {
	int64_t value;
	const auto ret = irio_getAuxAO_64(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, getAOEnable) {
	int32_t value;
	const auto ret = irio_getAOEnable(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, setAO) {
	const auto ret = irio_setAO(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, setAuxAO) {
	const auto ret = irio_setAuxAO(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(Analog64TestsAdapter, setAuxAO_64) {
	const auto ret = irio_setAuxAO_64(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsAdapter, setAOEnable) {
	const auto ret = irio_setAOEnable(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogCouplingTestsAdapter, getAICoupling) {
	TIRIOCouplingMode coupling;
	const auto ret = irio_getAICoupling(&p_DrvPvt, &coupling, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogCouplingTestsAdapter, setAICoupling) {
	TIRIOCouplingMode coupling = IRIO_coupling_DC;
	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

/////////////////////////////////////////////////////////////////
/////// Error Analog Tests
/////////////////////////////////////////////////////////////////


TEST_F(AnalogCouplingTestsAdapter, setAICouplingOOBWarning) {
	TIRIOCouplingMode coupling = static_cast<TIRIOCouplingMode>(99);
	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(AnalogCouplingTestsAdapter, setAICouplingNotInitialized) {
	TIRIOCouplingMode coupling = IRIO_coupling_DC;

	irio_closeDriver(&p_DrvPvt, 0, &status);

	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_error) << "Expected error";
	EXPECT_EQ(status.detailCode, Generic_Error);
	EXPECT_EQ(ret, IRIO_error);
}

TEST_F(AnalogCouplingTestsAdapter, setAICouplingUnsupportedCoupling) {
	TIRIOCouplingMode coupling = IRIO_coupling_NULL;

	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(AnalogCouplingTestsAdapter, getAICouplingNotInitialized) {
	TIRIOCouplingMode coupling;

	irio_closeDriver(&p_DrvPvt, 0, &status);

	const auto ret = irio_getAICoupling(&p_DrvPvt, &coupling, &status);

	EXPECT_EQ(status.code, IRIO_error) << "Expected error";
	EXPECT_EQ(status.detailCode, Generic_Error);
	EXPECT_EQ(ret, IRIO_error);
}


