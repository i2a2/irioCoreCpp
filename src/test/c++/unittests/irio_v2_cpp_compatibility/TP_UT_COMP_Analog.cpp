#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_compatibility.h"
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


class AnalogTestsCompatibility: public BaseTestsCompatibility {
public:
	AnalogTestsCompatibility():
		BaseTestsCompatibility("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::FlexRIO);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
		irio_resetStatus(&status);
	}

	~AnalogTestsCompatibility() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

class ErrorAnalogTestsCompatibility: public AnalogTestsCompatibility {};

class Analog64TestsCompatibility: public BaseTestsCompatibility {
public:
	Analog64TestsCompatibility():
		BaseTestsCompatibility("../../../resources/7854", "Rseries_OnlyResources_7854")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
		irio_resetStatus(&status);
	}

	~Analog64TestsCompatibility() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

class AnalogCouplingTestsCompatibility: public BaseTestsCompatibility {
public:
	AnalogCouplingTestsCompatibility():
		BaseTestsCompatibility("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::FlexRIO);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
		irio_resetStatus(&status);
	}

	~AnalogCouplingTestsCompatibility() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

///////////////////////////////////////////////////////////////
///// Analog Tests
///////////////////////////////////////////////////////////////
TEST_F(AnalogTestsCompatibility, getAI) {
	int32_t value;
	const auto ret = irio_getAI(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, getAuxAI) {
	int32_t value;
	const auto ret = irio_getAuxAI(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(Analog64TestsCompatibility, getAuxAI_64) {
	int64_t value;
	const auto ret = irio_getAuxAI_64(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, getAO) {
	int32_t value;
	const auto ret = irio_getAO(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, getAuxAO) {
	int32_t value;
	const auto ret = irio_getAuxAO(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(Analog64TestsCompatibility, getAuxAO_64) {
	int64_t value;
	const auto ret = irio_getAuxAO_64(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, getAOEnable) {
	int32_t value;
	const auto ret = irio_getAOEnable(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, setAO) {
	const auto ret = irio_setAO(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, setAuxAO) {
	const auto ret = irio_setAuxAO(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(Analog64TestsCompatibility, setAuxAO_64) {
	const auto ret = irio_setAuxAO_64(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogTestsCompatibility, setAOEnable) {
	const auto ret = irio_setAOEnable(&p_DrvPvt, 0, 1, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogCouplingTestsCompatibility, getAICoupling) {
	TIRIOCouplingMode coupling;
	const auto ret = irio_getAICoupling(&p_DrvPvt, &coupling, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(AnalogCouplingTestsCompatibility, setAICoupling) {
	TIRIOCouplingMode coupling = IRIO_coupling_DC;
	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

/////////////////////////////////////////////////////////////////
/////// Error Analog Tests
/////////////////////////////////////////////////////////////////


TEST_F(AnalogCouplingTestsCompatibility, setAICouplingOOBWarning) {
	TIRIOCouplingMode coupling = static_cast<TIRIOCouplingMode>(99);
	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(AnalogCouplingTestsCompatibility, setAICouplingNotInitialized) {
	TIRIOCouplingMode coupling = IRIO_coupling_DC;

	irio_closeDriver(&p_DrvPvt, 0, &status);

	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_error) << "Expected error";
	EXPECT_EQ(status.detailCode, Generic_Error);
	EXPECT_EQ(ret, IRIO_error);
}

TEST_F(AnalogCouplingTestsCompatibility, setAICouplingUnsupportedCoupling) {
	TIRIOCouplingMode coupling = IRIO_coupling_NULL;

	const auto ret = irio_setAICoupling(&p_DrvPvt, coupling, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(AnalogCouplingTestsCompatibility, getAICouplingNotInitialized) {
	TIRIOCouplingMode coupling;

	irio_closeDriver(&p_DrvPvt, 0, &status);

	const auto ret = irio_getAICoupling(&p_DrvPvt, &coupling, &status);

	EXPECT_EQ(status.code, IRIO_error) << "Expected error";
	EXPECT_EQ(status.detailCode, Generic_Error);
	EXPECT_EQ(ret, IRIO_error);
}


