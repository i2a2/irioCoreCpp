#include "irioError.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <iostream>
#include <unordered_map>

std::unique_ptr<char> errorMsg_ptr;

int irio_initStatus(TStatus *status) {
	return irio_resetStatus(status);
}

int irio_resetStatus(TStatus *status) {
	status->code = IRIO_success;
	status->detailCode = Success;
	errorMsg_ptr.reset(nullptr);
	status->msg = nullptr;
	return IRIO_success;
}

void mergeStatus(TStatus *status, const TErrorDetailCode detailCode,
		const std::string &errorMsg, const bool verbose = false) {
	std::string typeMsg;
	std::ostream* output;
	if (detailCode < Success) {
		status->code = IRIO_error;
		typeMsg = "[ERROR] ";
		output = &std::cerr;
	} else if (detailCode > Success) {
		status->code = IRIO_warning;
		typeMsg = "[WARN] ";
		output = &std::cerr;
	} else {
		status->code = IRIO_success;
		typeMsg = "[MSG] ";
		output = &std::cout;
	}
	status->detailCode = detailCode;

	if (verbose) {
		*output << typeMsg << errorMsg << std::endl;
	}

	std::unique_ptr<char> aux;
	if (!errorMsg_ptr.get()) {
		size_t len = errorMsg.length() + 1;
		aux.reset(new char[len]);
		snprintf(aux.get(), len, "%s", errorMsg.c_str());
	} else {
		size_t len = strlen(errorMsg_ptr.get()) + 1 + errorMsg.length() + 1;
		aux.reset(new char[len]);
		snprintf(aux.get(), len, "%s\n%s", errorMsg_ptr.get(),
				errorMsg.c_str());
	}

	errorMsg_ptr.swap(aux);

	status->msg = errorMsg_ptr.get();
}

int irio_mergeStatus(TStatus *status, TErrorDetailCode code, int printMsg,
		const char *format, ...) {
	va_list argptr;
	va_start(argptr, format);
	char *newMsg = NULL;
	if (vasprintf(&newMsg, format, argptr) <= 0) {
		printf("\n\nERROR in irio_mergeStatus\n\n");
		va_end(argptr);
		return -1;
	}
	va_end(argptr);

	mergeStatus(status, code, newMsg, printMsg);

	return 0;
}

std::unordered_map<TErrorDetailCode, std::string> errorMap = {
		{Generic_Error, "Error generated outside irio. "
						"Check error messages for more detailed information"},
		{HardwareNotFound_Error, 	"Specified RIO device not found. "
									"Review specified serial number."},
		{BitfileDownload_Error, "Error occurred downloading the bitfile. "
								"Check if bitfile was compiled for the specified target."},
		{InitDone_Error, 	"Init done wait ended in timeout. "
							"Check if the connected adapter module is the intended "
							"and is properly connected. "
							"Check the initialization logic of the project."},
		{IOModule_Error, 	"Connected IO module is not the expected IO Module. "
							"Review bitfile downloaded."},
		{NIRIO_API_Error, 	"NI-RIO C API returned error. "
							"Check returned error in error log."},
		{ListRIODevicesCommand_Error, "System call for listing connected RIO devices "
									  "failed. Check if NI-RIO driver is installed "
									  "and the executable is accessible."},
		{ListRIODevicesParsing_Error, 	"Failed to parse system call for listing "
										"connected RIO. Check version of "
										"NI-RIO Driver installed."},
		{SignatureNotFound_Error,	"Can't find bitfile signature string in the "
									"given header file. "
									"Check NI FPGA C API Generator version used."},
		{ResourceNotFound_Error,	"A mandatory resource wasn't found. "
									"Check error messages to find missing resources."},
		{SignatureValueNotValid_Error, 	"Can't read value of bitfile signature "
										"in the given header file. "
										"Check NI FPGA C API Generator version used."},
		{ResourceValueNotValid_Error, 	"Can't read address of a resource in the "
										"given header file. "
										"Check NI FPGA C API Generator version used."},
		{MemoryAllocation_Error, 	"IRIO tried to allocate memory unsuccessfully. "
									"Check possible memory leaks in user application."},
		{BitfileNotFound_Error, 	"Bitfile file for specified project was not found. "
									"Check project name and search path."},
		{HeaderNotFound_Error, 	"Header file for specified project was not found. "
								"Check project name and search path."},
		{FileAccess_Error, 	"A problem occur while reading a file. "
							"Check file permissions."},
		{FileNotFound_Error, 	"Tried to open a file and failed. "
								"Check file name and search path."},
		{FeatureNotImplemented_Error, 	"The target device or device profile are not "
										"supported. Check values in the project "
										"with the design rules"},
		{Success, "Device status is OK"},
		{TemporaryFileDelete_Warning, 	"Could not delete temporary file. "
										"Check /tmp/ folder permissions "
										"or previous warnings/errors."},
		{ResourceNotFound_Warning, 	"An optional resource (or part of it for complex "
									"resources) wasn't found. Check error messages to "
									"find missing resources."},
		{FPGAAlreadyRunning_Warning, 	"Tried change FPGAStart value after FPGA was "
										"started or FPGA was started before downloading "
										"the bitfile (previous session). "
										"If this is not the intended behaviour, reset the FPGA."},
		{Read_NIRIO_Warning, 	"Tried to access an existing FPGA resource "
								"unsuccessfully. Check error messages to "
								"find NI-RIO driver error code."},
		{Read_Resource_Warning, "Tried to access a non-existing FPGA resource. "
								"Check implemented resources in the project or "
								"enabling verbosity mode in irio_initDriver call."},
		{Write_NIRIO_Warning, 	"Tried to write in an existing FPGA resource "
								"unsuccessfully. "
								"Check error messages to find NI-RIO driver error code."},
		{Write_Resource_Warning, 	"Tried to write in a non-existing FPGA resource. "
									"Check implemented resources in the project or "
									"enabling verbosity mode in irio_initDriver call."},
		{ConfigDMA_Warning, 	"Tried to configure/start/stop DMA FIFO unsuccessfully. "
								"Check error messages to find NI-RIO driver error code."},
		{ConfigUART_Warning, 	"Tried to configure UART baud rate unsuccessfully. "
								"UART seems busy."},
		{ValueOOB_Warning, 	"Tried to write a value Out of Bounds. "
							"Check design rules for operational range."},
		{Generic_Warning, 	"Warning generated outside irio. "
							"Check warning messages for more detailed information"},
		{DAQtimeout_Warning, 	"Timeout reached when waiting for data "
								"from data acquisition"},
		{CLSLinetimeout_Warning, 	"Timeout reached when waiting "
									"for cameralink serial data"},
		{ResourceRelease_Warning, "NIRIO error while releasing Fifo elements"}
};


int irio_getErrorString(TErrorDetailCode error, char **str) {
	*str = nullptr;
	const auto it = errorMap.find(error);

	if(it != errorMap.end()) {
		asprintf(str, it->second.c_str());
	} else {
		asprintf(str, "Error code not defined");
	}
	return IRIO_success;
}
