#include <stdio.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <algorithm>

#include "irioV2InstanceManager.h"

#include "errorsIrio.h"
#include "profiles/profileBase.h"
#include "terminals/terminalsDMADAQ.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioResourceFinder.h"

using irio::Irio;
using irio::PROFILE_ID;

using irio::errors::IrioV2Error;
using irio::errors::BFPParseBitfileError;
using irio::errors::ResourceNotFoundError;
using irio::errors::FPGAVIVersionMismatchError;
using irio::errors::UnsupportedDevProfileError;
using irio::errors::UnsupportedPlatformError;
using irio::errors::NiFpgaError;
using irio::errors::NiFpgaErrorDownloadingBitfile;
using irio::errors::NiFpgaFPGAAlreadyRunning;
using irio::errors::RIODeviceNotFoundError;
using irio::errors::TerminalNotImplementedError;
using irio::errors::UnsupportedAICouplingForModule;
using irio::errors::InitializationTimeoutError;
using irio::errors::ModulesNotOKError;
using irio::errors::UnsupportedPlatformError;
using irio::errors::UnsupportedDevProfileError;

struct DMATtoHostStruct {
	std::unique_ptr<std::uint16_t> nch;
	std::unique_ptr<std::uint8_t> frameType;
	std::unique_ptr<std::uint8_t> sampleSize;
	std::unique_ptr<std::uint16_t> blockNWords;
};
std::unordered_map<irioDrv_t*, DMATtoHostStruct> map_DMA;

std::unordered_map<irioDrv_t*, std::unique_ptr<std::uint32_t>> map_sgfref;

////////////////////////////////////////////////////
/// Local functions
////////////////////////////////////////////////////

