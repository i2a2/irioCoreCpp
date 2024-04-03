#include "dma.h"

namespace irio {
namespace bfp {

DMA::DMA(const std::string &_name, const FpgaTypes &_fpgaType,
		const ElemTypes &_elemType, const std::uint32_t &_address,
		const size_t &_numElem) :
		Resource(_name, _fpgaType, _elemType, _address, _numElem) {
	m_isTargetToHost = _fpgaType == FpgaTypes::FpgaType_DMATtH;
}

std::uint32_t DMA::getDMANumber() const {
	return getAddress();
}

bool DMA::isTargetToHost() const {
	return m_isTargetToHost;
}

bool DMA::isHostToTarget() const {
	return !m_isTargetToHost;
}

DMA DMA::processDMA(const pugi::xml_node &dmaNode) {
	std::string name;
	FpgaTypes fpgaType;
	ElemTypes elemType;
	std::uint32_t address;
	size_t numElem;

	bool isTtH;
	std::string typeName;

	name = dmaNode.attribute("name").as_string();
	address = dmaNode.child("Number").text().as_uint();

	typeName =
			dmaNode.select_node("DataType/SubType").node().text().as_string();
	elemType = getElemTypeFromStr(typeName);

	isTtH = dmaNode.child("Direction").text().as_string()
			== std::string("TargetToHost");
	fpgaType = isTtH ? FpgaTypes::FpgaType_DMATtH : FpgaTypes::FpgaType_DMAHtT;

	numElem = dmaNode.child("NumberOfElements").text().as_uint();

	return DMA(name, fpgaType, elemType, address, numElem);
}

}  // namespace bfp
}  // namespace iriov2
