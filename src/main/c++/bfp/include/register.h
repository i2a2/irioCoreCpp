#pragma once

#include "resource.h"
#include <pugixml.hpp>



namespace bfp{

class Register: public Resource{
public:
	Register(	const std::string &_name,
				const FpgaTypes &_fpgaType,
				const ElemTypes &_elemType,
				const std::uint32_t &_address,
				const size_t &_numElem = 1);

	bool isControl() const;
	bool isIndicator() const;
	bool isArray() const;

private:
	bool m_isControl;
	bool m_isArray;
};

Register processRegister(const pugi::xml_node& registerNode, const std::uint32_t& baseAddress);

}


