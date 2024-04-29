#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <chrono>

#include <irioCoreCpp.h>
#include "irioFixture.h"


using namespace irio;

class CRIOIO : public IrioFixture {
 public:
  CRIOIO() : IrioFixture("IO") {
    if(boardType != "cRIO") {
        throw std::runtime_error("Expected a cRIO. Got " + boardType);
    }
  }
};

TEST_F(CRIOIO, CheckTerminals) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_NO_THROW(irio.getTerminalsCommon()) << "Common terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsCRIO())
		<< "cRIO terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAnalog()) << "Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog())
		<< "Aux Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsDigital())
		<< "Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxDigital())
		<< "Aux Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration())
		<< "Signal Generation terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsIO()) << "IO terminals not found";

	EXPECT_THROW(irio.getTerminalsFlexRIO(), errors::TerminalNotImplementedError)
		<< "FlexRIO terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsIMAQ(), errors::TerminalNotImplementedError)
		<< "IMAQ terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsDAQ(), errors::TerminalNotImplementedError)
		<< "DAQ terminals found when they should have not";	
}
