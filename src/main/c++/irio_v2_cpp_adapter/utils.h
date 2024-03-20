#pragma once
#include <functional>

#include "irioDataTypes.h"
#include "irioError.h"
#include "irioV2InstanceManager.h"

using iriov2::errors::ResourceNotFoundError;
using iriov2::errors::NiFpgaError;
using iriov2::errors::TerminalNotImplementedError;

template<TErrorDetailCode R,
		 TErrorDetailCode T,
		 TErrorDetailCode N>
int operationGeneric(const std::function<void()> &func, TStatus *status,
		const bool verbosity);

int getOperationGeneric(const std::function<void()> &func,
		TStatus *status, bool verbosity);

int setOperationGeneric(const std::function<void()> &func, TStatus *status,
		bool verbosity);
