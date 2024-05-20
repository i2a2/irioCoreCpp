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

TEST(Common, InitCloseDriver) {
    irioDrv_t drv;
    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, StartFPGA) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    startFPGA(&drv);

    int32_t start = -1;
    if (verbose_test) cout << "[TEST] Getting FPGAStart" << endl;
    st = irio_getFPGAStart(&drv, &start, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] FPGAStart read = " << start << endl;
    EXPECT_EQ(st, IRIO_success);
    EXPECT_EQ(start, 1);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, GetDevTemp) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Reading temperature from device" << endl;
	int32_t reading = -1;
	st = irio_getDevTemp(&drv, &reading, &status);
	float temp = 0.25f * reading;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	if (verbose_test) cout << "[TEST] Temperature = " << std::setprecision(4) << temp << "°C" << endl;

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, GetDevQualityStatus) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    startFPGA(&drv);

    int quality = -1;
    if (verbose_test) cout << "[TEST] Reading DevQualityStatus" << endl;
    st = irio_getDevQualityStatus(&drv, &quality, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] DevQualityStatus = " << quality << endl;
    EXPECT_EQ(st, IRIO_success);
    EXPECT_NE(quality, -1);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, GetIRIOVersion) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    startFPGA(&drv);

    std::array<char, 7> version;
    st = irio_getVersion(version.data(), &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] IRIO version = " << std::string(version.begin(), version.end()) << endl;
    EXPECT_EQ(st, IRIO_success);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, GetFPGAVIVersion) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    startFPGA(&drv);

    const int VIVersionLength = 4;
    std::array<char, VIVersionLength> version;
    uint64_t valueLength;
    st = irio_getFPGAVIVersion(&drv, version.data(), VIVersionLength, &valueLength, &status);
    logErrors(st, status);
    if (verbose_test) cout << "[TEST] VIVersionLength version = " << std::string(version.begin(), version.end()) << endl;
    EXPECT_EQ(st, IRIO_success);
    EXPECT_GT(valueLength, 0);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, GetDevProfile) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    startFPGA(&drv);

	if (verbose_test) cout << "[TEST] Reading device profile" << endl;
	int32_t reading = -1;
	st = irio_getDevProfile(&drv, &reading, &status);
	logErrors(st, status);
	if (verbose_test) cout << "[TEST] Device profile = " << reading << endl;
	EXPECT_EQ(st, IRIO_success);
    EXPECT_NE(reading, -1);

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}
TEST(Common, GetSetDebugMode) {
    irioDrv_t drv;
	TStatus status;
	irio_initStatus(&status);
    int verbose_test = getVerboseEnv();

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
	startFPGA(&drv);

	// Setting debug mode
	setDebugMode(&drv, 0);

	// Reading debug mode 
	int mode_read = -1;
	st = irio_getDebugMode(&drv, &mode_read, &status);
	if (verbose_test) cout << "[TEST] Debug mode read = " << mode_read << endl;
	logErrors(st, status);
	EXPECT_EQ(st, IRIO_success);
	EXPECT_EQ(mode_read, 0);
	irio_resetStatus(&status);

	st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Error closing driver";
}