void initDrvPvt(irioDrv_t *p_DrvPvt, const char *DeviceSerialNumber,
				const char *RIODeviceModel, const char *FPGAversion,
				int verbosity) {
	// TODO: Fix this
	memset(p_DrvPvt, 0x00, sizeof(irioDrv_t));

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

void fillModules(const irio::Platform &platform, irioDrv_t *p_DrvPvt,
				 const irio::Irio *iriov2) {
	std::vector<std::uint16_t> auxModules;
	switch (platform.platformID) {
	case irio::PLATFORM_ID::FlexRIO:
		p_DrvPvt->moduleFlexRIO =
			iriov2->getTerminalsFlexRIO().getInsertedIOModuleID();
		p_DrvPvt->numModulescRIO = 0;
		break;
	case irio::PLATFORM_ID::cRIO:
		auxModules = iriov2->getTerminalsCRIO().getInsertedIOModulesID();
		p_DrvPvt->numModulescRIO =
			std::min<std::uint16_t>(CRIO_MAX_MODULES, auxModules.size());
		std::copy_n(auxModules.begin(), p_DrvPvt->numModulescRIO,
					p_DrvPvt->modulescRIO);
		break;
	default:
		p_DrvPvt->numModulescRIO = 0;
		break;
	}
}

void fillPlatformData(const Irio *iriov2, irioDrv_t *p_DrvPvt) {
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

	fillModules(platform, p_DrvPvt, iriov2);

	// TODO: GPU
}

void fillCVData(const Irio *iriov2, irioDrv_t *p_DrvPvt) {
	const auto analog = iriov2->getTerminalsAnalog();
	p_DrvPvt->CVADC = analog.getCVADC();
	p_DrvPvt->CVDAC = analog.getCVDAC();
	p_DrvPvt->maxAnalogOut = analog.getMaxValAO();
	p_DrvPvt->minAnalogOut = analog.getMinValAO();
}

void fillIMAQData(const Irio *iriov2, irioDrv_t *p_DrvPvt) {
	// TODO
}

void fillSGFref(const Irio *iriov2, irioDrv_t *p_DrvPvt) {
	p_DrvPvt->NoOfSG = iriov2->getTerminalsSignalGeneration().getSGNo();
	const auto it =
		map_sgfref.emplace(p_DrvPvt, new std::uint32_t[p_DrvPvt->NoOfSG]);
	const auto auxSGFrefs =
		iriov2->getTerminalsSignalGeneration().getVectorSGFrefs();
	std::copy_n(auxSGFrefs.begin(), p_DrvPvt->NoOfSG, it.first->second.get());
	p_DrvPvt->SGfref = it.first->second.get();
}

void fillDMATtoHOST(const Irio *irio, irioDrv_t *p_DrvPvt) {
	const auto it = map_DMA.emplace(p_DrvPvt, DMATtoHostStruct());
	const auto maxDMA = irio->getPlatform().maxDMA;
	it.first->second.nch.reset(new std::uint16_t[maxDMA]);
	it.first->second.frameType.reset(new std::uint8_t[maxDMA]);
	it.first->second.sampleSize.reset(new std::uint8_t[maxDMA]);
	it.first->second.blockNWords.reset(new std::uint16_t[maxDMA]);

	for(size_t i = 0; i < maxDMA; ++i) {
		try {
			it.first->second.nch.get()[i] = irio->getTerminalsDAQ().getNCh(i);
			it.first->second.frameType.get()[i] = static_cast<std::uint8_t>(
				irio->getTerminalsDAQ().getFrameType(i));
			it.first->second.sampleSize.get()[i] =
				irio->getTerminalsDAQ().getSampleSize(i);
			it.first->second.blockNWords.get()[i] =
				irio->getTerminalsDAQ().getLengthBlock(i);
		} catch (ResourceNotFoundError&) {
			it.first->second.nch.get()[i] = 0;
			it.first->second.frameType.get()[i] = 0;
			it.first->second.sampleSize.get()[i] = 0;
			it.first->second.blockNWords.get()[i] = 0;
		}
	}
}

void fillDrvPvtData(const Irio *iriov2, irioDrv_t *p_DrvPvt) {
	fillPlatformData(iriov2, p_DrvPvt);

	p_DrvPvt->Fref = iriov2->getFref();

	const auto profile = iriov2->getProfileID();
	p_DrvPvt->devProfile = static_cast<std::uint8_t>(profile);

	p_DrvPvt->minSamplingRate = iriov2->getMinSamplingRate();
	p_DrvPvt->maxSamplingRate = iriov2->getMaxSamplingRate();

	fillSGFref(iriov2, p_DrvPvt);

	switch (profile) {
	case PROFILE_ID::FLEXRIO_CPUDAQ:
	case PROFILE_ID::FLEXRIO_GPUDAQ:
	case PROFILE_ID::CRIO_DAQ:
	case PROFILE_ID::R_DAQ:
		fillCVData(iriov2, p_DrvPvt);
		fillDMATtoHOST(iriov2, p_DrvPvt);
		break;
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
	initDrvPvt(p_DrvPvt, DeviceSerialNumber, RIODeviceModel, FPGAversion,
			   verbosity);

	std::string bitfilePath = std::string(bitfileDir) + "/" + STRINGNAME_PREFIX
	+ std::string(projectName) + STRINGNAME_BITFILEEXT;

	status->code = IRIO_success;
	status->detailCode = Success;
	try {
		const auto pairAux = IrioV2InstanceManager::createInstance(
			bitfilePath, p_DrvPvt->DeviceSerialNumber,
			p_DrvPvt->FPGAVIStringversion, verbosity);

		const auto irioV2ptr = pairAux.first;
		p_DrvPvt->session = pairAux.second;
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
	} catch (UnsupportedPlatformError &e) {
		irio_mergeStatus(status, ResourceValueNotValid_Error, p_DrvPvt->verbosity,
						"%s", e.what());
	} catch (UnsupportedDevProfileError &e) {
		irio_mergeStatus(status, ResourceValueNotValid_Error, p_DrvPvt->verbosity,
						"%s", e.what());
	} catch (IrioV2Error &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
	} catch (...) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity,
				"Unknown exception caught");
	}

	return status->code;
}

