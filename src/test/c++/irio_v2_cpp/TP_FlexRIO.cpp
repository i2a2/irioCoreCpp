#include <gtest/gtest.h>
#include <iostream>
#include <math.h>
#include "errorsIrio.h"
#include "irioFixture.h"
#include "irio_v2.h"
#include "modules.h"
#include "irioUtils.h"

using namespace iriov2;

class FlexRIOFixture: public IrioFixture {
public:
	FlexRIOFixture(const std::string &typeBitfile): IrioFixture("FlexRIO", typeBitfile){ }
};

class FlexRIOOnlyResources: public ::testing::Test, public FlexRIOFixture {
public:
	FlexRIOOnlyResources(): FlexRIOFixture("OnlyResources"){};
};

class FlexRIONoModule: public ::testing::Test, public FlexRIOFixture {
public:
	FlexRIONoModule(): FlexRIOFixture("NoModule"){};
};

class FlexRIOCPUDAQ: public ::testing::Test, public FlexRIOFixture {
public:
	FlexRIOCPUDAQ(): FlexRIOFixture("CPUDAQ"){};
};

class FlexRIOMod5761: public ::testing::Test, public FlexRIOFixture {
public:
	FlexRIOMod5761(): FlexRIOFixture("Mod5761"){};
};


/////////////////////////////////////////////////////////////
/// OnlyResources Tests
/////////////////////////////////////////////////////////////

/**
 * Test verifies correct initialization with OnlyResources bitfile
 */
TEST_F(FlexRIOOnlyResources, InitClose){
	const std::string bitfilePath = getBitfilePath();
	try{
		IrioV2 irio(bitfilePath, serialNumber, "4.0");
	}catch(std::exception &e){
		FAIL() << "Error at IrioV2's constructor (" + std::string(e.what()) + ")";
	}
}

/**
 * Test verifies that when there is a mismatch in the FPGAVIversion,
 * an exception is thrown
 */
TEST_F(FlexRIOOnlyResources, FPGAVIversionMismatch){
	const std::string bitfilePath = getBitfilePath();
	try{
		IrioV2 irio(bitfilePath, serialNumber, "0.0");
		FAIL() << "FPGAVIversion mismatch exception not thrown";
	}catch(errors::FPGAVIVersionMismatchError&){
		SUCCEED();
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

/**
 * The test verifies that the resources in a bitfile are found correctly,
 * even if there are a large number of them.
 */
TEST_F(FlexRIOOnlyResources, Resources){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	EXPECT_EQ(irio.getTerminalsDAQ().countDMAs(), 1) << "Unexpected number of DMAs";
	EXPECT_EQ(irio.getTerminalsAnalog().getNumAI(), 0) << "Unexpected number of analog inputs";
	EXPECT_EQ(irio.getTerminalsAnalog().getNumAO(), 2) << "Unexpected number of analog outputs";
	EXPECT_EQ(irio.getTerminalsDigital().getNumDI(), 54) << "Unexpected number of digital inputs";
	EXPECT_EQ(irio.getTerminalsDigital().getNumDO(), 54) << "Unexpected number of digital outputs";
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAI(), 16) << "Unexpected number of aux analog inputs";
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAO(), 16) << "Unexpected number of aux analog outputs";
	EXPECT_EQ(irio.getTerminalsAuxDigital().getNumAuxDI(), 16) << "Unexpected number of aux digital inputs";
	EXPECT_EQ(irio.getTerminalsAuxDigital().getNumAuxDO(), 16) << "Unexpected number of aux digital outputs";
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGNo(), 2) << "Unexpected number of signal generators";
}

/**
 * Test verifies that if terminals are missing, an exception it thrown
 * TODO: Multiple tests, each with different terminals missing (analog, digital, etc)
 */
