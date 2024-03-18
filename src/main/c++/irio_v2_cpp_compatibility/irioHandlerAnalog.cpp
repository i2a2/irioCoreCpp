#include "irioHandlerAnalog.h"
#include "irioV2InstanceManager.h"
#include "utils.h"

using iriov2::errors::ResourceNotFoundError;
using iriov2::errors::NiFpgaError;

iriov2::TerminalsAnalog getTerminalsAnalog(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioV2InstanceManager::getInstance(rioSerial, session)
			->getTerminalsAnalog();
}

iriov2::TerminalsAuxAnalog getTerminalsAuxAnalog(const std::string &rioSerial,
		const std::uint32_t session) {
	return IrioV2InstanceManager::getInstance(rioSerial, session)
			->getTerminalsAuxAnalog();
}

int irio_getAI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAI(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAuxAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxAI(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAI_64(irioDrv_t *p_DrvPvt, int n, int64_t *value,
		TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAuxAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxAI64(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAO(irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAuxAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxAO(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAuxAO_64(irioDrv_t *p_DrvPvt, int n, int64_t *value,
		TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAuxAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAuxAO64(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_getAOEnable(irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).getAOEnable(n);
	};

	return getEnumTerminal(f, value, status, p_DrvPvt->verbosity);
}

int irio_setAO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setAO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_setAuxAO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsAuxAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setAuxAO(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_setAuxAO_64(irioDrv_t *p_DrvPvt, int n, int64_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsAuxAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setAuxAO64(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

int irio_setAOEnable(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		return getTerminalsAnalog(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session).setAOEnable(n, value);
	};

	return setEnumTerminal(f, status, p_DrvPvt->verbosity);
}

