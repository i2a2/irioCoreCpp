#pragma once

#include <string>
#include <pugixml.hpp>
#include "resource.h"


namespace iriov2 {
namespace bfp {

/**
 * Class holding information about one DMA
 *
 * @ingroup BFP
 */
class DMA: public Resource {
 public:
	/**
	 * Contructor.
	 *
	 * Initializes the information about the DMA
	 *
	 * @param _name			Name of the DMA
	 * @param _fpgaType		FPGA DMA type
	 * @param _elemType		Type of the DMA's elements
	 * @param _address		Address/Number of the DMA
	 * @param _numElem		Number of the elements in the DMA
	 */
	DMA(const std::string &_name, const FpgaTypes &_fpgaType,
			const ElemTypes &_elemType, const std::uint32_t &_address,
			const size_t &_numElem = 1);

	DMA() = default;

	/**
	 * Creates a DMA object from an XML node
	 *
	 * @param dmaNode	XML node with the info of the DMA
	 * @return	DMA constructed with the information in the XML
	 */
	static DMA processDMA(const pugi::xml_node &dmaNode);

	/**
	 * Returns the number that identifies this DMA
	 *
	 * @return Identifier of the DMA
	 */
	std::uint32_t getDMANumber() const;

	/**
	 * Returns whether the DMA is Target to Host (TtH) or not
	 * @return True if it is TtH
	 */
	bool isTargetToHost() const;

	/**
	 * Returns whether the DMA is Host to Target (HtT) or not
	 * @return True if it is HtT
	 */
	bool isHostToTarget() const;

 private:
	bool m_isTargetToHost = false;
};

}  // namespace bfp
}  // namespace iriov2

