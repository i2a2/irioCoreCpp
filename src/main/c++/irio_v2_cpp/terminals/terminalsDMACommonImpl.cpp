#include <terminals/terminalsDMACommonImpl.h>
#include <errorsIrio.h>
#include <utils.h>
#include <memory>


namespace iriov2{

TerminalsDMACommonImpl::TerminalsDMACommonImpl(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const std::string &nameTermNCh,
			const std::string &nameTermFrameType,
			const std::string &nameTermSampleSize,
			const std::string &nameTermOverflows,
			const std::string &nameTermDMA,
			const std::string &nameTermDMAEnable):
				TerminalsBase(session),
				m_nameTermOverflows(nameTermOverflows),
				m_nameTermDMA(nameTermDMA),
				m_nameTermDMAEnable(nameTermDMAEnable){

	//Find Overflows (it is one uint16 where each bit is the status)
	m_overflowsAddr = parsedBitfile.getRegister(nameTermOverflows).address;

	//Find NCh. Get initial values and store them, no need to read them each time.
	utils::findArrayRegReadToVector<std::uint16_t>(parsedBitfile, m_session, nameTermNCh, m_nCh,
			&NiFpga_ReadArrayU16);

	//Find FrameType. Get initial values and store them, no need to read them each time.
	std::vector<std::uint8_t> auxVecFrameType;
	utils::findArrayRegReadToVector<std::uint8_t>(parsedBitfile, m_session, nameTermFrameType,
			auxVecFrameType, &NiFpga_ReadArrayU8);


	m_frameType.reserve(auxVecFrameType.size());
	for (uint8_t value : auxVecFrameType) {
		m_frameType.push_back(static_cast<FrameType>(value));
	}

	//Find SampleSize. Get initial values and store them, no need to read them each time.
	utils::findArrayRegReadToVector<std::uint8_t>(parsedBitfile, m_session, nameTermSampleSize,
			m_sampleSize, &NiFpga_ReadArrayU8);

	//Find DMAs
	utils::findAndInsertEnumDMAs(parsedBitfile, nameTermDMA, platform.maxDMA, m_mapDMA);

	//Find DMAEnable. Get initial values and store them, no need to read them each time.
	utils::findAndInsertEnumRegisters(parsedBitfile, nameTermDMAEnable, platform.maxDMA, m_mapEnable);

	if(m_mapDMA.size() != m_mapEnable.size()){
		throw errors::ResourceNotFoundError(
				"Mismatch in number of " + nameTermDMA + " and " + nameTermDMAEnable
						+ " terminals");
	}
}

std::uint16_t TerminalsDMACommonImpl::getNCh(const std::uint32_t n) const {
	if(n >= m_nCh.size()){
		const std::string err = std::to_string(n) + " is not a valid DMA ID";
		throw errors::ResourceNotFoundError(err);
	}

	return m_nCh.at(n);
}

void TerminalsDMACommonImpl::startDMAImpl(
		const std::uint32_t &dma) const{
	//TODO: For the moment, do the same as in the old lib, configure the DMA to a lot of space
	auto status = NiFpga_ConfigureFifo(m_session, dma, SIZE_HOST_DMAS);
	utils::throwIfNotSuccessNiFpga(status,
			"Error configuring " + m_nameTermDMA + std::to_string(dma));
	status = NiFpga_StartFifo(m_session, dma);
	utils::throwIfNotSuccessNiFpga(status,
			"Error starting " + m_nameTermDMA + std::to_string(dma));
}

void TerminalsDMACommonImpl::startDMA(const std::uint32_t n) const {
	const auto it = m_mapDMA.find(n);
	if(it == m_mapDMA.end()){
		const std::string err = std::to_string(n) + " is not a valid DMA";
		throw errors::ResourceNotFoundError(err);
	}

	startDMAImpl(it->second);

	cleanDMA(n);
}

void TerminalsDMACommonImpl::startAllDMAs() const {
	for(const auto& values:m_mapDMA){
		startDMAImpl(values.second);
	}

	cleanAllDMAs();
}

void TerminalsDMACommonImpl::stopDMA(const std::uint32_t n) const {
	const auto it = m_mapDMA.find(n);
	if(it == m_mapDMA.end()){
		const std::string err = std::to_string(n) + " is not a valid DMA";
		throw errors::ResourceNotFoundError(err);
	}

	const auto status = NiFpga_StopFifo(m_session, it->second);
	utils::throwIfNotSuccessNiFpga(status, "Error stopping " + m_nameTermDMA + std::to_string(n));
}

void TerminalsDMACommonImpl::stopAllDMAs() const {
	for(const auto& values:m_mapDMA){
		const auto status = NiFpga_StopFifo(m_session, values.second);
		utils::throwIfNotSuccessNiFpga(status, "Error stopping " + m_nameTermDMA + std::to_string(values.first));
	}
}

size_t TerminalsDMACommonImpl::countDMAs() const {
	return m_mapDMA.size();
}

void TerminalsDMACommonImpl::cleanDMAImpl(
		const std::uint32_t &dma) const {
	NiFpga_Status status;
	size_t elementsRemaining;
	std::uint64_t aux;
	status = NiFpga_ReadFifoU64(m_session, dma, &aux, 0, 0, &elementsRemaining);
	utils::throwIfNotSuccessNiFpga(status, "Error reading " + m_nameTermDMA + std::to_string(dma));

	//TODO: Find better way?
	static const size_t sizeCleanBuffer = SIZE_HOST_DMAS;
	std::unique_ptr<std::uint64_t> buffer(new std::uint64_t[sizeCleanBuffer]);

	size_t elementsToRead;
	while(elementsRemaining > 0){
		elementsToRead = elementsRemaining > sizeCleanBuffer? sizeCleanBuffer:elementsRemaining;

		status = NiFpga_ReadFifoU64(m_session, dma, buffer.get(), elementsToRead, 1, &elementsRemaining);
		utils::throwIfNotSuccessNiFpga(status, "Error reading " + m_nameTermDMA + std::to_string(dma));
	}
}

void TerminalsDMACommonImpl::cleanDMA(const std::uint32_t n) const {
	const auto dmaNum = utils::getAddressEnumResource(m_mapDMA, n, m_nameTermDMA);

	cleanDMAImpl(dmaNum);
}

void TerminalsDMACommonImpl::cleanAllDMAs() const {
	for(const auto& values:m_mapDMA){
		cleanDMA(values.second);
	}
}

bool TerminalsDMACommonImpl::isDMAEnable(const std::uint32_t n) const {
	const auto addr = utils::getAddressEnumResource(m_mapEnable, n, m_nameTermDMAEnable);

	NiFpga_Bool val;
	const auto status = NiFpga_ReadBool(m_session, addr, &val);
	utils::throwIfNotSuccessNiFpga(status,
			"Error reading " + m_nameTermDMAEnable + std::to_string(n));

	return static_cast<bool>(val);
}

void TerminalsDMACommonImpl::enableDMA(const std::uint32_t n) const {
	enaDisDMA(n, true);
}

void TerminalsDMACommonImpl::disableDMA(const std::uint32_t n) const {
	enaDisDMA(n, false);
}

void TerminalsDMACommonImpl::enaDisDMA(const std::uint32_t n, bool enaDis) const {
	const auto addr = utils::getAddressEnumResource(m_mapEnable, n, m_nameTermDMAEnable);

	const auto status = NiFpga_WriteBool(m_session, addr, static_cast<NiFpga_Bool>(enaDis));
	utils::throwIfNotSuccessNiFpga(status, "Error writing " +  m_nameTermDMAEnable + std::to_string(n));
}

bool TerminalsDMACommonImpl::getDMAOverflow(const std::uint16_t n) const {
	return static_cast<bool>(getAllDMAOverflows() & (1<<n));
}

std::uint16_t TerminalsDMACommonImpl::getAllDMAOverflows() const {
	std::uint16_t overflows;
	const auto status = NiFpga_ReadU16(m_session, m_overflowsAddr, &overflows);
	utils::throwIfNotSuccessNiFpga(status, "Error reading " + m_nameTermOverflows);

	return overflows;
}

FrameType TerminalsDMACommonImpl::getFrameType(const std::uint32_t n) const {
	if(n >= m_frameType.size()){
		const std::string err = std::to_string(n) + " is not a valid DMA ID";
		throw errors::ResourceNotFoundError(err);
	}

	return m_frameType.at(n);
}

std::vector<FrameType> TerminalsDMACommonImpl::getAllFrameType() const {
	return m_frameType;
}

std::uint8_t TerminalsDMACommonImpl::getSampleSize(const std::uint32_t n) const {
	if(n >= m_sampleSize.size()){
		const std::string err = std::to_string(n) + " is not a valid DMA ID";
		throw errors::ResourceNotFoundError(err);
	}

	return m_sampleSize.at(n);
}

std::vector<std::uint8_t> TerminalsDMACommonImpl::getAllSampleSizes() const {
	return m_sampleSize;
}

size_t TerminalsDMACommonImpl::readDataNonBlocking(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data) {
	return readData(n, elementsToRead, data, false);
}

size_t TerminalsDMACommonImpl::readDataBlocking(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data,
		std::uint32_t timeout) {
	return readData(n, elementsToRead, data, true, timeout);
}

size_t TerminalsDMACommonImpl::readData(
		const std::uint32_t n,
		size_t elementsToRead,
		std::uint64_t *data,
		bool block,
		std::uint32_t timeout) const {

	const auto dmaNum = utils::getAddressEnumResource(m_mapDMA, n, m_nameTermDMA);

	size_t elementsRead = 0;
	NiFpga_Status status;
	if(block){
		status = NiFpga_ReadFifoU64(m_session, dmaNum, data, elementsToRead, timeout, nullptr);
		//Special case when is timeout, inform the user of this specific case
		if(status == NiFpga_Status_FifoTimeout){
			throw errors::DMAReadTimeout(m_nameTermDMA, dmaNum);
		}
		utils::throwIfNotSuccessNiFpga(status, "Error reading " + m_nameTermDMA + std::to_string(n));
		elementsRead = elementsToRead;
	}else{
		size_t elementsRemaining;
		//Test how many elements are available right now
		status = NiFpga_ReadFifoU64(m_session, dmaNum, data, 0, 0, &elementsRemaining);
		utils::throwIfNotSuccessNiFpga(status, "Error reading " + m_nameTermDMA + std::to_string(n));
		//If not enough, do not read anything and return
		if(elementsRemaining >= elementsToRead){
			status = NiFpga_ReadFifoU64(m_session, dmaNum, data, elementsToRead, 1, nullptr);
			utils::throwIfNotSuccessNiFpga(status, "Error reading " + m_nameTermDMA + std::to_string(n));
			elementsRead = elementsToRead;
		}
	}

	return elementsRead;
}

}
