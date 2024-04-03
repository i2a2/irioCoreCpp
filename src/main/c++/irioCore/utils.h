#pragma once
#include <functional>

#include "irioDataTypes.h"
#include "irioError.h"
#include "irioV2InstanceManager.h"

using irio::errors::ResourceNotFoundError;
using irio::errors::NiFpgaError;
using irio::errors::TerminalNotImplementedError;

template<TErrorDetailCode R,
		 TErrorDetailCode T,
		 TErrorDetailCode N>
int operationGeneric(const std::function<void()> &func, TStatus *status,
		const bool verbosity);

int getOperationGeneric(const std::function<void()> &func,
		TStatus *status, bool verbosity);

int setOperationGeneric(const std::function<void()> &func, TStatus *status,
		bool verbosity);
