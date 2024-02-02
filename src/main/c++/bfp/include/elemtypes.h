#pragma once

#include <string>

namespace bfp{

/**
 * Supported types for the elements of the different Resources in the Bitfile
 */
enum ElemTypes{
	Bool,      /**< Bool */
	I8,        /**< I8 */
	U8,        /**< U8 */
	I16,       /**< I16 */
	U16,       /**< U16 */
	I32,       /**< I32 */
	U32,       /**< U32 */
	I64,       /**< I64 */
	U64,       /**< U64 */
	Unsupported/**< Unsupported */
};

ElemTypes getElemTypeFromStr(const std::string &typeName);

}


