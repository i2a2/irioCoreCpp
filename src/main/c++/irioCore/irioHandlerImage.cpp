#include <cstring>
#include <algorithm>

#include "irioHandlerImage.h"
#include "irioInstanceManager.h"
#include "utils.h"
#include "imaqTypes.h"

using irio::CLSignalMapping;
using irio::CLMode;
using irio::UARTBaudRates;
using irio::errors::CLUARTInvalidBaudRate;

int irio_configCL(irioDrv_t *p_DrvPvt, int32_t fvalHigh, int32_t lvalHigh,
				  int32_t dvalHigh, int32_t spareHigh, int32_t controlEnable,
				  int32_t linescan, CL_SignalMapping_T signalMapping,
				  CL_Config_T configuration, TStatus *status) {
	static const std::unordered_map<CL_SignalMapping_T, CLSignalMapping>
		signalMappingConversion = {
            {CL_STANDARD, CLSignalMapping::STANDARD},
            {CL_BASLER_10T, CLSignalMapping::BASLER_10T},
            {CL_VOSSKUHLER_10T, CLSignalMapping::VOSSKUHLER_10T},
        };
    static const std::unordered_map<CL_Config_T, CLMode>
		modeConversion = {
            {CL_BASE, CLMode::BASE},
            {CL_MEDIUM, CLMode::MEDIUM},
            {CL_FULL, CLMode::FULL},
        };

    const auto itSigMap = signalMappingConversion.find(signalMapping);
	if (itSigMap == signalMappingConversion.end()) {
		irio_mergeStatus(status, ValueOOB_Warning, p_DrvPvt->verbosity,
						 "%d is not a supported value for signalMapping",
						 signalMapping);
		return IRIO_warning;
	}

	const auto itMode = modeConversion.find(configuration);
    if (itMode == modeConversion.end()) {
		irio_mergeStatus(status, ValueOOB_Warning, p_DrvPvt->verbosity,
						 "%d is not a supported value for configuration",
						 configuration);
		return IRIO_warning;
	}

	const auto f = [p_DrvPvt, fvalHigh, lvalHigh, dvalHigh, spareHigh,
					controlEnable, linescan, itSigMap, itMode] {
		getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
			.configCameraLink(fvalHigh, lvalHigh, dvalHigh, spareHigh,
							  controlEnable, linescan, itSigMap->second,
							  itMode->second);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_sendCLuart(irioDrv_t *p_DrvPvt, const char *msg, int msg_size,
					TStatus *status) {
	const auto f = [p_DrvPvt, msg, msg_size] {
		getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
			.sendUARTMsg(std::vector<std::uint8_t>(msg, msg+msg_size));
	};

	// sendUARTMsg could throw CLUARTTimeout, but not if the timeout is 0, which
	// in this case is
	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getCLuart(irioDrv_t *p_DrvPvt, int data_size, char *data,
				   int *msg_size, TStatus *status) {
	const auto f = [p_DrvPvt, data_size, data, msg_size] {
		auto msg =
			getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
				.recvUARTMsg(data_size);
		std::memcpy(data, msg.data(),
					std::min<size_t>(data_size, msg.size()));
        *msg_size = msg.size();
	};

	// recvUARTMsg could throw CLUARTTimeout, but not if the timeout is 0, which
	// in this case is
    return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getUARTBaudRate(irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status) {
	const auto f = [p_DrvPvt, value] {
		auto aux =
			getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
				.getUARTBaudRate();
		*value = static_cast<std::uint8_t>(aux);
	};

	try {
    	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
	} catch(CLUARTInvalidBaudRate &e) {
		irio_mergeStatus(status, ValueOOB_Warning, p_DrvPvt->verbosity, "%s",
						 e.what());
		return status->code;
	}
}

int irio_setUARTBaudRate(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status) {
	static const std::unordered_map<int32_t, UARTBaudRates>
		baudRatesConversion = {
			{0, UARTBaudRates::BR96},	{1, UARTBaudRates::BR192},
			{3, UARTBaudRates::BR384},	{4, UARTBaudRates::BR576},
			{5, UARTBaudRates::BR1152}, {6, UARTBaudRates::BR2304},
			{7, UARTBaudRates::BR4608}, {8, UARTBaudRates::BR9216},
		};
	const auto itBaudRate = baudRatesConversion.find(value);
	if (itBaudRate == baudRatesConversion.end()) {
		irio_mergeStatus(status, ValueOOB_Warning, p_DrvPvt->verbosity,
						 "%d is not a supported value for UART baud rate",
						 value);
		return IRIO_warning;
	}

	const auto f = [p_DrvPvt, itBaudRate] {
		getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
			.setUARTBaudRate(itBaudRate->second);
	};

	// setUARTBaudRate could throw CLUARTTimeout, but not if the timeout is 0,
	// which in this case is
	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getUARTBreakIndicator(irioDrv_t *p_DrvPvt, int32_t *value,
							   TStatus *status) {
    const auto f = [p_DrvPvt, value] {
		*value =
			getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
				.getUARTBreakIndicator();
    };

    return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getUARTFramingError(irioDrv_t *p_DrvPvt, int32_t *value,
							 TStatus *status) {
	const auto f = [p_DrvPvt, value] {
		*value =
			getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
				.getUARTFramingError();
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getUARTFrammingError(irioDrv_t *p_DrvPvt, int32_t *value,
							  TStatus *status) {
	return irio_getUARTFramingError(p_DrvPvt, value, status);
}

int irio_getUARTOverrunError(irioDrv_t *p_DrvPvt, int32_t *value,
							 TStatus *status) {
	const auto f = [p_DrvPvt, value] {
		*value =
			getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
				.getUARTOverrunError();
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}