int irio_closeDriver(irioDrv_t *p_DrvPvt, uint32_t mode, TStatus *status) {
	try {
		const auto iriov2 = IrioV2InstanceManager::getInstance(
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session);

		iriov2->setCloseAttribute(mode);
		map_sgfref.erase(p_DrvPvt);
		IrioV2InstanceManager::destroyInstance(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session);
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
		irio::CouplingMode> couplingMap =
			{ { IRIO_coupling_NULL, irio::CouplingMode::None }, {
					IRIO_coupling_AC, irio::CouplingMode::AC }, {
					IRIO_coupling_DC, irio::CouplingMode::DC }, };
	const auto it = couplingMap.find(value);

	if (it == couplingMap.end()) {
		irio_mergeStatus(status, ValueOOB_Warning,
				p_DrvPvt->verbosity,
				"Incorrect value for configuring AC/DC coupling mode");
		return IRIO_warning;
	}

	try {
		const auto iriov2 = IrioV2InstanceManager::getInstance(
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session);

		iriov2->getTerminalsAnalog().setAICouplingMode(it->second);
		fillCVData(iriov2, p_DrvPvt);
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
	const std::unordered_map<irio::CouplingMode,
							TIRIOCouplingMode> conversionTable = {
			{irio::CouplingMode::AC, TIRIOCouplingMode::IRIO_coupling_AC},
			{irio::CouplingMode::DC, TIRIOCouplingMode::IRIO_coupling_DC},
			{irio::CouplingMode::None, TIRIOCouplingMode::IRIO_coupling_NULL},
	};
	const TIRIOCouplingMode valueNotFound = static_cast<TIRIOCouplingMode>(9);

	try {
		const auto iriov2 = IrioV2InstanceManager::getInstance(
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session);

		const auto auxCoup = iriov2->getTerminalsAnalog().getAICouplingMode();
		const auto it = conversionTable.find(auxCoup);
		if(it == conversionTable.end()) {
			*value = valueNotFound;
		} else {
			*value = it->second;
		}
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, p_DrvPvt->verbosity, "%s",
				e.what());
		return IRIO_error;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, ResourceNotFound_Error, p_DrvPvt->verbosity,
				"%s", e.what());
		return IRIO_error;
	}

	return IRIO_success;
}

