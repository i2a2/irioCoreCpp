#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "terminals/names/namesTerminalsCommon.h"


using namespace iriov2;


class CRIOTests: public BaseTests{
public:
	CRIOTests():
			BaseTests("../../resources/9159/NiFpga_cRIO_CPUDAQ_9159.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::cRIO);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_CRIOMODULESOK).address,
						1);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_INSERTEDIOMODULESID).address,
						insertedIOModulesIDFake, sizeof(insertedIOModulesIDFake)/sizeof(uint16_t));
	}

	const uint16_t insertedIOModulesIDFake[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
};

class ErrorCRIOTests: public CRIOTests{};


///////////////////////////////////////////////////////////////
///// cRIO Tests
///////////////////////////////////////////////////////////////

TEST_F(CRIOTests, TerminalsFlexRIO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsCRIO());
}

TEST_F(CRIOTests, cRIOModulesOk){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsCRIO().getcRIOModulesOk(), true);
}

TEST_F(CRIOTests, InsertedIOModulesID){
	IrioV2 irio(bitfilePath, "0", "9.9");
	auto modules = irio.getTerminalsCRIO().getInsertedIOModulesID();
	EXPECT_EQ(modules.size(), sizeof(insertedIOModulesIDFake)/sizeof(uint16_t));
	for(size_t i = 0; i < modules.size(); ++i){
		EXPECT_EQ(modules[i], insertedIOModulesIDFake[i]);
	}
}

///////////////////////////////////////////////////////////////
///// cRIO Error Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorCRIOTests, ModulesNotOKError) {
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_CRIOMODULESOK).address,
						0);
	IrioV2 irio(bitfilePath, "0", "9.9");

	EXPECT_THROW(irio.startFPGA(100);,
		errors::ModulesNotOKError);
}

TEST_F(ErrorCRIOTests, FlexRIOTerminalNotImplementedError) {
	IrioV2 irio(bitfilePath, "0", "9.9");

	EXPECT_THROW(irio.getTerminalsFlexRIO();,
		errors::TerminalNotImplementedError);
}
