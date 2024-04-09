#include <unistd.h>

#include "terminals/impl/terminalsDMAIMAQImpl.h"
#include "terminals/names/namesTerminalsDMAIMAQ.h"
#include "utils.h"
#include "errorsIrio.h"

namespace irio {

TerminalsDMAIMAQImpl::TerminalsDMAIMAQImpl(
	ParserManager* parserManager, const NiFpga_Session& session,
	const Platform& platform, const std::string& nameTermNCh,
	const std::string& nameTermFrameType, const std::string& nameTermSampleSize,
	const std::string& nameTermOverflows, const std::string& nameTermDMA,
	const std::string& nameTermDMAEnable) :
    TerminalsDMACommonImpl(parserManager, session, platform, nameTermNCh,
				nameTermFrameType, nameTermSampleSize, nameTermOverflows,
				nameTermDMA, nameTermDMAEnable) {
	findUART(parserManager);
    findCLConfig(parserManager);
}

void TerminalsDMAIMAQImpl::findCLConfig(ParserManager* parserManager) {
	parserManager->findRegisterAddress(
		TERMINAL_SIGNALMAPPING, GroupResource::IMAQ, &m_signalMapping_addr);
	parserManager->findRegisterAddress(
		TERMINAL_CONFIGURATION, GroupResource::IMAQ, &m_configuration_addr);
	parserManager->findRegisterAddress(TERMINAL_LINESCAN, GroupResource::IMAQ,
									   &m_lineScan_addr);
	parserManager->findRegisterAddress(TERMINAL_FVALHIGH, GroupResource::IMAQ,
									   &m_fvalHigh_addr);
	parserManager->findRegisterAddress(TERMINAL_LVALHIGH, GroupResource::IMAQ,
									   &m_lvalHigh_addr);
	parserManager->findRegisterAddress(TERMINAL_DVALHIGH, GroupResource::IMAQ,
									   &m_dvalHigh_addr);
	parserManager->findRegisterAddress(TERMINAL_SPAREHIGH, GroupResource::IMAQ,
									   &m_spareHigh_addr);
	parserManager->findRegisterAddress(
		TERMINAL_CONTROLENABLE, GroupResource::IMAQ, &m_controlEnable_addr);
}

void TerminalsDMAIMAQImpl::findUART(ParserManager* parserManager) {
	parserManager->findRegisterAddress(TERMINAL_UARTBAUDRATE,
									   GroupResource::IMAQ, &m_baudRate_addr);
	parserManager->findRegisterAddress(
		TERMINAL_UARTSETBAUDRATE, GroupResource::IMAQ, &m_setBaudRate_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTTXREADY,
									   GroupResource::IMAQ, &m_txReady_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTTXBYTE, GroupResource::IMAQ,
									   &m_txByte_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTTRANSMIT,
									   GroupResource::IMAQ, &m_transmit_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTRXREADY,
									   GroupResource::IMAQ, &m_rxReady_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTRXBYTE, GroupResource::IMAQ,
									   &m_rxByte_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTRECEIVE,
									   GroupResource::IMAQ, &m_receive_addr);
	parserManager->findRegisterAddress(TERMINAL_UARTBREAKINDICATOR,
									   GroupResource::IMAQ,
									   &m_breakIndicator_addr);
	parserManager->findRegisterAddress(
		TERMINAL_UARTFRAMMINGERROR, GroupResource::IMAQ, &m_framingError_addr);
	parserManager->findRegisterAddress(
		TERMINAL_UARTOVERRUNERROR, GroupResource::IMAQ, &m_overrunError_addr);
}

void TerminalsDMAIMAQImpl::configCameraLinkImpl(
	const std::int32_t fvalHigh, const std::int32_t lvalHigh,
	const std::int32_t dvalHigh, const std::int32_t spareHigh,
	const std::int32_t controlEnable, const std::int32_t linescan,
	const CLSignalMapping& signalMapping, const CLMode& mode) {
    NiFpga_Status status;
    status = NiFpga_WriteBool(m_session, m_fvalHigh_addr, fvalHigh);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_FVALHIGH));

    status = NiFpga_WriteBool(m_session, m_lvalHigh_addr, lvalHigh);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_LVALHIGH));

	status = NiFpga_WriteBool(m_session, m_dvalHigh_addr, dvalHigh);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_DVALHIGH));

	status = NiFpga_WriteBool(m_session, m_spareHigh_addr, spareHigh);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_SPAREHIGH));

	status = NiFpga_WriteBool(m_session, m_controlEnable_addr, controlEnable);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_CONTROLENABLE));

    std::uint8_t signalMappingAux = static_cast<std::uint8_t>(signalMapping);
    status = NiFpga_WriteU8(m_session, m_signalMapping_addr, signalMappingAux);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_SIGNALMAPPING));

	std::uint8_t modeAux = static_cast<std::uint8_t>(mode);
	status = NiFpga_WriteU8(m_session, m_configuration_addr, modeAux);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_CONFIGURATION));

	status = NiFpga_WriteBool(m_session, m_lineScan_addr, linescan);
	utils::throwIfNotSuccessNiFpga(
		status, "Error configuring " + std::string(TERMINAL_LINESCAN));
}

