#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <chrono>

#include <irioCoreCpp.h>
#include "irioFixture.h"


using namespace irio;

class RSeriesCPUDAQ : public IrioFixture {
 public:
  RSeriesCPUDAQ() : IrioFixture("CPUDAQ") {
	if(boardType != "Rseries") {
        throw std::runtime_error("Expected a Rseries. Got " + boardType);
    }
  }
};

class RSeriesCPUDAQError : public RSeriesCPUDAQ {};

TEST_F(RSeriesCPUDAQ, CheckTerminals) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_NO_THROW(irio.getTerminalsCommon()) << "Common terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAnalog()) << "Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog())
		<< "Aux Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsDigital())
		<< "Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxDigital())
		<< "Aux Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration())
		<< "Signal Generation terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsDAQ()) << "DAQ terminals not found";

	EXPECT_THROW(irio.getTerminalsCRIO(), errors::TerminalNotImplementedError)
		<< "cRIO terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsIMAQ(), errors::TerminalNotImplementedError)
		<< "IMAQ terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsIO(), errors::TerminalNotImplementedError)
		<< "IO terminals found when they should have not";
}

TEST_F(RSeriesCPUDAQ, GetSetSGSignalType) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	auto sigGen = irio.getTerminalsSignalGeneration();
	EXPECT_NO_THROW(sigGen.setSGSignalType(0, 0));

	const auto type = sigGen.getSGSignalType(0);
	EXPECT_EQ(type, 0);
}

TEST_F(RSeriesCPUDAQ, GetSetSGUpdateRate) {
	int decimation = 10;
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGUpdateRateDecimation(channel, decimation));
	uint32_t decConf;
	EXPECT_NO_THROW(decConf = sigGen.getSGUpdateRate(channel));;
	EXPECT_EQ(decConf, decimation);
}

TEST_F(RSeriesCPUDAQ, GetSetSGFreq) {
	int decimation = 12;
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGFreqDecimation(channel, decimation));
	uint32_t decConf;
	EXPECT_NO_THROW(decConf = sigGen.getSGFreq(channel));;
	EXPECT_EQ(decConf, decimation);
}

TEST_F(RSeriesCPUDAQ, GetSetSGAmp) {
	int sigAmp = 4096; // 0.517563 V
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGAmp(channel, sigAmp));
	uint32_t ampRead;
	EXPECT_NO_THROW(ampRead = sigGen.getSGAmp(channel));;
	EXPECT_EQ(ampRead, sigAmp);
}

TEST_F(RSeriesCPUDAQ, GetSetSGPhase) {
	int phase = 1;
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGPhase(channel, phase));
	uint32_t phaseRead;
	EXPECT_NO_THROW(phaseRead = sigGen.getSGPhase(channel));;
	EXPECT_EQ(phaseRead, phase);
}


TEST_F(RSeriesCPUDAQ, GetSetEnableAO) {
	int32_t channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	auto analog = irio.getTerminalsAnalog();
	irio.startFPGA();

	EXPECT_NO_THROW(analog.setAOEnable(channel, true));
	bool aoEnable;
	EXPECT_NO_THROW(aoEnable = analog.getAOEnable(channel));
	EXPECT_EQ(aoEnable, true);

	EXPECT_NO_THROW(analog.setAOEnable(channel, false));
	EXPECT_NO_THROW(aoEnable = analog.getAOEnable(channel));
	EXPECT_EQ(aoEnable, false);
}


TEST_F(RSeriesCPUDAQ, GetSetAO) {
	int32_t channel = 0;

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int64_t> dist(min + 1, max - 1);

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	auto analog = irio.getTerminalsAnalog();
	irio.startFPGA();

	analog.setAOEnable(channel, true);

	try {
		std::vector<int64_t> values = {min, dist(gen), dist(gen), dist(gen),
									   max};
		for (const auto& v : values) {
			// Write AO
			analog.setAO(channel, v);

			// Read AO
			auto readV = analog.getAO(channel);
			EXPECT_EQ(readV, v) << "Value read from AO is incorrect";

			// Check the value in auxAI9
			readV = irio.getTerminalsAuxAnalog().getAuxAI(9);
			EXPECT_EQ(readV, v) << "Value read from auxAI9 is incorrect";
		}
	} catch (errors::IrioError& e) {
		FAIL() << e.what();
	}
}

