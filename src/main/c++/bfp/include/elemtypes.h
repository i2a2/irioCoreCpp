#pragma once

#include <string>

namespace iriov2 {
namespace bfp {

/**
 * Supported types for the elements of the different Resources in the Bitfile
 *
 * @ingroup BFP
 */
enum class ElemTypes {
	Bool, /**< Bool */
	I8, /**< I8 */
	U8, /**< U8 */
	I16, /**< I16 */
	U16, /**< U16 */
	I32, /**< I32 */
	U32, /**< U32 */
	I64, /**< I64 */
	U64, /**< U64 */
	Unsupported/**< Unsupported */
};

/**
 * Parses a string and outputs a ElemType if exists an equivalence.
 * If it does not match, return ElemTypes::Unsupported
 *
 * @param typeName	String with the type
 * @return The equivalent ElemTypes
 *
 * @ingroup BFP
 */
ElemTypes getElemTypeFromStr(const std::string &typeName);

}  // namespace bfp
}  // namespace iriov2
