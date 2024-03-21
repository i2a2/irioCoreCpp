#pragma once

namespace iriov2 {
namespace bfp {

/**
 * Type of Resource possible in the Bitfile
 *
 * @ingroup BFP
 */
enum class FpgaTypes : std::uint8_t {
	FpgaType_Control, /**< FpgaType_Control */
	FpgaType_Indicator, /**< FpgaType_Indicator */
	FpgaType_ArrayControl, /**< FpgaType_ArrayControl */
	FpgaType_ArrayIndicator,/**< FpgaType_ArrayIndicator */
	FpgaType_DMATtH, /**< FpgaType_DMATtH */
	FpgaType_DMAHtT /**< FpgaType_DMAHtT */
};
}  // namespace bfp
}  // namespace iriov2
