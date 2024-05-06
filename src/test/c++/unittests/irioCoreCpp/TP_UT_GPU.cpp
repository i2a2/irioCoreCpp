#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures.h"
#include "fff_nifpga.h"

#include "irioCoreCpp.h"
#include "errorsIrio.h"

#include "profiles/profileCPUDAQFlexRIO.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsFlexRIO.h"


using namespace irio;


class FlexRIOGPUDAQ: public BaseTests {
public:
	FlexRIOGPUDAQ():
			BaseTests("../../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);
        setValueForReg(ReadFunctions::NiFpga_ReadU8,
					   bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
					   PROFILE_VALUE_DAQGPU);
	}
};

class FlexRIOGPUIMAQ: public BaseTests {
public:
	FlexRIOGPUIMAQ():
			BaseTests("../../../resources/7966/NiFpga_FlexRIO_OnlyResources_7966.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);
        setValueForReg(ReadFunctions::NiFpga_ReadU8,
					   bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
					   PROFILE_VALUE_IMAQGPU);
	}
};


///////////////////////////////////////////////////////////////
/// GPU DAQ Tests
///////////////////////////////////////////////////////////////
TEST_F(FlexRIOGPUDAQ, ProfileNotImplementedError){
	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9"),
				 errors::ProfileNotImplementedError);
}

///////////////////////////////////////////////////////////////
/// GPU IMAQ Tests
///////////////////////////////////////////////////////////////
TEST_F(FlexRIOGPUIMAQ, ProfileNotImplementedError){
	EXPECT_THROW(Irio irio(bitfilePath, "0", "V9.9"),
				 errors::ProfileNotImplementedError);
}
