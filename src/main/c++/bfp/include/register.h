#pragma once

#include "resource.h"
#include <pugixml.hpp>

namespace bfp {

/**
 * Manages registers.
 */
class Register: public Resource {
public:
	/**
	 * Constructor
	 * @param _name			Name of the register
	 * @param _fpgaType		FPGA register type
	 * @param _elemType		Type of the element of the register
	 * @param _address		Address of the register
	 * @param _numElem		Number of elements of the register (Default:1)
	 */
	Register(
			const std::string &_name,
			const FpgaTypes &_fpgaType,
			const ElemTypes &_elemType,
			const std::uint32_t &_address,
			const size_t &_numElem = 1);

	/**
	 * Returns if the register is a control
	 * @return true if it is a control
	 */
	bool isControl() const;

	/**
	 * Returns if the register is an indicator
	 * @return true if it is an indicator
	 */
	bool isIndicator() const;

	/**
	 * Returns if the register is an array
	 * @return true if it is an array
	 */
	bool isArray() const;

private:
	bool m_isControl;
	bool m_isArray;
};

Register processRegister(const pugi::xml_node &registerNode, const std::uint32_t &baseAddress);

}

