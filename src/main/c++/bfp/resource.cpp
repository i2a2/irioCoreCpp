#include "resource.h"

namespace irio {
namespace bfp {

Resource::Resource(const std::string &_name, const FpgaTypes &_fpgaType,
		const ElemTypes &_elemType, const std::uint32_t &_address,
		const size_t &_numElem) :
		name(_name), fpgaType(_fpgaType), elemType(_elemType), address(
				_address), numElem(_numElem) {
}

std::uint32_t Resource::getAddress() const {
	return address;
}

ElemTypes Resource::getElemType() const {
	return elemType;
}

FpgaTypes Resource::getFpgaType() const {
	return fpgaType;
}

const std::string& Resource::getName() const {
	return name;
}

size_t Resource::getNumElem() const {
	return numElem;
}

}  // namespace bfp
}  // namespace iriov2
