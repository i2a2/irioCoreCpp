#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDigital.h"

using namespace iriov2;

class DigitalTests: public BaseTests {
public:
	DigitalTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);

		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_DI+std::to_string(0)).address,
						diFake);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_DO+std::to_string(0)).address,
						doFake);
	}

	const uint8_t diFake = true;
	const uint8_t doFake = true;
};


///////////////////////////////////////////////////////////////
///// Digital Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(DigitalTests, TerminalsDigital){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsDigital());
}

TEST_F(DigitalTests, NumDI){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsDigital().getNumDI(), 2);
}

TEST_F(DigitalTests, NumDO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsDigital().getNumDO(), 2);
}

TEST_F(DigitalTests, getDI){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsDigital().getDI(0), diFake);
}

TEST_F(DigitalTests, getDO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsDigital().getDO(0), doFake);
}

TEST_F(DigitalTests, setDO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsDigital().setDO(0, true));
}
