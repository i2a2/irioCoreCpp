#pragma once

#include <stdexcept>
#include <string>

namespace irio {
namespace errors {

/**
 * Base IRIO error
 *
 * @ingroup Errors
 */
class IrioError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

/**
 * Exception when terminal has not been implemented in the profile
 *
 * @ingroup Errors
 */
class TerminalNotImplementedError: public IrioError {
	using IrioError::IrioError;
 public:
	TerminalNotImplementedError() :
			IrioError("Terminal not implemented in the profile") {
	}
};

/**
 * Exception when a resource (Register/DMA) is not found
 * @ingroup Errors
 */
class ResourceNotFoundError: public IrioError {
	using IrioError::IrioError;
 public:
	ResourceNotFoundError() :
			IrioError("Resource not found") {
	}

	/**
	 * Exception when a resource (Register/DMA) is not found.
	 * Indicating that the resource number provided does not match any
	 * enumeration resource.
	 * 
	 * @param resourceNumber	Number of resource 
	 * @param resourceName 		Number of the resource
	 */
	ResourceNotFoundError(const std::uint32_t resourceNumber,
			const std::string &resourceName) :
			IrioError(std::to_string(resourceNumber)
					+ " is not a valid " + resourceName + " resource") {	}
};

/**
 * Exception when FPGAVIVersion is not the one expected
 * @ingroup Errors
 */
class FPGAVIVersionMismatchError: public IrioError {
 public:
	FPGAVIVersionMismatchError() :
			IrioError("FPGAVIversion mismatch") {
	}

	/**
	 * Exception indicating that there is a version mismatch and which version
	 * have been found that do not match.
	 * 
	 * @param foundVersion 		Version found in the bitfile
	 * @param expectedVersion 	Expected version
	 
	 */
	FPGAVIVersionMismatchError(const std::string &foundVersion,
			const std::string &expectedVersion) :
			IrioError(
					"FPGAVIVserion mismatch (" + foundVersion + " != "
							+ expectedVersion + ")") {
	}
};

/**
 * Exception when the platform read from the FPGA does not match any of the
 * supported ones (See \ref platforms.h)
 * @ingroup Errors
 */
class UnsupportedPlatformError: public IrioError {
 public:
	UnsupportedPlatformError()
		: IrioError("Platform specified is not supported") {}
	/**
	 * Exception when the platform read from the FPGA does not match any of the
	 * supported ones. Indicated the platform found.
	 * 
	 * @param platform Invalid platform value
	 */
	explicit UnsupportedPlatformError(const std::uint8_t platform)
		: IrioError(std::to_string(platform) +
						"is not a supported platform value") {}
};

/**
 * Exception when the DevProfile read from the FPGA does not matches any of the supported ones or
 * if the profile is not supported by the platform
 *
 * @ingroup Errors
 */
class UnsupportedDevProfileError: public IrioError {
 public:
	UnsupportedDevProfileError() :
			IrioError("DevProfile specified is not supported") {
	}

	/**
	 * Invalid DevProfile. Error message specifies the DevProfiel found and the
	 * Platform being used
	 *
	 * @param devProfile 	Invalid DevProfile
	 * @param platform 		Platform on which the profile specified does not
	 * 						exist
	 */
	UnsupportedDevProfileError(const std::uint8_t devProfile,
			const std::uint8_t platform) :
			IrioError(
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
class InitializationTimeoutError: public IrioError {
 public:
	InitializationTimeoutError() :
			IrioError("InitDone not ready in time") {
	}
};

/**
 * Exception when a timeout occurs in a CL UART operation
 *
 * @ingroup Errors
 */
class CLUARTTimeout: public IrioError {
 public:
	CLUARTTimeout() :
			IrioError("CL UART timeout") {
	}
};

/**
 * Exception when the baud rate read is not a valid value
 *
 * @ingroup Errors
 */
class CLUARTInvalidBaudRate: public IrioError {
 public:
	CLUARTInvalidBaudRate() :
			IrioError("Invalid UART baud rate") {
	}
};

/**
 * Exception when the modules installed in the platform are not the ones expected or
 * the FPGA inform that there is a problem with them
 *
 * @ingroup Errors
 */
class ModulesNotOKError: public IrioError {
	using IrioError::IrioError;
};

/**
 * Exception when an error occurs while searching for the RIO Device
 *
 * @ingroup Errors
 */
class RIODiscoveryError: public IrioError {
	using IrioError::IrioError;
};

/**
 * Exception when the specified RIO Device is not found
 *
 * @ingroup Errors
 */
class RIODeviceNotFoundError: public IrioError {
 public:
	RIODeviceNotFoundError() :
			IrioError("") {
	}

	/**
	 * RIO Device nor found. Error message specified the serialNumber that was
	 * searched.
	 *
	 * @param serialNumber	Serial number of the not found RIO device
	 */
	explicit RIODeviceNotFoundError(const std::string &serialNumber) :
			IrioError("No RIO device with serial number " + serialNumber) {
	}
};

/**
 * Exception when an error returns from a NiFpga_* function
 *
 * @ingroup Errors
 */
class NiFpgaError: public IrioError {
	using IrioError::IrioError;
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
class DMAReadTimeout: public IrioError {
 public:
	/**
	 * Exception when a timeout expires while trying to read a DMA
	 * 
	 * @param nameTermDMA 	Name of the DMA that generated the timeout
	 * @param n 			Number of the DMA that generated the timeout
	 */
	DMAReadTimeout(const std::string &nameTermDMA, const std::uint32_t &n) :
			IrioError("Timeout reading " + nameTermDMA + std::to_string(n)) {
	}
};

/**
 * Exception when an error occurs while parsing the bitfile
 *
 * @ingroup Errors
 */
class BFPParseBitfileError: public IrioError {
 public:
	/**
	 * Exception class for errors that occur during bitfile parsing
	 *
	 * @param bitfile	Bitfile on which the error occurred
	 */
	explicit BFPParseBitfileError(const std::string &bitfile)
		: IrioError("Error parsing " + bitfile) {}

  /**
   * Exception class for errors that occur during bitfile parsing. Adds an error
   * description
   *
   * @param bitfile 		Bitfile on which the error occurred
   * @param errDescription 	Extra description of the problem
   */
  BFPParseBitfileError(const std::string &bitfile,
					   const std::string &errDescription)
	  : IrioError("Error parsing " + bitfile + ". Error: " + errDescription) {
  }
};

/**
 * Exception when trying to set an invalid AI coupling mode for
 * a module
 *
 * @ingroup Errors
 */
class UnsupportedAICouplingForModule: public IrioError {
 public:
	UnsupportedAICouplingForModule() :
			IrioError("Unsupported AI coupling mode for module") {
	}
};

}  // namespace errors
}  // namespace irio
