#pragma once

#include <stdexcept>
#include <string>

namespace irio {
namespace errors {

/**
 * Base IrioV2 error
 *
 * @ingroup Errors
 */
class IrioV2Error: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

/**
 * Exception when terminal has not been implemented in the profile
 *
 * @ingroup Errors
 */
class TerminalNotImplementedError: public IrioV2Error {
	using IrioV2Error::IrioV2Error;
 public:
	TerminalNotImplementedError() :
			IrioV2Error("Terminal not implemented in the profile") {
	}
};

/**
 * Exception when resource (Register/DMA) is not found
 * @ingroup Errors
 */
class ResourceNotFoundError: public IrioV2Error {
	using IrioV2Error::IrioV2Error;
 public:
	ResourceNotFoundError() :
			IrioV2Error("Resource not found") {
	}

	ResourceNotFoundError(const std::uint32_t resourceNumber,
			const std::string &resourceName) :
			IrioV2Error(std::to_string(resourceNumber)
					+ " is not a valid " + resourceName + " resource") {	}
};

/**
 * Exception when FPGAVIVersion is not the one expected
 * @ingroup Errors
 */
class FPGAVIVersionMismatchError: public IrioV2Error {
 public:
	FPGAVIVersionMismatchError() :
			IrioV2Error("FPGAVIversion mismatch") {
	}
	FPGAVIVersionMismatchError(const std::string &foundVersion,
			const std::string &expectedVersion) :
			IrioV2Error(
					"FPGAVIVserion mismatch (" + foundVersion + " != "
							+ expectedVersion + ")") {
	}
};

/**
 * Exception when the platform read from the FPGA does not match any of the supported ones (See \ref platforms.h)
 * @ingroup Errors
 */
class UnsupportedPlatformError: public IrioV2Error {
 public:
	UnsupportedPlatformError() :
			IrioV2Error("Platform specified is not supported") {
	}
	explicit UnsupportedPlatformError(const std::uint8_t platform) :
			IrioV2Error(std::to_string(platform)
						+ "is not a supported platform value") {
	}
};

/**
 * Exception when the DevProfile read from the FPGA does not matches any of the supported ones or
 * if the profile is not supported by the platform
 *
 * @ingroup Errors
 */
class UnsupportedDevProfileError: public IrioV2Error {
 public:
	UnsupportedDevProfileError() :
			IrioV2Error("DevProfile specified is not supported") {
	}
	UnsupportedDevProfileError(const std::uint8_t devProfile,
			const std::uint8_t platform) :
			IrioV2Error(
					"DevProfile " + std::to_string(devProfile)
							+ " is not valid for the platform "
							+ std::to_string(platform)) {
	}
};

/**
 * Exception when InitDone is not ready within a specified timeout
 *
 * @ingroup Errors
 */
class InitializationTimeoutError: public IrioV2Error {
 public:
	InitializationTimeoutError() :
			IrioV2Error("InitDone not ready in time") {
	}
};

/**
 * Exception when the modules installed in the platform are not the ones expected or
 * the FPGA inform that there is a problem with them
 *
 * @ingroup Errors
 */
class ModulesNotOKError: public IrioV2Error {
	using IrioV2Error::IrioV2Error;
};

/**
 * Exception when an error occurs while searching for the RIO Device
 *
 * @ingroup Errors
 */
class RIODiscoveryError: public IrioV2Error {
	using IrioV2Error::IrioV2Error;
};

/**
 * Exception when the specified RIO Device is not found
 *
 * @ingroup Errors
 */
class RIODeviceNotFoundError: public IrioV2Error {
 public:
	RIODeviceNotFoundError() :
			IrioV2Error("") {
	}
	explicit RIODeviceNotFoundError(const std::string &serialNumber) :
			IrioV2Error("No RIO device with serial number " + serialNumber) {
	}
};

/**
 * Exception when an error returns from a NiFpga_* function
 *
 * @ingroup Errors
 */
class NiFpgaError: public IrioV2Error {
	using IrioV2Error::IrioV2Error;
};

/**
 * Specific error when an error occurs while downloading the bitfile into the FPGA
 *
 * @ingroup Errors
 */
class NiFpgaErrorDownloadingBitfile: public NiFpgaError {
	using NiFpgaError::NiFpgaError;
};

/**
 * Specific error when NiFpga_Run return that the FPGA VI is already running
 *
 * @ingroup Errors
 */
class NiFpgaFPGAAlreadyRunning: public NiFpgaError {
	using NiFpgaError::NiFpgaError;
};


/**
 * Exception when a timeout expires while trying to read a DMA
 *
 * @ingroup Errors
 */
class DMAReadTimeout: public IrioV2Error {
 public:
	DMAReadTimeout(const std::string &nameTermDMA, const std::uint32_t &n) :
			IrioV2Error("Timeout reading " + nameTermDMA + std::to_string(n)) {
	}
};

/**
 * Exception when an error occurs while parsing the bitfile
 *
 * @ingroup Errors
 */
class BFPParseBitfileError: public IrioV2Error {
 public:
	explicit BFPParseBitfileError(const std::string &bitfile) :
			IrioV2Error("Error parsing " + bitfile) {
	}

	BFPParseBitfileError(const std::string &bitfile,
			const std::string &errDescription) :
			IrioV2Error(
					"Error parsing " + bitfile + ". Error: " + errDescription) {
	}
};

/**
 * Exception when trying to set an invalid AI coupling mode for
 * a module
 *
 * @ingroup Errors
 */
class UnsupportedAICouplingForModule: public IrioV2Error {
 public:
	UnsupportedAICouplingForModule() :
			IrioV2Error("Unsupported AI coupling mode for module") {
	}
};

}  // namespace errors
}  // namespace iriov2
