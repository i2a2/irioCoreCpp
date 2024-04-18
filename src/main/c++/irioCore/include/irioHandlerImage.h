/****************************************************************************
 * \file irioHandlerImage.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief CameraLink handler methods for IRIO driver.
 * \date Sept., 2010 (Last Review July 2015)
 * \copyright (C) 2010-2015 Universidad Politécnica de Madrid (UPM)
 * \par License:
 * 	\n This project is released under the GNU Public License version 2.
 * \cond
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * \endcond
 *****************************************************************************/

#pragma once

#include "irioDataTypes.h"

#include <stdint.h>

/**
 * Enum Type for CameraLink supported configurations
 *
 * Specify the result of a method call
 */
typedef enum {
	CL_BASE,  //!< CameraLink Base mode
	CL_MEDIUM,  //!< CameraLink Medium mode
	CL_FULL   //!< CameraLink Full mode
/*	CL_DUAL_BASE, // !< CameraLink DualBase mode Not supported yet
	CL_EXTENDED // !< CameraLink extended mode Not supported yet*/
} CL_Config_T;

/**
 * Enum Type for CameraLink supported extended modes
 * Note: Extended mode is not currently supported
 *
 * Specify the result of a method call
 */
typedef enum {
	CL_STANDARD,     //!< CameraLink Standard Mapping
	CL_BASLER_10T,   //!< CameraLink Basler 10 Tap Mapping
	CL_VOSSKUHLER_10T   //!< CameraLink Vosskuhler 10 Tap Mapping
} CL_SignalMapping_T;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Configure CameraLink frame grabber
 *
 * Configure CameraLink frame grabber with the camera parameters.
 * This configuration includes:
 * 	\n Set whether the CaneraLink signals FVAL, LVAL, DVAL, and SPARE are active high or active low.
 * 	\n Set whether the CameraLink control signals will be driven by the FPGA or not.
 * 	\n Set the scanning mode between regular and line scan.
 * 	\n Set the signal mapping when using extended configuration mode.
 * 	\n Set the configuration mode.
 * This configuration must be done before running the FPGA.
 *
 * Errors may occur if any of the ports were not found or while writing to the ports.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] fvalHigh 0=FVAL active low 1=FVAL active high
 * @param[in] lvalHigh 0=LVAL active low 1=LVAL active high
 * @param[in] dvalHigh 0=DVAL active low 1=DVAL active high
 * @param[in] spareHigh 0=SPARE active low 1=SPARE active high
 * @param[in] controlEnable 0=High impedance on conrol signals 1=Control signals driver by the FPGA.
 * @param[in] linescan 0=Regular scan (complete image) 1=Line scan
 * @param[in] signalMapping Signal map to be used.
 * @param[in] configuration Configuration to be used.
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_configCL(irioDrv_t *p_DrvPvt, int32_t fvalHigh, int32_t lvalHigh,
		int32_t dvalHigh, int32_t spareHigh, int32_t controlEnable,
		int32_t linescan, CL_SignalMapping_T signalMapping,
		CL_Config_T configuration, TStatus *status);

/**
 * Sends a UART message to the CameraLink camera
 *
 * Streams the given string to the CameraLink camera connected to the frame grabber.
 * Errors may occur if any of the necessary ports was not found or while reading/writing from/to the ports.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] msg String to stream
 * @param[in] msg_size Size of the string message
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_sendCLuart(irioDrv_t *p_DrvPvt, const char *msg, int msg_size,
					TStatus *status);

/**
 * Reads a message from the CameraLink camera. This function is unsafe!!
 *
 * This function is unsafe!! If a message exceed the length of the reserved
 * memory by \p data there is no check to avoid a buffer overflow.
 * Use \ref irio_getCLuartWithBufferSize insted. This funciton is maintaned 
 * only due to compatibility reasons with software previous to 1.2.0
 * 
 * Reads a message streamed from the CameraLink camera. This method blocks until
 * a message is read.
 *
 * The buffer \p data is an user allocated buffer
 * 
 * Errors may occur if any of the necessary ports was not found or while
 * reading/writing from/to the ports.
 * 
 * @deprecated 
 * This function is unsafe!! If a message exceed the length of the reserved
 * memory by \p data there is no check to avoid a buffer overflow.
 * Use \ref irio_getCLuartWithBufferSize insted. This funciton is maintaned 
 * only due to compatibility reasons with software previous to 1.2.0
 *	
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] data 	Previously allocated buffer where message read will be
 *						stored. Always ends in \0
 * @param[out] msg_size Size of the message read.
 * @param[out] status	Warning and error messages produced during the execution
 * 						of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getCLuart(const irioDrv_t *p_DrvPvt, char *data, int *msg_size,
				   TStatus *status)
	__attribute__((deprecated("Unsafe!! Use irio_getCLuartWithBufferSize")));

/**
 * Reads a message from the CameraLink camera
 *
 * Reads a message streamed from the CameraLink camera. This method blocks until
 * a message is read.
 *
 * The buffer \p data is an user allocated buffer of length \p data_size.
 *
 * Errors may occur if any of the necessary ports was not found or while
 * reading/writing from/to the ports.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] data_size Size of previously allocated buffer (data) where message
 * 						read will be stored
 * @param[out] data 	Previously allocated buffer where message read will be
 *						stored. Always ends in \0
 * @param[out] msg_size Size of the message read. This may be bigger than
 * 						\p data_size. However, the data written to \p data
 * 						will always be up to \p data_size bytes
 * @param[out] status	Warning and error messages produced during the execution
 * 						of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getCLuartWithBufferSize(const irioDrv_t *p_DrvPvt, int data_size, char *data, int *msg_size, TStatus *status);

/**
 * Read UART Baud Rate
 *
 * Reads the value from the CameraLink UART baud rate.
 * Errors may occur if any of the necessary ports was not found or while reading/writing from/to the ports.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value  Current value of baud rate. Values from 0 to 7 corresponds respectively to:9.6, 19.2, 38.4, 57.6, 115.2, 230.4, 460.8, and 921.6 (kbps)
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getUARTBaudRate(const irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status);

/**
 * Set UART Baud Rate
 *
 * Sets a new value for CameraLink UART baud rate.
 * Errors may occur if any of the necessary ports was not found or while reading/writing from/to the ports.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] value  Baud rate to set. Values from 0 to 7 corresponds respectively to:9.6, 19.2, 38.4, 57.6, 115.2, 230.4, 460.8, and 921.6 (kbps)
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setUARTBaudRate(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status);

/**
 * Read UART Break Indicator
 *
 * Reads the value from CameraLink UART break indicator port. A non-zero value returned indicates an interruption in the UART communication.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current value of UART break indicator
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getUARTBreakIndicator(const irioDrv_t *p_DrvPvt, int32_t *value,
		TStatus *status);

/**
 * Read UART Framing Error
 *
 * Reads the value from CameraLink UART framing error port. A non-zero value returned indicates problems while receiving UART data.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current value of UART framing error
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getUARTFramingError(const irioDrv_t *p_DrvPvt, int32_t *value,
							 TStatus *status);

int irio_getUARTFrammingError(const irioDrv_t *p_DrvPvt, int32_t *value,
							  TStatus *status)
	__attribute__((deprecated("Use irio_getUARTFramingError")));

/**
 * Read UART Overrun Error
 *
 * Reads the value from CameraLink UART overrun error port. A non-zero value returned indicates an overrun in a UART buffer, causing data loss.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current value of UART overrun error
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getUARTOverrunError(const irioDrv_t *p_DrvPvt, int32_t *value,
		TStatus *status);

#ifdef __cplusplus
}
#endif
