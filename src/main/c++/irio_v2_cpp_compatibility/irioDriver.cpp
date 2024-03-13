#include <stdio.h>
#include <cstring>
#include <iostream>

#include "irioV2InstanceManager.h"

#include "errorsIrio.h"
#include "profiles/profileBase.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioResourceFinder.h"

using iriov2::IrioV2;
using iriov2::PROFILE_ID;

using iriov2::errors::IrioV2Error;
using iriov2::errors::BFPParseBitfileError;
using iriov2::errors::ResourceNotFoundError;
using iriov2::errors::FPGAVIVersionMismatchError;
using iriov2::errors::UnsupportedDevProfileError;
using iriov2::errors::UnsupportedPlatformError;
using iriov2::errors::NiFpgaError;
using iriov2::errors::RIODeviceNotFoundError;


std::unique_ptr<char> appCallID_ptr;
std::unique_ptr<char> projectName_ptr;
std::unique_ptr<char> errorMsg_ptr;

////////////////////////////////////////////////////
/// Local functions
////////////////////////////////////////////////////

// Errors
void mergeStatus(TStatus *status, const TErrorDetailCode detailCode,
		const std::string &errorMsg, const bool verbose = false) {
	if(detailCode < Success) {
		status->code = IRIO_error;
	} else if (detailCode > Success) {
		status->code = IRIO_warning;
	} else {
		status->code = IRIO_success;
	}
	status->detailCode = detailCode;

	if(verbose) {
		std::cout << errorMsg << std::endl;
	}

	std::unique_ptr<char> aux;
	if(!errorMsg_ptr.get()) {
		size_t len = errorMsg.length() + 1;
		aux.reset(new char[len]);
		snprintf(aux.get(), len, "%s", errorMsg.c_str());
	} else {
		size_t len = strlen(errorMsg_ptr.get()) + 1 + errorMsg.length() + 1;
		aux.reset(new char[len]);
		snprintf(aux.get(), len, "%s\n%s", errorMsg_ptr.get(), errorMsg.c_str());
	}

	errorMsg_ptr.swap(aux);

	status->msg = errorMsg_ptr.get();
}

// Other

void initDrvPvt(irioDrv_t *p_DrvPvt, const char *appCallID,
		const char *DeviceSerialNumber, const char *RIODeviceModel,
		const char *projectName, const char *FPGAversion, int verbosity) {
	memset(p_DrvPvt, 0x00, sizeof(irioDrv_t));
	// Driver struct initialization
	p_DrvPvt->headerFile = NULL;
	p_DrvPvt->fpgaStarted = 0;
	memset(p_DrvPvt->FPGAVIversion, 0, SHORT_CHAR_STRING);

	// EPICS portName
	appCallID_ptr.reset(new char[strlen(appCallID)+1]);
	snprintf(appCallID_ptr.get(), strlen(appCallID)+1, "%s", appCallID);
	p_DrvPvt->appCallID = appCallID_ptr.get();

	// Project Name
	projectName_ptr.reset(new char[strlen(projectName)+1]);
	snprintf(projectName_ptr.get(), strlen(projectName)+1, "%s", projectName);
	p_DrvPvt->projectName = projectName_ptr.get();

	// Device Serial Number
	snprintf(p_DrvPvt->DeviceSerialNumber, DEVICESERIALNUMBERLENGTH, "%s",
			DeviceSerialNumber);

	// Device Model
	snprintf(p_DrvPvt->RIODeviceModel, RIODEVICEMODELLENGTH, "%s",
			RIODeviceModel);

	// FPGA VI Version
	snprintf(p_DrvPvt->FPGAVIStringversion, SHORT_CHAR_STRING, "%s",
			FPGAversion);

	// RIO Device.
	snprintf(p_DrvPvt->fpgaRIO, FPGARIOLENGTH, "RIO");

	// Verbosity
	p_DrvPvt->verbosity = verbosity;
}

void fillPlatformData(const IrioV2 *iriov2, irioDrv_t *p_DrvPvt) {
	const auto platform = iriov2->getPlatform();
	p_DrvPvt->platform = static_cast<std::uint8_t>(platform.platformID);

	p_DrvPvt->max_analoginputs = platform.maxAI;
	p_DrvPvt->max_analogoutputs = platform.maxAO;
	p_DrvPvt->max_auxanaloginputs = platform.maxAuxAI;
	p_DrvPvt->max_auxanalogoutputs = platform.maxAuxAO;
	p_DrvPvt->max_digitalsinputs = platform.maxDigital;
	p_DrvPvt->max_digitalsoutputs = platform.maxDigital;
	p_DrvPvt->max_auxdigitalsinputs = platform.maxAuxDigital;
	p_DrvPvt->max_auxdigitalsoutputs = platform.maxAuxDigital;

	p_DrvPvt->max_dmas = platform.maxDMA;

	p_DrvPvt->max_numberofSG = platform.maxSG;

	// TODO: GPU
}

