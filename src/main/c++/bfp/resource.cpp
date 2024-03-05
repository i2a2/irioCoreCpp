#include "resource.h"

namespace iriov2 {
namespace bfp {

Resource::Resource(const std::string &_name, const FpgaTypes &_fpgaType,
		const ElemTypes &_elemType, const std::uint32_t &_address,
		const size_t &_numElem) :
		name(_name), fpgaType(_fpgaType), elemType(_elemType), address(
				_address), numElem(_numElem) {
}
}  // namespace bfp
}  // namespace iriov2
