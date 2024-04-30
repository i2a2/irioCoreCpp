#define CCS_VERSION

#include <gtest/gtest.h>
#include <irioCoreCpp.h>

#include <chrono>
#include <iostream>
#include <random>
#include <climits>

#include "irioFixture.h"

using namespace irio;


double sineCorrelation(const std::vector<double>& signal, int f, int fs) {
    double max_corr = 0;

    // Generate sine
    std::vector<double> sine;
    for (int n = 0; n < fs/f; n++) {
        sine.push_back(sin(2.0*M_PI*f/fs*n));
    }

    // Calculate autocorrelation
    double autocorrelation = 0;
    for (int n = 0; n < static_cast<int>(sine.size()); n++)
        autocorrelation += sine[n] * sine[n];

    // Calculate max correlation
    for (int n = -sine.size(); n < static_cast<int>(signal.size()); n++) {
        double corr = 0;
        for (int m = 0; m < static_cast<int>(signal.size()); m++) {
            if (!((m+n) < 0 || (m+n) >= static_cast<int>(sine.size())))
                corr += signal[m] * sine[m + n];
        }
        if (corr > max_corr) max_corr = corr;
    }

    return max_corr/autocorrelation;
}

class FlexRIOCPUDAQMod5761 : public IrioFixture {
   public:
	FlexRIOCPUDAQMod5761() : IrioFixture("Mod5761") {}
};

class FlexRIOCPUDAQMod5761Error : public FlexRIOCPUDAQMod5761 {};

TEST_F(FlexRIOCPUDAQMod5761, CheckModule){
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	const auto analog = irio.getTerminalsAnalog();

	const auto module = analog.getModuleConnected();
	ASSERT_EQ(module, ModulesType::FlexRIO_NI5761) << "The connected module is not a NI5761";
}


TEST_F(FlexRIOCPUDAQMod5761, GetSetEnableAO) {
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

TEST_F(FlexRIOCPUDAQMod5761, GetSetAO) {
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

TEST_F(FlexRIOCPUDAQMod5761, cleanAllDMAs) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();

	auto daq = irio.getTerminalsDAQ();
	EXPECT_NO_THROW(daq.cleanAllDMAs());
}

TEST_F(FlexRIOCPUDAQMod5761, readDataNonBlocking){
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

TEST_F(FlexRIOCPUDAQMod5761, readDataNonBlockingNoData){
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

TEST_F(FlexRIOCPUDAQMod5761, readDataBlocking){
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

TEST_F(FlexRIOCPUDAQMod5761, readDataBlockingTimeout){
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

	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);

	EXPECT_THROW(daq.readDataBlocking(DMANum, lengthBlock*blocksToRead, data.get(), timeout); // @suppress("Goto statement used")
					, errors::DMAReadTimeout) << "Timeout exception was not raised";

	daq.disableDMA(DMANum);
	commonTerm.setDAQStop();
	daq.cleanDMA(DMANum);
}


TEST_F(FlexRIOCPUDAQMod5761, readSine) {
	// Parameters
	const uint32_t blocksToRead = 1;
	const uint32_t sampFreq = 500000;
	const uint32_t dmaID = 0;
	const uint32_t dmaChannel = 2;
	const uint32_t sgChannel = 0;
	const uint32_t sgAmp = 2048;
	const uint32_t sgSigFreq = 10000;
	const uint32_t sgUpdRate = 10000000; 
	const uint32_t ignoreFirstSamples = 10;
	const uint32_t samplesToCompare = 100;
	const uint8_t signalType = 1;
	const uint32_t analogResNum = 0;
	const double corrThreshold = 0.99;


	Irio irio(getBitfilePath(), serialNumber, "V1.2");
	auto common = irio.getTerminalsCommon();
	auto analog = irio.getTerminalsAnalog();
	auto sg = irio.getTerminalsSignalGeneration();
	auto daq = irio.getTerminalsDAQ();

	irio.startFPGA();
	common.setDebugMode(false);
	analog.setAICouplingMode(CouplingMode::AC);

	// Configure signal generator
	const auto sg_fref = sg.getSGFref(sgChannel);
	sg.setSGUpdateRateDecimation(sgChannel, sg_fref/sgUpdRate);
	const auto SGFreq = static_cast<int>(sgSigFreq*(UINT_MAX/static_cast<double>(sgUpdRate)));
	sg.setSGFreqDecimation(sgChannel, SGFreq);
	sg.setSGAmp(sgChannel, sgAmp);
	sg.setSGSignalType(sgChannel, signalType);

	analog.setAOEnable(analogResNum, true);

	// Get Parameters
	const auto NCh = daq.getNCh(dmaID);
	const auto BlockNWords = daq.getLengthBlock(dmaID);

	// Setup DMA
	daq.startDMA(dmaID);
	daq.setSamplingRateDecimation(dmaID, common.getFref()/sampFreq);
	daq.enableDMA(dmaID);
	common.setDAQStart();

	const size_t dataToRead = blocksToRead*BlockNWords; 
	std::vector<uint64_t> vec(dataToRead);
	daq.readDataBlocking(dmaID, dataToRead, vec.data(), 5000);
	uint16_t* data = reinterpret_cast<uint16_t*>(vec.data());
	
	// Normalize signal
	std::vector<double> signal;
	for (size_t i = ignoreFirstSamples; i < ignoreFirstSamples + samplesToCompare; ++i) {
		signal.push_back(static_cast<float>(reinterpret_cast<int16_t*>(data)[(i * NCh) + dmaChannel])/sgAmp);
	}

	double corrCoef = sineCorrelation(signal, sgSigFreq, sampFreq);
	EXPECT_GE(corrCoef, corrThreshold);

	daq.disableDMA(dmaID);
	common.setDAQStop();
	daq.cleanAllDMAs();
}


/// Error FlexRIO DAQ Tests

TEST_F(FlexRIOCPUDAQMod5761Error, InvalidAnalogResource) {
	uint32_t invalidChannel = static_cast<std::uint32_t>(-1);

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	auto analog = irio.getTerminalsAnalog();
	irio.startFPGA();

	EXPECT_THROW(analog.getAI(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(analog.getAO(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(analog.getAOEnable(invalidChannel), errors::ResourceNotFoundError);

	EXPECT_THROW(analog.setAO(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(analog.setAOEnable(invalidChannel, 0), errors::ResourceNotFoundError);
}

TEST_F(FlexRIOCPUDAQMod5761Error, InvalidDAQResource) {
	uint32_t invalidChannel = static_cast<std::uint32_t>(-1);

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();

	auto daq = irio.getTerminalsDAQ();

	EXPECT_THROW(daq.getNCh(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.getDMAOverflow(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.getFrameType(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.getSampleSize(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.startDMA(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.stopDMA(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.cleanDMA(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.isDMAEnable(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.enableDMA(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.disableDMA(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.enaDisDMA(invalidChannel, false), errors::ResourceNotFoundError);
	EXPECT_THROW(daq.readData(invalidChannel, 0, nullptr, false), errors::ResourceNotFoundError);
}
