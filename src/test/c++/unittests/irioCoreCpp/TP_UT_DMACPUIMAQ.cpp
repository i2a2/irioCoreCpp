#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDMACPUCommon.h"
#include "terminals/names/namesTerminalsDMAIMAQ.h"

using namespace irio;

class DMACPUIMAQTests: public BaseTests {
public:
    DMACPUIMAQTests():
        BaseTests("../../../resources/7966/NiFpga_FlexRIO_CPUIMAQ_7966.lvbitx",
                    false) {    }

    void SetUp() override {
        init_ok_fff_nifpga();
        setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);
        setValueForReg(ReadFunctions::NiFpga_ReadU8,
                        bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
                        PROFILE_VALUE_IMAQ);
        setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU8,
						bfp.getRegister(TERMINAL_DMATTOHOSTSAMPLESIZE).getAddress(),
						sampleSizeFake, 2);
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
                        0);
        setValueForReg(ReadFunctions::NiFpga_ReadU16,
                        bfp.getRegister(TERMINAL_UARTBREAKINDICATOR).getAddress(),
                        breakIndicatorFake);
        setValueForReg(ReadFunctions::NiFpga_ReadU16,
                        bfp.getRegister(TERMINAL_UARTFRAMINGERROR).getAddress(),
                        framingErrorFake);
        setValueForReg(ReadFunctions::NiFpga_ReadU16,
                        bfp.getRegister(TERMINAL_UARTOVERRUNERROR).getAddress(),
                        overrunErrorFake);
    }
	
    const std::uint8_t sampleSizeFake[2] = {4,8};
    const std::uint16_t breakIndicatorFake = 255;
    const std::uint16_t framingErrorFake = 256;
    const std::uint16_t overrunErrorFake = 257;
};

class ErrorDMACPUIMAQTests: public DMACPUIMAQTests{};


///////////////////////////////////////////////////////////////
/// IMAQCPU Terminals Tests
///////////////////////////////////////////////////////////////

TEST_F(DMACPUIMAQTests, getTerminalsIMAQ){
    Irio irio(bitfilePath, "0", "V9.9");
    EXPECT_NO_THROW(irio.getTerminalsIMAQ());
}

TEST_F(DMACPUIMAQTests, configCameraLink){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();

	EXPECT_NO_THROW(imaq.configCameraLink(
		1, 1, 1, 1, 1, 0, CLSignalMapping::STANDARD, CLMode::FULL));
}

TEST_F(DMACPUIMAQTests, sendUARTMsg){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();

    EXPECT_NO_THROW(imaq.sendUARTMsg("test"));
}

TEST_F(DMACPUIMAQTests, recvUARTMsg){
    Irio irio(bitfilePath, "0", "V9.9");

    NiFpga_ReadU8_fake.custom_fake = [](NiFpga_Session, uint32_t, uint8_t* value){
        static std::string msg = "Message received";
		static size_t i = 0;

		*value = msg[i%(msg.length()+1)];
		i++;
        return NiFpga_Status_Success;
	};

    auto imaq = irio.getTerminalsIMAQ();
    std::string msg;
    EXPECT_NO_THROW(msg = imaq.recvUARTMsg(17));
    EXPECT_STREQ(std::string("Message received").c_str(), msg.c_str());
}

TEST_F(DMACPUIMAQTests, setUARTBaudRate){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_NO_THROW(imaq.setUARTBaudRate(UARTBaudRates::BR1152));
}

TEST_F(DMACPUIMAQTests, getUARTBaudRate){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_EQ(imaq.getUARTBaudRate(), UARTBaudRates::BR96);
}

TEST_F(DMACPUIMAQTests, getUARTBreakIndicator){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_EQ(imaq.getUARTBreakIndicator(), breakIndicatorFake);
}

TEST_F(DMACPUIMAQTests, getUARTFramingError){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_EQ(imaq.getUARTFramingError(), framingErrorFake);
}

TEST_F(DMACPUIMAQTests, getUARTOverrunError){
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_EQ(imaq.getUARTOverrunError(), overrunErrorFake);
}

TEST_F(DMACPUIMAQTests, readImage){
    const size_t numPixels = 1920;
    std::unique_ptr<std::uint64_t> data(new std::uint64_t[numPixels]);
    
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_NO_THROW(imaq.readImage(0, numPixels, data.get(), true));
}

TEST_F(DMACPUIMAQTests, readImageNonBlocking){
    const size_t numPixels = 1920;
    std::unique_ptr<std::uint64_t> data(new std::uint64_t[numPixels]);
    
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_NO_THROW(imaq.readImageNonBlocking(0, numPixels, data.get()));
}

TEST_F(DMACPUIMAQTests, readImageBlocking){
    const size_t numPixels = 1920;
    std::unique_ptr<std::uint64_t> data(new std::uint64_t[numPixels]);
    
    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_NO_THROW(imaq.readImageBlocking(0, numPixels, data.get()));
}

///////////////////////////////////////////////////////////////
/// Error IMAQCPU Terminals Tests
///////////////////////////////////////////////////////////////

TEST_F(ErrorDMACPUIMAQTests, sendUARTMsgTimeout){
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
				   bfp.getRegister(TERMINAL_UARTTXREADY).getAddress(), 0);

	Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();

	EXPECT_THROW(imaq.sendUARTMsg("test", 1), irio::errors::CLUARTTimeout);
}

TEST_F(ErrorDMACPUIMAQTests, recvUARTMsgTimeout){
    setValueForReg(ReadFunctions::NiFpga_ReadBool,
                        bfp.getRegister(TERMINAL_UARTRECEIVE).getAddress(),
                        1);

	Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_THROW(imaq.recvUARTMsg(1,1), irio::errors::CLUARTTimeout);
}

TEST_F(ErrorDMACPUIMAQTests, setUARTBaudRateTimeout1){
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
				   bfp.getRegister(TERMINAL_UARTSETBAUDRATE).getAddress(), 1);

    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
	EXPECT_THROW(imaq.setUARTBaudRate(UARTBaudRates::BR1152, 1),
				 irio::errors::CLUARTTimeout);
}

TEST_F(ErrorDMACPUIMAQTests, setUARTBaudRateTimeout2){
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
				   bfp.getRegister(TERMINAL_UARTSETBAUDRATE).getAddress(), 1);
    NiFpga_ReadBool_fake.custom_fake = [](NiFpga_Session, uint32_t, NiFpga_Bool* value){
        static std::uint8_t values[2] = {0,1};
        static bool isFirst = true;
        if(isFirst) {
            *value = values[0];
            isFirst = false;
        } else {
            *value = values[1];
        }

        return NiFpga_Status_Success;
	};

    Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
	EXPECT_THROW(imaq.setUARTBaudRate(UARTBaudRates::BR1152, 1),
				 irio::errors::CLUARTTimeout);
}

TEST_F(ErrorDMACPUIMAQTests, getUARTBaudRateInvalidBaudRate){
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
				   bfp.getRegister(TERMINAL_UARTBAUDRATE).getAddress(), 128);

	Irio irio(bitfilePath, "0", "V9.9");
    auto imaq = irio.getTerminalsIMAQ();
    EXPECT_THROW(imaq.getUARTBaudRate(), irio::errors::CLUARTInvalidBaudRate);
}
