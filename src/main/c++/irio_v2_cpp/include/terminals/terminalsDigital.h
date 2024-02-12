#pragma once

#include <NiFpga.h>
#include <bfp.h>
#include <platforms.h>

namespace iriov2{

class TerminalsDigital
{
public:
    TerminalsDigital(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, Platform& platform);

    bool getDI(const std::uint32_t n) const;
    bool getD0(const std::uint32_t n) const;

    void setDO(const std::uint32_t n, const bool value) const;

private:
    NiFpga_Session m_session;

    std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDI;
    std::unordered_map<std::uint32_t, const std::uint32_t> m_mapDO;
    
};

}