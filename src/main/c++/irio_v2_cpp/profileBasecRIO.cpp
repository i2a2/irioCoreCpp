#include "profileBasecRIO.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "utils.h"

namespace iriov2{
    ProfileBasecRIO::ProfileBasecRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session):
        ProfileBase(parsedBitfile, session)
    {
        m_criomodulesok_addr = parsedBitfile.getRegister(TERMINAL_CRIOMODULESOK).address;
        const auto regModulesID = parsedBitfile.getRegister(TERMINAL_INSERTEDIOMODULESID);
        m_insertediomodulesid_addr = regModulesID.address;
        m_numModules = regModulesID.numElem;
    }

    bool ProfileBasecRIO::getcRIOModulesOk() const
    {
        NiFpga_Bool aux;
        auto status = NiFpga_ReadBool(m_session, m_criomodulesok_addr, &aux);
        throwIfNotSuccessNiFpga(status, "Error reading " + std::string(TERMINAL_CRIOMODULESOK));
        return static_cast<bool>(aux);
    }

    std::vector<std::uint16_t> ProfileBasecRIO::getInsertedIOModulesID() const
    {
        static std::vector<std::uint16_t> ret(m_numModules);
        auto status = NiFpga_ReadArrayU16(m_session, m_insertediomodulesid_addr, ret.data(), m_numModules);
        throwIfNotSuccessNiFpga(status, "Error reading " + std::string(TERMINAL_INSERTEDIOMODULESID));
        return ret;
    }
}