#pragma once
#include <string>
#include <functional>

#include "irioDataTypes.h"
#include "irioError.h"
#include "irioInstanceManager.h"

using irio::errors::ResourceNotFoundError;
using irio::errors::NiFpgaError;
using irio::errors::TerminalNotImplementedError;

irio::TerminalsAnalog getTerminalsAnalog(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsAuxAnalog getTerminalsAuxAnalog(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsDigital getTerminalsDigital(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsAuxDigital getTerminalsAuxDigital(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsSignalGeneration getTerminalsSG(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsDMACommon getTerminalsDMA(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsDMACommon getTerminalsDMA(const irio::Irio *irio);

irio::TerminalsDMADAQ getTerminalsDAQ(const std::string &rioSerial,
		const std::uint32_t session);

irio::TerminalsDMAIMAQ getTerminalsIMAQ(const std::string &rioSerial,
		const std::uint32_t session);

template<TErrorDetailCode R,
		 TErrorDetailCode T,
		 TErrorDetailCode N>
int operationGeneric(const std::function<void()> &func, TStatus *status,
		const bool verbosity);

int getOperationGeneric(const std::function<void()> &func,
		TStatus *status, bool verbosity);

int setOperationGeneric(const std::function<void()> &func, TStatus *status,
		bool verbosity);
