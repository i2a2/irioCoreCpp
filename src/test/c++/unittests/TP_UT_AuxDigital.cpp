#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsAuxDigital.h"

using namespace iriov2;

class AuxDigitalTests: public BaseTests {
public:
	AuxDigitalTests():
		BaseTests("../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);

		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_AUXDI+std::to_string(0)).address,
						auxDIFake);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_AUXDO+std::to_string(0)).address,
						auxDOFake);
	}

	const uint8_t auxDIFake = true;
	const uint8_t auxDOFake = true;
};


///////////////////////////////////////////////////////////////
///// Digital Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(AuxDigitalTests, TerminalsAuxDigital){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsAuxDigital());
}

TEST_F(AuxDigitalTests, NumAuxDI){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAuxDigital().getNumAuxDI(), 2);
}

TEST_F(AuxDigitalTests, NumDO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAuxDigital().getNumAuxDO(), 2);
}

TEST_F(AuxDigitalTests, getDI){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAuxDigital().getAuxDI(0), auxDIFake);
}

TEST_F(AuxDigitalTests, getDO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAuxDigital().getAuxDO(0), auxDOFake);
}

TEST_F(AuxDigitalTests, setDO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsAuxDigital().setAuxDO(0, true));
}
