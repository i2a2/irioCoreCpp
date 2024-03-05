#pragma once

namespace iriov2 {

/**
 * Data format of the data read
 *
 * @ingroup DMATerminals
 */
enum class FrameType {
	FormatA = 0,/**< Format used for DAQ samples */
	FormatB = 1 /**< Not yet defined */
};

}  // namespace iriov2
