#include <gtest/gtest.h>
#include <iostream>
#include "irio_v2.h"

using namespace iriov2;

TEST(FlexRIO, ResourcesCPUDAQ){
   std::string bitfile = "../../resources/NiFpga_FlexRIO_CPUDAQ_7966.lvbitx";

   IrioV2 irio(bitfile, "", "0x0177A2AD", "4.0", "TEST", true); //TODO: Fix this with actual values

   auto numAI = irio.analog()->getNumAI();
   ASSERT_EQ(numAI, 2);

   auto b = irio.analog()->getAI(0);

   irio.digital();

}