TEST_F(RSeriesCPUDAQ, cleanAllDMAs) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();

	auto daq = irio.getTerminalsDAQ();
	EXPECT_NO_THROW(daq.cleanAllDMAs());
}

TEST_F(RSeriesCPUDAQ, readDataNonBlocking){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t channelDMA = 2;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	Irio irio(bitfilePath, serialNumber, "V1.2");
	const auto commonTerm = irio.getTerminalsCommon();
	irio.startFPGA();
	commonTerm.setDebugMode(false);

	auto sg = irio.getTerminalsSignalGeneration();
	auto analog = irio.getTerminalsAnalog();
	auto daq = irio.getTerminalsDAQ();

	// Enable AO0 and set to 2048 and configure SGSignalType0
	sg.setSGSignalType(channelSG, signalType);
	analog.setAO(analogNum, valDCSG);
	analog.setAOEnable(analogNum, true);

	// DMA DAQ Parameters
	const auto nCh = daq.getNCh(DMANum);
	const auto lengthBlock = daq.getLengthBlock(DMANum);

	// Setup DMAQ
	const std::uint16_t decimation = commonTerm.getFref()/samplingRate;
	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);
	commonTerm.setDAQStart();

	::usleep(100000);

	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	auto sizeRead = daq.readDataNonBlocking(DMANum, lengthBlock*blocksToRead, data.get());
	EXPECT_EQ(sizeRead, lengthBlock*blocksToRead) << "Incorrect number of elements read";

	daq.disableDMA(DMANum);
	commonTerm.setDAQStop();
	
	daq.cleanDMA(DMANum);

	std::uint16_t *dataAux = reinterpret_cast<std::uint16_t*>(data.get());
	for (size_t i = 0; i < blocksToRead * lengthBlock; i++) {
		if (dataAux[(i * nCh) + channelDMA] != valDCSG) {
			FAIL() << "Incorrect data read";
		}
	}
}

TEST_F(RSeriesCPUDAQ, readDataNonBlockingNoData){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	Irio irio(bitfilePath, serialNumber, "V1.2");
	const auto commonTerm = irio.getTerminalsCommon();
	irio.startFPGA();
	commonTerm.setDebugMode(false);

	auto sg = irio.getTerminalsSignalGeneration();
	auto analog = irio.getTerminalsAnalog();
	auto daq = irio.getTerminalsDAQ();

	// Enable AO0 and set to 2048 and configure SGSignalType0
	sg.setSGSignalType(channelSG, signalType);
	analog.setAO(analogNum, valDCSG);
	analog.setAOEnable(analogNum, true);

	// DMA DAQ Parameters
	const auto lengthBlock = daq.getLengthBlock(DMANum);

	// Setup DMAQ
	const std::uint16_t decimation = commonTerm.getFref()/samplingRate;
	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);

	::usleep(100000);

	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	auto sizeRead = daq.readDataNonBlocking(DMANum, lengthBlock*blocksToRead, data.get());
	EXPECT_EQ(sizeRead, 0) << "Data was read when it was not expected";

	daq.disableDMA(DMANum);
	commonTerm.setDAQStop();
	
	daq.cleanDMA(DMANum);
}

