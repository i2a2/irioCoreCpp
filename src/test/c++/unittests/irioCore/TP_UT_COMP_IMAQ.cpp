#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDMACPUCommon.h"
#include "terminals/names/namesTerminalsDMAIMAQ.h"
#include "platforms.h"
#include "profilesTypes.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioHandlerImage.h"

using namespace irio;

class IMAQTestsAdapter: public BaseTestsAdapter {
public:
	IMAQTestsAdapter():
		BaseTestsAdapter("../../../resources/7966", "FlexRIO_CPUIMAQ_7966",
							false)
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);
        setValueForReg(ReadFunctions::NiFpga_ReadU8,
                        bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
                        PROFILE_VALUE_IMAQ);

		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "V9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			const std::string errMsg =
				"Unable to initialize driver: " + std::string(status.msg);
			throw std::runtime_error(errMsg);
		}
	}

	void SetUp() override {
		init_ok_fff_nifpga();
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
                        bfp.getRegister(TERMINAL_UARTTXREADY).getAddress(),
                        1);
        setValueForReg(ReadFunctions::NiFpga_ReadBool,
                        bfp.getRegister(TERMINAL_UARTRXREADY).getAddress(),
                        1);
        setValueForReg(ReadFunctions::NiFpga_ReadBool,
                        bfp.getRegister(TERMINAL_UARTRXREADY).getAddress(),
                        1);
        setValueForReg(ReadFunctions::NiFpga_ReadBool,
                        bfp.getRegister(TERMINAL_UARTRECEIVE).getAddress(),
                        0);
        setValueForReg(ReadFunctions::NiFpga_ReadBool,
                        bfp.getRegister(TERMINAL_UARTSETBAUDRATE).getAddress(),
                        0);
        setValueForReg(ReadFunctions::NiFpga_ReadU8,
                        bfp.getRegister(TERMINAL_UARTBAUDRATE).getAddress(),
                        5);
        setValueForReg(ReadFunctions::NiFpga_ReadU16,
                        bfp.getRegister(TERMINAL_UARTBREAKINDICATOR).getAddress(),
                        breakIndicatorFake);
        setValueForReg(ReadFunctions::NiFpga_ReadU16,
                        bfp.getRegister(TERMINAL_UARTFRAMINGERROR).getAddress(),
                        framingErrorFake);
        setValueForReg(ReadFunctions::NiFpga_ReadU16,
                        bfp.getRegister(TERMINAL_UARTOVERRUNERROR).getAddress(),
                        overrunErrorFake);
		
		irio_initStatus(&status);
	}

	void TearDown() override {
		irio_resetStatus(&status);
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	~IMAQTestsAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;

	const std::uint16_t breakIndicatorFake = 255;
    const std::uint16_t framingErrorFake = 256;
    const std::uint16_t overrunErrorFake = 257;
private:
	std::uint16_t nwords[1] = {1};
};

class ErrorIMAQTestsAdapter: public IMAQTestsAdapter {};


///////////////////////////////////////////////////////////////
/// IMAQ Tests
///////////////////////////////////////////////////////////////
TEST_F(IMAQTestsAdapter, configCL) {
	const auto ret = irio_configCL(&p_DrvPvt, 1, 1, 1, 1, 0, 0, CL_STANDARD,
								   CL_BASE, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(IMAQTestsAdapter, sendCLuart) {
	const char* msg = "test\n";
	const auto ret =
		irio_sendCLuart(&p_DrvPvt, msg, sizeof(msg) / sizeof(char), &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(IMAQTestsAdapter, getCLuart) {
	const size_t lenData = 17;
	std::unique_ptr<char> data(new char[lenData]);
	int msgSize;

	NiFpga_ReadU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t* value){
        static std::string msg = "Message received";
		static size_t i = 0;

		*value = msg[i%(msg.length()+1)];
		i++;
        return NiFpga_Status_Success;
	};

	const auto ret =
		irio_getCLuartWithBufferSize(&p_DrvPvt, lenData, data.get(), &msgSize, &status);

	EXPECT_STREQ(std::string("Message received").c_str(),
				 std::string(data.get()).c_str());
	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(IMAQTestsAdapter, getUARTBaudRate) {
	const int expectedBR = 5;
	int br;
	const auto ret = irio_getUARTBaudRate(&p_DrvPvt, &br, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(br, expectedBR);
}

TEST_F(IMAQTestsAdapter, setUARTBaudRate) {
	const int baudRate = 4;
	const auto ret = irio_setUARTBaudRate(&p_DrvPvt, baudRate, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
}

TEST_F(IMAQTestsAdapter, getUARTBreakIndicator) {
	int value;
	const auto ret = irio_getUARTBreakIndicator(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(value, breakIndicatorFake);
}

TEST_F(IMAQTestsAdapter, getUARTFramingError) {
	int value;
	const auto ret = irio_getUARTFramingError(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(value, framingErrorFake);
}

// getUARTFrammingError is deprecated (double m), so ignore the compiler warning
// for this test
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
TEST_F(IMAQTestsAdapter, getUARTFrammingError) {
	int value;
	const auto ret = irio_getUARTFrammingError(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(value, framingErrorFake);
}
#pragma GCC diagnostic pop

TEST_F(IMAQTestsAdapter, getUARTOverrunError) {
	int value;
	const auto ret = irio_getUARTOverrunError(&p_DrvPvt, &value, &status);

	EXPECT_EQ(status.code, IRIO_success) << status.msg;
	EXPECT_EQ(ret, IRIO_success);
	EXPECT_EQ(value, overrunErrorFake);
}


/////////////////////////////////////////////////////////////////
/// Error IMAQ Tests
/////////////////////////////////////////////////////////////////
TEST_F(ErrorIMAQTestsAdapter, configCLBadSignalMapping) {
	std::uint32_t badSigMap = 125;
	const auto ret = irio_configCL(&p_DrvPvt, 1, 1, 1, 1, 0, 0,
								   static_cast<CL_SignalMapping_T>(badSigMap),
								   CL_BASE, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning)
		<< "Incorrect error detail code";
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorIMAQTestsAdapter, configCLBadConfiguration) {
	std::uint32_t badConfig = 125;
	const auto ret =
		irio_configCL(&p_DrvPvt, 1, 1, 1, 1, 0, 0, CL_STANDARD,
					  static_cast<CL_Config_T>(badConfig), &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning)
		<< "Incorrect error detail code";
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorIMAQTestsAdapter, getUARTBaudRateInvalid) {
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
				   bfp.getRegister(TERMINAL_UARTBAUDRATE).getAddress(), 125);
	int br;
	const auto ret = irio_getUARTBaudRate(&p_DrvPvt, &br, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}

TEST_F(ErrorIMAQTestsAdapter, setUARTBaudRateInvalid) {
	const auto ret = irio_setUARTBaudRate(&p_DrvPvt, 125, &status);

	EXPECT_EQ(status.code, IRIO_warning) << "Expected warning";
	EXPECT_EQ(status.detailCode, ValueOOB_Warning);
	EXPECT_EQ(ret, IRIO_warning);
}
