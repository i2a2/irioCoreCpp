#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"


using namespace iriov2;

class SignalGenerationTests: public BaseTests {
public:
	SignalGenerationTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGFREF+std::to_string(0)).address,
						frefFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_SGSIGNALTYPE+std::to_string(0)).address,
						sigTypeFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGAMP+std::to_string(0)).address,
						ampFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGPHASE+std::to_string(0)).address,
						phaseFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGUPDATERATE+std::to_string(0)).address,
						updateRateFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGFREQ+std::to_string(0)).address,
						freqFake);
	}

	const uint32_t frefFake = 1234567890;
	const uint8_t sigTypeFake = 2;
	const uint32_t phaseFake = 1;
	const uint32_t ampFake = 3;
	const uint32_t freqFake = 1234;
	const uint32_t updateRateFake = 98765;
};

class ErrorSignalGenerationTests: public SignalGenerationTests { };


///////////////////////////////////////////////////////////////
///// Signal Generation Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(SignalGenerationTests, TerminalsSignalGeneration) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration());
}

TEST_F(SignalGenerationTests, SGNo){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGNo(), 2);
}

TEST_F(SignalGenerationTests, SGFref){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGFref(0), frefFake);
}

TEST_F(SignalGenerationTests, getSGSignalType){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGSignalType(0), sigTypeFake);
}

TEST_F(SignalGenerationTests, getSGAmp){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGAmp(0), ampFake);
}

TEST_F(SignalGenerationTests, getSGPhase){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGPhase(0), phaseFake);
}

TEST_F(SignalGenerationTests, getSGFreq){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGFreq(0), freqFake);
}

TEST_F(SignalGenerationTests, getSGUpdateRate){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGUpdateRate(0), updateRateFake);
}

TEST_F(SignalGenerationTests, setSGSignalType){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGSignalType(0, 1));
}

TEST_F(SignalGenerationTests, setSGAmp){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGAmp(0, 1));
}

TEST_F(SignalGenerationTests, setSGPhase){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGPhase(0, 1));}

TEST_F(SignalGenerationTests, setSGFreq){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGFreq(0, 1));
}

TEST_F(SignalGenerationTests, setSGUpdateRate){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGUpdateRate(0, 1));
}

///////////////////////////////////////////////////////////////
///// Errors Signal Generation Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorSignalGenerationTests, MismatchSGTerminals){
	EXPECT_THROW(IrioV2 irio(
				"../../../resources/failResources/7854/NiFpga_Rseries_MismatchSG_7854.lvbitx",
				"0", "9.9");,
				errors::ResourceNotFoundError);
}

TEST_F(ErrorSignalGenerationTests, NoSGNo){
	IrioV2 irio(
				"../../../resources/failResources/7854/NiFpga_Rseries_NoSG_7854.lvbitx",
				"0", "9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGNo(), 0);
}

TEST_F(ErrorSignalGenerationTests, InvalidSGFrefTerminal) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_THROW(irio.getTerminalsSignalGeneration().getSGFref(99);,
		errors::ResourceNotFoundError);
}
