#pragma once

#include <cstdint>

namespace irio {
/**
 * Possible values of UART baud rates.
 */
enum class UARTBaudRates : std::uint8_t {
	BR96 = 0,	/**< Baud rate 9.6 kbps */
	BR192 = 1,	/**< Baud rate 19.2 kbps */
	BR384 = 2,	/**< Baud rate 38.4 kbps */
	BR576 = 3,	/**< Baud rate 57.6 kbps */
	BR1152 = 4, /**< Baud rate 115.2 kbps */
	BR2304 = 5, /**< Baud rate 230.4 kbps */
	BR4608 = 6, /**< Baud rate 460.8 kbps */
	BR9216 = 7	/**< Baud rate 921.6 kbps */
};


/**
 * Possible signal mappings for the Camera Link interface.
 */
enum class CLSignalMapping : std::uint8_t {
	STANDARD = 0,		/**< CameraLink Standard Mapping*/
	BASLER_10T = 1,		/**< CameraLink Basler 10 Tap Mapping*/
	VOSSKUHLER_10T = 2	/**< CameraLink Vosskulher 10 Tap Mapping*/
};

/**
 * Possible modes for the Camera Link interface.
 */
enum class CLMode : std::uint8_t {
	BASE = 0,	/**< CameraLink Base mode*/
	MEDIUM = 1,	/**< CameraLink Medium mode*/
	FULL = 2	/**< CameraLink Full mode*/
};

}  // namespace irio
