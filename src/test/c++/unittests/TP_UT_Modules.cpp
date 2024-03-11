#include "fixtures.h"
#include "fff_nifpga.h"

#include "irio_v2.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsAnalog.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "modules.h"

using namespace iriov2;

template<ModulesType M>
void setFlexRIOConnectedModule(const bfp::BFP &bfp){
	NiFlexRio_GetAttribute_fake.custom_fake =
			[](NiFpga_Session, int32_t, int32_t, void *value) {
		*reinterpret_cast<uint32_t*>(value) =
				static_cast<std::uint32_t>(M);
		return NiFpga_Status_Success;
	};

	setValueForReg(ReadFunctions::NiFpga_ReadU32,
					bfp.getRegister(TERMINAL_INSERTEDIOMODULEID).address,
					static_cast<std::uint32_t>(M));
}

class ModulesFlexRIO: public BaseTests{
public:
	ModulesFlexRIO():
		BaseTests("../../resources/7966/NiFpga_FlexRIO_CPUDAQ_7966.lvbitx") {
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
						bfp.getRegister(TERMINAL_PLATFORM).address,
						PLATFORM_ID::FlexRIO);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_RIOADAPTERCORRECT).address,
						1);
	}

};

class ModulesCRIO: public BaseTests{
public:
	ModulesCRIO():
		BaseTests("../../resources/9159/NiFpga_cRIO_CPUDAQ_9159.lvbitx") {
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
								bfp.getRegister(TERMINAL_PLATFORM).address,
								PLATFORM_ID::cRIO);
		setValueForReg(ReadFunctions::NiFpga_ReadBool,
						bfp.getRegister(TERMINAL_CRIOMODULESOK).address,
						1);
		setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU16,
						bfp.getRegister(TERMINAL_INSERTEDIOMODULESID).address,
						insertedIOModulesIDFake, 1);
	}

	const uint16_t insertedIOModulesIDFake[1] =
		{static_cast<std::uint16_t>(ModulesType::cRIO_NI92xx)};
};

class ModulesRSeries: public BaseTests{
public:
	ModulesRSeries():
		BaseTests("../../resources/7854/NiFpga_Rseries_CPUDAQ_7854.lvbitx") {
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
							bfp.getRegister(TERMINAL_PLATFORM).address,
							PLATFORM_ID::RSeries);
	}
};


TEST_F(ModulesFlexRIO, NI5761){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5761>(bfp);
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(),
			ModulesType::FlexRIO_NI5761);
	EXPECT_EQ(irio.getTerminalsFlexRIO().getInsertedIOModuleID(),
			static_cast<std::uint32_t>(ModulesType::FlexRIO_NI5761));
}

TEST_F(ModulesFlexRIO, NI5781){
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5781>(bfp);
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(),
			ModulesType::FlexRIO_NI5781);
	EXPECT_EQ(irio.getTerminalsFlexRIO().getInsertedIOModuleID(),
				static_cast<std::uint32_t>(ModulesType::FlexRIO_NI5781));
}

TEST_F(ModulesFlexRIO, NI6581) {
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI6581>(bfp);
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(),
			ModulesType::FlexRIO_NI6581);
	EXPECT_EQ(irio.getTerminalsFlexRIO().getInsertedIOModuleID(),
				static_cast<std::uint32_t>(ModulesType::FlexRIO_NI6581));
}

TEST_F(ModulesFlexRIO, NI5734) {
	setFlexRIOConnectedModule<ModulesType::FlexRIO_NI5734>(bfp);
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(),
			ModulesType::FlexRIO_NI5734);
	EXPECT_EQ(irio.getTerminalsFlexRIO().getInsertedIOModuleID(),
				static_cast<std::uint32_t>(ModulesType::FlexRIO_NI5734));
}

TEST_F(ModulesCRIO, NI92XX) {

	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(),
			ModulesType::cRIO_NI92xx);
	const auto vecMod = irio.getTerminalsCRIO().getInsertedIOModulesID();

	EXPECT_EQ(vecMod[0], static_cast<std::uint16_t>(ModulesType::cRIO_NI92xx));
}

TEST_F(ModulesRSeries, NoModule) {
	IrioV2 irio(bitfilePath, "0", "9.9");
	EXPECT_EQ(irio.getTerminalsAnalog().getModuleConnected(),
				ModulesType::NoModule);
}
