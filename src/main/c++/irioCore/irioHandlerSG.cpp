#include "irioHandlerSG.h"
#include "irioInstanceManager.h"
#include "irioError.h"
#include "irioUtils.h"

int irio_getSGSignalType(const irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSGSignalType(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setSGSignalType(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).setSGSignalType(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGFreq(const irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSGFreq(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setSGFreq(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).setSGFreq(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGPhase(const irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSGPhase(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setSGPhase(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).setSGPhase(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGAmp(const irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSGAmp(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setSGAmp(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).setSGAmp(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGUpdateRate(const irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSGUpdateRate(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setSGUpdateRate(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).setSGUpdateRate(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGFref(const irioDrv_t *p_DrvPvt, int n, uint32_t *SGFref,
		TStatus *status) {
	const auto f = [n, SGFref, p_DrvPvt] {
		*SGFref = getTerminalsSG(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSGFref(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getFref(const irioDrv_t *p_DrvPvt, int32_t *Fref, TStatus *status) {
	const auto f = [Fref, p_DrvPvt] {
		*Fref = IrioInstanceManager::getInstance(
				p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session)->getFref();
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGCVDAC(const irioDrv_t *p_DrvPvt, double *SGCVDAC, TStatus *status) {
	const auto f = [SGCVDAC, p_DrvPvt] {
		*SGCVDAC = IrioInstanceManager::getInstance(
					p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session)
						->getTerminalsAnalog().getCVDAC();
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getSGCVADC(const irioDrv_t *p_DrvPvt, double *SGCVADC, TStatus *status) {
	const auto f = [SGCVADC, p_DrvPvt] {
		*SGCVADC = IrioInstanceManager::getInstance(
					p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session)
						->getTerminalsAnalog().getCVADC();
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getNumSG(const irioDrv_t *p_DrvPvt, size_t *value, TStatus *status) {
	*value = p_DrvPvt->numSG;
	status->code = IRIO_success;
	return IRIO_success;
}
