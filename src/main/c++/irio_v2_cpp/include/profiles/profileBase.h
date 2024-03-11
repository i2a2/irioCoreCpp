#pragma once

#include <terminals/terminals.h>

namespace iriov2 {

/**
 * All possible types of profiles
 */
enum class PROFILE_ID {
	FLEXRIO_CPUDAQ = 0,/**< Data acquisition in CPU profile for FlexRIO devices */
	FLEXRIO_CPUIMAQ, /**< Image acquisition in CPU profile for FlexRIO devices */
	FLEXRIO_GPUDAQ, /**< Data acquisition in GPU profile for FlexRIO devices */
	FLEXRIO_GPUIMAQ,/**< Image acquisition in GPU profile for FlexRIO devices */
	CRIO_DAQ, /**< Data acquisition profile for cRIO devices */
	CRIO_IO, /**< Point by Point profile for cRIO devices */
	R_DAQ /**< Data acquisition profile for R Series devices */
};




/**
 * Base profile class.
 *
 * Allows the user to access terminals for read/write operations.
 * If a terminal group is not in the profile,
 * an \ref iriov2::errors::TerminalNotImplementedError exception will be thrown.
 *
 * @ingroup Profiles
 */
class ProfileBase {
 public:
	/**
	 * Value of DevProfile when DAQ
	 */
	static const std::uint8_t PROFILE_VALUE_DAQ = 0;
	/**
	 * Value of DevProfile when IMAQ
	 */
	static const std::uint8_t PROFILE_VALUE_IMAQ = 1;
	/**
	 * Value of DevProfile when DAQGPU
	 */
	static const std::uint8_t PROFILE_VALUE_DAQGPU = 2;
	/**
	 * Value of DevProfile when IMAQGPU
	 */
	static const std::uint8_t PROFILE_VALUE_IMAQGPU = 3;
	/**
	 * Value of DevProfile when IO
	 */
	static const std::uint8_t PROFILE_VALUE_IO = 1;

	/**
	 * Allows the user to access terminals for read/write operations.
	 *
	 * @param id	Identification of the profile
	 */
	explicit ProfileBase(const PROFILE_ID &id);

	virtual ~ProfileBase() = default;

	/**
	 * Access to the analog group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Analog terminals
	 */
	virtual TerminalsAnalog analog() const;

	/**
	 * Access to the digital group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Digital terminals
	 */
	virtual TerminalsDigital digital() const = 0;

	/**
	 * Access to the aux analog group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Aux analog terminals
	 */
	virtual TerminalsAuxAnalog auxAnalog() const = 0;

	/**
	 * Access to the aux digital group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Aux digital terminals
	 */
	virtual TerminalsAuxDigital auxDigital() const = 0;

	/**
	 * Access to the cRIO group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return cRIO terminals
	 */
	virtual TerminalscRIO cRIO() const;

	/**
	 * Access to the FlexRIO group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 * @return FlexRIO terminals
	 */
	virtual TerminalsFlexRIO flexRIO() const;

	/**
	 * Access to the Signal Generation group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Signal Generation terminals
	 */
	virtual TerminalsSignalGeneration signalGeneration() const;

	/**
	 * Access to the DMA DAQ group terminals. Depending on the profile configured, this can be terminals
	 * for CPU DAQ or GPU DAQ. The methods and functionality implemented for each case behaves the same.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return DMA DAQ terminals
	 */
	virtual TerminalsDMADAQ daq() const;

	/**
	 * Profile type
	 */
	const PROFILE_ID profileID;
};

}  // namespace iriov2
