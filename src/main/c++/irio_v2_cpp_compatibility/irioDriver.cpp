#include <stdio.h>
#include <cstring>
#include <iostream>
#include <functional>

#include "irioV2InstanceManager.h"

#include "errorsIrio.h"
#include "profiles/profileBase.h"
#include "terminals/terminalsDMADAQ.h"

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
using iriov2::errors::NiFpgaErrorDownloadingBitfile;
using iriov2::errors::NiFpgaFPGAAlreadyRunning;
using iriov2::errors::RIODeviceNotFoundError;
using iriov2::errors::TerminalNotImplementedError;
using iriov2::errors::UnsupportedAICouplingForModule;
using iriov2::errors::InitializationTimeoutError;
using iriov2::errors::ModulesNotOKError;

std::unique_ptr<char> appCallID_ptr;
std::unique_ptr<char> projectName_ptr;

////////////////////////////////////////////////////
/// Local functions
////////////////////////////////////////////////////

void initDrvPvt(irioDrv_t *p_DrvPvt, const char *appCallID,
		const char *DeviceSerialNumber, const char *RIODeviceModel,
		const char *projectName, const char *FPGAversion, int verbosity) {
	memset(p_DrvPvt, 0x00, sizeof(irioDrv_t));
	// Driver struct initialization
	p_DrvPvt->headerFile = NULL;
	p_DrvPvt->fpgaStarted = 0;
	memset(p_DrvPvt->FPGAVIversion, 0, SHORT_CHAR_STRING);

	// EPICS portName
	appCallID_ptr.reset(new char[strlen(appCallID) + 1]);
	snprintf(appCallID_ptr.get(), strlen(appCallID) + 1, "%s", appCallID);
	p_DrvPvt->appCallID = appCallID_ptr.get();

	// Project Name
	projectName_ptr.reset(new char[strlen(projectName) + 1]);
	snprintf(projectName_ptr.get(), strlen(projectName) + 1, "%s", projectName);
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

	p_DrvPvt->fpgaStarted = 0;
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
	strncpy(p_DrvPvt->FPGAVIversion, iriov2->getFPGAVIversion().c_str(),
		SHORT_CHAR_STRING);

	const auto profile = iriov2->getProfileID();
	p_DrvPvt->devProfile = static_cast<std::uint8_t>(profile);

	p_DrvPvt->minSamplingRate = iriov2->getMinSamplingRate();
	p_DrvPvt->maxSamplingRate = iriov2->getMaxSamplingRate();

	switch (profile) {
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
		const char *FPGAversion, int verbosity, const char*,
		const char *bitfileDir, irioDrv_t *p_DrvPvt, TStatus *status) {
	if (!status) {
		return IRIO_error;
	}

	irio_initStatus(status);

	initDrvPvt(p_DrvPvt, appCallID, DeviceSerialNumber, RIODeviceModel,
			projectName, FPGAversion, verbosity);

	std::string bitfilePath = std::string(bitfileDir) + "/" + STRINGNAME_PREFIX
	+ p_DrvPvt->projectName + STRINGNAME_BITFILEEXT;

	status->code = IRIO_success;
	status->detailCode = Success;
	try {
		const auto irioV2ptr = IrioV2InstanceManager::getInstance(bitfilePath,
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->FPGAVIStringversion);

		p_DrvPvt->session = 1;
		fillDrvPvtData(irioV2ptr, p_DrvPvt);
	} catch (BFPParseBitfileError &e) {
		irio_mergeStatus(status, BitfileNotFound_Error, p_DrvPvt->verbosity,
				"%s", e.what());
	} catch (NiFpgaErrorDownloadingBitfile &e) {
		irio_mergeStatus(status, BitfileDownload_Error, p_DrvPvt->verbosity,
				"%s", e.what());
	} catch (ResourceNotFoundError &e) {
		irio_mergeStatus(status, ResourceNotFound_Error, p_DrvPvt->verbosity,
				"%s", e.what());
	} catch (FPGAVIVersionMismatchError &e) {
		irio_mergeStatus(status, NIRIO_API_Error, p_DrvPvt->verbosity, "%s",
				e.what());
	} catch (RIODeviceNotFoundError &e) {
		irio_mergeStatus(status, HardwareNotFound_Error, p_DrvPvt->verbosity,
				"%s", e.what());
	} catch (IrioV2Error &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
	}

	return status->code;
}

int irio_closeDriver(irioDrv_t *p_DrvPvt, uint32_t mode, TStatus *status) {
	try {
		const auto iriov2 = IrioV2InstanceManager::getInstance();

		iriov2->setCloseAttribute(mode);
		IrioV2InstanceManager::destroyInstance();

		// EPICS portName
		appCallID_ptr.reset(nullptr);
		p_DrvPvt->appCallID = nullptr;

		// Project Name
		projectName_ptr.reset(nullptr);
		p_DrvPvt->projectName = nullptr;
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error,
			p_DrvPvt->verbosity, "%s", e.what());
		return IRIO_error;
	}
	irio_resetStatus(status);

	return IRIO_success;
}

