#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsAuxAnalog.h"

using namespace irio;

class AuxAnalogTests: public BaseTests {
public:
	AuxAnalogTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::RSeries);

		setValueForReg(ReadFunctions::NiFpga_ReadI32,
						bfp.getRegister(TERMINAL_AUXAI+std::to_string(0)).getAddress(),
						auxAIFake);
		setValueForReg(ReadFunctions::NiFpga_ReadI32,
						bfp.getRegister(TERMINAL_AUXAO+std::to_string(0)).getAddress(),
						auxAOFake);
	}

	const std::int32_t auxAIFake = 1234;
	const std::int32_t auxAOFake = 5678;
};

class AuxAnalog64Test: public BaseTests {
public:
	AuxAnalog64Test() :
		BaseTests(
				"../../../resources/7854/NiFpga_Rseries_OnlyResources_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::RSeries);
		setValueForReg(ReadFunctions::NiFpga_ReadI64,
						bfp.getRegister(TERMINAL_AUX64AI+std::to_string(0)).getAddress(),
						aux64AIFake);
		setValueForReg(ReadFunctions::NiFpga_ReadI64,
						bfp.getRegister(TERMINAL_AUX64AO+std::to_string(0)).getAddress(),
						aux64AOFake);
	}

	const std::int64_t aux64AIFake = 1234;
	const std::int64_t aux64AOFake = 5678;
};

///////////////////////////////////////////////////////////////
///// Aux Analog Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(AuxAnalogTests, TerminalsAuxAnalog){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog());
}

TEST_F(AuxAnalogTests, NumAuxAI){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAI(), 16);
}

TEST_F(AuxAnalogTests, NumAuxAO){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAO(), 16);
}

TEST_F(AuxAnalogTests, getAuxAI){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getAuxAI(0), auxAIFake);
}

TEST_F(AuxAnalogTests, getAuxAO){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getAuxAO(0), auxAOFake);
}

TEST_F(AuxAnalogTests, setAuxAO){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog().setAuxAO(0, auxAOFake));
}

TEST_F(AuxAnalog64Test, getAuxAI64){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getAuxAI64(0), aux64AIFake);
}

TEST_F(AuxAnalog64Test, getAuxAO64){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getAuxAO64(0), aux64AOFake);
}

TEST_F(AuxAnalog64Test, setAuxAO64){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog().setAuxAO64(0, aux64AOFake));
}

TEST_F(AuxAnalog64Test, NumAuxAI64){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAI64(), 16);
}

TEST_F(AuxAnalog64Test, NumAuxAO64){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAO64(), 15);
}
