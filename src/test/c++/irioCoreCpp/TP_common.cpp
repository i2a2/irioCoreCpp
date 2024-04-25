#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <chrono>

#include "irioFixture.h"
#include "irioCoreCpp.h"

using namespace irio;

class CommonTests : public IrioFixture {
 public:
  CommonTests() : IrioFixture("NoModule") {}
};

class CommonTestsError : public CommonTests {};

class CommonTestsResources : public IrioFixture {
 public:
  CommonTestsResources() : IrioFixture("OnlyResources") {}
};

class CommonTestsResourcesError : public CommonTestsResources {};

TEST_F(CommonTests, InitClose) {
    const std::string bitfilePath = getBitfilePath();
	try {
		Irio irio(bitfilePath, serialNumber, "V1.2");
	} catch (std::exception &e) {
		FAIL() << "Error at irioCore's constructor (" + std::string(e.what()) +
					  ")";
	}
}

TEST_F(CommonTests, StartFPGA) {
    const std::string bitfilePath = getBitfilePath();
    Irio irio(bitfilePath, serialNumber, "V1.2");

    try {
        irio.startFPGA();
    } catch(std::exception& e) {
        FAIL() << e.what();
    } catch (...) {
        FAIL() << "An unexpected exception was raised";
    }
}

TEST_F(CommonTests, DevTemp){
	const std::string bitfilePath = getBitfilePath();

	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();

	irio.getTerminalsCommon().getDevTemp();
}

TEST_F(CommonTests, DevQualityStatus) {
    const std::string bitfilePath = getBitfilePath();
    Irio irio(bitfilePath, serialNumber, "V1.2");

    EXPECT_EQ(irio.getTerminalsCommon().getDevQualityStatus(), 0);
}

TEST_F(CommonTests, FPGAVIversion) {
    const std::string bitfilePath = getBitfilePath();
    Irio irio(bitfilePath, serialNumber, "V1.2");

    EXPECT_EQ(irio.getTerminalsCommon().getFPGAVIversion(), "V1.2");
}

TEST_F(CommonTests, DebugMode){
	const std::string bitfilePath = getBitfilePath();

	Irio irio(bitfilePath, serialNumber, "V1.2");
	const auto commonTerm = irio.getTerminalsCommon();
	irio.startFPGA();
	commonTerm.setDebugMode(false);
	EXPECT_FALSE(commonTerm.getDebugMode());

	commonTerm.setDebugMode(true);
	EXPECT_TRUE(commonTerm.getDebugMode());
}

TEST_F(CommonTests, GetMinSamplingRate){
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_DOUBLE_EQ(irio.getTerminalsCommon().getMinSamplingRate(),
					 1525.9021896696422);
}

TEST_F(CommonTests, GetMaxSamplingRate){
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_DOUBLE_EQ(irio.getTerminalsCommon().getMaxSamplingRate(), 100e6);
}

TEST_F(CommonTestsResources, Resources){
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

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

TEST_F(CommonTests, GetSetAuxAIO32) {
	const std::string bitfilePath = getBitfilePath();
	const size_t numAuxAnalog = 5;
	const size_t numTests = 10;

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int32_t> dist(min + 1, max - 1);

	Irio irio(bitfilePath, serialNumber, "V1.2");
	auto auxAnalog = irio.getTerminalsAuxAnalog();

	irio.startFPGA();
	
	ASSERT_GE(auxAnalog.getNumAuxAI(), numAuxAnalog) << "Insufficient number of auxAI";
	ASSERT_GE(auxAnalog.getNumAuxAO(), numAuxAnalog) << "Insufficient number of auxAO";

	std::int32_t valueWrite;
	std::int32_t valueRead;
	for(size_t n = 0; n < numAuxAnalog; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = dist(gen);
			auxAnalog.setAuxAO(n, valueWrite);
			valueRead = auxAnalog.getAuxAO(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxAO" << n;

			valueRead = auxAnalog.getAuxAI(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value written in auxAO" << n
					<< " was not read in auxAI" << n;
		}
	}
}

TEST_F(CommonTests, GetSetAuxAIO64) {
	const std::string bitfilePath = getBitfilePath();
	const size_t numAuxAnalog = 5;
	const size_t numTests = 10;

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int64_t> dist(min + 1, max - 1);

	Irio irio(bitfilePath, serialNumber, "V1.2");
	auto auxAnalog = irio.getTerminalsAuxAnalog();

	irio.startFPGA();
	
	ASSERT_GE(auxAnalog.getNumAuxAI64(), numAuxAnalog) << "Insufficient number of auxAI";
	ASSERT_GE(auxAnalog.getNumAuxAO64(), numAuxAnalog) << "Insufficient number of auxAO";

	std::int32_t valueWrite;
	std::int32_t valueRead;
	for(size_t n = 0; n < numAuxAnalog; ++n){
		for(size_t t = 0; t < numTests; ++t){
			valueWrite = dist(gen);
			auxAnalog.setAuxAO64(n, valueWrite);
			valueRead = auxAnalog.getAuxAO64(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value was not written in auxAO" << n;

			valueRead = auxAnalog.getAuxAI64(n);
			EXPECT_EQ(valueWrite, valueRead) << "The value written in auxAO" << n
					<< " was not read in auxAI" << n;
		}
	}
}

TEST_F(CommonTests, GetSetAuxDIO) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	auto auxDigital = irio.getTerminalsAuxDigital();
	try {
		for (int i = 0; i < 6; ++i) {
			for (int v : {0, 1}) {
				// Write value
				auxDigital.setAuxDO(i, v);

				// Read written value
				const auto written = auxDigital.getAuxDO(i);
				EXPECT_EQ(written, v);

				// Read value from input
				const auto read = auxDigital.getAuxDI(i);
				EXPECT_EQ(read, v);
			}
		}
	} catch (errors::IrioError& e) {
		FAIL() << e.what();
	}
}

/// Error Common Tests

TEST_F(CommonTestsError, FPGAVIversionMismatch){
	const std::string bitfilePath = getBitfilePath();
	try{
		Irio irio(bitfilePath, serialNumber, "V0.0");
		FAIL() << "FPGAVIversion mismatch exception not thrown";
	}catch(errors::FPGAVIVersionMismatchError&){
		SUCCEED();
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

TEST_F(CommonTestsResources, ResourcesMissing){
	const std::string bitfilePath = getBitfilePath("failResources");

	try{
		Irio irio(bitfilePath, serialNumber, "V1.2");
		FAIL() << "Missing resources not detected";
	}catch(errors::ResourceNotFoundError&){
		SUCCEED();
	}catch(std::exception &e){
		FAIL() << "An unexpected exception was raised. " + std::string(e.what());
	}catch(...){
		FAIL() << "An unexpected exception was raised.";
	}
}

TEST_F(CommonTestsError, RIODeviceNotFoundError){
	const std::string bitfilePath = getBitfilePath();
	EXPECT_THROW(Irio irio(bitfilePath, "0", "V1.2");,
		errors::RIODeviceNotFoundError);
}
