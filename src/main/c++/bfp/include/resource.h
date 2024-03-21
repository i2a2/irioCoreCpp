#pragma once

#include <string>

#include "fpgatypes.h"
#include "elemtypes.h"

namespace iriov2 {
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

	std::uint32_t getAddress() const;
	ElemTypes getElemType() const;
	FpgaTypes getFpgaType() const;
	const std::string& getName() const;
	size_t getNumElem() const;

 private:
	std::string name = "";
	FpgaTypes fpgaType = FpgaTypes::FpgaType_Control;
	ElemTypes elemType = ElemTypes::Bool;
	std::uint32_t address = 0;
	size_t numElem = 0;
};
}  // namespace bfp
}  // namespace iriov2
