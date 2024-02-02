#pragma once

namespace bfp{

/**
 * Type of Resource possible in the Bitfile
 */
enum FpgaTypes {
	FpgaType_Control,       /**< FpgaType_Control */
	FpgaType_Indicator,     /**< FpgaType_Indicator */
	FpgaType_ArrayControl,  /**< FpgaType_ArrayControl */
	FpgaType_ArrayIndicator,/**< FpgaType_ArrayIndicator */
	FpgaType_DMATtH,        /**< FpgaType_DMATtH */
	FpgaType_DMAHtT         /**< FpgaType_DMAHtT */
};
}


