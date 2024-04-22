#pragma once

#include <string>

#include "terminals/impl/terminalsBaseImpl.h"

namespace irio {

/**
 * Class implementing the functionaility related
 * to terminals common to all profiles
 * 
 * @ingroup Terminals
 */
class TerminalsCommonImpl: public TerminalsBaseImpl {
 public:
    TerminalsCommonImpl(ParserManager *parserManager,
            const NiFpga_Session &session);

    std::string getFPGAVIversionImpl() const;

    std::uint32_t getFrefImpl() const;

    bool getInitDoneImpl() const;

    std::uint8_t getDevQualityStatusImpl() const;

    std::int16_t getDevTempImpl() const;

    bool getDAQStartStopImpl() const;

    bool getDebugModeImpl() const;

    void setDAQStartImpl() const;

    void setDAQStopImpl() const;

    void setDAQStartStopImpl(const bool &start) const;

    void setDebugModeImpl(const bool &debug) const;

    double getMinSamplingRateImpl() const;

    double getMaxSamplingRateImpl() const;

 private:
    std::string m_fpgaviversion;
    std::uint32_t m_fref;
    double m_minSamplingRate;
    double m_maxSamplingRate;

    std::uint32_t m_initdone_addr;
    std::uint32_t m_devqualitystatus_addr;
    std::uint32_t m_devtemp_addr;
    std::uint32_t m_daqstartstop_addr;
    std::uint32_t m_debugmode_addr;
};
}  // namespace irio