TEST_F(RSeriesCPUDAQ, readDataBlocking){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t channelDMA = 2;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	Irio irio(bitfilePath, serialNumber, "V1.2");
	const auto commonTerm = irio.getTerminalsCommon();
	irio.startFPGA();
	commonTerm.setDebugMode(false);

	auto sg = irio.getTerminalsSignalGeneration();
	auto analog = irio.getTerminalsAnalog();
	auto daq = irio.getTerminalsDAQ();

	// Enable AO0 and set to 2048 and configure SGSignalType0
	sg.setSGSignalType(channelSG, signalType);
	analog.setAO(analogNum, valDCSG);
	analog.setAOEnable(analogNum, true);

	// DMA DAQ Parameters
	const auto nCh = daq.getNCh(DMANum);
	const auto lengthBlock = daq.getLengthBlock(DMANum);

	// Setup DMAQ
	const std::uint16_t decimation = commonTerm.getFref()/samplingRate;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	const auto timeout = static_cast<std::uint32_t>(1.5*std::ceil(lengthBlock*blocksToRead*1000/samplingRate));

	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);
	commonTerm.setDAQStart();

	try{
		daq.readDataBlocking(DMANum, lengthBlock*blocksToRead, data.get(), timeout);
	}catch(errors::DMAReadTimeout&){
		FAIL() << "Timeout occurred reading DMA when it should have not";
	}

	daq.disableDMA(DMANum);
	commonTerm.setDAQStop();
	
	daq.cleanDMA(DMANum);

	std::uint16_t *dataAux = reinterpret_cast<std::uint16_t*>(data.get());
	for (size_t i = 0; i < blocksToRead * lengthBlock; i++) {
		if (dataAux[(i * nCh) + channelDMA] != valDCSG) {
			FAIL() << "Incorrect data read";
		}
	}
}

TEST_F(RSeriesCPUDAQ, readDataBlockingTimeout){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	Irio irio(bitfilePath, serialNumber, "V1.2");
	const auto commonTerm = irio.getTerminalsCommon();
	irio.startFPGA();
	commonTerm.setDebugMode(false);

	auto sg = irio.getTerminalsSignalGeneration();
	auto analog = irio.getTerminalsAnalog();
	auto daq = irio.getTerminalsDAQ();

	// Enable AO0 and set to 2048 and configure SGSignalType0
	sg.setSGSignalType(channelSG, signalType);
	analog.setAO(analogNum, valDCSG);
	analog.setAOEnable(analogNum, true);

	// DMA DAQ Parameters
	const auto lengthBlock = daq.getLengthBlock(DMANum);

	// Setup DMAQ
	const std::uint16_t decimation = commonTerm.getFref()/samplingRate;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	const auto timeout = static_cast<std::uint32_t>(std::ceil(lengthBlock*blocksToRead*1000/samplingRate));

	
	daq.cleanDMA(DMANum);

	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);

	EXPECT_THROW(daq.readDataBlocking(DMANum, lengthBlock*blocksToRead, data.get(), timeout); // @suppress("Goto statement used")
					, errors::DMAReadTimeout) << "Timeout exception was not raised";

	daq.disableDMA(DMANum);
	commonTerm.setDAQStop();
	
	daq.cleanDMA(DMANum);
}

/// Error RSeries DAQ Tests

TEST_F(RSeriesCPUDAQError, InvalidAuxAIO) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto auxAnalog = irio.getTerminalsAuxAnalog();

	EXPECT_THROW(auxAnalog.setAuxAO(static_cast<std::uint32_t>(-1), 1), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.setAuxAO64(static_cast<std::uint32_t>(-1), 1), errors::ResourceNotFoundError);

	EXPECT_THROW(auxAnalog.getAuxAI(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.getAuxAO(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.getAuxAI64(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.getAuxAO64(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
}

TEST_F(RSeriesCPUDAQError, InvalidAuxDIO) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto auxDigital = irio.getTerminalsAuxDigital();

	EXPECT_THROW(auxDigital.setAuxDO(static_cast<std::uint32_t>(-1), 1), errors::ResourceNotFoundError);

	EXPECT_THROW(auxDigital.getAuxDI(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxDigital.getAuxDO(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
}

TEST_F(RSeriesCPUDAQError, InvalidSGResource) {
	uint32_t invalidChannel = static_cast<std::uint32_t>(-1);

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_THROW(sigGen.getSGAmp(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGFref(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGFreq(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGPhase(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGSignalType(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGUpdateRate(invalidChannel), errors::ResourceNotFoundError);

	EXPECT_THROW(sigGen.setSGAmp(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGFreqDecimation(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGPhase(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGSignalType(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGUpdateRateDecimation(invalidChannel, 0), errors::ResourceNotFoundError);
}
