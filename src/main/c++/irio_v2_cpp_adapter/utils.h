#pragma once
#include "irioDataTypes.h"
#include "irioError.h"
#include "irioV2InstanceManager.h"

using iriov2::errors::ResourceNotFoundError;
using iriov2::errors::NiFpgaError;
using iriov2::errors::TerminalNotImplementedError;

template<typename F,
		TErrorDetailCode R = Read_Resource_Warning,
		TErrorDetailCode T = Read_Resource_Warning,
		TErrorDetailCode N = Read_NIRIO_Warning>
int operationGeneric(const F func, TStatus *status, const bool verbosity) {
	try {
		func();
	} catch (IrioV2NotInitializedError &e) {
		irio_mergeStatus(status, Generic_Error, verbosity, e.what());
		return IRIO_error;
	} catch (ResourceNotFoundError &e) {
		irio_mergeStatus(status, R, verbosity, e.what());
		return IRIO_warning;
	} catch (TerminalNotImplementedError &e) {
		irio_mergeStatus(status, T, verbosity, e.what());
		return IRIO_warning;
	} catch (NiFpgaError &e) {
		irio_mergeStatus(status, N, verbosity, e.what());
		return IRIO_warning;
	}

	return IRIO_success;
}

template<typename F, typename T>
int getOperationGeneric(const F func, T *value, TStatus *status,
		bool verbosity) {
	const auto f = [&func, value] {
		*value = func();
	};

	return operationGeneric(f, status, verbosity);
}

template<typename F>
int setOperationGeneric(const F func, TStatus *status, bool verbosity) {
	return operationGeneric<const F, Write_Resource_Warning,
			Write_Resource_Warning,
			Write_NIRIO_Warning>(func, status, verbosity);
}
