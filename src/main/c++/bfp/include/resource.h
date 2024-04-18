#pragma once

#include <string>

#include "fpgatypes.h"
#include "elemtypes.h"

namespace irio {
namespace bfp {

/**
 * Class holding information about one resource (Register or DMA)
 *
 * @ingroup BFP
 */
class Resource {
 public:
	/**
	 * Stores information about the resource
	 * 
	 * @param _name	Resource name
	 * @param _fpgaType	FPGA type (Control, Indicator, ArrayControl, ArrayIndicator, DMATtH, DMAHtT)
	 * @param _elemType	Element type
	 * @param _address	Address used to refer to the Resource. If Register then a device address, if DMA, its number
	 * @param _numElem	Number of elements of the Resource
	 */
	Resource(const std::string &_name, const FpgaTypes &_fpgaType,
			const ElemTypes &_elemType, const std::uint32_t &_address,
			const size_t &_numElem = 1);

	Resource() = default;

	/**
	 * Gets the address of the resource.
	 *
	 * @return The address of the resource.
	 */
	std::uint32_t getAddress() const;

	/**
	 * Gets the element type of the resource.
	 *
	 * @return The element type of the resource.
	 */
	ElemTypes getElemType() const;

	/**
	 * Gets the FPGA type of the resource.
	 *
	 * @return The FPGA type of the resource.
	 */
	FpgaTypes getFpgaType() const;

	/**
	 * Gets the name of the resource.
	 *
	 * @return The name of the resource.
	 */
	const std::string &getName() const;

	/**
	 * Gets the number of elements of the resource.
	 *
	 * @return The number of elements of the resource.
	 */
	size_t getNumElem() const;

 private:
	/// Name of the resource
	std::string name = "";
	/// FPGA type of the resource
	FpgaTypes fpgaType = FpgaTypes::FpgaType_Control;
	/// Element type of the resource
	ElemTypes elemType = ElemTypes::Bool;
	/// Address of the resource (including the base address)
	std::uint32_t address = 0;
	/// Number of elements of the resource
	size_t numElem = 0;
};
}  // namespace bfp
}  // namespace irio
