#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsAnalog.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "modules.h"


using namespace iriov2;

template<ModulesType M>
void setFlexRIOConnectedModule(){
	NiFlexRio_GetAttribute_fake.custom_fake =
			[](NiFpga_Session, int32_t, int32_t, void *value) {
		*reinterpret_cast<uint32_t*>(value) =
				static_cast<std::uint32_t>(M);
		return NiFpga_Status_Success;
	};
}

class AnalogTests: public BaseTests{
public:
	AnalogTests():
			BaseTests("../../resources/7966/NiFpga_FlexRIO_CPUDAQ_7966.lvbitx")
	{
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						FLEXRIO_PLATFORM_VALUE);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_RIOADAPTERCORRECT).address,
						1);

		setValueForReg(ReadFunctions::NiFpga_ReadI32,
						bfp.getRegister(TERMINAL_AI+std::to_string(0)).address,
						aiFake);

		setValueForReg(ReadFunctions::NiFpga_ReadI32,
						bfp.getRegister(TERMINAL_AO+std::to_string(0)).address,
						aoFake);

		setValueForReg(ReadFunctions::NiFpga_ReadI32,
						bfp.getRegister(TERMINAL_AOENABLE+std::to_string(0)).address,
						aoEnableFake);

		setFlexRIOConnectedModule<ModulesType::NoModule>();

	}

	const std::int32_t aiFake = 1234;
	const std::int32_t aoFake = 5678;
	const std::int32_t aoEnableFake = 1526;
};

class ErrorAnalogTests: public AnalogTests{};


///////////////////////////////////////////////////////////////
///// Analog Terminals Tests
///////////////////////////////////////////////////////////////
TEST_F(AnalogTests, TerminalsAnalog){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsAnalog());
}

TEST_F(AnalogTests, NumAI){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getNumAI(), 2);
}

TEST_F(AnalogTests, NumAO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getNumAO(), 2);
}

TEST_F(AnalogTests, ModuleConnected){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(), ModulesType::NoModule);
}

TEST_F(AnalogTests, CVADC){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_DOUBLE_EQ(irio.getTerminalsAnalog().getCVADC(), 1.035 / 8191);
}

TEST_F(AnalogTests, CVDAC){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_DOUBLE_EQ(irio.getTerminalsAnalog().getCVDAC(), 8191 / 1.035);
}

TEST_F(AnalogTests, MaxValAO){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_DOUBLE_EQ(irio.getTerminalsAnalog().getMaxValAO(), 1.0);
}

TEST_F(AnalogTests, MinValAO){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_DOUBLE_EQ(irio.getTerminalsAnalog().getMinValAO(), -1.0);
}

TEST_F(AnalogTests, getAICoupling){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getAICouplingMode(), CouplingMode::AC);
}

TEST_F(AnalogTests, setAICoupling){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>();

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsAnalog().setAICouplingMode(CouplingMode::DC));
	EXPECT_EQ(irio.getTerminalsAnalog().getAICouplingMode(), CouplingMode::DC);
}

TEST_F(AnalogTests, getAI){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getAI(0), aiFake);
}

TEST_F(AnalogTests, getAO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getAO(0), aoFake);
}

TEST_F(AnalogTests, getAOEnable){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getAOEnable(0), aoEnableFake);
}

TEST_F(AnalogTests, setAO){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsAnalog().setAO(0, aoFake));
}

TEST_F(AnalogTests, setAOEnable){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_NO_THROW(irio.getTerminalsAnalog().setAOEnable(0, aoEnableFake));
}

///////////////////////////////////////////////////////////////
///// Error Analog Terminals Tests
///////////////////////////////////////////////////////////////

TEST_F(ErrorAnalogTests, UnsupportedAICouplingForModule){
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_THROW(irio.getTerminalsAnalog().setAICouplingMode(CouplingMode::None);,
			errors::UnsupportedAICouplingForModule);
}

TEST_F(ErrorAnalogTests, MismatchAOandAOEnable){
	EXPECT_THROW(IrioV2 irio(
			"../../resources/failResources/7966/NiFpga_FlexRIO_MismatchAOAOEnable_7966.lvbitx",
			"0", "9.9");,
			errors::ResourceNotFoundError);
}
