#pragma once

#include <NiFpga.h>
#include <bfp.h>
#include <platforms.h>
#include <unordered_map>

namespace iriov2{

class TerminalsAuxAnalog
{
public:
    TerminalsAuxAnalog(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, Platform& platform);

    std::int32_t getAuxAI(const std::uint32_t n) const;
    std::int16_t getAuxAO(const std::uint32_t n) const;

    void setAuxAO(const std::uint32_t n, const std::int16_t value) const;

private:
    NiFpga_Session m_session;

    std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAI;
    std::unordered_map<std::uint32_t, const std::uint32_t> m_mapAuxAO;
};

}