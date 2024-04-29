#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <chrono>

#include <irioCoreCpp.h>
#include "irioFixture.h"


using namespace irio;

class FlexRIOCPUDAQ : public IrioFixture {
 public:
  FlexRIOCPUDAQ() : IrioFixture("CPUDAQ") {}
};

class FlexRIOCPUDAQError : public FlexRIOCPUDAQ {};

TEST_F(FlexRIOCPUDAQ, CheckTerminals) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	EXPECT_NO_THROW(irio.getTerminalsCommon()) << "Common terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsFlexRIO())
		<< "FlexRIO terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAnalog()) << "Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxAnalog())
		<< "Aux Analog terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsDigital())
		<< "Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsAuxDigital())
		<< "Aux Digital terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsSignalGeneration())
		<< "Signal Generation terminals not found";
	EXPECT_NO_THROW(irio.getTerminalsDAQ()) << "DAQ terminals not found";

	EXPECT_THROW(irio.getTerminalsCRIO(), errors::TerminalNotImplementedError)
		<< "cRIO terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsIMAQ(), errors::TerminalNotImplementedError)
		<< "IMAQ terminals found when they should have not";
	EXPECT_THROW(irio.getTerminalsIO(), errors::TerminalNotImplementedError)
		<< "IO terminals found when they should have not";
}

TEST_F(FlexRIOCPUDAQ, GetSetAuxAIO32) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int32_t> dist(min + 1, max - 1);

	auto auxAnalog = irio.getTerminalsAuxAnalog();
	// For each device and value, write value, read from output and read and
	// compare from input
	try {
		for (int i = 0; i < 6; ++i) {
			std::vector<int32_t> values = {min, dist(gen), dist(gen), dist(gen),
										   max};
			for (auto v : values) {
				// Write value
				auxAnalog.setAuxAO(i, v);

				// Read written value
				const auto written = auxAnalog.getAuxAO(i);
				EXPECT_EQ(written, v);

				// Read value from input
				const auto read = auxAnalog.getAuxAI(i);
				EXPECT_EQ(read, v);
			}
		}
	} catch (errors::IrioError& e) {
		FAIL() << e.what();
	}
}

TEST_F(FlexRIOCPUDAQ, GetSetAuxAIO64) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	// Initialize random engine
	int min = 0, max = 100;
	std::mt19937 gen(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int64_t> dist(min + 1, max - 1);

	auto auxAnalog = irio.getTerminalsAuxAnalog();
	// For each device and value, write value, read from output and read and
	// compare from input
	try {
		for (int i = 0; i < 6; ++i) {
			std::vector<int64_t> values = {min, dist(gen), dist(gen), dist(gen),
										   max};
			for (auto v : values) {
				// Write value
				auxAnalog.setAuxAO64(i, v);

				// Read written value
				const auto written = auxAnalog.getAuxAO64(i);
				EXPECT_EQ(written, v);

				// Read value from input
				const auto read = auxAnalog.getAuxAI64(i);
				EXPECT_EQ(read, v);
			}
		}
	} catch (errors::IrioError& e) {
		FAIL() << e.what();
	}
}

TEST_F(FlexRIOCPUDAQ, GetSetAuxDIO) {
    const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	auto auxDigital = irio.getTerminalsAuxDigital();
	try {
		for (int i = 0; i < 6; ++i) {
			for (int v : {0, 1}) {
				// Write value
				auxDigital.setAuxDO(i, v);

				// Read written value
				const auto written = auxDigital.getAuxDO(i);
				EXPECT_EQ(written, v);

				// Read value from input
				const auto read = auxDigital.getAuxDI(i);
				EXPECT_EQ(read, v);
			}
		}
	} catch (errors::IrioError& e) {
		FAIL() << e.what();
	}
}

TEST_F(FlexRIOCPUDAQ, GetSetSGSignalType) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();

	auto sigGen = irio.getTerminalsSignalGeneration();
	EXPECT_NO_THROW(sigGen.setSGSignalType(0, 0));

	const auto type = sigGen.getSGSignalType(0);
	EXPECT_EQ(type, 0);
}

TEST_F(FlexRIOCPUDAQ, GetSetSGUpdateRate) {
	int decimation = 10;
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGUpdateRateDecimation(channel, decimation));
	uint32_t decConf;
	EXPECT_NO_THROW(decConf = sigGen.getSGUpdateRate(channel));;
	EXPECT_EQ(decConf, decimation);
}

TEST_F(FlexRIOCPUDAQ, GetSetSGFreq) {
	int decimation = 12;
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGFreqDecimation(channel, decimation));
	uint32_t decConf;
	EXPECT_NO_THROW(decConf = sigGen.getSGFreq(channel));;
	EXPECT_EQ(decConf, decimation);
}

TEST_F(FlexRIOCPUDAQ, GetSetSGAmp) {
	int sigAmp = 4096; // 0.517563 V
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGAmp(channel, sigAmp));
	uint32_t ampRead;
	EXPECT_NO_THROW(ampRead = sigGen.getSGAmp(channel));;
	EXPECT_EQ(ampRead, sigAmp);
}

TEST_F(FlexRIOCPUDAQ, GetSetSGPhase) {
	int phase = 1;
	int channel = 0;

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");
	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_NO_THROW(sigGen.setSGPhase(channel, phase));
	uint32_t phaseRead;
	EXPECT_NO_THROW(phaseRead = sigGen.getSGPhase(channel));;
	EXPECT_EQ(phaseRead, phase);
}

/// Error FlexRIO DAQ Tests

TEST_F(FlexRIOCPUDAQError, InvalidAuxAIO) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto auxAnalog = irio.getTerminalsAuxAnalog();

	EXPECT_THROW(auxAnalog.setAuxAO(static_cast<std::uint32_t>(-1), 1), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.setAuxAO64(static_cast<std::uint32_t>(-1), 1), errors::ResourceNotFoundError);

	EXPECT_THROW(auxAnalog.getAuxAI(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.getAuxAO(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.getAuxAI64(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxAnalog.getAuxAO64(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
}

TEST_F(FlexRIOCPUDAQError, InvalidAuxDIO) {
	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto auxDigital = irio.getTerminalsAuxDigital();

	EXPECT_THROW(auxDigital.setAuxDO(static_cast<std::uint32_t>(-1), 1), errors::ResourceNotFoundError);

	EXPECT_THROW(auxDigital.getAuxDI(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
	EXPECT_THROW(auxDigital.getAuxDO(static_cast<std::uint32_t>(-1)), errors::ResourceNotFoundError);
}

TEST_F(FlexRIOCPUDAQError, InvalidSGResource) {
	uint32_t invalidChannel = static_cast<std::uint32_t>(-1);

	const std::string bitfilePath = getBitfilePath();
	Irio irio(bitfilePath, serialNumber, "V1.2");

	irio.startFPGA();
	auto sigGen = irio.getTerminalsSignalGeneration();

	EXPECT_THROW(sigGen.getSGAmp(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGFref(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGFreq(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGPhase(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGSignalType(invalidChannel), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.getSGUpdateRate(invalidChannel), errors::ResourceNotFoundError);

	EXPECT_THROW(sigGen.setSGAmp(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGFreqDecimation(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGPhase(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGSignalType(invalidChannel, 0), errors::ResourceNotFoundError);
	EXPECT_THROW(sigGen.setSGUpdateRateDecimation(invalidChannel, 0), errors::ResourceNotFoundError);
}