int irio_getCVADC(irioDrv_t *p_DrvPvt, double *cvadc, TStatus *status) {
	*cvadc = p_DrvPvt->CVADC;

	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getCVDAC(irioDrv_t *p_DrvPvt, double *cvdac, TStatus *status) {
	*cvdac = p_DrvPvt->CVDAC;

	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getMaxAOValue(irioDrv_t *p_DrvPvt, float *maxAOVal, TStatus *status) {
	*maxAOVal = p_DrvPvt->maxAnalogOut;

	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getMinAOValue(irioDrv_t *p_DrvPvt, float *minAOVal, TStatus *status) {
	*minAOVal = p_DrvPvt->minAnalogOut;

	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getVersion(char *version, TStatus*) {
	snprintf(version, sizeof(IRIOVERSION), "%s", IRIOVERSION);
	return IRIO_success;
}

int irio_setFPGAStart(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	try {
		const auto irio = IrioV2InstanceManager::getInstance(
			p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session);

		if (p_DrvPvt->fpgaStarted) {
			irio_mergeStatus(status, FPGAAlreadyRunning_Warning,
							 p_DrvPvt->verbosity,
							 "FPGA status can not be changed after started");
			return IRIO_warning;
		}

		// Why even have the parameter then???
		if (value) {
			irio->startFPGA();
			p_DrvPvt->fpgaStarted = 1;
			p_DrvPvt->initDone = 1;
		}
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

	return IRIO_success;
}

int irio_getFPGAStart(irioDrv_t *p_DrvPvt, int32_t *value, TStatus*) {
	*value = p_DrvPvt->fpgaStarted;
	return IRIO_success;
}

int irio_getFPGAVIVersion(irioDrv_t *p_DrvPvt, char *value, size_t maxLength,
		size_t *valueLength, TStatus *status) {
	if (strlen(p_DrvPvt->FPGAVIStringversion) > maxLength) {
		*valueLength = maxLength;
		irio_mergeStatus(status, Read_Resource_Warning, p_DrvPvt->verbosity,
						 "FPGAVIVersion did not fit in the given pointer. Will "
						 "be truncated");
		return IRIO_warning;
	} else {
		*valueLength = strlen(p_DrvPvt->FPGAVIStringversion);
	}

	snprintf(value, *valueLength+1, "%s", p_DrvPvt->FPGAVIStringversion);

	return IRIO_success;
}

template<typename T>
int getCommon(int32_t *value, TStatus *status, const irioDrv_t *p_DrvPvt,
		T (irio::Irio::*funcGet)() const, const std::string &funcName) {
	try {
		const auto irio = IrioV2InstanceManager::getInstance(
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session);

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
		void (irio::Irio::*funcSet)(const T&) const,
		const std::string &funcName) {
	try {
		const auto irio = IrioV2InstanceManager::getInstance(
				p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session);

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
			&irio::Irio::getDevQualityStatus, "DevQualityStatus");
}

int irio_getDevTemp(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	return getCommon(value, status, p_DrvPvt, &irio::Irio::getDevTemp,
			"DevTemp");
}

int irio_getDevProfile(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	*value = p_DrvPvt->devProfile;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_setDebugMode(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	return setCommon(value, status, p_DrvPvt, &irio::Irio::setDebugMode,
			"DebugMode");
}

int irio_getDebugMode(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	return getCommon(value, status, p_DrvPvt, &irio::Irio::getDebugMode,
			"DebugMode");
}

int irio_setDAQStartStop(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	return setCommon(value, status, p_DrvPvt, &irio::Irio::setDAQStartStop,
			"DAQStartStop");
}

int irio_getDAQStartStop(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	return getCommon(value, status, p_DrvPvt, &irio::Irio::getDAQStartStop,
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

int irio_getPlatformType(irioDrv_t *p_DrvPvt, uint8_t *value, TStatus *status) {
	*value = p_DrvPvt->platform;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getInitDone(irioDrv_t *p_DrvPvt, uint8_t *value, TStatus *status) {
	*value = p_DrvPvt->initDone;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getFlexRIOModule(irioDrv_t *p_DrvPvt, uint32_t *value,
						  TStatus *status) {
	if (p_DrvPvt->platform ==
		static_cast<uint8_t>(irio::PLATFORM_ID::FlexRIO)) {
		*value = p_DrvPvt->moduleFlexRIO;
		status->code = IRIO_success;
		return IRIO_success;
	} else {
		irio_mergeStatus(status, Generic_Warning,
				p_DrvPvt->verbosity, "The board platform is not FlexRIO");
		return IRIO_warning;
	}
}

int irio_getcRIOModules(irioDrv_t *p_DrvPvt, uint16_t **modules,
						size_t *numModules, TStatus *status) {
	if (p_DrvPvt->platform ==
		static_cast<uint8_t>(irio::PLATFORM_ID::cRIO)) {
		*modules = p_DrvPvt->modulescRIO;
		*numModules = p_DrvPvt->numModulescRIO;
		status->code = IRIO_success;
		return IRIO_success;
	} else {
		*numModules = 0;
		irio_mergeStatus(status, Generic_Warning,
				p_DrvPvt->verbosity, "The board platform is not cRIO");
		return IRIO_warning;
	}
}

int irio_getMaxNumberOfDMAs(irioDrv_t *p_DrvPvt, uint16_t *maxNumDMAs,
							TStatus *status) {
	*maxNumDMAs = p_DrvPvt->max_dmas;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfGPUDMAs(irioDrv_t *p_DrvPvt, uint16_t *maxNumGPUDMAs,
							   TStatus *status) {
	*maxNumGPUDMAs = p_DrvPvt->max_dmas_gpu;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfAI(irioDrv_t *p_DrvPvt, uint16_t *maxAI,
						  TStatus *status) {
	*maxAI = p_DrvPvt->max_analoginputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfAO(irioDrv_t *p_DrvPvt, uint16_t *maxAO,
						  TStatus *status) {
	*maxAO = p_DrvPvt->max_analogoutputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfDI(irioDrv_t *p_DrvPvt, uint16_t *maxDI,
						  TStatus *status) {
	*maxDI = p_DrvPvt->max_digitalsinputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfDO(irioDrv_t *p_DrvPvt, uint16_t *maxDO,
						  TStatus *status) {
	*maxDO = p_DrvPvt->max_digitalsoutputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfAuxAI(irioDrv_t *p_DrvPvt, uint16_t *maxAuxAI,
							 TStatus *status) {
	*maxAuxAI = p_DrvPvt->max_auxanaloginputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfAuxAO(irioDrv_t *p_DrvPvt, uint16_t *maxAuxAO,
							 TStatus *status) {
	*maxAuxAO = p_DrvPvt->max_auxanalogoutputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfAuxDI(irioDrv_t *p_DrvPvt, uint16_t *maxAuxDI,
							 TStatus *status) {
	*maxAuxDI = p_DrvPvt->max_auxdigitalsinputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfAuxDO(irioDrv_t *p_DrvPvt, uint16_t *maxAuxDO,
							 TStatus *status) {
	*maxAuxDO = p_DrvPvt->max_auxdigitalsoutputs;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxNumberOfSG(irioDrv_t *p_DrvPvt, uint16_t *maxSG,
						  TStatus *status) {
	*maxSG = p_DrvPvt->max_numberofSG;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMaxSamplingRate(irioDrv_t *p_DrvPvt, float *maxSamplingRate,
							TStatus *status) {
	*maxSamplingRate = p_DrvPvt->maxSamplingRate;
	status->code = IRIO_success;

	return IRIO_success;
}

int irio_getMinSamplingRate(irioDrv_t *p_DrvPvt, float *minSamplingRate,
							TStatus *status) {
	*minSamplingRate = p_DrvPvt->minSamplingRate;
	status->code = IRIO_success;

	return IRIO_success;
}
