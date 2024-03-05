#include "register.h"
#include <exception>

namespace iriov2 {
namespace bfp {

Register::Register(const std::string &_name, const FpgaTypes &_fpgaType,
		const ElemTypes &_elemType, const std::uint32_t &_address,
		const size_t &_numElem) :
		Resource(_name, _fpgaType, _elemType, _address, _numElem) {
	m_isArray = fpgaType == FpgaTypes::FpgaType_ArrayControl
			|| fpgaType == FpgaTypes::FpgaType_ArrayIndicator;
	m_isControl = fpgaType == FpgaTypes::FpgaType_ArrayControl
			|| fpgaType == FpgaTypes::FpgaType_Control;
}

bool Register::isArray() const {
	return m_isArray;
}
bool Register::isControl() const {
	return m_isControl;
}
bool Register::isIndicator() const {
	return !m_isControl;
}

Register Register::processRegister(const pugi::xml_node &registerNode,
		const std::uint32_t &baseAddress) {
	std::string name;
	FpgaTypes fpgaType;
	ElemTypes elemType;
	std::uint32_t address;
	size_t numElem;

	bool isIndicator;
	pugi::xml_node datatypeNode;
	std::string typeName;

	name = registerNode.child("Name").text().as_string();
	isIndicator = registerNode.child("Indicator").text().as_bool();
	address = baseAddress + registerNode.child("Offset").text().as_uint();

	datatypeNode = registerNode.child("Datatype").first_child();
	if (std::string(datatypeNode.name()) == "Array") {
		fpgaType =
				isIndicator ?
						FpgaTypes::FpgaType_ArrayIndicator :
						FpgaTypes::FpgaType_ArrayControl;
		numElem = datatypeNode.child("Size").text().as_uint();
		typeName = std::string(datatypeNode.child("Type").first_child().name());
	} else {
		fpgaType =
				isIndicator ?
						FpgaTypes::FpgaType_Indicator :
						FpgaTypes::FpgaType_Control;
		numElem = 1;
		typeName = std::string(datatypeNode.name());
	}

	elemType = getElemTypeFromStr(typeName);

	return Register(name, fpgaType, elemType, address, numElem);
}
}  // namespace bfp
}  // namespace iriov2
