#include "irioHandlerDMA.h"
#include "irioInstanceManager.h"
#include "irioError.h"
#include "utils.h"
#include "profilesTypes.h"

using irio::PROFILE_ID;

using irio::errors::ResourceNotFoundError;
using irio::errors::NiFpgaError;
using irio::errors::TerminalNotImplementedError;
using irio::errors::DMAReadTimeout;

int irio_setUpDMAsTtoHost(irioDrv_t *p_DrvPvt, TStatus *status) {
	const auto f = [p_DrvPvt] {
		return getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).startAllDMAs();
	};

	const auto ret = operationGeneric<
			Read_Resource_Warning,
			Read_Resource_Warning,
			ConfigDMA_Warning>(f, status, p_DrvPvt->verbosity);

	return ret;
}

int irio_closeDMAsTtoHost(irioDrv_t *p_DrvPvt, TStatus *status) {
	const auto f = [p_DrvPvt] {
		return getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).stopAllDMAs();
	};

	const auto ret = operationGeneric<
				Read_Resource_Warning,
				Read_Resource_Warning,
				ConfigDMA_Warning>(f, status, p_DrvPvt->verbosity);

	return ret;
}

int irio_cleanDMAsTtoHost(irioDrv_t *p_DrvPvt, TStatus *status) {
	const auto f = [p_DrvPvt] {
		return getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).cleanAllDMAs();
	};

	const auto ret = operationGeneric<
				Read_Resource_Warning,
				Read_Resource_Warning,
				ConfigDMA_Warning>(f, status, p_DrvPvt->verbosity);

	return ret;
}

int irio_cleanDMATtoHost(irioDrv_t *p_DrvPvt, int n, uint64_t*,
		size_t, TStatus *status) {
	const auto f = [n, p_DrvPvt] {
		return getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).cleanDMA(n);
	};

	const auto ret = operationGeneric<
				Read_Resource_Warning,
				Read_Resource_Warning,
				ConfigDMA_Warning>(f, status, p_DrvPvt->verbosity);

	return ret;
}

int irio_getDMATtoHostOverflow(irioDrv_t *p_DrvPvt, int32_t *value,
		TStatus *status) {
	const auto f = [value, p_DrvPvt] {
		*value = getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getAllDMAOverflows();
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATtoHostSamplingRate(irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsDAQ(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getSamplingRateDecimation(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setDMATtoHostSamplingRate(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsDAQ(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).setSamplingRateDecimation(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATtoHostEnable(irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		*value = getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).isDMAEnable(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_setDMATtoHostEnable(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status) {
	const auto f = [n, value, p_DrvPvt] {
		getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).enaDisDMA(n, value);
	};

	return setOperationGeneric(f, status, p_DrvPvt->verbosity);
}

size_t getElementsToRead(const irio::FrameType &frameType,
		const int NBlocks, const std::uint16_t lengthBlock) {
	size_t elementsToRead = 0;
	switch (frameType) {
	case irio::FrameType::FormatA:
		elementsToRead = NBlocks * lengthBlock;
		break;
	case irio::FrameType::FormatB:
		// each DMA data block includes two extra U64 words to include timestamp
		elementsToRead = NBlocks * (lengthBlock + 2);
		break;
	}
	return elementsToRead;
}

int irio_getDMATtoHOSTBlockNWords(irioDrv_t *p_DrvPvt, uint16_t *Nwords,
		TStatus *status) {
	const auto f = [Nwords, p_DrvPvt] {
		*Nwords = getTerminalsDAQ(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getLengthBlock(0);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATtoHOSTNCh(irioDrv_t *p_DrvPvt, uint16_t *NCh, TStatus *status) {
	const auto f = [NCh, p_DrvPvt] {
		*NCh = getTerminalsDMA(p_DrvPvt->DeviceSerialNumber,
					p_DrvPvt->session).getNCh(0);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATtoHostData(irioDrv_t *p_DrvPvt, int NBlocks, int n,
		uint64_t *data, int *elementsRead, TStatus *status) {
	const auto f = [n, NBlocks, data, elementsRead, p_DrvPvt] {
		const auto term = getTerminalsDAQ(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session);

		size_t lengthBlock = term.getLengthBlock(n);
		size_t elementsToRead = getElementsToRead(term.getFrameType(n), NBlocks,
						term.getLengthBlock(n));

		*elementsRead = term.readDataNonBlocking(n, elementsToRead, data)/lengthBlock;
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATtoHostData_timeout(irioDrv_t *p_DrvPvt, int NBlocks, int n,
		uint64_t *data, int *elementsRead, uint32_t timeout, TStatus *status) {
	const auto f = [n, NBlocks, data, timeout, elementsRead, p_DrvPvt] {
		const auto term = getTerminalsDAQ(p_DrvPvt->DeviceSerialNumber,
				p_DrvPvt->session);
		size_t lengthBlock = term.getLengthBlock(n);
		size_t elementsToRead = getElementsToRead(term.getFrameType(n), NBlocks,
						term.getLengthBlock(n));

		*elementsRead = term.readDataBlocking(n, elementsToRead, data, timeout)
				/ lengthBlock;
	};

	try {
		return getOperationGeneric(f, status, p_DrvPvt->verbosity);
	} catch (DMAReadTimeout &e) {
		irio_mergeStatus(status, Read_NIRIO_Warning, p_DrvPvt->verbosity,
				e.what());
		return IRIO_warning;
	}
}

int irio_getDMATtoHostImage(irioDrv_t *p_DrvPvt, int imageSize, int n,
		uint64_t *data, int *elementsRead, TStatus *status) {
	const auto f = [n, imageSize, data, elementsRead, p_DrvPvt] {
		*elementsRead =
			getTerminalsIMAQ(p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
				.readDataNonBlocking(n, imageSize, data);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATTtoHostFrameType(irioDrv_t *p_DrvPvt, int n,
								 uint8_t *frameType, TStatus *status) {
	const auto f = [n, frameType, p_DrvPvt] {
		*frameType = static_cast<std::uint8_t>(
			IrioInstanceManager::getInstance(p_DrvPvt->DeviceSerialNumber,
											   p_DrvPvt->session)
				->getTerminalsDAQ()
				.getFrameType(n));
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getDMATTtoHostSampleSize(irioDrv_t *p_DrvPvt, int n,
								 uint8_t *sampleSize, TStatus *status) {
	const auto f = [n, sampleSize, p_DrvPvt] {
		*sampleSize = IrioInstanceManager::getInstance(
						  p_DrvPvt->DeviceSerialNumber, p_DrvPvt->session)
						  ->getTerminalsDAQ()
						  .getSampleSize(n);
	};

	return getOperationGeneric(f, status, p_DrvPvt->verbosity);
}

int irio_getNumDMA(irioDrv_t *p_DrvPvt, size_t *value, TStatus *status) {
	*value = p_DrvPvt->numDMA;
	status->code = IRIO_success;
	return IRIO_success;
}
