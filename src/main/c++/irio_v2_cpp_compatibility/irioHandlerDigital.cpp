#include "irioHandlerDigital.h"
#include "irioV2InstanceManager.h"
#include "utils.h"

iriov2::TerminalsDigital getTerminalsDigital() {
	return IrioV2InstanceManager::getInstance()->getTerminalsDigital();
}

iriov2::TerminalsAuxDigital getTerminalsAuxDigital() {
	return IrioV2InstanceManager::getInstance()->getTerminalsAuxDigital();
}

int irio_getDI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsDigital().getDI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxDI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsAuxDigital().getAuxDI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getDO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsDigital().getDO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_setDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value] {
		return getTerminalsDigital().setDO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_getAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n] {
		return getTerminalsAuxDigital().getAuxDO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_setAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value] {
		return getTerminalsAuxDigital().setAuxDO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}
