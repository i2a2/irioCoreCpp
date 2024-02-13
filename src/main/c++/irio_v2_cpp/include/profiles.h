#pragma once

#include <memory>

#include <bfp.h>

#include <terminals/terminalsCommon.h>
#include <terminals/terminalsAnalog.h>
#include <terminals/terminalsDigital.h>


namespace iriov2{

class Profile: public TerminalsCommon
{
public:
    Profile(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);
    
    const std::shared_ptr<const TerminalsAnalog> analog();
    const std::shared_ptr<const TerminalsDigital> digital();

protected:
    std::shared_ptr<const TerminalsAnalog> m_analog;
    std::shared_ptr<const TerminalsDigital> m_digital;
};

}