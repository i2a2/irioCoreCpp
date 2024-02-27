#include <gtest/gtest.h>
#include <iostream>
#include "errorsIrio.h"
#include "irioFixture.h"
#include "irio_v2.h"
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

	EXPECT_EQ(irio.daq()->countDMAs(), 1) << "Unexpected number of DMAs";
	EXPECT_EQ(irio.analog()->getNumAI(), 0) << "Unexpected number of analog inputs";
	EXPECT_EQ(irio.analog()->getNumAO(), 2) << "Unexpected number of analog outputs";
	EXPECT_EQ(irio.digital()->getNumDI(), 54) << "Unexpected number of digital inputs";
	EXPECT_EQ(irio.digital()->getNumDO(), 54) << "Unexpected number of digital outputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAI(), 16) << "Unexpected number of aux analog inputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAO(), 16) << "Unexpected number of aux analog outputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDI(), 16) << "Unexpected number of aux digital inputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDO(), 16) << "Unexpected number of aux digital outputs";
	EXPECT_EQ(irio.signalGeneration()->getSGNo(), 2) << "Unexpected number of signal generators";
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
		irio.flexRIO();
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
		irio.cRIO();
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
	auto auxAnalog = irio.auxAnalog();

	irio.startFPGA();
	ASSERT_GE(auxAnalog->getNumAuxAI(), numAuxAnalog) << "Insufficient number of auxAI";
	ASSERT_GE(auxAnalog->getNumAuxAO(), numAuxAnalog) << "Insufficient number of auxAO";

	std::int32_t valueWrite;
	std::int32_t valueRead;
	for(size_t n = 0; n < numAuxAnalog; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = rnd.getRandom();
			auxAnalog->setAuxAO(n, valueWrite);
			valueRead = auxAnalog->getAuxAO(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxAO" << n;

			valueRead = auxAnalog->getAuxAI(n);
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
	auto auxAnalog = irio.auxAnalog();

	irio.startFPGA();
	ASSERT_GE(auxAnalog->getNumAuxAI64(), numAuxAnalog) << "Insufficient number of auxAI64";
	ASSERT_GE(auxAnalog->getNumAuxAO64(), numAuxAnalog) << "Insufficient number of auxAO64";

	std::int64_t valueWrite;
	std::int64_t valueRead;
	for(size_t n = 0; n < numAuxAnalog; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = rnd.getRandom();
			auxAnalog->setAuxAO64(n, valueWrite);
			valueRead = auxAnalog->getAuxAO64(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxAO64" << n;

			valueRead = auxAnalog->getAuxAI64(n);
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
	auto auxDigital = irio.auxDigital();

	irio.startFPGA();
	ASSERT_GE(auxDigital->getNumAuxDI(), numAuxDigital) << "Insufficient number of auxDI";
	ASSERT_GE(auxDigital->getNumAuxDO(), numAuxDigital) << "Insufficient number of auxDO";

	bool valueWrite;
	bool valueRead;
	for(size_t n = 0; n < numAuxDigital; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = rnd.getRandom();
			auxDigital->setAuxDO(n, valueWrite);
			valueRead = auxDigital->getAuxDO(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxDO" << n;

			valueRead = auxDigital->getAuxDI(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value written in auxDO" << n
					<< " was not read in auxDI" << n;
		}
	}
}

TEST_F(FlexRIONoModule, DevTemp){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");

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

TEST_F(FlexRIONoModule, SGSignalType){
	const std::string bitfilePath = getBitfilePath();

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	auto sg = irio.signalGeneration();

	ASSERT_GE(sg->getSGNo(), 1);

	irio.startFPGA();
	irio.setDebugMode(false);

	const auto st = sg->getSGSignalType(0);
	EXPECT_EQ(st, 0);
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

	EXPECT_EQ(irio.daq()->countDMAs(), 2) << "Unexpected number of DMAs";
	EXPECT_EQ(irio.analog()->getNumAI(), 2) << "Unexpected number of analog inputs";
	EXPECT_EQ(irio.analog()->getNumAO(), 2) << "Unexpected number of analog outputs";
	EXPECT_EQ(irio.digital()->getNumDI(), 2) << "Unexpected number of digital inputs";
	EXPECT_EQ(irio.digital()->getNumDO(), 2) << "Unexpected number of digital outputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAI(), 2) << "Unexpected number of aux analog inputs";
	EXPECT_EQ(irio.auxAnalog()->getNumAuxAO(), 2) << "Unexpected number of aux analog outputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDI(), 2) << "Unexpected number of aux digital inputs";
	EXPECT_EQ(irio.auxDigital()->getNumAuxDO(), 2) << "Unexpected number of aux digital outputs";
	EXPECT_EQ(irio.signalGeneration()->getSGNo(), 2) << "Unexpected number of signal generators";
}


/////////////////////////////////////////////////////////////
/// FlexRIOMod5761 Tests
/////////////////////////////////////////////////////////////

TEST_F(FlexRIOMod5761, AOEnable){
	const std::string bitfilePath = getBitfilePath();
	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const auto analog = irio.analog();
	ASSERT_GE(analog->getNumAO(), 1);
	bool aux;

	irio.startFPGA();

	analog->setAOEnable(0, false);
	aux = analog->getAOEnable(0);
	EXPECT_FALSE(aux);
	analog->setAOEnable(0, true);
	aux = analog->getAOEnable(0);
	EXPECT_TRUE(aux);
	analog->setAOEnable(0, false);
	aux = analog->getAOEnable(0);
	EXPECT_FALSE(aux);
}

TEST_F(FlexRIOMod5761, AO){
	const std::string bitfilePath = getBitfilePath();
	const size_t numTests = 100;
	IntUniformDistribution<std::int32_t> rnd(0,1);
	const std::uint32_t idAO = 0;
	const std::uint32_t idAuxAI = 9;

	IrioV2 irio(bitfilePath, serialNumber, "4.0");
	const auto analog = irio.analog();
	const auto auxAnalog = irio.auxAnalog();

	irio.startFPGA();
	irio.setDebugMode(false);

	std::int32_t valueWrite;
	std::int32_t valueRead;

	analog->setAOEnable(0, true);
	for(size_t i = 0; i < numTests; ++i){
		valueWrite = rnd.getRandom();
		analog->setAO(idAO, valueWrite);
		valueRead = analog->getAO(idAO);
		EXPECT_EQ(valueWrite, valueRead) << "The value was not written in AO0";

		valueRead = auxAnalog->getAuxAI(idAuxAI);
		EXPECT_EQ(valueWrite, valueRead) << "The value written in AO0 was not read in auxAI9";
	}

}

