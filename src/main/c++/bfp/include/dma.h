#pragma once

#include "resource.h"
#include <pugixml.hpp>

namespace bfp{

class DMA: public Resource{
public:
	DMA(const std::string &_name,
		const FpgaTypes &_fpgaType,
		const ElemTypes &_elemType,
		const std::uint32_t &_address,
		const size_t &_numElem = 1);

	std::uint32_t getDMANumber() const;

	bool isTargetToHost() const;
	bool isHostToTarget() const;

private:
	bool m_isTargetToHost;
};

DMA processDMA(const pugi::xml_node &dmaNode);

}