TEST_F(FlexRIOOnlyResources, ResourcesMissing){
	const std::string bitfilePath = getBitfilePath("failResources");

	try{
		IrioV2 irio(bitfilePath, serialNumber, "4.0");
		FAIL() << "Missing resources not detected";
	}catch(errors::ResourceNotFoundError&){
		SUCCEED();
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

/**
 * Test verifies that when using a FlexRIO board, the terminals specific
 * to FlexRIO are  available.
 */
TEST_F(FlexRIOOnlyResources, flexRIOTerminalsAvailable){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	try{
		irio.getTerminalsFlexRIO();
	}catch(errors::TerminalNotImplementedError&){
		FAIL() << "FlexRIO terminals not available";
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

/**
 * Test verifies that when using a FlexRIO board, the terminals specific
 * to cRIO are not available.
 */
TEST_F(FlexRIOOnlyResources, cRIOTerminalsNotAvailable){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	try{
		irio.getTerminalsCRIO();
		FAIL() << "cRIO resources found in FlexRIO profile";
	}catch(errors::TerminalNotImplementedError&){
		SUCCEED();
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

/////////////////////////////////////////////////////////////
/// FlexRIONoModule Tests
/////////////////////////////////////////////////////////////

TEST_F(FlexRIONoModule, StartFPGA){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	try{
		irio.startFPGA();
	}catch(errors::InitializationTimeoutError &e){
		FAIL() << e.what();
	}catch(errors::ModulesNotOKError &e){
		FAIL() << e.what();
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

TEST_F(FlexRIONoModule, AuxAnalog){
	const std::string bitfilePath = getBitfilePath();
	const size_t numAuxAnalog = 6;
	const size_t numTests = 100;
	IntUniformDistribution<std::int32_t> rnd;

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	auto auxAnalog = irio.getTerminalsAuxAnalog();

	irio.startFPGA();
	ASSERT_GE(auxAnalog.getNumAuxAI(), numAuxAnalog) << "Insufficient number of auxAI";
	ASSERT_GE(auxAnalog.getNumAuxAO(), numAuxAnalog) << "Insufficient number of auxAO";

	std::int32_t valueWrite;
	std::int32_t valueRead;
	for(size_t n = 0; n < numAuxAnalog; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = rnd.getRandom();
			auxAnalog.setAuxAO(n, valueWrite);
			valueRead = auxAnalog.getAuxAO(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxAO" << n;

			valueRead = auxAnalog.getAuxAI(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value written in auxAO" << n
					<< " was not read in auxAI" << n;
		}
	}
}

TEST_F(FlexRIONoModule, AuxAnalog64){
	const std::string bitfilePath = getBitfilePath();
	const size_t numAuxAnalog = 6;
	const size_t numTests = 100;
	IntUniformDistribution<std::int64_t> rnd;

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	auto auxAnalog = irio.getTerminalsAuxAnalog();

	irio.startFPGA();
	ASSERT_GE(auxAnalog.getNumAuxAI64(), numAuxAnalog) << "Insufficient number of auxAI64";
	ASSERT_GE(auxAnalog.getNumAuxAO64(), numAuxAnalog) << "Insufficient number of auxAO64";

	std::int64_t valueWrite;
	std::int64_t valueRead;
	for(size_t n = 0; n < numAuxAnalog; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = rnd.getRandom();
			auxAnalog.setAuxAO64(n, valueWrite);
			valueRead = auxAnalog.getAuxAO64(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxAO64" << n;

			valueRead = auxAnalog.getAuxAI64(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value written in aux64AO" << n
					<< " was not read in aux64AI" << n;
		}
	}
}

TEST_F(FlexRIONoModule, AuxDigital){
	const std::string bitfilePath = getBitfilePath();
	const size_t numAuxDigital = 6;
	const size_t numTests = 100;
	IntUniformDistribution<std::uint8_t> rnd(0,1);

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	auto auxDigital = irio.getTerminalsAuxDigital();

	irio.startFPGA();
	ASSERT_GE(auxDigital.getNumAuxDI(), numAuxDigital) << "Insufficient number of auxDI";
	ASSERT_GE(auxDigital.getNumAuxDO(), numAuxDigital) << "Insufficient number of auxDO";

	bool valueWrite;
	bool valueRead;
	for(size_t n = 0; n < numAuxDigital; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = rnd.getRandom();
			auxDigital.setAuxDO(n, valueWrite);
			valueRead = auxDigital.getAuxDO(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxDO" << n;

			valueRead = auxDigital.getAuxDI(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value written in auxDO" << n
					<< " was not read in auxDI" << n;
		}
	}
}

TEST_F(FlexRIONoModule, DevTemp){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	irio.startFPGA();

	irio.getDevTemp();
}

TEST_F(FlexRIONoModule, DebugMode){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	irio.startFPGA();
	irio.setDebugMode(false);
	EXPECT_FALSE(irio.getDebugMode());

	irio.setDebugMode(true);
	EXPECT_TRUE(irio.getDebugMode());
}

//TODO: Check how the SG works, decimation?
//TEST_F(FlexRIONoModule, SGSignalType){
//	const std::string bitfilePath = getBitfilePath();
//
//	IrioV2 irio(bitfilePath, serialNumber, "4.0");
//	auto sg = irio.getTerminalsSignalGeneration();
//
//	ASSERT_GE(sg.getSGNo(), 1);
//
//	irio.startFPGA();
//	irio.setDebugMode(false);
//
//	const auto st = sg.getSGSignalType(0);
//	EXPECT_EQ(st, 0);
//}
//
//TEST_F(FlexRIONoModule, SGUpdateRate){
//	const std::uint32_t channel = 0;
//	const std::uint32_t updateRate = 10000000;
//
//	const std::string bitfilePath = getBitfilePath();
//	IrioV2 irio(bitfilePath, serialNumber, "4.0");
//	const auto sg = irio.getTerminalsSignalGeneration();
//
//	irio.startFPGA();
//	irio.setDebugMode(false);
//
//	sg.setSGUpdateRate(channel, updateRate);
//	EXPECT_EQ(sg.getSGUpdateRate(channel), updateRate) << "SGUpdateRate was not configured properly";
//}
//
//TEST_F(FlexRIONoModule, SGSignalFreq){
//	const std::uint32_t channel = 0;
//	const std::uint32_t signalFreq = 10000;
//
//	const std::string bitfilePath = getBitfilePath();
//	IrioV2 irio(bitfilePath, serialNumber, "4.0");
//	const auto sg = irio.getTerminalsSignalGeneration();
//
//	irio.startFPGA();
//	irio.setDebugMode(false);
//
//
//	auto fref = irio.getFref();
//	auto a = sg.getSGFref(channel);
//	auto b = sg.getSGUpdateRate(channel);
//
//	sg.setSGFreq(channel, signalFreq);
//	EXPECT_EQ(sg.getSGFreq(channel), signalFreq) << "SGSignalFreq was not configured properly";
//}
//
TEST_F(FlexRIONoModule, SGSignalAmp){
	const std::uint32_t channel = 0;
	const std::uint32_t signalAmp = 4096;

	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const auto sg = irio.getTerminalsSignalGeneration();

	irio.startFPGA();
	irio.setDebugMode(false);

	sg.setSGAmp(channel, signalAmp);
	EXPECT_EQ(sg.getSGAmp(channel), signalAmp) << "SGAmp was not configured properly";
}

/////////////////////////////////////////////////////////////
/// FlexRIOCPUDAQ Tests
/////////////////////////////////////////////////////////////

//TODO: Is this test necessary? It is already covered in the NoModule one
/**
 * Test checks that all CPUDAQ terminals are identified
 * correctly at driver initialization
 */
TEST_F(FlexRIOCPUDAQ, Resources){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	EXPECT_EQ(irio.getTerminalsDAQ().countDMAs(), 2) << "Unexpected number of DMAs";
	EXPECT_EQ(irio.getTerminalsAnalog().getNumAI(), 2) << "Unexpected number of analog inputs";
	EXPECT_EQ(irio.getTerminalsAnalog().getNumAO(), 2) << "Unexpected number of analog outputs";
	EXPECT_EQ(irio.getTerminalsDigital().getNumDI(), 2) << "Unexpected number of digital inputs";
	EXPECT_EQ(irio.getTerminalsDigital().getNumDO(), 2) << "Unexpected number of digital outputs";
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAI(), 2) << "Unexpected number of aux analog inputs";
	EXPECT_EQ(irio.getTerminalsAuxAnalog().getNumAuxAO(), 2) << "Unexpected number of aux analog outputs";
	EXPECT_EQ(irio.getTerminalsAuxDigital().getNumAuxDI(), 2) << "Unexpected number of aux digital inputs";
	EXPECT_EQ(irio.getTerminalsAuxDigital().getNumAuxDO(), 2) << "Unexpected number of aux digital outputs";
	EXPECT_EQ(irio.getTerminalsSignalGeneration().getSGNo(), 2) << "Unexpected number of signal generators";
}


/////////////////////////////////////////////////////////////
/// FlexRIOMod5761 Tests
/////////////////////////////////////////////////////////////

TEST_F(FlexRIOMod5761, CheckModule){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	const auto analog = irio.getTerminalsAnalog();

	const auto module = analog.getModuleConnected();
	ASSERT_EQ(module, ModulesType::FlexRIO_NI5761) << "The connected module is not a NI5761";
}

TEST_F(FlexRIOMod5761, AOEnable){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const auto analog = irio.getTerminalsAnalog();
	ASSERT_GE(analog.getNumAO(), 1);
	bool aux;

	irio.startFPGA();

	analog.setAOEnable(0, false);
	aux = analog.getAOEnable(0);
	EXPECT_FALSE(aux);
	analog.setAOEnable(0, true);
	aux = analog.getAOEnable(0);
	EXPECT_TRUE(aux);
	analog.setAOEnable(0, false);
	aux = analog.getAOEnable(0);
	EXPECT_FALSE(aux);
}

TEST_F(FlexRIOMod5761, AO){
	const std::string bitfilePath = getBitfilePath();
	const size_t numTests = 100;
	IntUniformDistribution<std::int32_t> rnd;
	const std::uint32_t idAO = 0;
	const std::uint32_t idAuxAI = 9;

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const auto analog = irio.getTerminalsAnalog();
	const auto auxAnalog = irio.getTerminalsAuxAnalog();

	irio.startFPGA();
	irio.setDebugMode(false);

	std::int32_t valueWrite;
	std::int32_t valueRead;

	analog.setAOEnable(0, true);
	for(size_t i = 0; i < numTests; ++i){
		valueWrite = rnd.getRandom();
		analog.setAO(idAO, valueWrite);
		valueRead = analog.getAO(idAO);
		EXPECT_EQ(valueWrite, valueRead) << "The value was not written in AO0";

		valueRead = auxAnalog.getAuxAI(idAuxAI);
		EXPECT_EQ(valueWrite, valueRead) << "The value written in AO0 was not read in auxAI9";
	}

}

TEST_F(FlexRIOMod5761, DMAClean){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	irio.startFPGA();
	irio.setDebugMode(false);
	irio.getTerminalsDAQ().cleanDMA(0);
	irio.getTerminalsDAQ().cleanAllDMAs();
}

TEST_F(FlexRIOMod5761, DMAStartStop){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	irio.startFPGA();
	irio.setDebugMode(false);

	irio.getTerminalsDAQ().startAllDMAs();
	irio.getTerminalsDAQ().stopAllDMAs();

	irio.getTerminalsDAQ().startDMA(0);
	irio.getTerminalsDAQ().stopDMA(0);
}

TEST_F(FlexRIOMod5761, DMAEnableDisable){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	irio.startFPGA();
	irio.setDebugMode(false);

	EXPECT_FALSE(irio.getTerminalsDAQ().isDMAEnable(0));
	irio.getTerminalsDAQ().enableDMA(0);
	EXPECT_TRUE(irio.getTerminalsDAQ().isDMAEnable(0));
	irio.getTerminalsDAQ().disableDMA(0);
	EXPECT_FALSE(irio.getTerminalsDAQ().isDMAEnable(0));

	irio.getTerminalsDAQ().enaDisDMA(0, true);
	EXPECT_TRUE(irio.getTerminalsDAQ().isDMAEnable(0));
	irio.getTerminalsDAQ().enaDisDMA(0, false);
	EXPECT_FALSE(irio.getTerminalsDAQ().isDMAEnable(0));
}

TEST_F(FlexRIOMod5761, DMASamplingRate){
	const std::uint32_t samplingRate = 500000;
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const std::uint32_t fref = irio.getFref();
	const std::uint32_t decimation = fref / samplingRate;

	irio.startFPGA();
	irio.setDebugMode(false);

	irio.getTerminalsDAQ().setSamplingRateDecimation(0, decimation);
	const auto readDecimation = irio.getTerminalsDAQ().getSamplingRateDecimation(0);
	EXPECT_EQ(samplingRate, fref/readDecimation) << "Unable to configure sampling rate";
}

TEST_F(FlexRIOMod5761, AICoupling){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const auto analog = irio.getTerminalsAnalog();

	ASSERT_EQ(analog.getAICouplingMode(), CouplingMode::AC);
	EXPECT_DOUBLE_EQ(analog.getCVADC(), 1.035/8191) << "Incorrect CVADC in AC";
	EXPECT_DOUBLE_EQ(analog.getCVDAC(), 8191/1.035) << "Incorrect CVDAC in AC";
	EXPECT_DOUBLE_EQ(analog.getMaxValAO(), 1.0) << "Incorrect maxAO in AC";
	EXPECT_DOUBLE_EQ(analog.getMinValAO(), -1.0) << "Incorrect minAO in AC";

	analog.setAICouplingMode(CouplingMode::DC);
	ASSERT_EQ(analog.getAICouplingMode(), CouplingMode::DC);
	ASSERT_EQ(irio.getTerminalsAnalog().getAICouplingMode(), CouplingMode::DC);
	EXPECT_DOUBLE_EQ(analog.getCVADC(), 0.635/8191) << "Incorrect CVADC in DC";
	EXPECT_DOUBLE_EQ(analog.getCVDAC(), 8191/0.635) << "Incorrect CVDAC in DC";
	EXPECT_DOUBLE_EQ(analog.getMaxValAO(), 0.635) << "Incorrect maxAO in DC";
	EXPECT_DOUBLE_EQ(analog.getMinValAO(), -0.635) << "Incorrect minAO in DC";

	EXPECT_THROW(analog.setAICouplingMode(CouplingMode::None), // @suppress("Goto statement used")
			errors::UnsupportedAICouplingForModule);
}

TEST_F(FlexRIOMod5761, DAQStartStop){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	irio.startFPGA();
	irio.setDebugMode(false);

	EXPECT_FALSE(irio.getDAQStartStop());

	irio.setDAQStart();
	EXPECT_TRUE(irio.getDAQStartStop());

	irio.setDAQStop();
	EXPECT_FALSE(irio.getDAQStartStop());
}

TEST_F(FlexRIOMod5761, DMADAQParameters){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");

	irio.startFPGA();
	irio.setDebugMode(false);

	auto daq = irio.getTerminalsDAQ();
	daq.enableDMA(0);
	EXPECT_EQ(daq.getLengthBlock(0), 4096) << "Incorrect length of DMA block";
	EXPECT_EQ(daq.getNCh(0), 4) << "Incorrect number of DMA channels";
}

TEST_F(FlexRIOMod5761, DMADAQGetDataNoTimeout){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t channelDMA = 2;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	irio.startFPGA();
	irio.setDebugMode(false);

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
	const std::uint16_t decimation = irio.getFref()/samplingRate;
	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);
	irio.setDAQStart();

	::usleep(100000);

	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	auto sizeRead = daq.readDataNonBlocking(DMANum, lengthBlock*blocksToRead, data.get());
	EXPECT_EQ(sizeRead, lengthBlock*blocksToRead) << "Incorrect number of elements read";

	daq.disableDMA(DMANum);
	irio.setDAQStop();
	daq.cleanDMA(DMANum);

	std::uint16_t *dataAux = reinterpret_cast<std::uint16_t*>(data.get());
	for (size_t i = 0; i < blocksToRead * lengthBlock; i++) {
		if (dataAux[(i * nCh) + channelDMA] != valDCSG) {
			FAIL() << "Incorrect data read";
		}
	}
}

TEST_F(FlexRIOMod5761, DMADAQGetDataNoTimeoutNoData){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	irio.startFPGA();
	irio.setDebugMode(false);

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
	const std::uint16_t decimation = irio.getFref()/samplingRate;
	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);

	::usleep(100000);

	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	auto sizeRead = daq.readDataNonBlocking(DMANum, lengthBlock*blocksToRead, data.get());
	EXPECT_EQ(sizeRead, 0) << "Data was read when it was not expected";

	daq.disableDMA(DMANum);
	irio.setDAQStop();
	daq.cleanDMA(DMANum);
}

TEST_F(FlexRIOMod5761, DMADAQGetDataTimeout){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t channelDMA = 2;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	irio.startFPGA();
	irio.setDebugMode(false);

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
	const std::uint16_t decimation = irio.getFref()/samplingRate;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	const auto timeout = static_cast<std::uint32_t>(std::ceil(lengthBlock*blocksToRead*1000/samplingRate));

	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);
	irio.setDAQStart();

	try{
		daq.readDataBlocking(DMANum, lengthBlock*blocksToRead, data.get(), timeout);
	}catch(errors::DMAReadTimeout&){
		FAIL() << "Timeout occurred reading DMA when it should have not";
	}

	daq.disableDMA(DMANum);
	irio.setDAQStop();
	daq.cleanDMA(DMANum);

	std::uint16_t *dataAux = reinterpret_cast<std::uint16_t*>(data.get());
	for (size_t i = 0; i < blocksToRead * lengthBlock; i++) {
		if (dataAux[(i * nCh) + channelDMA] != valDCSG) {
			FAIL() << "Incorrect data read";
		}
	}
}


TEST_F(FlexRIOMod5761, DMADAQGetDataTimeoutNoData){
	const std::string bitfilePath = getBitfilePath();
	const size_t blocksToRead = 1;
	const std::uint32_t samplingRate = 500000;
	const std::uint32_t channelSG = 0;
	const std::uint32_t DMANum = 0;
	const std::uint32_t analogNum = 0;
	const std::uint32_t valDCSG = 2048;
	const std::uint8_t signalType = 0; //DC

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	irio.startFPGA();
	irio.setDebugMode(false);

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
	const std::uint16_t decimation = irio.getFref()/samplingRate;
	std::unique_ptr<std::uint64_t> data(new std::uint64_t[lengthBlock*blocksToRead]);
	const auto timeout = static_cast<std::uint32_t>(std::ceil(lengthBlock*blocksToRead*1000/samplingRate));

	daq.startDMA(DMANum);
	daq.setSamplingRateDecimation(DMANum, decimation);
	daq.enableDMA(DMANum);

	EXPECT_THROW(daq.readDataBlocking(DMANum, lengthBlock*blocksToRead, data.get(), timeout); // @suppress("Goto statement used")
					, errors::DMAReadTimeout) << "Timeout exception was not raised";

	daq.disableDMA(DMANum);
	irio.setDAQStop();
	daq.cleanDMA(DMANum);
}