size_t TerminalsDMAIMAQImpl::readImageNonBlockingImpl(
	const std::uint32_t n, const size_t imagePixelSize,
	std::uint64_t* imageRead) const {
	return readImageImpl(n, imagePixelSize, imageRead, false);
}

size_t TerminalsDMAIMAQImpl::readImageBlockingImpl(
	const std::uint32_t n, const size_t imagePixelSize,
	std::uint64_t* imageRead, const std::uint32_t timeout) const {
	return readImageImpl(n, imagePixelSize, imageRead, true, timeout);
}

size_t TerminalsDMAIMAQImpl::readImageImpl(const std::uint32_t n,
									   const size_t imagePixelSize,
									   std::uint64_t* imageRead,
									   const bool blockRead,
									   const std::uint32_t timeout) const {
	const size_t elementsToRead = imagePixelSize * getSampleSizeImpl(n)/8;
    return readDataImpl(n, elementsToRead, imageRead, blockRead, timeout);
}

void TerminalsDMAIMAQImpl::sendUARTMsgImpl(
	const std::string& msg, const std::uint32_t timeout) const {
	NiFpga_Status status;
	for (const char& c : msg) {
        NiFpga_Bool txReady = 0;
        std::uint32_t countTimeout = 0;
        status = NiFpga_ReadBool(m_session, m_txReady_addr, &txReady);
        utils::throwIfNotSuccessNiFpga(
            status, "Error waiting for " + std::string(TERMINAL_UARTTXREADY));
        while (!txReady && (timeout == 0 || countTimeout < timeout)) {
            usleep(1000);
            countTimeout++;
            status = NiFpga_ReadBool(m_session, m_txReady_addr, &txReady);
			utils::throwIfNotSuccessNiFpga(
				status,
				"Error waiting for " + std::string(TERMINAL_UARTTXREADY));
		}

		if (timeout != 0 && countTimeout >= timeout) {
			throw errors::CLUARTTimeout();
		}

		status = NiFpga_WriteU8(m_session, m_txByte_addr, c);
		utils::throwIfNotSuccessNiFpga(status,
									   "Error writting CL UART message");

		status = NiFpga_WriteBool(m_session, m_transmit_addr, 1);
		utils::throwIfNotSuccessNiFpga(status,
									   "Error transmitting CL UART message");
	}
}

std::string TerminalsDMAIMAQImpl::recvUARTMsgImpl(
	const size_t bytesToRecv,
	const std::uint32_t timeout) const {
	NiFpga_Status status;
	std::string recvMsg;
	NiFpga_Bool rxReady = 0;
	std::uint32_t countTimeout = 0;

	recvMsg.reserve(bytesToRecv);

	status = NiFpga_ReadBool(m_session, m_rxReady_addr, &rxReady);
	utils::throwIfNotSuccessNiFpga(
		status, "Error waiting for " + std::string(TERMINAL_UARTRXREADY));

	size_t bytesRead = 0;
	while(bytesRead < bytesToRecv) {
		while (!rxReady && (timeout == 0 || countTimeout < timeout)) {
			usleep(1000);
			countTimeout++;
			status = NiFpga_ReadBool(m_session, m_rxReady_addr, &rxReady);
			utils::throwIfNotSuccessNiFpga(
				status,
				"Error waiting for " + std::string(TERMINAL_UARTRXREADY));
		}

		if (timeout != 0 && countTimeout >= timeout) {
			throw errors::CLUARTTimeout();
		}

		status = NiFpga_WriteBool(m_session, m_receive_addr, 1);
		utils::throwIfNotSuccessNiFpga(status,
									   "Error enabling receiving UART data");

		NiFpga_Bool isDataReady;
		do {
			status = NiFpga_ReadBool(m_session, m_receive_addr, &isDataReady);
		} while (!isDataReady);

		std::uint8_t charAux;
		status = NiFpga_ReadU8(m_session, m_rxByte_addr, &charAux);
		utils::throwIfNotSuccessNiFpga(status, "Error receiving UART data");
		recvMsg.push_back(charAux);
		bytesRead++;
	}

    return recvMsg;
}