void fillCVData(const IrioV2 *iriov2, irioDrv_t *p_DrvPvt) {
	const auto analog = iriov2->getTerminalsAnalog();
	p_DrvPvt->CVADC = analog.getCVADC();
	p_DrvPvt->CVDAC = analog.getCVDAC();
	p_DrvPvt->maxAnalogOut = analog.getMaxValAO();
	p_DrvPvt->minAnalogOut = analog.getMinValAO();
}

void fillIMAQData(const IrioV2 *iriov2, irioDrv_t *p_DrvPvt) {
	// TODO
}

void fillDrvPvtData(const IrioV2 *iriov2, irioDrv_t *p_DrvPvt) {
	fillPlatformData(iriov2, p_DrvPvt);

	p_DrvPvt->Fref = iriov2->getFref();
	strncpy(p_DrvPvt->FPGAVIversion,
			iriov2->getFPGAVIversion().c_str(), SHORT_CHAR_STRING);

	const auto profile = iriov2->getProfileID();
	p_DrvPvt->devProfile = static_cast<std::uint8_t>(profile);

	switch(profile) {
	case PROFILE_ID::FLEXRIO_CPUDAQ:
	case PROFILE_ID::FLEXRIO_GPUDAQ:
	case PROFILE_ID::CRIO_DAQ:
	case PROFILE_ID::R_DAQ:
	case PROFILE_ID::CRIO_IO:
		fillCVData(iriov2, p_DrvPvt);
		break;
	case PROFILE_ID::FLEXRIO_CPUIMAQ:
	case PROFILE_ID::FLEXRIO_GPUIMAQ:
		fillIMAQData(iriov2, p_DrvPvt);
		break;
	}
}

////////////////////////////////////////////////////
/// Library API
////////////////////////////////////////////////////

int irio_initDriver(const char *appCallID, const char *DeviceSerialNumber,
		const char *RIODeviceModel, const char *projectName,
		const char *FPGAversion, int verbosity, const char *headerDir,
		const char *bitfileDir, irioDrv_t *p_DrvPvt, TStatus *status) {
	if (!status) {
		return IRIO_error;
	}

	initDrvPvt(p_DrvPvt, appCallID, DeviceSerialNumber, RIODeviceModel,
			projectName, FPGAversion, verbosity);

	std::string bitfilePath = std::string(bitfileDir) + "/" + STRINGNAME_PREFIX
			+ p_DrvPvt->projectName + STRINGNAME_BITFILEEXT;

	status->detailCode = Success;
	try {
		const auto irioV2ptr = IrioV2InstanceManager::getInstance(bitfilePath,
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->FPGAVIStringversion);

		p_DrvPvt->session = 1;
		fillDrvPvtData(irioV2ptr, p_DrvPvt);
	} catch (BFPParseBitfileError &e) {
		mergeStatus(status, BitfileNotFound_Error, e.what(), p_DrvPvt->verbosity);
	} catch (ResourceNotFoundError &e) {
		mergeStatus(status, ResourceNotFound_Error, e.what(), p_DrvPvt->verbosity);
	} catch (FPGAVIVersionMismatchError &e) {
		mergeStatus(status, NIRIO_API_Error, e.what(), p_DrvPvt->verbosity);
	} catch (RIODeviceNotFoundError &e) {
		mergeStatus(status, HardwareNotFound_Error, e.what(), p_DrvPvt->verbosity);
	} catch (IrioV2Error &e) {
		mergeStatus(status, Generic_Error, e.what(), p_DrvPvt->verbosity);
	}

	return status->code;
}

int irio_closeDriver(irioDrv_t *p_DrvPvt, uint32_t mode, TStatus *status) {
	const auto iriov2 = IrioV2InstanceManager::getInstance();
	if(!iriov2) {
		mergeStatus(status, Generic_Error, "Driver not initialized",
				p_DrvPvt->verbosity);
		return IRIO_error;
	}

	iriov2->setCloseAttribute(mode);
	IrioV2InstanceManager::destroyInstance();

	// EPICS portName
	appCallID_ptr.reset(nullptr);
	p_DrvPvt->appCallID = nullptr;

	// Project Name
	projectName_ptr.reset(nullptr);
	p_DrvPvt->projectName = nullptr;

	return IRIO_success;
}


