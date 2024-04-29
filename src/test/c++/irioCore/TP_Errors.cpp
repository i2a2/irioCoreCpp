
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

// irioDriver.c tests
TEST(IRIOError, InitWithNullStatus) {
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    if (verbose_test) cout << "[TEST] Initializing driver with NULL status. Should fail to start" << endl;
    EXPECT_EQ(
        irio_initDriver("ErrorTestInitNull", "Serial", "Device", "projectName", "FPGAversion", 0, "headerDir", "bitfileDir", NULL, NULL), 
        IRIO_error
    );
}
TEST(IRIOError, InvalidDirs) {
    irioDrv_t drv;
    TStatus status;
    irio_initStatus(&status);
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    if (verbose_test) cout << "[TEST] Initializing driver with invalid directories. Should fail to start" << endl;
    EXPECT_EQ(
        irio_initDriver("ErrorInvalidDirs", "Serial", "Device", "projectName", "FPGAversion", 0, "invalid", "invalid", &drv, &status), 
        IRIO_error
    );
}
TEST(IRIOError, StartAlredyStarted) {
    irioDrv_t drv;
    TStatus status;   
    int verbose_test = std::stoi(TestUtilsIRIO::getEnvVar("VerboseTest"));

    int st = initDriver(IRIOProfile::NoModule, &drv);
	ASSERT_EQ(st, 0) << "[TEST] Error initializing driver";
    if (verbose_test) cout << "[TEST] Starting FPGA for the first time (should succeed)" << endl;
    startFPGA(&drv);

    if (verbose_test) cout << "[TEST] Starting FPGA for the second time (should fail)" << endl;
    int startStatus = irio_setFPGAStart(&drv,1,&status);
	TestUtilsIRIO::logErrors(startStatus, status);
	EXPECT_NE(startStatus, IRIO_success);
}
// irioError.c tests
TEST(IRIOError, GetErrorString) {
    TErrorDetailCode errorCode;
    char* errorString;

    errorCode = Generic_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Error generated outside irio. Check error messages for more detailed information", errorString);

    errorCode = HardwareNotFound_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Specified RIO device not found. Review specified serial number.", errorString);

    errorCode = BitfileDownload_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Error occurred downloading the bitfile. Check if bitfile was compiled for the specified target.", errorString);

    errorCode = InitDone_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Init done wait ended in timeout. Check if the connected adapter module is the intended and is properly connected. Check the initialization logic of the project.", errorString);

    errorCode = IOModule_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Connected IO module is not the expected IO Module. Review bitfile downloaded.", errorString);

    errorCode = NIRIO_API_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("NI-RIO C API returned error. Check returned error in error log.", errorString);

    errorCode = ListRIODevicesCommand_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("System call for listing connected RIO devices failed. Check if NI-RIO driver is installed and the executable is accessible.", errorString);

    errorCode = ListRIODevicesParsing_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Failed to parse system call for listing connected RIO. Check version of NI-RIO Driver installed.", errorString);

    errorCode = SignatureNotFound_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Can't find bitfile signature string in the given header file. Check NI FPGA C API Generator version used.", errorString);

    errorCode = ResourceNotFound_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("A mandatory resource wasn't found. Check error messages to find missing resources.", errorString);

    errorCode = SignatureValueNotValid_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Can't read value of bitfile signature in the given header file. Check NI FPGA C API Generator version used.", errorString);

    errorCode = ResourceValueNotValid_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Can't read address of a resource in the given header file. Check NI FPGA C API Generator version used.", errorString);

    errorCode = MemoryAllocation_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("IRIO tried to allocate memory unsuccessfully. Check possible memory leaks in user application.", errorString);

    errorCode = BitfileNotFound_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Bitfile file for specified project was not found. Check project name and search path.", errorString);

    errorCode = HeaderNotFound_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Header file for specified project was not found. Check project name and search path.", errorString);

    errorCode = FileAccess_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("A problem occur while reading a file. Check file permissions.", errorString);

    errorCode = FileNotFound_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to open a file and failed. Check file name and search path.", errorString);

    errorCode = FeatureNotImplemented_Error;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("The target device or device profile are not supported. Check values in the project with the design rules", errorString);

    errorCode = Success;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Device status is OK", errorString);

    errorCode = TemporaryFileDelete_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Could not delete temporary file. Check /tmp/ folder permissions or previous warnings/errors.", errorString);

    errorCode = ResourceNotFound_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("An optional resource (or part of it for complex resources) wasn't found. Check error messages to find missing resources.", errorString);

    errorCode = FPGAAlreadyRunning_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried change FPGAStart value after FPGA was started or FPGA was started before downloading the bitfile (previous session). If this is not the intended behaviour, reset the FPGA.", errorString);

    errorCode = Read_NIRIO_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to access an existing FPGA resource unsuccessfully. Check error messages to find NI-RIO driver error code.", errorString);

    errorCode = Read_Resource_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to access a non-existing FPGA resource. Check implemented resources in the project or enabling verbosity mode in irio_initDriver call.", errorString);

    errorCode = Write_NIRIO_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to write in an existing FPGA resource unsuccessfully. Check error messages to find NI-RIO driver error code.", errorString);

    errorCode = Write_Resource_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to write in a non-existing FPGA resource. Check implemented resources in the project or enabling verbosity mode in irio_initDriver call.", errorString);

    errorCode = ConfigDMA_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to configure/start/stop DMA FIFO unsuccessfully. Check error messages to find NI-RIO driver error code.", errorString);

    errorCode = ConfigUART_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to configure UART baud rate unsuccessfully. UART seems busy.", errorString);

    errorCode = ValueOOB_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Tried to write a value Out of Bounds. Check design rules for operational range.", errorString);

    errorCode = Generic_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Warning generated outside irio. Check warning messages for more detailed information", errorString);

    errorCode = DAQtimeout_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Timeout reached when waiting for data from data acquisition", errorString);

    errorCode = CLSLinetimeout_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Timeout reached when waiting for cameralink serial data", errorString);

    errorCode = ResourceRelease_Warning;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("NIRIO error while releasing Fifo elements", errorString);

    errorCode = (TErrorDetailCode)99;
    irio_getErrorString(errorCode, &errorString);
    EXPECT_STREQ("Error code not defined", errorString);
}