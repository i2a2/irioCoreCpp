#include <terminalsAuxDigital.h>
#include <terminals/names/namesTerminalsAuxDigital.h>
#include <utils.h>

namespace iriov2{

    TerminalsAuxDigital::TerminalsAuxDigital(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, Platform &platform):
        m_session(session)
    {
        //Find DI
        findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXDI, platform.maxDigital, m_mapAuxDI);

        //Find DO
        findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXDO, platform.maxDigital, m_mapAuxDO);

        if((m_mapAuxDI.size() + m_mapAuxDO.size()) > platform.maxDigital){
            throw std::runtime_error("More Aux Digital terminals than supported");
        }
    }

    bool getAuxDigital(const NiFpga_Session &session,
                    const std::uint32_t n,
                    const std::unordered_map<std::uint32_t, const std::uint32_t> &mapTerminals,
                    const std::string& terminalName){
        const auto addr = getAddressEnumResource(mapTerminals, n, terminalName);

        std::uint8_t aux;
        auto status = NiFpga_ReadBool(session, addr, &aux);
        throwIfNotSuccessNiFpga(status, "Error reading terminal " + 
                terminalName + std::to_string(n));

        return static_cast<bool>(aux);
    }

    bool TerminalsAuxDigital::getAuxDI(const std::uint32_t n) const
    {
        return getAuxDigital(m_session, n, m_mapAuxDI, TERMINAL_AUXDI);
    }

    bool TerminalsAuxDigital::getAuxD0(const std::uint32_t n) const
    {
        return getAuxDigital(m_session, n, m_mapAuxDO, TERMINAL_AUXDO);
    }

    void TerminalsAuxDigital::setAuxDO(const std::uint32_t n, const bool value) const
    {
        const auto addr = getAddressEnumResource(m_mapAuxDO, n, TERMINAL_AUXDO);
        
        auto status = NiFpga_WriteBool(m_session, addr, static_cast<NiFpga_Bool>(value));
        throwIfNotSuccessNiFpga(status, "Error writing terminal "
                + std::string(TERMINAL_AUXDO) + std::to_string(n));
    }
}