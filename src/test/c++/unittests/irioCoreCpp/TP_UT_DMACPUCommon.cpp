#include <memory>

#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDMACPUCommon.h"


using namespace iriov2;


class DMACPUCommonTests: public BaseTests {
public:
	DMACPUCommonTests():
		BaseTests("../../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::RSeries);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_DMATTOHOSTNCH).getAddress(),
						nchFake, 2);

		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU8,
						bfp.getRegister(TERMINAL_DMATTOHOSTFRAMETYPE).getAddress(),
						frameTypeFake, 2);

		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU8,
						bfp.getRegister(TERMINAL_DMATTOHOSTSAMPLESIZE).getAddress(),
						sampleSizeFake, 2);

		setValueForReg(ReadFunctions::NiFpga_ReadU16,
						bfp.getRegister(TERMINAL_DMATTOHOSTOVERFLOWS).getAddress(),
						overflowsFake);

		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_DMATTOHOSTENABLE+std::to_string(0)).getAddress(),
						1);
	}

	const std::uint16_t nchFake[2] = {5,2};
	const std::uint8_t frameTypeFake[2] = {1, 0};
	const std::uint8_t sampleSizeFake[2] = {4,8};
	const std::uint16_t overflowsFake = 0b0000000000000010;
};

class ErrorDMACPUCommonTests: public DMACPUCommonTests { };


///////////////////////////////////////////////////////////////
///// DMACPU Common Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(DMACPUCommonTests, TerminalsDMACPU){
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ());
}

TEST_F(DMACPUCommonTests, countDMAs) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().countDMAs(), 2);
}

TEST_F(DMACPUCommonTests, NCh) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getNCh(0), nchFake[0]);
}

TEST_F(DMACPUCommonTests, FrameType) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getFrameType(0), static_cast<FrameType>(frameTypeFake[0]));
}

TEST_F(DMACPUCommonTests, AllFrameType) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	const auto frametypes = irio.getTerminalsDAQ().getAllFrameType();
	EXPECT_EQ(frametypes.size(), 2);
}

TEST_F(DMACPUCommonTests, SampleSize) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getSampleSize(0), sampleSizeFake[0]);
}

TEST_F(DMACPUCommonTests, AllSampleSize) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	const auto samplesizes = irio.getTerminalsDAQ().getAllSampleSizes();
	EXPECT_EQ(samplesizes.size(), 2);
}

TEST_F(DMACPUCommonTests, DMAOverflow) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getDMAOverflow(0), false);
	EXPECT_EQ(irio.getTerminalsDAQ().getDMAOverflow(1), true);
}

TEST_F(DMACPUCommonTests, DMAOverflows) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().getAllDMAOverflows(), overflowsFake);
}

TEST_F(DMACPUCommonTests, enableDMA) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().enableDMA(0));
}

TEST_F(DMACPUCommonTests, disableDMA) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().disableDMA(0));
}

TEST_F(DMACPUCommonTests, enaDisDMA) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().enaDisDMA(0, true));
}

TEST_F(DMACPUCommonTests, isDMAEnable) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_EQ(irio.getTerminalsDAQ().isDMAEnable(0), true);
}

NiFpga_Status funcReturnElemRem(NiFpga_Session, uint32_t, uint64_t *data,
		size_t numberOfElements, uint32_t, size_t *elementsRemaining) {
	std::memset(data, numberOfElements, 7);
	if(elementsRemaining)
		*elementsRemaining = 100;

	return NiFpga_Status_Success;
}

NiFpga_Status funcReturnNoElemRem(NiFpga_Session, uint32_t,
			uint64_t *data,
		size_t numberOfElements, uint32_t, size_t *elementsRemaining) {
	std::memset(data, numberOfElements, 7);
	if(elementsRemaining)
		*elementsRemaining = 0;

	return NiFpga_Status_Success;
}

TEST_F(DMACPUCommonTests, startDMA) {
	auto (*custom_fakes[])(NiFpga_Session, uint32_t, uint64_t*, size_t,
			uint32_t, size_t*) -> NiFpga_Status = {funcReturnElemRem, funcReturnNoElemRem};

    SET_CUSTOM_FAKE_SEQ(NiFpga_ReadFifoU64, custom_fakes, 2);

	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().startDMA(0));
}

TEST_F(DMACPUCommonTests, startAllDMAs) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().startAllDMAs());
}

TEST_F(DMACPUCommonTests, stopDMA) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().stopDMA(0));
}

TEST_F(DMACPUCommonTests, stopAllDMAs) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().stopAllDMAs());
}

TEST_F(DMACPUCommonTests, cleanDMA) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().cleanDMA(0));
}

TEST_F(DMACPUCommonTests, cleanAllDMAs) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().cleanAllDMAs());
}

TEST_F(DMACPUCommonTests, readData) {
	const size_t numElem = 10;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[numElem]);

	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().readData(0, numElem, data.get(), true, 500));
}

TEST_F(DMACPUCommonTests, readDataBlocking) {
	const size_t numElem = 10;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[numElem]);

	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().readDataBlocking(0, numElem, data.get()));
}

TEST_F(DMACPUCommonTests, readDataNonBlocking) {
	auto (*custom_fakes[])(NiFpga_Session, uint32_t, uint64_t*, size_t,
			uint32_t, size_t*) -> NiFpga_Status = {funcReturnElemRem, funcReturnNoElemRem};

	SET_CUSTOM_FAKE_SEQ(NiFpga_ReadFifoU64, custom_fakes, 2);

	const size_t numElem = 10;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[numElem]);

	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_NO_THROW(irio.getTerminalsDAQ().readDataNonBlocking(0, numElem, data.get()));
}

///////////////////////////////////////////////////////////////
///// Error DMACPU Common Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(ErrorDMACPUCommonTests, InvalidDMAIDNCh) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().getNCh(10);,
			errors::ResourceNotFoundError);
}

TEST_F(ErrorDMACPUCommonTests, DMAReadTimeout) {
	const size_t numElem = 10;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[numElem]);

	NiFpga_ReadFifoU64_fake.custom_fake = [](NiFpga_Session, uint32_t,
				uint64_t* data, size_t numberOfElements, uint32_t, size_t* elementsRemaining) {
		return NiFpga_Status_FifoTimeout;
	};
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().readDataBlocking(0, numElem, data.get());,
		errors::DMAReadTimeout);
}

TEST_F(ErrorDMACPUCommonTests, startDMAInvalidDMAID) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().startDMA(10);,
			errors::ResourceNotFoundError);
}

TEST_F(ErrorDMACPUCommonTests, stopDMAInvalidDMAID) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().stopDMA(10);,
			errors::ResourceNotFoundError);
}

TEST_F(ErrorDMACPUCommonTests, frameTypeInvalidDMAID) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().getFrameType(10);,
			errors::ResourceNotFoundError);
}

TEST_F(ErrorDMACPUCommonTests, sampleSizeInvalidDMAID) {
	IrioV2 irio(bitfilePath, "0", "V9.9");
	EXPECT_THROW(irio.getTerminalsDAQ().getSampleSize(10);,
			errors::ResourceNotFoundError);
}

TEST_F(ErrorDMACPUCommonTests, MistmatchDMADMAEnable) {
	EXPECT_THROW(
		IrioV2 irio("../../../resources/failResources/7854/NiFpga_Rseries_MismatchDMADMAEnable_7854.lvbitx", "0", "V9.9");,
		errors::ResourceNotFoundError
	);
}
