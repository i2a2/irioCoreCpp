#include "elemtypes.h"

namespace iriov2{
namespace bfp{

ElemTypes getElemTypeFromStr(const std::string &typeName) {
	if (typeName == "Boolean")
		return ElemTypes::Bool;
	else if (typeName == "I8")
		return ElemTypes::I8;
	else if (typeName == "U8" || typeName == "EnumU8")
		return ElemTypes::U8;
	else if (typeName == "I16")
		return ElemTypes::I16;
	else if (typeName == "U16" || typeName == "EnumU16")
		return ElemTypes::U16;
	else if (typeName == "I32")
		return ElemTypes::I32;
	else if (typeName == "U32" || typeName == "EnumU32")
		return ElemTypes::U32;
	else if (typeName == "I64")
		return ElemTypes::I64;
	else if (typeName == "U64" || typeName == "EnumU64")
		return ElemTypes::U64;
	else {
		// SGL and FXP are supported by LabVIEW FPGA but not the C API, so we
		// mark them as unsupported and ignore them for now. Anything else we
		// didn't expect is an error.
		return ElemTypes::Unsupported;
	}
}
}
}
