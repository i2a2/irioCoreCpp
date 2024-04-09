#include "fixtures.h"

#include "fff_nifpga.h"

#include "profiles/profileBase.h"
#include "terminals/names/namesTerminalsCommon.h"
#include "terminals/names/namesTerminalsSignalGeneration.h"
#include "terminals/names/namesTerminalscRIO.h"
#include "terminals/names/namesTerminalsFlexRIO.h"
#include "platforms.h"

using namespace irio;

BaseTests::BaseTests(const std::string bitfile, const bool includeSG):
		bfp(bitfile, false), bitfilePath(bitfile){

	resetFakes();
	FFF_RESET_HISTORY();
	init_ok_fff_nifpga();

	searchRIODevice_fake.return_val = "MockDevice";
	setValueForReg(ReadArrayFunctions::NiFpga_ReadArrayU8,
			bfp.getRegister(TERMINAL_FPGAVIVERSION).getAddress(),
			fpgaVIversionFake, 2);
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_DEVPROFILE).getAddress(),
			PROFILE_VALUE_DAQ);
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_PLATFORM).getAddress(),
			PLATFORM_ID::FlexRIO);
	setValueForReg(ReadFunctions::NiFpga_ReadU32,
			bfp.getRegister(TERMINAL_FREF).getAddress(), frefFake);
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_DEVQUALITYSTATUS).getAddress(),
			devQualityStatusFake);
	setValueForReg(ReadFunctions::NiFpga_ReadI16,
			bfp.getRegister(TERMINAL_DEVTEMP).getAddress(), devTempFake);
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_DAQSTARTSTOP).getAddress(),
			daqStartStopFake);
	setValueForReg(ReadFunctions::NiFpga_ReadU8,
			bfp.getRegister(TERMINAL_DEBUGMODE).getAddress(), debugModeFake);
	setValueForReg(ReadFunctions::NiFpga_ReadBool,
			bfp.getRegister(TERMINAL_INITDONE).getAddress(), 1);

	if (includeSG) {
		setValueForReg(ReadFunctions::NiFpga_ReadU8,
					   bfp.getRegister(TERMINAL_SGNO).getAddress(), numSGFake);
	}

	NiFlexRio_GetAttribute_fake.custom_fake = [](NiFpga_Session, int32_t,
			int32_t, void *value) {
		*reinterpret_cast<uint32_t*>(value) =
				static_cast<std::uint32_t>(ModulesType::NoModule);
		return NiFpga_Status_Success;
	};
}

BaseTests::~BaseTests(){
	resetFakes();
}

void BaseTests::resetFakes() {
	RESET_FAKE(irio::searchRIODevice);
	reset_fff_nifpga();
}
