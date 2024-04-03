#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "errorsIrio.h"

#include "bfp.h"
#include "profiles/profileCPUDAQFlexRIO.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsFlexRIO.h"


using namespace irio;


class FlexRIOTests: public BaseTests {
public:
	FlexRIOTests():
			BaseTests("../../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_RIOADAPTERCORRECT).getAddress(),
						1);
		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_INSERTEDIOMODULEID).getAddress(),
						insertedIOModuleIDFake);
	}

	const uint32_t insertedIOModuleIDFake = 1234;
};

class ErrorFlexRIOTests: public FlexRIOTests {};


///////////////////////////////////////////////////////////////
///// FlexRIO Tests
///////////////////////////////////////////////////////////////
TEST_F(FlexRIOTests, TerminalsFlexRIO){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsFlexRIO());
}

TEST_F(FlexRIOTests, RIOAdapterCorrect){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsFlexRIO().getRIOAdapterCorrect(), true);
}

TEST_F(FlexRIOTests, InsertedIOModuleID){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsFlexRIO().getInsertedIOModuleID(), insertedIOModuleIDFake);
}

///////////////////////////////////////////////////////////////
///// FlexRIO Error Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorFlexRIOTests, ModuleNotOKError) {
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
			bfp.getRegister(TERMINAL_RIOADAPTERCORRECT).getAddress(), 0);
	Irio irio(bitfilePath, "0", "V9.9");

	EXPECT_THROW(irio.startFPGA(100);,
		errors::ModulesNotOKError);
}

TEST_F(ErrorFlexRIOTests, cRIOTerminalNotImplementedError) {
	Irio irio(bitfilePath, "0", "V9.9");

	EXPECT_THROW(irio.getTerminalsCRIO();,
		errors::TerminalNotImplementedError);
}
