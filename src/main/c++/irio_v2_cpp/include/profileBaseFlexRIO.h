#pragma once

#include "profileBase.h"

namespace iriov2{

class ProfileBaseFlexRIO: public ProfileBase
{
public:
    ProfileBaseFlexRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

    bool getRIOAdapterCorrect() const;
    std::uint32_t getInsertedIOModuleID() const;

private:
    std::uint32_t m_rioadaptercorrect_addr;
    std::uint32_t m_insertediomoduleid_addr;
};

}