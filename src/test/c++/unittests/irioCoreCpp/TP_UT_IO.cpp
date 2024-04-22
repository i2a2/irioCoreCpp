#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "terminals/names/namesTerminalsCRIO.h"
#include "terminals/names/namesTerminalsIO.h"
#include "terminals/names/namesTerminalsCommon.h"


using namespace irio;


class CRIOIOTests: public BaseTests{
public:
	CRIOIOTests():
			BaseTests("../../../resources/9159/NiFpga_cRIO_IO.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::cRIO);
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
					   bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
					   PROFILE_VALUE_IO);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_CRIOMODULESOK).getAddress(),
						1);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_INSERTEDIOMODULESID).getAddress(),
						insertedIOModulesIDFake, sizeof(insertedIOModulesIDFake)/sizeof(uint16_t));
		setValueForReg(ReadFunctions::NiFpga_ReadU16,
						bfp.getRegister(TERMINAL_SAMPLINGRATE+std::to_string(0)).getAddress(),
						fakeDec);
	}

	const uint16_t insertedIOModulesIDFake[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	const uint16_t fakeDec = 12;
};



///////////////////////////////////////////////////////////////
/// cRIO IO Tests
///////////////////////////////////////////////////////////////

TEST_F(CRIOIOTests, TerminalsIO){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsIO());
}

TEST_F(CRIOIOTests, getNumIOSamplingRate){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsIO().getNumIOSamplingRate(), 2);
}

TEST_F(CRIOIOTests, getSamplingRateDecimation){
	Irio irio(bitfilePath, "0", "V9.9");
	auto dec = irio.getTerminalsIO().getSamplingRateDecimation(0);
	EXPECT_EQ(dec, fakeDec);
}

TEST_F(CRIOIOTests, setSamplingRateDecimation){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsIO().setSamplingRateDecimation(0,1));
}
