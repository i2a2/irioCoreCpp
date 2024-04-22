#include <gtest/gtest.h>

#include <cstdlib>
#include <string>
#include <random>
#include <chrono>

// IRIO Library
extern "C" {
#include <irioDataTypes.h>
#include <irioDriver.h>
#include <irioError.h>
#include <irioHandlerAnalog.h>
#include <irioHandlerDMA.h>
#include <irioHandlerDigital.h>
#include <irioHandlerImage.h>
#include <irioHandlerSG.h>
#include <irioResourceFinder.h>
#include <limits.h>
#include <math.h>
}

#include "testUtils-IRIO.h"

using namespace TestUtilsIRIO;
using std::cout; 
using std::endl;

/**
 * TP-IRL-3000 Checking for FlexRIO basic initialization and close functions.
 * 
 * This test checks the IRIO library functions: irio_initDriver and
 * irio_closeDriver in the FlexRIO platform. The objective is to check that
 * all terminals are identified correctly by the irio_initDriver. This test uses
 * a set of multiple bitfiles containing multiple resources. Besides, the test
 * try to show that the IRIO library detects inconsistences in the FPGA
 * configuration.
 *
 * Implemented in tests:
 * - FlexRIOResources.ResourcesCPUDAQ
 * - FlexRIOResources.ResourcesCPUIMAQ
 * - FlexRIOResources.ResourcesMAXIO
 * - FlexRIOResources.ResourcesMissing
 */