int irio_setAICoupling(irioDrv_t *p_DrvPvt, TIRIOCouplingMode value,
		TStatus *status) {
	static const std::unordered_map<TIRIOCouplingMode,
		iriov2::CouplingMode> couplingMap =
			{ { IRIO_coupling_NULL, iriov2::CouplingMode::None }, {
					IRIO_coupling_AC, iriov2::CouplingMode::AC }, {
					IRIO_coupling_DC, iriov2::CouplingMode::DC }, };
	const auto it = couplingMap.find(value);

	if (it == couplingMap.end()) {
		irio_mergeStatus(status, ValueOOB_Warning,
				p_DrvPvt->verbosity,
				"Incorrect value for configuring AC/DC coupling mode");
		return IRIO_error;
	}

	try {
		const auto iriov2 = IrioV2InstanceManager::getInstance();

		iriov2->getTerminalsAnalog().setAICouplingMode(it->second);
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_error;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, ResourceNotFound_Error, p_DrvPvt->verbosity,
				"%s", e.what());
		return IRIO_error;
	} catch (UnsupportedAICouplingForModule &e) {
		irio_mergeStatus(status, ValueOOB_Warning, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

int irio_getAICoupling(irioDrv_t *p_DrvPvt, TIRIOCouplingMode *value,
		TStatus *status) {
	try {
		const auto iriov2 = IrioV2InstanceManager::getInstance();

		const auto auxCoup = iriov2->getTerminalsAnalog().getAICouplingMode();
		*value = static_cast<TIRIOCouplingMode>(auxCoup);
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_error;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, ResourceNotFound_Error, p_DrvPvt->verbosity,
				"%s", e.what());
		return IRIO_error;
	} catch (UnsupportedAICouplingForModule &e) {
		irio_mergeStatus(status, ValueOOB_Warning, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

int irio_getVersion(char *version, TStatus*) {
	snprintf(version, sizeof(IRIOVERSION), "%s", IRIOVERSION);
	return IRIO_success;
}

int irio_setFPGAStart(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	if (p_DrvPvt->fpgaStarted) {
		irio_mergeStatus(status, FPGAAlreadyRunning_Warning,
				p_DrvPvt->verbosity, "FPGA status can not be changed after started");
		return IRIO_warning;
	}

	// Why even have the parameter then???
	if (value) {
		try {
			const auto irio = IrioV2InstanceManager::getInstance();

			irio->startFPGA();
			p_DrvPvt->fpgaStarted = 1;
		} catch (IrioV2NotInitializedError &e) {
			irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
					e.what());
			return IRIO_error;
		} catch (NiFpgaFPGAAlreadyRunning &e) {
			irio_mergeStatus(status, FPGAAlreadyRunning_Warning,
					p_DrvPvt->verbosity, "%s", e.what());
			return IRIO_warning;
		} catch (NiFpgaError &e) {
			irio_mergeStatus(status, NIRIO_API_Error, p_DrvPvt->verbosity, "%s",
					e.what());
			return IRIO_error;
		} catch (InitializationTimeoutError &e) {
			irio_mergeStatus(status, InitDone_Error, p_DrvPvt->verbosity, "%s",
					e.what());
			return IRIO_error;
		} catch (ModulesNotOKError &e) {
			irio_mergeStatus(status, IOModule_Error, p_DrvPvt->verbosity, "%s",
					e.what());
			return IRIO_error;
		}
	}

	return IRIO_success;
}

int irio_getFPGAVIVersion(irioDrv_t *p_DrvPvt, char *value, size_t maxLength,
		size_t *valueLength, TStatus *status) {
	if (strlen(p_DrvPvt->FPGAVIStringversion) > maxLength) {
		*valueLength = maxLength;
		irio_mergeStatus(status, Read_Resource_Warning,
				p_DrvPvt->verbosity,
				"FPGAVIVersion did not fit in the given pointer. Will be truncated");
		return IRIO_warning;
	} else {
		*valueLength = strlen(p_DrvPvt->FPGAVIStringversion);
	}

	strncpy(value, p_DrvPvt->FPGAVIStringversion, *valueLength);

	return IRIO_success;
}

template<typename T>
int getCommon(int32_t *value, TStatus *status, const irioDrv_t *p_DrvPvt,
		T (iriov2::IrioV2::*funcGet)() const, const std::string &funcName) {
	try {
		const auto irio = IrioV2InstanceManager::getInstance();

		*value = (irio->*funcGet)();
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_error;
	} catch (NiFpgaError &e) {
		irio_mergeStatus(status, Read_NIRIO_Warning,
				p_DrvPvt->verbosity, "%s read error: ", funcName.c_str(), e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

template<typename T>
int setCommon(int32_t value, TStatus *status, const irioDrv_t *p_DrvPvt,
		void (iriov2::IrioV2::*funcSet)(const T&) const,
		const std::string &funcName) {
	try {
		const auto irio = IrioV2InstanceManager::getInstance();

		(irio->*funcSet)(value);
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_error;
	} catch (NiFpgaError &e) {
		irio_mergeStatus(status, Read_NIRIO_Warning,
				p_DrvPvt->verbosity, "%s set error: ", funcName.c_str(), e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

int irio_getDevQualityStatus(irioDrv_t *p_DrvPvt, int32_t *value,
		TStatus *status) {
	return getCommon(value, status, p_DrvPvt,
			&iriov2::IrioV2::getDevQualityStatus, "DevQualityStatus");
}

int irio_getDevTemp(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	return getCommon(value, status, p_DrvPvt, &iriov2::IrioV2::getDevTemp,
			"DevTemp");
}

int irio_getDevProfile(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	*value = p_DrvPvt->devProfile;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_setDebugMode(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	return setCommon(value, status, p_DrvPvt, &iriov2::IrioV2::setDebugMode,
			"DebugMode");
}

int irio_getDebugMode(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	return getCommon(value, status, p_DrvPvt, &iriov2::IrioV2::getDebugMode,
			"DebugMode");
}

int irio_setDAQStartStop(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	return setCommon(value, status, p_DrvPvt, &iriov2::IrioV2::setDebugMode,
			"DAQStartStop");
}

int irio_getDAQStartStop(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	return getCommon(value, status, p_DrvPvt, &iriov2::IrioV2::getDAQStartStop,
			"DAQStartStop");
}

int irio_setSamplingRate(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	// TODO: SamplingRate IO Profile
}

int irio_getSamplingRate(irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	// TODO: SamplingRate IO Profile
}
