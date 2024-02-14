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
	enum PROFILE_ID{
		FLEXRIO_DAQ = 0,
		FLEXRIO_IMAQ,
		FLEXRIO_GPUDAQ,
		FLEXRIO_GPUIMAQ,
		CRIO_DAQ,
		CRIO_IO,
		R_DAQ
	};

	static const std::uint8_t PROFILE_VALUE_DAQ = 0;
	static const std::uint8_t PROFILE_VALUE_IMAQ = 1;
	static const std::uint8_t PROFILE_VALUE_DAQGPU = 2;
	static const std::uint8_t PROFILE_VALUE_IMAQGPU = 3;
	static const std::uint8_t PROFILE_VALUE_IO = 1;

    Profile(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const std::uint8_t id);
    
    const std::shared_ptr<const TerminalsAnalog> analog();
    const std::shared_ptr<const TerminalsDigital> digital();

    const std::uint8_t profileID;

protected:
    std::shared_ptr<const TerminalsAnalog> m_analog;
    std::shared_ptr<const TerminalsDigital> m_digital;
};

}
