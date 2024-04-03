#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDMADAQCPU.h"
#include "platforms.h"
#include "modules.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioHandlerDMA.h"

using namespace irio;


class DMATestsAdapter: public BaseTestsAdapter {
public:
	DMATestsAdapter():
		BaseTestsAdapter("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);

		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_DMATTOHOSTBLOCKNWORDS).getAddress(),
						nwords, sizeof(nwords)/sizeof(std::uint16_t));

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

	~DMATestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
private:
	std::uint16_t nwords[1] = {1};
};

class ErrorDMATestsAdapter: public DMATestsAdapter {};


///////////////////////////////////////////////////////////////
///// DMA Tests
///////////////////////////////////////////////////////////////
TEST_F(DMATestsAdapter, setUpDMAsTtoHost) {
	int32_t value;
	const auto ret = irio_setUpDMAsTtoHost(&p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, closeDMAsTtoHost) {
	int32_t value;
	const auto ret = irio_closeDMAsTtoHost(&p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, cleanDMAsTtoHost) {
	int32_t value;
	const auto ret = irio_cleanDMAsTtoHost(&p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, cleanDMATtoHost) {
	int32_t value;
	const auto ret = irio_cleanDMATtoHost(&p_DrvPvt, 0, nullptr, 0, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHostOverflow) {
	int32_t value;
	const auto ret = irio_getDMATtoHostOverflow(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHostSamplingRate) {
	int32_t value;
	const auto ret = irio_getDMATtoHostSamplingRate(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, setDMATtoHostSamplingRate) {
	int32_t value = 1;
	const auto ret = irio_setDMATtoHostSamplingRate(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHostEnable) {
	int32_t value;
	const auto ret = irio_getDMATtoHostEnable(&p_DrvPvt, 0, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, setDMATtoHostEnable) {
	int32_t value = 1;
	const auto ret = irio_setDMATtoHostEnable(&p_DrvPvt, 0, value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHOSTBlockNWords) {
	uint16_t value;
	const auto ret = irio_getDMATtoHOSTBlockNWords(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHOSTNCh) {
	uint16_t value;
	const auto ret = irio_getDMATtoHOSTNCh(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHostData) {
	int32_t elementsRead;
	uint64_t data[4096];
	const auto ret = irio_getDMATtoHostData(&p_DrvPvt, 1, 0, data,
			&elementsRead, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(DMATestsAdapter, getDMATtoHostData_timeout) {
	int32_t elementsRead;
	uint64_t data[4096];
	const auto ret = irio_getDMATtoHostData_timeout(&p_DrvPvt, 1, 0, data,
			&elementsRead, 1000, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

/////////////////////////////////////////////////////////////////
/////// Error DMA Tests
/////////////////////////////////////////////////////////////////
TEST_F(ErrorDMATestsAdapter, NiFpgaErrorSetUp) {
	NiFpga_StartFifo_fake.custom_fake = [](NiFpga_Session, uint32_t) {
		return NiFpga_Status_InternalError;
	};

	int32_t value;
	const auto ret = irio_setUpDMAsTtoHost(&p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, ConfigDMA_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorDMATestsAdapter, NiFpgaErrorCloseDMAs) {
	NiFpga_StopFifo_fake.custom_fake = [](NiFpga_Session, uint32_t) {
		return NiFpga_Status_InternalError;
	};

	int32_t value;
	const auto ret = irio_closeDMAsTtoHost(&p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, ConfigDMA_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorDMATestsAdapter, NiFpgaErrorCleanDMAs) {
	NiFpga_ReadFifoU64_fake.custom_fake = [](NiFpga_Session, uint32_t,
			uint64_t*, size_t, uint32_t, size_t*) {
		return NiFpga_Status_InternalError;
	};

	int32_t value;
	const auto ret = irio_cleanDMAsTtoHost(&p_DrvPvt, &status);

	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, ConfigDMA_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorDMATestsAdapter, NiFpgaErrorCleanDMA) {
	NiFpga_ReadFifoU64_fake.custom_fake = [](NiFpga_Session, uint32_t,
			uint64_t*, size_t, uint32_t, size_t*) {
		return NiFpga_Status_InternalError;
	};

	int32_t value;
	const auto ret = irio_cleanDMATtoHost(&p_DrvPvt, 0, nullptr, 0, &status);

	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, ConfigDMA_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorDMATestsAdapter, ErrorTimeoutGetDMATtoHostData_timeout) {
	NiFpga_ReadFifoU64_fake.custom_fake = [](NiFpga_Session, uint32_t,
			uint64_t*, size_t, uint32_t, size_t*) {
		return NiFpga_Status_FifoTimeout;
	};

	int32_t elementsRead;
	uint64_t data[256];
	const auto ret = irio_getDMATtoHostData_timeout(&p_DrvPvt, 1, 0, data,
			&elementsRead, 1000, &status);

	EXPECT_EQ(status.code, IRIO_warning);
	EXPECT_EQ(status.detailCode, Read_NIRIO_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

