#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <chrono>

#include <irioCoreCpp.h>
#include "irioFixture.h"


using namespace irio;

class FlexRIOCPUIMAQ : public IrioFixture {
 public:
  FlexRIOCPUIMAQ() : IrioFixture("CPUIMAQ") {
	if(boardType != "FlexRIO") {
        throw std::runtime_error("Expected a FlexRIO. Got " + boardType);
    }
  }
};

class FlexRIOCPUIMAQError : public FlexRIOCPUIMAQ {};

TEST_F(FlexRIOCPUIMAQ, CheckTerminals) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_NO_THROW(irio.getTerminalsCommon()) << "Common terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsFlexRIO())
		<< "FlexRIO terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog())
		<< "Aux Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsDigital())
		<< "Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxDigital())
		<< "Aux Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsIMAQ()) << "IMAQ terminals not found";

	EXPECT_THROW(irio.getTerminalsCRIO(), errors::TerminalNotImplementedError)
		<< "cRIO terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsAnalog(), errors::TerminalNotImplementedError)
		<< "Analog terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsSignalGeneration(), errors::TerminalNotImplementedError)
		<< "Signal Generation terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsIO(), errors::TerminalNotImplementedError)
		<< "IO terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsDAQ(), errors::TerminalNotImplementedError)
		<< "DAQ terminals found when they should have not";		
}

TEST_F(FlexRIOCPUIMAQ, InitConfigCL) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_NO_THROW(irio.getTerminalsIMAQ().configCameraLink(
		1, 1, 1, 1, 1, 0, CLSignalMapping::STANDARD, CLMode::FULL));
}

TEST_F(FlexRIOCPUIMAQ, readImage) {
	const size_t totalImages = 1000;
	const uint32_t dmaN = 0;
	const size_t imageWidth = 256;
	const size_t imageHeight = 256;
	size_t maxCounter;

	const auto aux = std::getenv("maxCounter");
	if(!aux) {
		maxCounter=65536;
	} else {
		maxCounter = std::stoul(aux);
	}

    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	auto imaq = irio.getTerminalsIMAQ();

	imaq.configCameraLink(1, 1, 1, 1, 1, 0, CLSignalMapping::STANDARD,
						  CLMode::FULL);
	irio.startFPGA();
	imaq.enableDMA(dmaN);
	irio.getTerminalsCommon().setDAQStart();

	size_t imageCount = 0;
	size_t pixelsRead;
	std::uint16_t curCounter = 0;
	std::uint16_t lastCounter = curCounter;
	std::unique_ptr<uint64_t[]> buffer(new uint64_t[imageWidth * imageHeight / 8]);

	while(imageCount < totalImages) {
		pixelsRead = imaq.readImageNonBlocking(dmaN, imageHeight * imageWidth, buffer.get());

		if(pixelsRead == imageHeight * imageWidth) {
			curCounter = reinterpret_cast<std::uint16_t*>(buffer.get())[0];
			if (imageCount != 0) { // Don't compare first image
				ASSERT_EQ((lastCounter + 1) % maxCounter, curCounter)
					<< "Error on image " << imageCount << " counter"
					<< std::endl;
			}
			lastCounter = curCounter;
			imageCount++;
		} else {
			usleep(1000);
		}
	}

}

TEST_F(FlexRIOCPUIMAQ, sendUARTMsg) {
	const std::string str = "HELO\n";
	const std::vector<std::uint8_t> charactersToSend(str.begin(), str.end());

    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	std::cout << "[TEST] Open the EDTpdv terminal and press enter here."
			  << std::endl;
	std::cin.get();

	auto imaq = irio.getTerminalsIMAQ();
	imaq.configCameraLink(1, 1, 1, 1, 1, 0, CLSignalMapping::STANDARD,
						  CLMode::FULL);

	irio.startFPGA();
	imaq.setUARTBaudRate(UARTBaudRates::BR96);

	imaq.sendUARTMsg(charactersToSend);
	std::cout << "[TEST] Message send. Check EDTpdv terminal application"
			  << std::endl;
}

TEST_F(FlexRIOCPUIMAQ, recvUARTMsg) {
	const size_t bytes2recv = 11;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	auto imaq = irio.getTerminalsIMAQ();
	imaq.configCameraLink(1, 1, 1, 1, 1, 0, CLSignalMapping::STANDARD,
						  CLMode::FULL);

	irio.startFPGA();
	imaq.setUARTBaudRate(UARTBaudRates::BR96);

	const auto msg = imaq.recvUARTMsg(bytes2recv);
	std::cout << "[TEST] Write a message up to " << bytes2recv - 1
			  << " characters on the EDTpdv terminal and press enter here"
			  << std::endl;
	std::cin.get();
	
	
	std::cout << "[TEST] Message received (" << msg.size() << " elements):" << std::endl;
	std::cout << "\tBytes: ";
	for (const auto &byte : msg) {
		std::cout << static_cast<int>(byte) << ' ';
	}
	std::cout << std::endl;
	std::cout << "\tString: '" << std::string(msg.begin(), msg.end()) << "'" << std::endl;
}
