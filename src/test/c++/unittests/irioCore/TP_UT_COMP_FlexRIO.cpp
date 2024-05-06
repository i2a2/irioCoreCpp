#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <NiFpga.h>

#include "fixtures_adapter.h"
#include "fff_nifpga.h"

#include "bfp.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsDMADAQCPU.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "platforms.h"
#include "modules.h"

#include "irioDriver.h"
#include "irioError.h"
#include "irioHandlerDMA.h"

using namespace irio;

template<ModulesType M>
void setFlexRIOConnectedModule(const bfp::BFP &bfp){
	NiFlexRio_GetAttribute_fake.custom_fake =
			[](NiFpga_Session, int32_t, int32_t, void *value) {
		*reinterpret_cast<uint32_t*>(value) =
				static_cast<std::uint32_t>(M);
		return NiFpga_Status_Success;
	};

	setValueForReg(ReadFunctions::NiFpga_ReadU32,
					bfp.getRegister(TERMINAL_INSERTEDIOMODULEID).getAddress(),
					static_cast<std::uint32_t>(M));
}

class FlexRIOAdapter: public BaseTestsAdapter {
public:
	FlexRIOAdapter():
		BaseTestsAdapter("../../../resources/7966", "FlexRIO_CPUDAQ_7966")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
						PLATFORM_ID::FlexRIO);

        setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>(bfp);
		auto ret = irio_initDriver("test", "0", "TestModel",
					projectName.c_str(), "V9.9", false,
					nullptr, bitfileDir.c_str(), &p_DrvPvt, &status);

		if(ret != IRIO_success) {
			throw std::runtime_error("Unable to initialize driver");
		}
	}

	void SetUp() override {
		irio_initStatus(&status);
	}

	void TearDown() override {
		irio_resetStatus(&status);
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	~FlexRIOAdapter() {
		irio_closeDriver(&p_DrvPvt, 0, &status);
	}

	TStatus status;
	irioDrv_t p_DrvPvt;
};

class ErrorFlexRIOAdapter: public FlexRIOAdapter {};


TEST_F(FlexRIOAdapter, getFlexRIOModule) {
    uint32_t value;
    const auto ret = irio_getFlexRIOModule(&p_DrvPvt, &value, &status);
    EXPECT_EQ(ret, IRIO_success);
    EXPECT_EQ(status.code, IRIO_success);
    EXPECT_EQ(value, static_cast<std::uint32_t>(ModulesType::FlexRIO_NI5761));
}

TEST_F(ErrorFlexRIOAdapter, getcRIOModules) {
    const uint16_t *values;
    size_t numModules;
    
    const auto ret = irio_getcRIOModules(&p_DrvPvt, &values, &numModules, &status);
    EXPECT_EQ(ret, IRIO_warning);
    EXPECT_EQ(status.code, IRIO_warning);
    EXPECT_EQ(status.detailCode, Generic_Warning);
    EXPECT_EQ(numModules, 0);
}
