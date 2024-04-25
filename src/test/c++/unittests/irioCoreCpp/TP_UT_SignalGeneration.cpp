#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"


using namespace irio;

class SignalGenerationTests: public BaseTests {
public:
	SignalGenerationTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::RSeries);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGFREF+std::to_string(0)).getAddress(),
						fref0Fake);
		
		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGFREF+std::to_string(1)).getAddress(),
						fref1Fake);

		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_SGSIGNALTYPE+std::to_string(0)).getAddress(),
						sigTypeFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGAMP+std::to_string(0)).getAddress(),
						ampFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGPHASE+std::to_string(0)).getAddress(),
						phaseFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGUPDATERATE+std::to_string(0)).getAddress(),
						updateRateFake);

		setValueForReg(ReadFunctions::NiFpga_ReadU32,
						bfp.getRegister(TERMINAL_SGFREQ+std::to_string(0)).getAddress(),
						freqFake);
	}

	const uint32_t fref0Fake = 12345;
	const uint32_t fref1Fake = 67890;
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
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration());
}

TEST_F(SignalGenerationTests, SGNo){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGNo(), 2);
}

TEST_F(SignalGenerationTests, SGFref){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGFref(0), fref0Fake);
}

TEST_F(SignalGenerationTests, VectorSGFref){
	Irio irio(bitfilePath, "0", "V9.9");
	const auto vec = irio.getTerminalsSignalGeneration().getVectorSGFrefs();
	ASSERT_EQ(vec.size(), 2);
	EXPECT_EQ(vec[0], fref0Fake);
	EXPECT_EQ(vec[1], fref1Fake);
}

TEST_F(SignalGenerationTests, getSGSignalType){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGSignalType(0), sigTypeFake);
}

TEST_F(SignalGenerationTests, getSGAmp){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGAmp(0), ampFake);
}

TEST_F(SignalGenerationTests, getSGPhase){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGPhase(0), phaseFake);
}

TEST_F(SignalGenerationTests, getSGFreq){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGFreq(0), freqFake);
}

TEST_F(SignalGenerationTests, getSGUpdateRate){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGUpdateRate(0), updateRateFake);
}

TEST_F(SignalGenerationTests, setSGSignalType){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGSignalType(0, 1));
}

TEST_F(SignalGenerationTests, setSGAmp){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGAmp(0, 1));
}

TEST_F(SignalGenerationTests, setSGPhase){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGPhase(0, 1));}

TEST_F(SignalGenerationTests, setSGFreqDecimation){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGFreqDecimation(0, 1));
}

TEST_F(SignalGenerationTests, setSGUpdateRateDecimation){
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration().setSGUpdateRateDecimation(0, 1));
}

///////////////////////////////////////////////////////////////
///// Errors Signal Generation Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorSignalGenerationTests, MismatchSGTerminals){
	EXPECT_THROW(Irio irio(
				"../../../resources/failResources/7854/NiFpga_Rseries_MismatchSG_7854.lvbitx",
				"0", "V9.9");,
				errors::ResourceNotFoundError);
}

TEST_F(ErrorSignalGenerationTests, NoSGNo){
	Irio irio(
				"../../../resources/failResources/7854/NiFpga_Rseries_NoSG_7854.lvbitx",
				"0", "V9.9");
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGNo(), 0);
}

TEST_F(ErrorSignalGenerationTests, InvalidSGFrefTerminal) {
	Irio irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsSignalGeneration().getSGFref(99);,
		errors::ResourceNotFoundError);
}
