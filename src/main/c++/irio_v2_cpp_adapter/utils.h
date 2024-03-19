#pragma once
#include "irioDataTypes.h"
#include "irioError.h"
#include "irioV2InstanceManager.h"

using iriov2::errors::ResourceNotFoundError;
using iriov2::errors::NiFpgaError;
using iriov2::errors::TerminalNotImplementedError;

template<typename F>
int operationGeneric(F func, TStatus *status, bool verbosity) {
	try {
		func();
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, verbosity, e.what());
		return IRIO_error;
	} catch (ResourceNotFoundError &e) {
		irio_mergeStatus(status, Read_Resource_Warning, verbosity, e.what());
		return IRIO_warning;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, Read_Resource_Warning, verbosity, e.what());
		return IRIO_warning;
	} catch (NiFpgaError &e) {
		irio_mergeStatus(status, Read_NIRIO_Warning, verbosity, e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

template<typename F, typename T>
int getOperationGeneric(F func, T *value, TStatus *status,
		bool verbosity) {
	try {
		*value = func();
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, verbosity, e.what());
		return IRIO_error;
	} catch (ResourceNotFoundError &e) {
		irio_mergeStatus(status, Read_Resource_Warning, verbosity, e.what());
		return IRIO_warning;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, Read_Resource_Warning, verbosity, e.what());
		return IRIO_warning;
	} catch (NiFpgaError &e) {
		irio_mergeStatus(status, Read_NIRIO_Warning, verbosity, e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

template<typename F>
int setOperationGeneric(F func, TStatus *status, bool verbosity) {
	try {
		func();
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, verbosity, e.what());
		return IRIO_error;
	} catch (ResourceNotFoundError &e) {
		irio_mergeStatus(status, Write_Resource_Warning, verbosity, e.what());
		return IRIO_warning;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, Write_Resource_Warning, verbosity, e.what());
		return IRIO_warning;
	} catch (NiFpgaError &e) {
		irio_mergeStatus(status, Write_NIRIO_Warning, verbosity, e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}
