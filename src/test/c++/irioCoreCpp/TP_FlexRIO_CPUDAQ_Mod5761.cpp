#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <chrono>

#include <irioCoreCpp.h>
#include "irioFixture.h"


using namespace irio;

class FlexRIOCPUDAQMod5761 : public IrioFixture {
 public:
  FlexRIOCPUDAQMod5761() : IrioFixture("Mod5761") {}
};

class FlexRIOCPUDAQMod5761Error : public FlexRIOCPUDAQMod5761 {};


TEST_F(FlexRIOCPUDAQMod5761, GetSetEnableAO) {
  int32_t channel = 0;

  const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
  auto analog = irio.getTerminalsAnalog();
  irio.startFPGA();

  EXPECT_NO_THROW(analog.setAOEnable(channel, true));
  bool aoEnable;
  EXPECT_NO_THROW(aoEnable = analog.getAOEnable(channel));
  EXPECT_EQ(aoEnable, true);

  EXPECT_NO_THROW(analog.setAOEnable(channel, false));
  EXPECT_NO_THROW(aoEnable = analog.getAOEnable(channel));
  EXPECT_EQ(aoEnable, false);
}

/// Error FlexRIO DAQ Tests

TEST_F(FlexRIOCPUDAQMod5761Error, InvalidAnalogResource) {
  uint32_t invalidChannel = static_cast<std::uint32_t>(-1);

  const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
  auto analog = irio.getTerminalsAnalog();
  irio.startFPGA();

  EXPECT_THROW(analog.getAI(invalidChannel), errors::ResourceNotFoundError);
  EXPECT_THROW(analog.getAO(invalidChannel), errors::ResourceNotFoundError);
  EXPECT_THROW(analog.getAOEnable(invalidChannel), errors::ResourceNotFoundError);
  
  EXPECT_THROW(analog.setAO(invalidChannel, 0), errors::ResourceNotFoundError);
  EXPECT_THROW(analog.setAOEnable(invalidChannel, 0), errors::ResourceNotFoundError);
}
