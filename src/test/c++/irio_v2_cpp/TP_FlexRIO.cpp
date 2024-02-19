#include <gtest/gtest.h>
#include <iostream>
#include "irio_v2.h"

using namespace iriov2;

TEST(FlexRIO, ResourcesCPUDAQ){
   std::string bitfile = "../../resources/NiFpga_FlexRIO_CPUDAQ_7966.lvbitx";

   IrioV2 irio(bitfile, "0x0177A2AD", "4.0");

   EXPECT_EQ(irio.analog()->getNumAI(), 2) << "Unexpected number of analog inputs";
   EXPECT_EQ(irio.analog()->getNumAO(), 2) << "Unexpected number of analog outputs";
   EXPECT_EQ(irio.digital()->getNumDI(), 2) << "Unexpected number of digital inputs";
   EXPECT_EQ(irio.digital()->getNumDO(), 2) << "Unexpected number of digital outputs";
   EXPECT_EQ(irio.auxAnalog()->getNumAuxAI(), 2) << "Unexpected number of aux analog inputs";
   EXPECT_EQ(irio.auxAnalog()->getNumAuxAO(), 2) << "Unexpected number of aux analog outputs";
   EXPECT_EQ(irio.auxDigital()->getNumAuxDI(), 2) << "Unexpected number of aux digital inputs";
   EXPECT_EQ(irio.auxDigital()->getNumAuxDO(), 2) << "Unexpected number of aux digital outputs";
   EXPECT_EQ(irio.signalGeneration()->getSGNo(), 2) << "Unexpected number of signal generators";
}

