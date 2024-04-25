#include <gtest/gtest.h>
#include <iostream>

#include "irioFixture.h"
#include "irioCoreCpp.h"

using namespace irio;

class CommonTests : public IrioFixture {
 public:
  CommonTests() : IrioFixture("OnlyResources") {}
};

class CommonTestsError : public CommonTests {};

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

TEST_F(CommonTests, Resources){
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

TEST_F(CommonTestsError, ResourcesMissing){
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
