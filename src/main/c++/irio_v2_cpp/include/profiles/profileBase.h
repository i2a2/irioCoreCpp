#pragma once

#include <memory>
#include <bfp.h>
#include <terminals/terminals.h>
#include <profiles/profileBaseTerminals.h>


namespace iriov2{

/**
 * Base profile class.\n
 * Allows the user to access terminals for read/write operations.\n
 * If a terminal group is not in the profile, an exception will be thrown.
 */
class ProfileBase: public ProfileBaseTerminals
{
public:
	/**
	 * All possible types of profiles
	 */
	enum PROFILE_ID{
		FLEXRIO_DAQ = 0,/**< FLEXRIO_DAQ */
		FLEXRIO_IMAQ,   /**< FLEXRIO_IMAQ */
		FLEXRIO_GPUDAQ, /**< FLEXRIO_GPUDAQ */
		FLEXRIO_GPUIMAQ,/**< FLEXRIO_GPUIMAQ */
		CRIO_DAQ,       /**< CRIO_DAQ */
		CRIO_IO,        /**< CRIO_IO */
		R_DAQ           /**< R_DAQ */
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
	 * Allows the user to access terminals for read/write operations.\n
	 *
	 * @param parsedBitfile		Parsed bitfile to extract the information for the terminals
	 * @param session			Session for operations with the FPGA
	 * @param id				Identification of the profile
	 */
	ProfileBase(const bfp::BFP &parsedBitfile, const NiFpga_Session &session, const ProfileBase::PROFILE_ID &id);
    
	virtual ~ProfileBase() = default;

    /**
     * Access to the analog group terminals.\n
     * If the profile does not have them, an exception will be thrown.
     *
     * @return Pointer to the analog terminals
     */
    virtual const std::shared_ptr<const TerminalsAnalog> analog();

    /**
	 * Access to the digital group terminals.\n
	 * If the profile does not have them, an exception will be thrown.
	 *
	 * @return Pointer to the digital terminals
	 */
    virtual const std::shared_ptr<const TerminalsDigital> digital();

    /**
	 * Access to the aux analog group terminals.\n
	 * If the profile does not have them, an exception will be thrown.
	 *
	 * @return Pointer to the aux analog terminals
	 */
    virtual const std::shared_ptr<const TerminalsAuxAnalog> auxAnalog();

    /**
	 * Access to the aux digital group terminals.\n
	 * If the profile does not have them, an exception will be thrown.
	 *
	 * @return Pointer to the aux digital terminals
	 */
    virtual const std::shared_ptr<const TerminalsAuxDigital> auxDigital();

    /**
	 * Access to the cRIO group terminals.\n
	 * If the profile does not have them, an exception will be thrown.
	 *
	 * @return Pointer to the cRIO terminals
	 */
	virtual const std::shared_ptr<const TerminalscRIO> cRIO();

	/**
	 * Access to the FlexRIO group terminals.\n
	 * If the profile does not have them, an exception will be thrown.
	 *
	 * @return Pointer to the FlexRIO terminals
	 */
	virtual const std::shared_ptr<const TerminalsFlexRIO> flexRIO();

	/**
	 * Access to the Signal Generation group terminals.\n
	 * If the profile does not have them, an exception will be thrown.
	 *
	 * @return Pointer to the Signal Generation terminals
	 */
	virtual const std::shared_ptr<const TerminalsSignalGeneration> signalGeneration();

    /**
     * Profile type
     */
    const ProfileBase::PROFILE_ID profileID;
};

}
