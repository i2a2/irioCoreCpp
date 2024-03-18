#include "irioHandlerDigital.h"
#include "irioV2InstanceManager.h"
#include "utils.h"

iriov2::TerminalsDigital getTerminalsDigital(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioV2InstanceManager::getInstance(rioSerial, session)
			->getTerminalsDigital();
}

iriov2::TerminalsAuxDigital getTerminalsAuxDigital(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioV2InstanceManager::getInstance(rioSerial, session)
			->getTerminalsAuxDigital();
}

int irio_getDI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getDI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxDI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAuxDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxDI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getDO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getDO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_setDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setDO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_getAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAuxDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxDO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_setAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsAuxDigital(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setAuxDO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}
