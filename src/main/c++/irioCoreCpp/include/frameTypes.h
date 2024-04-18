#pragma once

namespace irio {

/**
 * Data format of the data read
 *
 * @ingroup DMATerminals
 */
enum class FrameType : std::uint8_t {
	FormatA = 0,/**< Format used for DAQ samples */
	FormatB = 1 /**< Not yet defined */
};

}  // namespace irio
