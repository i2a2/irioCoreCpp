#include "irioUtils.h"

using irio::PROFILE_ID;

irio::TerminalsAnalog getTerminalsAnalog(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
			->getTerminalsAnalog();
}

irio::TerminalsAuxAnalog getTerminalsAuxAnalog(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
			->getTerminalsAuxAnalog();
}

irio::TerminalsDigital getTerminalsDigital(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
			->getTerminalsDigital();
}

irio::TerminalsAuxDigital getTerminalsAuxDigital(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
			->getTerminalsAuxDigital();
}

irio::TerminalsSignalGeneration getTerminalsSG(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
				->getTerminalsSignalGeneration();
}

irio::TerminalsDMACommon getTerminalsDMA(const irio::Irio *irio) {
	const auto profile = irio->getProfileID();
	if (profile == PROFILE_ID::FLEXRIO_CPUIMAQ ||
		profile == PROFILE_ID::FLEXRIO_GPUIMAQ) {
		return irio->getTerminalsIMAQ();
	} else {
		return irio->getTerminalsDAQ();
	}
}

irio::TerminalsDMADAQ getTerminalsDAQ(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
		->getTerminalsDAQ();
}

irio::TerminalsDMAIMAQ getTerminalsIMAQ(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioInstanceManager::getInstance(rioSerial, session)
		->getTerminalsIMAQ();
}

irio::TerminalsDMACommon getTerminalsDMA(const std::string &rioSerial,
		const std::uint32_t session) {
	auto irio = IrioInstanceManager::getInstance(rioSerial, session);
	return getTerminalsDMA(irio);
}

template<TErrorDetailCode R,
		 TErrorDetailCode T,
		 TErrorDetailCode N>
int operationGeneric(const std::function<void()> &func, TStatus *status,
		const bool verbosity) {
	try {
		func();
	} catch (IrioNotInitializedError &e) {
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
	status->code = IRIO_success;
	return IRIO_success;
}

template int operationGeneric
<Read_Resource_Warning, Read_Resource_Warning, Read_NIRIO_Warning>
(const std::function<void()> &func, TStatus *status, const bool verbosity);
template int operationGeneric
<Read_Resource_Warning, Read_Resource_Warning, ConfigDMA_Warning>
(const std::function<void()> &func, TStatus *status, const bool verbosity);

int getOperationGeneric(const std::function<void()> &func,
		TStatus *status, bool verbosity) {
	return operationGeneric<Read_Resource_Warning,
			Read_Resource_Warning,
			Read_NIRIO_Warning>(func, status, verbosity);
}


int setOperationGeneric(const std::function<void()> &func, TStatus *status,
		bool verbosity) {
	return operationGeneric<Write_Resource_Warning,
			Write_Resource_Warning,
			Write_NIRIO_Warning>(func, status, verbosity);
}

