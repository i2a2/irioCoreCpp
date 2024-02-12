#pragma once

#include "profileBase.h"

namespace iriov2{

class ProfileBasecRIO: public ProfileBase
{
public:
    ProfileBasecRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

    bool getcRIOModulesOk() const;
    std::vector<std::uint16_t> getInsertedIOModulesID() const;

private:
    std::uint32_t m_criomodulesok_addr;
    std::uint32_t m_insertediomodulesid_addr;
    size_t m_numModules;
};

}