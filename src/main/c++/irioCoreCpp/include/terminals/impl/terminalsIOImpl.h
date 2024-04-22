#pragma once

#include <unordered_map>

#include "terminals/impl/terminalsBaseImpl.h"

namespace irio {

/**
 * Class implementing the cRIO IO functionality.
 *
 * @ingroup Terminals
 */
class TerminalsIOImpl: public TerminalsBaseImpl {
 public:
    TerminalsIOImpl(ParserManager *parserManager,
			const NiFpga_Session &session,
            const Platform &platform);

	void setSamplingRateDecimationImpl(const std::uint32_t &n,
									   const std::uint16_t dec) const;

	std::uint16_t getSamplingRateDecimationImpl(const std::uint32_t &n) const;

    size_t getNumIOSamplingRateImpl() const;
 private:
    std::unordered_map<std::uint32_t, const std::uint32_t> m_mapSamplingRate;
};

}  // namespace irio
