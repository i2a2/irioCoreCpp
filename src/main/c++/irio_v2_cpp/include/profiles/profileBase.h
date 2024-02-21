#pragma once

#include <memory>
#include <bfp.h>
#include <terminals/terminals.h>
#include <profiles/profileBaseTerminals.h>

namespace iriov2 {

/**
 * Base profile class.
 * Allows the user to access terminals for read/write operations.
 * If a terminal group is not in the profile,
 * an \ref iriov2::errors::TerminalNotImplementedError exception will be thrown.
 *
 * @ingroup Profiles
 */
class ProfileBase: public ProfileBaseTerminals {
public:
	/**
	 * All possible types of profiles
	 */
	enum PROFILE_ID {
		FLEXRIO_DAQ = 0,/**< Data acquisition profile for FlexRIO devices */
		FLEXRIO_IMAQ, /**< Image acquisition profile for FlexRIO devices */
		FLEXRIO_GPUDAQ, /**< Data acquisition in GPU profile for FlexRIO devices */
		FLEXRIO_GPUIMAQ,/**< Image acquisition in GPU profile for FlexRIO devices */
		CRIO_DAQ, /**< Data acquisition profile for cRIO devices */
		CRIO_IO, /**< Point by Point profile for cRIO devices */
		R_DAQ /**< Data acquisition profile for R Series devices */
	};

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
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @param parsedBitfile		Parsed bitfile to extract the information for the terminals
	 * @param session			Session for operations with the FPGA
	 * @param id				Identification of the profile
	 */
	ProfileBase(
			const bfp::BFP &parsedBitfile,
			const NiFpga_Session &session,
			const ProfileBase::PROFILE_ID &id);

	virtual ~ProfileBase() = default;

	/**
	 * Access to the analog group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Pointer to the analog terminals
	 */
	virtual const std::shared_ptr<const TerminalsAnalog> analog() const;

	/**
	 * Access to the digital group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Pointer to the digital terminals
	 */
	virtual const std::shared_ptr<const TerminalsDigital> digital() const;

	/**
	 * Access to the aux analog group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Pointer to the aux analog terminals
	 */
	virtual const std::shared_ptr<const TerminalsAuxAnalog> auxAnalog() const;

	/**
	 * Access to the aux digital group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Pointer to the aux digital terminals
	 */
	virtual const std::shared_ptr<const TerminalsAuxDigital> auxDigital() const;

	/**
	 * Access to the cRIO group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Pointer to the cRIO terminals
	 */
	virtual const std::shared_ptr<const TerminalscRIO> cRIO() const;

	/**
	 * Access to the FlexRIO group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 * @return Pointer to the FlexRIO terminals
	 */
	virtual const std::shared_ptr<const TerminalsFlexRIO> flexRIO() const;

	/**
	 * Access to the Signal Generation group terminals.
	 *
	 * @throw iriov2::errors::TerminalNotImplementedError	Terminals group not supported by the current profile
	 *
	 * @return Pointer to the Signal Generation terminals
	 */
	virtual const std::shared_ptr<const TerminalsSignalGeneration> signalGeneration() const;

	/**
	 * Profile type
	 */
	const ProfileBase::PROFILE_ID profileID;
};

}
