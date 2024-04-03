#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "terminals/impl/terminalsBaseImpl.h"
#include "frameTypes.h"

namespace irio {
/**
 * Class implementing the DMA functionality common
 * to the terminals with DMA
 *
 * @ingroup DMATerminals
 */
class TerminalsDMACommonImpl: public TerminalsBaseImpl{
 public:
	/**
	 * Constructor.
	 * Manages finding DMA resources common to all terminals
	 * that use them, DAQ/IMAQ. The name of these terminals
	 * are set by the constructor parameters, to allow reusing
	 * code when terminal functionality is the same but name
	 * changes (such is the case for CPU and GPU)
	 *
	 * @throw std::runtime_error	Size mismatch between \p nameTermDMA
	 * 								and \p nameTermDMAEnable
	 * @throw iriov2::errors::ResourceNotFoundError Resource specified not found
	 * @throw iriov2::errors::NiFpgaError Error occurred in an FPGA operation
	 *
	 * @param parserManager     	Pointer to class managing parsing the
	 * 								bitfile and finding its resources
	 * @param session				NiFpga_Session to be used in NiFpga
	 * 								related functions
	 * @param platform				Platform that is using the terminals.
	 * 								Used to know the maximum number of terminals
	 * 								that can be found
	 * @param nameTermNCh			Name of the terminal that describes the number
	 * 								of DMAs implemented in the FPGA
	 * @param nameTermFrameType		Name of the terminal that describes the
	 * 								frame type used in the DMA frame
	 * @param nameTermSampleSize	Name of the terminal with the size in bytes
	 * 								for the channel sample
	 * @param nameTermOverflows		Name of the terminal with the status of the
	 * 								different DMA FIFO
	 * @param nameTermDMA			Name of the DMAs
	 * @param nameTermDMAEnable		Name of the terminals that enable or disable
	 * 								write to DMA
	 */
	TerminalsDMACommonImpl(
			ParserManager *parserManager,
			const NiFpga_Session &session,
			const Platform &platform,
			const std::string &nameTermNCh,
			const std::string &nameTermFrameType,
			const std::string &nameTermSampleSize,
			const std::string &nameTermOverflows,
			const std::string &nameTermDMA,
			const std::string &nameTermDMAEnable);

	std::uint16_t getNChImpl(const std::uint32_t n) const;

	bool getDMAOverflowImpl(const std::uint16_t n) const;

	std::uint16_t getAllDMAOverflowsImpl() const;

	FrameType getFrameTypeImpl(const std::uint32_t n) const;

	std::vector<FrameType> getAllFrameTypeImpl() const;

	std::uint8_t getSampleSizeImpl(const std::uint32_t n) const;

	std::vector<std::uint8_t> getAllSampleSizesImpl() const;

	void startDMAImpl(const std::uint32_t n) const;

	void startAllDMAsImpl() const;

	void stopDMAImpl(const std::uint32_t n) const;

	void stopAllDMAsImpl() const;

	void cleanDMAImpl(const std::uint32_t n) const;

	void cleanAllDMAsImpl() const;

	bool isDMAEnableImpl(const std::uint32_t n) const;

	void enableDMAImpl(const std::uint32_t n) const;

	void disableDMAImpl(const std::uint32_t n) const;

	void enaDisDMAImpl(const std::uint32_t n, bool enaDis) const;

	size_t readDataNonBlockingImpl(const std::uint32_t n,
			size_t elementsToRead, std::uint64_t *data) const;

	size_t readDataBlockingImpl(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data,
			std::uint32_t timeout = 0) const;

	size_t readDataImpl(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data,
			bool blockRead,
			std::uint32_t timeout = 0) const;

	size_t countDMAsImpl() const;

 protected:
	template<typename T>
	bool findArrayRegReadToVector(ParserManager *parserManager,
			const GroupResource &group, bool optional,
			const NiFpga_Session &session, const std::string &nameReg,
			std::vector<T> *vec,
			std::function<NiFpga_Status(NiFpga_Session,
					std::uint32_t, T*, size_t)> readFunc);

	std::unordered_map<std::uint32_t, const std::uint32_t> getDMAMap() const;

 private:
	static const size_t SIZE_HOST_DMAS = 2048000;  // TODO: Why this number?

	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDMA;

	void startDMACommon(const std::uint32_t &dma) const;
	void cleanDMACommon(const std::uint32_t &dma) const;

	std::uint32_t m_overflowsAddr;

	std::vector<std::uint16_t> m_nCh;
	std::vector<FrameType> m_frameType;
	std::vector<std::uint8_t> m_sampleSize;

	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapEnable;

	const std::string m_nameTermOverflows;
	const std::string m_nameTermDMA;
	const std::string m_nameTermDMAEnable;
};
}  // namespace irio
