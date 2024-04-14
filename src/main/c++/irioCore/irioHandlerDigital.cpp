#include "irioHandlerDigital.h"
#include "irioInstanceManager.h"
#include "utils.h"

int irio_getDI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] () {
		*value = getTerminalsDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getDI(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getAuxDI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsAuxDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxDI(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getDO(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setDO(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsAuxDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxDO(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsAuxDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setAuxDO(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getNumDI(irioDrv_t *p_DrvPvt, size_t *value, TStatus *status) {
	*value = p_DrvPvt->numDI;
	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getNumDO(irioDrv_t *p_DrvPvt, size_t *value, TStatus *status) {
	*value = p_DrvPvt->numDO;
	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getNumAuxDI(irioDrv_t *p_DrvPvt, size_t *value, TStatus *status) {
	*value = p_DrvPvt->numAuxDI;
	status->code = IRIO_success;
	return IRIO_success;
}

int irio_getNumAuxDO(irioDrv_t *p_DrvPvt, size_t *value, TStatus *status) {
	*value = p_DrvPvt->numAuxDO;
	status->code = IRIO_success;
	return IRIO_success;
}
