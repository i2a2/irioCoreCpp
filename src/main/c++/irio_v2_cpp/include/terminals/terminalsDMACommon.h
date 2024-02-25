#pragma once

#include <terminals/terminalsBase.h>

namespace iriov2{
/**
 * @ingroup DMATerminals
 */
class TerminalsDMACommon: public TerminalsBase{
public:
	enum FrameType{
		FormatA = 0,
		FormatB = 1
	};

	TerminalsDMACommon(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const Platform &platform,
			const std::string &nameTermNCh,
			const std::string &nameTermFrameType,
			const std::string &nameTermSampleSize,
			const std::string &nameTermOverflows,
			const std::string &nameTermDMA,
			const std::string &nameTermDMAEnable);

	~TerminalsDMACommon() = default;

	std::uint16_t getNCh(const std::uint32_t n) const;

	/*TODO: Ask, should we allow use to configure the fifo? or just
	give a lot of memory to each? as the original lib did*/
//	void configureDMA(const std::uint32_t n, const size_t depth) const;
//	void configureAllDMAs(const size_t depth) const;

	void startDMA(const std::uint32_t n) const;
	void startAllDMAs() const;

	bool getDMAOverflow(const std::uint16_t mask) const;
	std::uint16_t getAllDMAOverflows() const;

	FrameType getFrameType(const std::uint32_t n) const;
	std::vector<FrameType> getAllFrameType() const;

	std::uint8_t getSampleSize(const std::uint32_t n) const;
	std::vector<std::uint8_t> getAllSampleSizes() const;

	void stopDMA(const std::uint32_t n) const;
	void stopAllDMAs() const;

	void cleanDMA(const std::uint32_t n) const;
	void cleanALLDMAs() const;

	bool isDMAEnable(const std::uint32_t n) const;
	void enableDMA(const std::uint32_t n) const;
	void disableDMA(const std::uint32_t n) const;
	void enaDisDMA(const std::uint32_t n, bool enaDis) const;

	size_t readDataNoBlock(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data);

	size_t readDataBlock(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data,
			std::uint32_t timeout = 0);

	size_t readData(
			const std::uint32_t n,
			size_t elementsToRead,
			std::uint64_t *data,
			bool blockRead,
			std::uint32_t timeout = 0) const;

	size_t getNumDMA() const;

protected:
	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDMA;

private:
	static const size_t SIZE_HOST_DMAS = 2048000; //TODO: Why this number?

	void startDMAImpl(const std::uint32_t &dma) const;
	void cleanDMAImpl(const std::uint32_t &dma) const;

	std::uint32_t m_overflowsAddr;

	std::vector<std::uint16_t> m_nCh;
	std::vector<FrameType> m_frameType;
	std::vector<std::uint8_t> m_sampleSize;

	std::unordered_map<std::uint32_t, const std::uint32_t> m_mapEnable;

	const std::string m_nameTermOverflows;
	const std::string m_nameTermDMA;
	const std::string m_nameTermDMAEnable;

};
}
