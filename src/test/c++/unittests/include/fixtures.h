#pragma once

#include <gtest/gtest.h>

#include "bfp.h"

///////////////////////////////////////////////////////////////
///// FIXTURES
///////////////////////////////////////////////////////////////
class BaseTests: public ::testing::Test {
public:
	BaseTests(const std::string bitfile);

	~BaseTests();

	void resetFakes();

	const std::string bitfilePath;
	const iriov2::bfp::BFP bfp;

	const uint8_t majorVersion = 9;
	const uint8_t minorVersion = 9;
	const uint8_t fpgaVIversionFake[2] = { majorVersion, minorVersion };
	const uint8_t numSGFake = 2;
	const uint32_t frefFake = std::numeric_limits<std::uint16_t>::max() * 10;
	const uint8_t devQualityStatusFake = 5;
	const int16_t devTempFake = 123;
	const uint8_t daqStartStopFake = 1;
	const uint8_t debugModeFake = 0;
};