void TerminalsDMAIMAQImpl::setUARTBaudRateImpl(
	const UARTBaudRates& baudRate, const std::uint32_t timeout) const {
	NiFpga_Status status;
	NiFpga_Bool setBR;
	std::uint32_t countTimeout;

	// Wait for SetBaudRate = false
	status = NiFpga_ReadBool(m_session, m_setBaudRate_addr, &setBR);
	utils::throwIfNotSuccessNiFpga(
		status, "Error reading " + std::string(TERMINAL_UARTSETBAUDRATE));
	while (setBR && (timeout == 0 || countTimeout < timeout)) {
		usleep(1000);
		countTimeout++;
		status = NiFpga_ReadBool(m_session, m_setBaudRate_addr, &setBR);
		utils::throwIfNotSuccessNiFpga(
			status, "Error reading " + std::string(TERMINAL_UARTSETBAUDRATE));
	}

	if (timeout != 0 && countTimeout >= timeout) {
		throw errors::CLUARTTimeout();
	}

	// Set Baud Rate
	status = NiFpga_WriteU8(m_session, m_baudRate_addr,
							static_cast<std::uint8_t>(baudRate));
	utils::throwIfNotSuccessNiFpga(status, "Error writing baud rate");
	status = NiFpga_WriteBool(m_session, m_setBaudRate_addr, 1);
	utils::throwIfNotSuccessNiFpga(status, "Error setting baud rate");

	// Wait for SetBaudRate = false to confirm is configured
	while (setBR && (timeout == 0 || countTimeout < timeout)) {
		usleep(1000);
		countTimeout++;
		status = NiFpga_ReadBool(m_session, m_setBaudRate_addr, &setBR);
		utils::throwIfNotSuccessNiFpga(
			status, "Error reading " + std::string(TERMINAL_UARTSETBAUDRATE));
	}

	if (timeout != 0 && countTimeout >= timeout) {
		throw errors::CLUARTTimeout();
	}
}

UARTBaudRates TerminalsDMAIMAQImpl::getUARTBaudRateImpl() const {
	const std::unordered_map<std::uint8_t, UARTBaudRates> conversionMap = {
		{0, UARTBaudRates::BR96},	{1, UARTBaudRates::BR192},
		{2, UARTBaudRates::BR384},	{3, UARTBaudRates::BR576},
		{4, UARTBaudRates::BR1152}, {5, UARTBaudRates::BR2304},
		{6, UARTBaudRates::BR4608}, {7, UARTBaudRates::BR9216},
	};

	std::uint8_t brAux;
	auto status = NiFpga_ReadU8(m_session, m_baudRate_addr, &brAux);
	utils::throwIfNotSuccessNiFpga(status, "Error getting baud rate");

	const auto it = conversionMap.find(brAux);
	if(it == conversionMap.end()) {
		throw errors::CLUARTInvalidBaudRate();
	}

	return it->second;
}

std::uint16_t TerminalsDMAIMAQImpl::getUARTBreakIndicatorImpl() const {
    std::uint16_t breakIndicator;
	const auto status =
		NiFpga_ReadU16(m_session, m_breakIndicator_addr, &breakIndicator);
	utils::throwIfNotSuccessNiFpga(
		status, "Error reading " + std::string(TERMINAL_UARTBREAKINDICATOR));
	return breakIndicator;
}

std::uint16_t TerminalsDMAIMAQImpl::getUARTFrammingErrorImpl() const {
	std::uint16_t frammingError;
	const auto status =
		NiFpga_ReadU16(m_session, m_framingError_addr, &frammingError);
	utils::throwIfNotSuccessNiFpga(
		status, "Error reading " + std::string(TERMINAL_UARTFRAMMINGERROR));
	return frammingError;
}

std::uint16_t TerminalsDMAIMAQImpl::getUARTOverrunErrorImpl() const {
    std::uint16_t overrunError;
	const auto status =
		NiFpga_ReadU16(m_session, m_framingError_addr, &overrunError);
	utils::throwIfNotSuccessNiFpga(
		status, "Error reading " + std::string(TERMINAL_UARTOVERRUNERROR));
	return overrunError;
}

}  // namespace irio
