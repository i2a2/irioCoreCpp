#pragma once

#include "terminals/impl/terminalsFlexRIOImpl.h"

namespace iriov2 {
/**
 * Class managing all terminals specific to
 * FlexRIO devices
 *
 * @ingroup Terminals
 * @ingroup FlexRIO
 */
class TerminalsFlexRIO{
public:
	/**
	 * Constructor.
	 * Manages finding the related terminals specific to FlexRIO devices.
	 *
	 * @throw iriov2::errors::ResourceNotFoundError		Some of the necessary resources were not found in the bitfile
	 *
	 * @param parsedBitfile Parsed bitfile with the FlexRIO specific terminals
	 * @param session		NiFpga_Session to be used in NiFpga related functions
	 */
	TerminalsFlexRIO(const bfp::BFP &parsedBitfile, const NiFpga_Session &session);

	TerminalsFlexRIO(const TerminalsFlexRIO &other);

	/**
	 * Returns whether the adapter connected in the FPGA is correct
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return true if no problem found, false otherwise
	 */
	bool getRIOAdapterCorrect() const;

	/**
	 * Return the ID of the module connected to the FlexRIO device
	 * @throw iriov2::errors::NiFpgaError	Error occurred in an FPGA operation
	 *
	 * @return ID of the module connected to the FlexRIO device
	 */
	std::uint32_t getInsertedIOModuleID() const;

private:
	std::shared_ptr<TerminalsFlexRIOImpl> m_impl;

};

}
