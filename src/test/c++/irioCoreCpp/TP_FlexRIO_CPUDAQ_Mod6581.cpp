#include <gtest/gtest.h>
#include <irioCoreCpp.h>

#include <chrono>
#include <iostream>
#include <random>

#include "irioFixture.h"

using namespace irio;

class FlexRIOCPUDAQMod6581 : public IrioFixture {
   public:
	FlexRIOCPUDAQMod6581() : IrioFixture("Mod6581") {}
};

class FlexRIOCPUDAQMod6581Error : public FlexRIOCPUDAQMod6581 {};

TEST_F(FlexRIOCPUDAQMod6581, CheckModule){
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	const auto analog = irio.getTerminalsAnalog();

	const auto module = analog.getModuleConnected();
	ASSERT_EQ(module, ModulesType::FlexRIO_NI6581) << "The connected module is not a NI6581";
}

TEST_F(FlexRIOCPUDAQMod6581, GetSetDIO) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	// Activates DI
	auto digitalAux = irio.getTerminalsAuxDigital();
	digitalAux.setAuxDO(6, true);

	auto digital = irio.getTerminalsDigital();
	try {
		for (int i = 0; i < 6; ++i) {
			for (bool v : {true, false}) {
				// Write value
				digital.setDO(i, v);

				// Read written value
				const auto written = digital.getDO(i);
				EXPECT_EQ(written, v) << "DO" << std::to_string(i) << " value is not the one expected";

				// Read value from input
				const auto read = digital.getDI(i);
				EXPECT_EQ(read, v) << "DI" << std::to_string(i) << " value is not the one expected";
			}
		}
	} catch (errors::IrioError& e) {
		FAIL() << e.what();
	}
}
