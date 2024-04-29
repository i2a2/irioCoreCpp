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
 * TP-IRL-1000 Driver Initialization
 * 
 * When the test program is executed, the first thing to be checked is the Driver Initialization.
 * The version of the installed library will be tested also. irio_initDriver and irio_getVersion
 * functions are tested
 * 
 * Requisites:
 * - NI9264 Connected in Slot1
 * - NI9205 Connected in Slot2
 * - NI9401 Connected in Slot3
 * - NI9477 Connected in Slot4
 * - NI9426 Connected in Slot5
 * - NI9476 Connected in Slot6
 * - NI9425 Connected in Slot7
 * 
 * Implemented in:
 * - cRIO.IOResources
*/
TEST(cRIO, IOResources) {
    irioDrv_t drv;
    irioResources_t res;
	int st = 0;
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    st = initDriver(IRIOProfile::IO, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Driver initialization failed";
    getResources(&drv, &res);

    // Expected resources:
    // 3 AI
	if (verbose_test) cout << "[TEST] Found " << res.AI << " AIs. Expected 3" << endl;
	EXPECT_EQ(res.AI, 3);
    // 3 AO
	if (verbose_test) cout << "[TEST] Found " << res.AO << " AOs. Expected 3" << endl;
	EXPECT_EQ(res.AO, 3);
    // 3 DI
	if (verbose_test) cout << "[TEST] Found " << res.DI << " DIs. Expected 3" << endl;
	EXPECT_EQ(res.DI, 3);
    // 3 DO
	if (verbose_test) cout << "[TEST] Found " << res.DO << " DOs. Expected 3" << endl;
	EXPECT_EQ(res.DO, 3);
    // 2 auxAI
	if (verbose_test) cout << "[TEST] Found " << res.auxAI << " auxAIs. Expected 2" << endl;
	EXPECT_EQ(res.auxAI, 2);
    // 2 auxAO
	if (verbose_test) cout << "[TEST] Found " << res.auxAO << " auxAOs. Expected 2" << endl;
	EXPECT_EQ(res.auxAO, 2);
    // 2 auxDI
	if (verbose_test) cout << "[TEST] Found " << res.auxDI << " auxDIs. Expected 2" << endl;
	EXPECT_EQ(res.auxDI, 2);
    // 2 auxDO
	if (verbose_test) cout << "[TEST] Found " << res.auxDO << " auxDOs. Expected 2" << endl;
	EXPECT_EQ(res.auxDO, 2);
    // 0 SG
	if (verbose_test) cout << "[TEST] Found " << res.SG << " SGs. Expected 0" << endl;
	EXPECT_EQ(res.SG, 0);
	/*TODO: Add samplingRate from cRIO
    // 1 Sampling Rate
	if (verbose_test) cout << "[TEST] Found " << res.samplingRate << " sampling rates. Expected 1" << endl;
	EXPECT_EQ(res.samplingRate, 1);
	*/

    st = closeDriver(&drv);
	ASSERT_EQ(st, 0) << "[TEST] Driver closing failed";
}