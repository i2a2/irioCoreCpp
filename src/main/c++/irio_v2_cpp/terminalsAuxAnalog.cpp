#include <terminals/terminalsAuxAnalog.h>
#include <terminals/names/namesTerminalsAuxAnalog.h>
#include <utils.h>

namespace iriov2{

TerminalsAuxAnalog::TerminalsAuxAnalog(const bfp::BFP &parsedBitfile, 
		const NiFpga_Session &session, Platform& platform):
				TerminalsBase(session)
{
    //Find AuxAI
    findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXAI, platform.maxAuxAI, m_mapAuxAI);

    //Find AuxAO
    findAndInsertEnumRegisters(parsedBitfile, TERMINAL_AUXAO, platform.maxAuxAO, m_mapAuxAO);
}

std::int32_t TerminalsAuxAnalog::getAuxAI(const std::uint32_t n) const{
    const std::uint32_t add = getAddressEnumResource(m_mapAuxAI, n, TERMINAL_AUXAI);
    std::int32_t aux;
    auto status = NiFpga_ReadI32(m_session, add, &aux);
    throwIfNotSuccessNiFpga(status, "Error reading terminal " + std::string(TERMINAL_AUXAI) + std::to_string(n));

    return aux;
}

std::int16_t TerminalsAuxAnalog::getAuxAO(const std::uint32_t n) const{
    const std::uint32_t add = getAddressEnumResource(m_mapAuxAO, n, TERMINAL_AUXAO);
    std::int16_t aux;
    auto status = NiFpga_ReadI16(m_session, add, &aux);
    throwIfNotSuccessNiFpga(status, "Error reading terminal " + std::string(TERMINAL_AUXAO) + std::to_string(n));

    return aux;
}

void TerminalsAuxAnalog::setAuxAO(const std::uint32_t n, const std::int16_t value) const{
    const std::uint32_t add = getAddressEnumResource(m_mapAuxAO, n, TERMINAL_AUXAO);
    auto status = NiFpga_WriteI16(m_session, add, value);
    throwIfNotSuccessNiFpga(status, "Error reading terminal " + std::string(TERMINAL_AUXAO) + std::to_string(n));
}

}