TEST(FlexRIOResources, ResourcesCPUDAQ) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    int st = initDriver(IRIOProfile::CPUDAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources:
    // 2 DMAs
    if (verbose_test) cout << "[TEST] Found " << res.DMA << " DMAs. Expected 2" << endl;
    EXPECT_EQ(res.DMA, 2);
    // 2 AI
	if (verbose_test) cout << "[TEST] Found " << res.AI << " AIs. Expected 2" << endl;
	EXPECT_EQ(res.AI, 2);
    // 2 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 2" << endl;
	EXPECT_EQ(res.AO, 2);
    // 2 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(res.auxAI, 2);
    // 2 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(res.auxAO, 2);
    // 2 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 2" << endl;
	EXPECT_EQ(res.DI, 2);
    // 2 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 2" << endl;
	EXPECT_EQ(res.DO, 2);
    // 2 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(res.auxDI, 2);
    // 2 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(res.auxDO, 2);
    // 2 SG
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 2" << endl;
	EXPECT_EQ(res.SG, 2);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOResources, ResourcesIMAQ) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    int st = initDriver(IRIOProfile::CPUIMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources
	// 1 IMAQ DMA
	if (verbose_test) cout << "[TEST] Found " << res.DMA << " IMAQ DMAs. Expected 1" << endl;
	EXPECT_EQ(res.DMA, 1);
    // 2 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(res.auxAI, 2);
    // 2 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(res.auxAO, 2);
    // 2 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 2" << endl;
	EXPECT_EQ(res.DI, 2);
    // 2 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 2" << endl;
	EXPECT_EQ(res.DO, 2);
    // 2 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(res.auxDI, 2);
    // 2 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(res.auxDO, 2);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOResources, ResourcesMAXIO) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    irioDrv_t drv;
    int st = initDriver(IRIOProfile::OnlyResources, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	irioResources_t res;
	getResources(&drv, &res);

    // Expected resources:
    // 1 DMAs
    if (verbose_test) cout << "[TEST] Found " << res.DMA << " DMAs. Expected 1" << endl;
    EXPECT_EQ(res.DMA , 1);
    // 2 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 2" << endl;
	EXPECT_EQ(res.AO, 2);
    // 16 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 16" << endl;
	EXPECT_EQ(res.auxAI, 16);
    // 16 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 16" << endl;
	EXPECT_EQ(res.auxAO, 16);
    // 16 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 16" << endl;
	EXPECT_EQ(res.auxDI, 16);
    // 16 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 16" << endl;
	EXPECT_EQ(res.auxDO, 16);
    // 54 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 54" << endl;
	EXPECT_EQ(res.DI, 54);
    // 54 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 54" << endl;
	EXPECT_EQ(res.DO, 54);
    // 2 SG
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 2" << endl;
	EXPECT_EQ(res.SG, 2);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}

/**
 * TP-IRL-3001 Checking for FlexRIO the irio_setAuxAO, irio_getAuxAI, irio_setAuxDO, 
 * irio_getAuxDI, irio_getDevTemp functions
 * 
 * This test checks the iRIO library functions: irio_initDriver,
 * irio_setFPGAStart, irio_setAuxAO, irio_getAuxAO, irio_getAuxAI,
 * irio_setAuxDO, irio_getAuxDO, irio_getAuxDI, irio_getDevTemp,
 * and irio_closeDriver in the FlexRIO platform. The bitfile used is
 * described in the FlexRIO template FlexRIOnoModule
 * 
 * Implemented in tests:
 * - FlexRIOnoModule.GetSetAuxAIO32
 * - FlexRIOnoModule.GetSetAuxAIO64
 * - FlexRIOnoModule.GetSetAuxDIO
*/
TEST(FlexRIOnoModule, GetSetAuxAIO32) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int32_t> dist(min + 1, max - 1);

	// For each device and value, write value, read from output and read and compare from input
	for (int i = 0; i < 6; ++i) {
		std::vector<int32_t> values = {min, dist(gen), dist(gen), dist(gen), max};
		for (auto v: values) {
			if (verbose_test) cout << "Writing " << v << " to AuxAO" << i << endl;
			// Write value 
			st = irio_setAuxAO(&drv, i, v, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			irio_resetStatus(&status);

			// Read written value
			int32_t written = -1;
			st = irio_getAuxAO(&drv, i, &written, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			EXPECT_EQ(written, v);
			irio_resetStatus(&status);

			// Read value from input
			int32_t read = -1;
			st = irio_getAuxAI(&drv, i, &read, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			if (verbose_test) cout << "Read " << v << " from AuxAI" << i << endl;
			EXPECT_EQ(read, v);
			irio_resetStatus(&status);
		}
	}

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOnoModule, GetSetAuxAIO64) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int64_t> dist(min + 1, max - 1);

	// For each device and value, write value, read from output and read and compare from input
	for (int i = 0; i < 6; ++i) {
		std::vector<int64_t> values = {min, dist(gen), dist(gen), dist(gen), max};
		for (auto v: values) {
			if (verbose_test) cout << "Writing " << v << " to AuxAO" << i << endl;
			// Write value 
			st = irio_setAuxAO_64(&drv, i, v, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			irio_resetStatus(&status);

			// Read written value
			int64_t written = -1;
			st = irio_getAuxAO_64(&drv, i, &written, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			EXPECT_EQ(written, v);
			irio_resetStatus(&status);

			// Read value from input
			int64_t read = -1;
			st = irio_getAuxAI_64(&drv, i, &read, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			if (verbose_test) cout << "Read " << v << " from AuxAI" << i << endl;
			EXPECT_EQ(read, v);
			irio_resetStatus(&status);
		}
	}

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOnoModule, GetSetAuxDIO) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	for (int i = 0; i < 6; ++i) {
		for (int v: {0, 1}) {
			if (verbose_test) cout << "Writing " << v << " to AuxDO" << i << endl;
			// Write value 
			st = irio_setAuxDO(&drv, i, v, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			irio_resetStatus(&status);

			// Read written value
			int written = -1;
			st = irio_getAuxDO(&drv, i, &written, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			EXPECT_EQ(written, v);
			irio_resetStatus(&status);

			// Read value from input
			int read = -1;
			st = irio_getAuxDI(&drv, i, &read, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			if (verbose_test) cout << "Read " << v << " from AuxAI" << i << endl;
			EXPECT_EQ(read, v);
			irio_resetStatus(&status);
		}
	}

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}

/**
 * TP-IRL-3002 Checking for FlexRIO PXIe7966/NI5761 analog data acquisition using FlexRIOModule5761 template
 * 
 * This test checks the iRIO library functions: irio_cleanDMAsTtoHost, irio_setUpDMAsTtoHost,
 * irio_setDMATtoHostEnable, irio_setDMATtoHostSamplingRate, irio_setDAQStartStop,
 * irio_getDMATtoHostData, irio_setDebugMode, irio_getDMATtoHostOverflow.
 * 
 * Implemented in:
 * - FlexRIODAQ5761.GetSetSGSignalType
 * - FlexRIODAQ5761.GetSetEnableAO0
 * - FlexRIODAQ5761.GetSetAO0
 * - FlexRIODAQ5761.CleanDMA
 * - FlexRIODAQ5761.SetupDMAToHost
 * - FlexRIODAQ5761.CloseDMA
 * - FlexRIODAQ5761.GetSetDMAToHostSamplingRate
 * - FlexRIODAQ5761.GetSetAICoupling
 * - FlexRIODAQ5761.GetSetDMAToHostEnable
 * - FlexRIODAQ5761.GetSetDAQStartStop
 * - FlexRIODAQ5761.GetDMAToHostParameters
 * - FlexRIODAQ5761.GetDMAToHostOverflow
 * - FlexRIODAQ5761.ReadDMADCNoTimeout
 * - FlexRIODAQ5761.ReadDMADCTimeout
 * - FlexRIODAQ5761.GetSetSGUpdateRate
 * - FlexRIODAQ5761.GetSetSGSignalFreq
 * - FlexRIODAQ5761.GetSetSGSignalAmp
 * - FlexRIODAQ5761.GetSetSGSignalPhase
 * - FlexRIODAQ5761.GetSGCVADC
 * - FlexRIODAQ5761.ReadDMASineTimeout
 * - FlexRIODAQ5761.ReadDMASineNoTimeout
*/
TEST(FlexRIODAQ5761, GetSetSGSignalType) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	SG::setSignalType(&drv, 0, 0);

	int read = -1;
	st = irio_getSGSignalType(&drv,0,&read,&status);
	if (verbose_test) cout << "[TEST] SGSignalType0 read = " << read << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(read, 0);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetEnableAO0) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Setting EnableAO0 to 1" << endl;
	st = irio_setAOEnable(&drv, 0, 1, &status);
	if (verbose_test) cout << "[TEST] EnableAO0 set" << (st ? " unsuccessfully" : " successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	int read = -1;
	if (verbose_test) cout << "[TEST] Reading EnableAO0" << endl;
	st = irio_getAOEnable(&drv, 0, &read, &status);
	if (verbose_test) cout << "[TEST] EnableAO0 = " << read << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(read, 1);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetAO0) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	// Initialize random engine
	int min = 0, max = 2048;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int32_t> dist(min + 1, max - 1);

    int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	// Enable AO0
	if (verbose_test) cout << "[TEST] Setting EnableAO0 to 1" << endl;
	st = irio_setAOEnable(&drv, 0, 1, &status);
	if (verbose_test) cout << "[TEST] EnableAO0 set" << (st ? " unsuccessfully" : " successfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	// Write and read min, max and 5 random values in-between
	for (int32_t v: {min, dist(gen), dist(gen), dist(gen), dist(gen), dist(gen), max}) {
		// Write
		if (verbose_test) cout << "[TEST] Writing " << v << " to AO0" << endl;
		st = irio_setAO(&drv, 0, v, &status);
		logErrors(st, status);
		EXPECT_EQ(st, IRIO_success);
		irio_resetStatus(&status);

		// Check
		int32_t check = -1;
		st = irio_getAO(&drv, 0, &check, &status);
		if (verbose_test) cout << "[TEST] Written " << check << " to AO0" << endl;
		logErrors(st, status);
		EXPECT_EQ(st, IRIO_success);
		EXPECT_EQ(check, v);
		irio_resetStatus(&status);

		// The value can be read on auxAI9
		if (verbose_test) cout << "[TEST] Reading from auxAI9" << endl;
		int32_t read = -1;
		st = irio_getAuxAI(&drv, 9, &read, &status);
		if (verbose_test) cout << "[TEST] Read " << read << " from auxAI9" << endl;
		logErrors(st, status);
		EXPECT_EQ(st, IRIO_success);
		EXPECT_EQ(read, v);
		irio_resetStatus(&status);
	}

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, CleanDMA) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);

    int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	DMAHost::cleanDMA(&drv);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, SetupDMAToHost) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	DMAHost::setupDMA(&drv);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, CloseDMA) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	DMAHost::setupDMA(&drv);

	if (verbose_test) cout << "[TEST] Closing DMAsTtoHost" << endl;
	st = irio_closeDMAsTtoHost(&drv, &status);
	if (verbose_test) cout << "[TEST] DMAs closed " << ((st == IRIO_success) ? "successfully" : "unsuccessfully") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetDMAToHostSamplingRate) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int32_t sampling_rate = 500000; // 50ksps (max 125Msps)
	if (verbose_test) cout << "[TEST] Sampling rate = " << sampling_rate << endl;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	int fref = DMAHost::setSamplingRate(&drv, sampling_rate);

	int32_t reading = -1;
	st = irio_getDMATtoHostSamplingRate(&drv, 0, &reading, &status);
	if (verbose_test) cout << "[TEST] Sampling rate of DMA0 set to " << reading << ". Sampling freq = " << fref/reading << " Samples/s" << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(reading, fref/sampling_rate);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetAICoupling) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int32_t sampling_rate = 500000; // 50ksps (max 125Msps)
	if (verbose_test) cout << "[TEST] Sampling rate = " << sampling_rate << endl;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

    TIRIOCouplingMode coupling = IRIO_coupling_AC;
	setAICoupling(&drv, coupling);
	TIRIOCouplingMode reading = IRIO_coupling_NULL;
	st = irio_getAICoupling(&drv, &reading, &status);
	if (verbose_test) cout << "[TEST] AI Coupling read = " << static_cast<int>(reading) << (reading == IRIO_coupling_DC ? " (DC)" : " (AC)")<< endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(coupling, reading);

    coupling = IRIO_coupling_DC;
	setAICoupling(&drv, coupling);
	reading = IRIO_coupling_NULL;
	st = irio_getAICoupling(&drv, &reading, &status);
	if (verbose_test) cout << "[TEST] AI Coupling read = " << static_cast<int>(reading) << (reading == IRIO_coupling_DC ? " (DC)" : " (AC)")<< endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(coupling, reading);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetDMAToHostEnable) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	DMAHost::setEnable(&drv, 0, 1);

	int reading = -1;
	st = irio_getDMATtoHostEnable(&drv, 0, &reading, &status);
	if (verbose_test) cout << "[TEST] DMATtoHostEnable0 read = " << reading << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(reading, 1);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetDAQStartStop) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	DMAHost::setDAQStartStop(&drv, 1);

	int reading = -1;
	st = irio_getDAQStartStop(&drv, &reading, &status);
	if (verbose_test) cout << "[TEST] DAQStartStop read = " << reading << (reading ? " (ON)" : " (OFF)") << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(reading, 1);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetDMAToHostParameters) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	DMAHost::setEnable(&drv, 0, 1);

	uint16_t BlockNWords = 0;
	if (verbose_test) cout << "[TEST] Reading DMA to Host BlockNWords" << endl;
	st = irio_getDMATtoHOSTBlockNWords(&drv, &BlockNWords, &status);
	if (verbose_test) cout << "[TEST] BlockNWords read = " << BlockNWords << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	uint16_t NCh = 0;
	if (verbose_test) cout << "[TEST] Reading DMA to Host NChannels" << endl;
	st = irio_getDMATtoHOSTNCh(&drv, &NCh, &status);
	if (verbose_test) cout << "[TEST] NChannels read = " << NCh << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetDMAToHostOverflow) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	DMAHost::setEnable(&drv, 0, 1);

	int32_t overflow = -1;
	if (verbose_test) cout << "[TEST] Reading DMA to Host Overflow" << endl;
	st = irio_getDMATtoHostOverflow(&drv, &overflow, &status);
	if (verbose_test) cout << "[TEST] Overflow read = " << overflow << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(overflow, 0);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, ReadDMADCNoTimeout) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	// Parameters
	int blocksToRead = 1;
	int sampling_freq = 500000;
	int channel = 2;
	int desired_value = 2048;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	string couplingStr = TestUtilsIRIO::getEnvVar("Coupling");
	setAICoupling(&drv, static_cast<TIRIOCouplingMode>(couplingStr == "1" || couplingStr == "DC"));
	// Enable AO0 and set to 2048 and configure SGSignalType0
	if (verbose_test) cout << "[TEST] Configuring the signal generator to output a DC of 2048" << endl;
	st  = irio_setSGSignalType(&drv,0,0,&status);
	st |= irio_setAO(&drv, 0, 2048, &status);
	st |= irio_setAOEnable(&drv, 0, 1, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] SG set" << (st ? " unsuccessfully" : " successfully") << endl;

	// Get Parameters
	uint16_t NCh = 0, BlockNWords = 0;
	st  = irio_getDMATtoHOSTNCh(&drv, &NCh, &status);
	st |= irio_getDMATtoHOSTBlockNWords(&drv, &BlockNWords, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	// Setup DMA
	DMAHost::setupDMA(&drv);
	DMAHost::setSamplingRate(&drv, sampling_freq);
	DMAHost::setEnable(&drv, 0, 1);
	DMAHost::setDAQStartStop(&drv, 1);

	int positiveTest = 0, negativeTest = 0;
	std::vector<uint64_t> vec = DMAHost::readDMAData(&drv, 0, blocksToRead, BlockNWords, sampling_freq); 
	uint16_t* data = reinterpret_cast<uint16_t*>(vec.data());
	for (int i = 0; i < blocksToRead * BlockNWords; i++) {
		if (data[(i * NCh) + channel] == desired_value) {
			positiveTest++;
		} else {
			negativeTest++;
		}
	}

	if (verbose_test) cout << "[TEST] Correct samples = " << positiveTest << ". Incorrect samples = " << negativeTest << endl;
	EXPECT_EQ(positiveTest, blocksToRead * BlockNWords);
	EXPECT_EQ(negativeTest, 0);

	DMAHost::setEnable(&drv, 0, 0);
	DMAHost::setDAQStartStop(&drv, 0);
	DMAHost::cleanDMA(&drv);
	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, ReadDMADCTimeout) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	// Parameters
	int blocksToRead = 1;
	int sampling_freq = 500000;
	int channel = 2;
	int desired_value = 2048;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	string couplingStr = TestUtilsIRIO::getEnvVar("Coupling");
	setAICoupling(&drv, static_cast<TIRIOCouplingMode>(couplingStr == "1" || couplingStr == "DC"));

	// Enable AO0 and set to 2048 and configure SGSignalType0
	if (verbose_test) cout << "[TEST] Configuring the signal generator to output a DC of 2048" << endl;
	st  = irio_setSGSignalType(&drv,0,0,&status);
	st |= irio_setAO(&drv, 0, 2048, &status);
	st |= irio_setAOEnable(&drv, 0, 1, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] SG set" << (st ? " unsuccessfully" : " successfully") << endl;

	// Get Parameters
	uint16_t NCh = 0, BlockNWords = 0;
	st  = irio_getDMATtoHOSTNCh(&drv, &NCh, &status);
	st |= irio_getDMATtoHOSTBlockNWords(&drv, &BlockNWords, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	// Setup DMA
	DMAHost::setupDMA(&drv);
	DMAHost::setSamplingRate(&drv, sampling_freq);
	DMAHost::setEnable(&drv, 0, 1);
	DMAHost::setDAQStartStop(&drv, 1);

	int positiveTest = 0, negativeTest = 0;
	std::vector<uint64_t> vec = DMAHost::readDMADataTimeout(&drv, 0, blocksToRead, BlockNWords, sampling_freq); 
	uint16_t* data = reinterpret_cast<uint16_t*>(vec.data());
	for (int i = 0; i < blocksToRead * BlockNWords; i++) {
		if (data[(i * NCh) + channel] == desired_value) {
			positiveTest++;
		} else {
			negativeTest++;
		}
	}

	if (verbose_test) cout << "[TEST] Correct samples = " << positiveTest << ". Incorrect samples = " << negativeTest << endl;
	EXPECT_EQ(positiveTest, blocksToRead * BlockNWords);
	EXPECT_EQ(negativeTest, 0);

	DMAHost::setEnable(&drv, 0, 0);
	DMAHost::setDAQStartStop(&drv, 0);
	DMAHost::cleanDMA(&drv);
	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetSGUpdateRate) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int update_rate = 10000000; // 10 MSps
	int channel = 0;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	uint32_t fref = SG::getFref(&drv, channel);
	SG::setUpdateRate(&drv, channel, update_rate, fref);

	int32_t read = -1;
	st = irio_getSGUpdateRate(&drv, channel, &read, &status);
	logErrors(st, status);
	if (verbose_test) cout << "[TEST] SGUpdateRate" << channel << " read = " << read << ", meaning " << fref/read << " Samples/s" << endl;
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(update_rate, fref/read);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetSGSignalFreq) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int sig_freq = 10000; // 10 kHz
	int update_rate = 10000000; // 10 MSps
	int channel = 0;
	double allowed_delta = 0.01;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	SG::setFsig(&drv, channel, update_rate, sig_freq);

	int32_t read = -1;
	st = irio_getSGFreq(&drv, channel, &read, &status);
	double read_fsig = read/static_cast<double>(UINT_MAX/static_cast<double>(update_rate));
	logErrors(st, status);
	if (verbose_test) cout << "[TEST] SGFreq" << channel << " read = " << read << ", meaning " << read_fsig << " Hz" << endl;
	EXPECT_EQ(st, IRIO_success);
	EXPECT_NEAR(read_fsig, static_cast<double>(sig_freq), allowed_delta);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetSGSignalAmp) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int sig_amp = 4096; // 0.517563 V
	int channel = 0;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	SG::setSigAmp(&drv, channel, sig_amp);
	double CVDAC = SG::getCVDAC(&drv);
	if (verbose_test) cout << "[TEST] SGAmp" << channel << " set = " << sig_amp << ", meaning " << std::setprecision(6) << sig_amp/CVDAC << " V" << endl;

	int32_t read = -1;
	st = irio_getSGAmp(&drv, channel, &read, &status);
	logErrors(st, status);
	if (verbose_test) cout << "[TEST] SGAmp" << channel << " read = " << read << ", meaning " << std::setprecision(6) << read/CVDAC << " V" << endl;
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(read, sig_amp);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSetSGSignalPhase) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int channel = 0;
	int phase = 1;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	SG::setSigPhase(&drv, channel, phase);

	if (verbose_test) cout << "[TEST] Reading SGSignalPhase" << channel << endl;
	int32_t read = -1;
	st = irio_getSGPhase(&drv, channel, &read, &status);
	if (verbose_test) cout << "[TEST] SGSignalPhase" << channel << " read = " << read << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(read, phase);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, GetSGCVADC) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);

	if (verbose_test) cout << "[TEST] Reading SGCVADC" << endl;
	double read = -1;
	st = irio_getSGCVADC(&drv, &read, &status);
	if (verbose_test) cout << "[TEST] SGCVADC read = " << std::setprecision(6) << read << " V" << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, ReadDMASineNoTimeout) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	// Parameters
	int blocksToRead = 1;
	int sampling_freq = 500000;
	int dma_channel = 2;
	int sg_channel = 0;
	int sg_amp = 2048;
	int sg_sigfreq = 10000;
	int sg_updrate = 10000000; 
	int ignore_first_samples = 10;
	int compare_samples = 100;
	double corr_threshold = 0.99;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	string couplingStr = TestUtilsIRIO::getEnvVar("Coupling");
	setAICoupling(&drv, static_cast<TIRIOCouplingMode>(couplingStr == "1" || couplingStr == "DC"));

	// Configure signal generator
	int32_t sg_fref = SG::getFref(&drv, sg_channel);
	SG::setUpdateRate(&drv, sg_channel, sg_updrate, sg_fref);
	SG::setFsig(&drv, sg_channel, sg_updrate, sg_sigfreq);
	SG::setSigAmp(&drv, sg_channel, sg_amp);
	SG::setSignalType(&drv, sg_channel, 1); // Sine function

	if (verbose_test) cout << "[TEST] Enabling AO0" << endl;
	st = irio_setAOEnable(&drv, 0, 1, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
    if (verbose_test) cout << "[TEST] AO0 enabled" << ((st) ? "unsuccessfully" : "successfully") << endl;

	// Get Parameters
	uint16_t NCh = 0, BlockNWords = 0;
	st  = irio_getDMATtoHOSTNCh(&drv, &NCh, &status);
	st |= irio_getDMATtoHOSTBlockNWords(&drv, &BlockNWords, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	// Setup DMA
	DMAHost::setupDMA(&drv);
	DMAHost::setSamplingRate(&drv, sampling_freq);
	DMAHost::setEnable(&drv, 0, 1);
	DMAHost::setDAQStartStop(&drv, 1);

	std::vector<uint64_t> vec = DMAHost::readDMAData(&drv, 0, blocksToRead, BlockNWords, sampling_freq); 
	uint16_t* data = reinterpret_cast<uint16_t*>(vec.data());
	
	// Normalize signal
	std::vector<double> signal;
	for (int i = ignore_first_samples; i < ignore_first_samples + compare_samples; ++i) {
		signal.push_back(static_cast<float>(reinterpret_cast<int16_t*>(data)[(i * NCh) + dma_channel])/sg_amp);
	}

	double corrCoef = sineCorrelation(signal, sg_sigfreq, sampling_freq);
	if (verbose_test) cout << "[TEST] Correlation coefficient to a sine: " << corrCoef << endl;
	EXPECT_GE(corrCoef, corr_threshold);

	DMAHost::setEnable(&drv, 0, 0);
	DMAHost::setDAQStartStop(&drv, 0);
	DMAHost::cleanDMA(&drv);
	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODAQ5761, ReadDMASineTimeout) {
	irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
	int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

	// Parameters
	int blocksToRead = 1;
	int sampling_freq = 500000;
	int dma_channel = 2;
	int sg_channel = 0;
	int sg_amp = 2048;
	int sg_sigfreq = 10000;
	int sg_updrate = 10000000; 
	int ignore_first_samples = 10;
	int compare_samples = 100;
	double corr_threshold = 0.99;

	int st = initDriver(IRIOProfile::Mod5761DAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);
	setDebugMode(&drv, 0);
	string couplingStr = TestUtilsIRIO::getEnvVar("Coupling");
	setAICoupling(&drv, static_cast<TIRIOCouplingMode>(couplingStr == "1" || couplingStr == "DC"));

	// Configure signal generator
	int32_t sg_fref = SG::getFref(&drv, sg_channel);
	SG::setUpdateRate(&drv, sg_channel, sg_updrate, sg_fref);
	SG::setFsig(&drv, sg_channel, sg_updrate, sg_sigfreq);
	SG::setSigAmp(&drv, sg_channel, sg_amp);
	SG::setSignalType(&drv, sg_channel, 1); // Sine function

	if (verbose_test) cout << "[TEST] Enabling AO0" << endl;
	st = irio_setAOEnable(&drv, 0, 1, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
    if (verbose_test) cout << "[TEST] AO0 enabled" << ((st) ? "unsuccessfully" : "successfully") << endl;

	// Get Parameters
	uint16_t NCh = 0, BlockNWords = 0;
	st  = irio_getDMATtoHOSTNCh(&drv, &NCh, &status);
	st |= irio_getDMATtoHOSTBlockNWords(&drv, &BlockNWords, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	// Setup DMA
	DMAHost::setupDMA(&drv);
	DMAHost::setSamplingRate(&drv, sampling_freq);
	DMAHost::setEnable(&drv, 0, 1);
	DMAHost::setDAQStartStop(&drv, 1);

	std::vector<uint64_t> vec = DMAHost::readDMADataTimeout(&drv, 0, blocksToRead, BlockNWords, sampling_freq); 
	uint16_t* data = reinterpret_cast<uint16_t*>(vec.data());
	
	// Normalize signal
	std::vector<double> signal;
	for (int i = ignore_first_samples; i < ignore_first_samples + compare_samples; ++i) {
		signal.push_back(static_cast<float>(reinterpret_cast<int16_t*>(data)[(i * NCh) + dma_channel])/sg_amp);
	}

	double corrCoef = sineCorrelation(signal, sg_sigfreq, sampling_freq);
	if (verbose_test) cout << "[TEST] Correlation coefficient to a sine: " << corrCoef << endl;
	EXPECT_GE(corrCoef, corr_threshold);

	DMAHost::setEnable(&drv, 0, 0);
	DMAHost::setDAQStartStop(&drv, 0);
	DMAHost::cleanDMA(&drv);
	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}

/**
 * TP-IRL-3003   Checking for FlexRIO PXIe7961/NI6581 digital I/O using FlexRIOModule6581 template
 * 
 * This test checks the iRIO library functions: irio_setDO, irio_getDI, irio_getDO.
 * 
 * Implemented in:
 * - FlexRIODIO6181.InitClose
 * - FlexRIODIO6181.StartFPGA
 * - FlexRIODIO6181.GetSetAuxDIO6581
 * - FlexRIODIO6181.GetSetDIO
*/
TEST(FlexRIODIO6581, InitClose) {
    irioDrv_t drv;
    int st = initDriver(IRIOProfile::Mod6581DIO, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODIO6581, StartFPGA) {
    irioDrv_t drv;

    int st = initDriver(IRIOProfile::Mod6581DIO, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	TStatus status;
	irio_initStatus(&status);
	int running = 0;
	st = irio_getFPGAStart(&drv, &running, &status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(running, 1);
	irio_resetStatus(&status);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODIO6581, GetSetAuxDIO6581) {
    irioDrv_t drv;
	int auxN = 6; // Test on AuxDIO6
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod6581DIO, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	for (int32_t b: {0, 1}) {
		if (verbose_test) cout << "[TEST] Writing " << b << " to AuxDO" << auxN << endl; 
		st = irio_setAuxDO(&drv, auxN, b, &status);	
		logErrors(st, status);
		EXPECT_EQ(st, IRIO_success);
		irio_resetStatus(&status);

		int32_t reading = -1;
		st = irio_getAuxDO(&drv, auxN, &reading, &status);
		logErrors(st, status);
		EXPECT_EQ(st, IRIO_success);
		EXPECT_EQ(reading, b);
		irio_resetStatus(&status);
		if (verbose_test) cout <<  "[TEST] Write " << ((st || reading != b) ? "unsuccessful" : "successful") << endl;
	}
    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIODIO6581, GetSetDIO) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod6581DIO, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Writing true to AuxDO6" << endl; 
	st = irio_setAuxDO(&drv, 6, 1, &status);	
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	for (int32_t i = 0; i < 8; i++) {
		for (int32_t b: {0, 1}) {
			if (verbose_test) cout << "[TEST] Writing " << b << " to DO" << i << endl; 
			st = irio_setDO(&drv, i, b, &status);	
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			irio_resetStatus(&status);

			int32_t DOreading = -1;
			st = irio_getDO(&drv, i, &DOreading, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			EXPECT_EQ(DOreading, b);
			irio_resetStatus(&status);
			if (verbose_test) cout <<  "[TEST] Write " << ((st || DOreading != b) ? "unsuccessful" : "successful") << endl;

			if (verbose_test) cout << "[TEST] Reading from DI" << i << endl;
			int32_t reading = -1;
			st = irio_getDI(&drv, i, &reading, &status);
			logErrors(st, status);
			EXPECT_EQ(st, IRIO_success);
			EXPECT_EQ(reading, b);
			irio_resetStatus(&status);
			if (verbose_test) cout << "[TEST] Got " << reading << " from DI" << i << endl;
		}
	}

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}

/**
 * TP-IRL-3004    iRIO library functions checking for FlexRIO template FlexRIOMod1483-7966
 * 
 * This test checks the iRIO library functions: irio_setUARTBaudRate, irio_sendCLuart, 
 * irio_getCLuart and irio_getDMATtoHOSTImage.  This test verifies: The image acquisition
 * using the FlexRIO PXIe-7966R + NI 1483 bundle and the CameraLink Simulator. The simulator
 * will be configured to send test images with an embedded frame counter. Using the library,
 * 1000 images will be acquired from the camera and check the integrity of the frame counter
 * value during the acquisition.  The UART communication between FlexRIO and CameraLink
 * Simulator. The simulator is configured to echo every message received back to the FlexRIO.
 * Then, a message is sent and wait to receive a response. Finally the messages sent and
 * received are compared.
 * 
 * Implemented in:
 * - FlexRIOIMAQ1483.InitClose
 * - FlexRIOIMAQ1483.StartFPGA
 * - FlexRIOIMAQ1483.InitConfigCL
 * - FlexRIOIMAQ1483.GetImages
 * - FlexRIOUART1483.GetUARTBaudRate
 * - FlexRIOUART1483.GetUARTBreakIndicator
 * - FlexRIOUART1483.GetUARTFrammingError
 * - FlexRIOUART1483.GetUARTOverrunError
 * - FlexRIOUART1483.SetUARTBaudRate
 * - FlexRIOUART1483.SendCLUART
 * - FlexRIOUART1483.GetCLUART
*/
TEST(FlexRIOIMAQ1483, InitClose) {
    irioDrv_t drv;
    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOIMAQ1483, StartFPGA) {
    irioDrv_t drv;

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	TStatus status;
	irio_initStatus(&status);
	int running = 0;
	st = irio_getFPGAStart(&drv, &running, &status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(running, 1);
	irio_resetStatus(&status);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOIMAQ1483, InitConfigCL) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOIMAQ1483, InitConfigCLWarning) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with invalid values, should throw a warning" << endl;
	st = irio_configCL(&drv, 0, 0, 0, 0, 0, 1, (CL_SignalMapping_T)99, (CL_Config_T)99, &status);
	logErrors(st, status);
	EXPECT_NE(st, IRIO_success);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful (expected)" : "successful (incorrect)") << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOIMAQ1483, GetImages) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));
    int maxCounter = std::stoi(TestUtilsIRIO::getEnvVar("maxCounter"));

	const int totalImages = 1000;
	const int dmaN = 0;
	const int imageWidth = 256, imageHeight = 256;
	std::unique_ptr<uint64_t[]> buffer(new uint64_t[imageWidth * imageHeight / 8]);

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;
	irio_resetStatus(&status);

	startFPGA(&drv);

	if (verbose_test) {
		cout << "[TEST] Reading " << totalImages << " images from DMA" << dmaN << ". Images are " << imageWidth << "x" << imageHeight << " pixels and have " << drv.DMATtoHOSTSampleSize[dmaN] << " bits per pixel." << endl;
	}

	DMAHost::setEnable(&drv, dmaN, 1);
	DMAHost::setDAQStartStop(&drv, 1);

	int imageCount = 0;
	int readCount = -1;
	uint16_t current_counter = 0, last_counter = 0;
	
	st = IRIO_success;
	while (imageCount < totalImages && st == IRIO_success) {
		st |= irio_getDMATtoHostImage(&drv, imageHeight * imageWidth, dmaN, buffer.get(), &readCount, &status);
		logErrors(st, status);
		EXPECT_EQ(st, IRIO_success);

		if (readCount == imageHeight * imageWidth) {
			current_counter = reinterpret_cast<uint16_t*>(buffer.get())[0]; // Assuming the counter is the first two bytes
			if (imageCount != 0 && (last_counter + 1)%maxCounter != current_counter) { // Don't compare first image
				if (verbose_test) std::cerr << "[TEST] Error on image " << imageCount << " counter" << endl;
				EXPECT_EQ(current_counter, (last_counter + 1)%maxCounter); // Alredy tested, only to print formatted 
			}
			if (verbose_test) cout << "[TEST] Frame " << std::setw(3) << std::setfill('0') << imageCount << ": Counter = " <<  current_counter << endl;
			last_counter = current_counter;
			imageCount++;
		} else {
			usleep(1000);
		}
	}
	EXPECT_EQ(st, IRIO_success);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, GetUARTBaudRate) {
	const int baudRateConversion[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 }; // sps
	
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	if (verbose_test) cout << "[TEST] Reading UARTBaudRate" << endl;
	int32_t uartBR = -1;
	st = irio_getUARTBaudRate(&drv, &uartBR, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_NE(uartBR, -1);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] UARTBaudRate = " << uartBR << " => Baud Rate = " << baudRateConversion[uartBR] << " sps" << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, GetUARTBreakIndicator) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	if (verbose_test) cout << "[TEST] Reading UARTBreakIndicator" << endl;
	int32_t uartBI = -1;
	st = irio_getUARTBreakIndicator(&drv, &uartBI, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_NE(uartBI, -1);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] UARTBreakIndicator = " << uartBI << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, GetUARTFrammingError) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	if (verbose_test) cout << "[TEST] Reading UARTFrammingError" << endl;
	int32_t uartFE = -1;
	st = irio_getUARTFramingError(&drv, &uartFE, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_NE(uartFE, -1);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] UARTFrammingError = " << uartFE << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, GetUARTOverrunError) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	if (verbose_test) cout << "[TEST] Reading UARTOverrunError" << endl;
	int32_t uartOE = -1;
	st = irio_getUARTOverrunError(&drv, &uartOE, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_NE(uartOE, -1);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] UARTOverrunError = " << uartOE << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, SetUARTBaudRate) {
	const int baudRateConversion[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 }; // sps
	const int targetBR = 0;
	
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Setting UARTBaudRate to " << baudRateConversion[targetBR] << " sps" << endl;
	st = irio_setUARTBaudRate(&drv, targetBR, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	if (verbose_test) cout << "[TEST] Reading UARTBaudRate" << endl;
	int32_t uartBR = -1;
	st = irio_getUARTBaudRate(&drv, &uartBR, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(uartBR, targetBR);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] UARTBaudRate = " << uartBR << " => Baud Rate = " << baudRateConversion[uartBR] << " sps" << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, SendCLUART) {
	const int baudRateConversion[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 }; // baud
	const int targetBR = 0;

	const string charactersToSend = "HELO\n";

    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	DMAHost::setupDMA(&drv);
	startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Setting UARTBaudRate to " << baudRateConversion[targetBR] << " baud" << endl;
	st = irio_setUARTBaudRate(&drv, targetBR, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	cout << "[TEST] Open the EDTpdv terminal and press enter here." << endl;
	std::cin.get();

	if (verbose_test) cout << "[TEST] Sending the message: " << charactersToSend << endl;
	st = irio_sendCLuart(&drv, charactersToSend.c_str(), charactersToSend.length(), &status );
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	cout << "[TEST] Message send " << (st ? "unsuccessfully" : "successfully. Check EDTpdv terminal application") << endl;

	sleep(1);
    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(FlexRIOUART1483, GetCLUART) {
	const int baudRateConversion[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 }; // baud
	const int targetBR = 0;
	const int msgsize = 11;

    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::Mod1483IMAQ, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";

	if (verbose_test) cout << "[TEST] Configuring CL with FVAL, LVAL, DVAL and SPARE High, control signals from the FPGA and no linescan. Signal mapping is STANDARD and the configuration is FULL mode" << endl;
	st = irio_configCL(&drv, 1, 1, 1, 1, 1, 0, CL_STANDARD, CL_FULL, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	DMAHost::setupDMA(&drv);
	startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Setting UARTBaudRate to " << baudRateConversion[targetBR] << " baud" << endl;
	st = irio_setUARTBaudRate(&drv, targetBR, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);
	if (verbose_test) cout << "[TEST] Configuration " << (st ? "unsuccessful" : "successful") << endl;

	int rcvsize = -1;
	char buffer[msgsize] = {};
	cout << "[TEST] Write a message up to " << msgsize - 1 << " characters on the EDTpdv terminal and press enter here" << endl;
	std::cin.get();
	st = irio_getCLuartWithBufferSize(&drv, msgsize, buffer, &rcvsize, &status);
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	irio_resetStatus(&status);

	if (buffer[rcvsize] == '\n') { // Remove last line break 
		buffer[rcvsize] = '\0';
	}
	cout << "[TEST] Message received: " << buffer << " (" << rcvsize << " characters)" << endl;

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}