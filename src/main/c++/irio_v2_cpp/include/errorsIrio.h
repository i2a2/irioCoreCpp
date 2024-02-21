#pragma once

#include <stdexcept>

namespace iriov2{

namespace errors{

/**
 * Exception when terminal has not been implemented in the profile
 */
class TerminalNotImplementedError: public std::runtime_error{
public:
	TerminalNotImplementedError(): std::runtime_error("Terminal not implemented in the profile"){}

	TerminalNotImplementedError(const std::string& err): std::runtime_error(err){}
};

/**
 * Exception when resource (Register/DMA) is not found
 */
class ResourceNotFoundError: public std::runtime_error{
public:
	ResourceNotFoundError(): std::runtime_error("Resource not found"){}

	ResourceNotFoundError(const std::string& err): std::runtime_error(err){}

	ResourceNotFoundError(const std::uint32_t resourceNumber, const std::string &resourceName) :
					std::runtime_error(
							std::to_string(resourceNumber) + " is not a valid " + resourceName
									+ " resource") { }
};

/**
 * Exception when FPGAVIVersion is not the one expected
 */
class FPGAVIVersionMismatchError: public std::runtime_error{
public:
	FPGAVIVersionMismatchError(): std::runtime_error("FPGAVIversion mismatch"){}
	FPGAVIVersionMismatchError(const std::string &foundVersion, const std::string &expectedVersion) :
					std::runtime_error(
							"FPGAVIVserion mismatch (" + foundVersion + " != " + expectedVersion
									+ ")") { }
};

/**
 * Exception when the platform read from the FPGA does not match any of the supported ones (See \ref platforms.h)
 */
class UnsupportedPlatformError: public std::runtime_error{
public:
	UnsupportedPlatformError(): std::runtime_error("Platform specified is not supported"){}
	UnsupportedPlatformError(const std::uint8_t platform):
		std::runtime_error(std::to_string(platform) + "is not a supported platform value") {}
};

/**
 * Exception when the DevProfile read from the FPGA does not matches any of the supported ones or
 * if the profile is not supported by the platform
 */
class UnsupportedDevProfileError: public std::runtime_error{
public:
	UnsupportedDevProfileError(): std::runtime_error("DevProfile specified is not supported"){}
	UnsupportedDevProfileError(const std::uint8_t devProfile, const std::uint8_t platform):
		std::runtime_error("DevProfile " + std::to_string(devProfile) +
				" is not valid for the platform " + std::to_string(platform)){}
};

/**
 * Exception when InitDone is not ready within a specified timeout
 */
class InitializationTimeoutError: public std::runtime_error{
public:
	InitializationTimeoutError(): std::runtime_error("InitDone not ready in time"){}
};

/**
 * Exception when the modules installed in the platform are not the ones expected or
 * the FPGA inform that there is a problem with them
 */
class ModulesNotOKError: public std::runtime_error{
public:
	ModulesNotOKError(const std::string &err): std::runtime_error(err){}
};

/**
 * Exception when an error occurs while searching for the RIO Device
 */
class RIODiscoveryError: public std::runtime_error{
public:
	RIODiscoveryError(const std::string &err): std::runtime_error(err){}
};

/**
 * Exception when the specified RIO Device is not found
 */
class RIODeviceNotFoundError: public std::runtime_error{
public:
	RIODeviceNotFoundError(): std::runtime_error(""){}
	RIODeviceNotFoundError(const std::string &serialNumber):
		std::runtime_error("No RIO device with serial number " + serialNumber){}
};

/**
 * Exception when an error returns from a NiFpga_* function
 */
class NiFpgaError: public std::runtime_error{
public:
	NiFpgaError(const std::string &errMsg): std::runtime_error(errMsg){}
};

}
}
