#include "irioHandlerAnalog.h"
#include "irioV2InstanceManager.h"
#include "utils.h"

using iriov2::errors::ResourceNotFoundError;
using iriov2::errors::NiFpgaError;

iriov2::TerminalsAnalog getTerminalsAnalog() {
	return IrioV2InstanceManager::getInstance()->getTerminalsAnalog();
}

iriov2::TerminalsAuxAnalog getTerminalsAuxAnalog() {
	return IrioV2InstanceManager::getInstance()->getTerminalsAuxAnalog();
}

int irio_getAI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsAnalog().getAI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsAuxAnalog().getAuxAI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAI_64(irioDrv_t *p_DrvPvt, int n, int64_t *value,
		TStatus *status) {
	const auto f = [n] {
		return getTerminalsAuxAnalog().getAuxAI64(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsAnalog().getAO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsAuxAnalog().getAuxAO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAO_64(irioDrv_t *p_DrvPvt, int n, int64_t *value,
		TStatus *status) {
	const auto f = [n] {
		return getTerminalsAuxAnalog().getAuxAO64(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAOEnable(irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n] {
		return getTerminalsAnalog().getAOEnable(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_setAO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value] {
		return getTerminalsAnalog().setAO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_setAuxAO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value] {
		return getTerminalsAuxAnalog().setAuxAO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_setAuxAO_64(irioDrv_t *p_DrvPvt, int n, int64_t value,
		TStatus *status) {
	const auto f = [n, value] {
		return getTerminalsAuxAnalog().setAuxAO64(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_setAOEnable(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value] {
		return getTerminalsAnalog().setAOEnable(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

