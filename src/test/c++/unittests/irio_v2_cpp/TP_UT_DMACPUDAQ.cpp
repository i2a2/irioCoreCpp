#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDMACPUCommon.h"
#include "terminals/names/namesTerminalsDMADAQCPU.h"


using namespace iriov2;


class DMACPUDAQTests: public BaseTests {
public:
	DMACPUDAQTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::RSeries);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_DMATTOHOSTNCH).address,
						nchFake, 2);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_DMATTOHOSTBLOCKNWORDS).address,
						lengthBlockFake, 2);
		setValueForReg(ReadFunctions::NiFpga_ReadU16,
						bfp.getRegister(TERMINAL_DMATTOHOSTSAMPLINGRATE
								+std::to_string(0)).address,
						samplingRateFake);
	}

	const std::uint16_t nchFake[2] = {5,2};
	const std::uint16_t lengthBlockFake[2] = {42,24};
	const std::uint16_t samplingRateFake = 12345;
};

class ErrorDMACPUDAQTests: public DMACPUDAQTests { };


///////////////////////////////////////////////////////////////
///// DMACPU DAQ Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(DMACPUDAQTests, TerminalsDMACPUDAQ){
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ());
}

TEST_F(DMACPUDAQTests, LengthBlock){
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getLengthBlock(0), 42);
}

TEST_F(DMACPUDAQTests, getSamplingRateDecimation){
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getSamplingRateDecimation(0), samplingRateFake);
}

TEST_F(DMACPUDAQTests, setSamplingRateDecimation){
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().setSamplingRateDecimation(0, 1));
}

///////////////////////////////////////////////////////////////
///// Error DMACPU DAQ Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorDMACPUDAQTests, LengthBlockInvalidDMAID){
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().getLengthBlock(10);,
		errors::ResourceNotFoundError);
}

TEST_F(ErrorDMACPUDAQTests, MistmatchDMALengthBlock) {
	EXPECT_THROW(
		IrioV2 irio("../../../resources/failResources/7854/NiFpga_Rseries_MismatchDMALengthBlock_7854.lvbitx", "0", "V9.9");,
		errors::ResourceNotFoundError
	);
}

TEST_F(ErrorDMACPUDAQTests, MistmatchDMASamplingRate) {
	EXPECT_THROW(
		IrioV2 irio("../../../resources/failResources/7854/NiFpga_Rseries_MistmatchDMASamplingRate_7854.lvbitx", "0", "V9.9");,
		errors::ResourceNotFoundError
	);
}

