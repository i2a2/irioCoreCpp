/****************************************************************************
 * \file irioHandlerDigital.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Digital I/O handler methods for IRIO Driver
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Read a Digital input
 *
 * Reads the value from a digital input port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the digital input port to read (DIn)
 * @param[out] value Current value of the digital input
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getDI(const irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status);

/**
 * Read an auxiliary digital input
 *
 * Reads the value from an auxiliary digital input port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary digital input port to read (auxDIn)
 * @param[out] value  Current value of the auxiliary digital input
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getAuxDI(const irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status);

/**
 * Reads a digital output
 *
 * Reads the value from a digital output port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the digital output port to read (DOn)
 * @param[out] value  Current value of the digital output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getDO(const irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status);

/**
 * Write a digital output
 *
 * Writes the given value in a digital output port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the digital output to write (DOn)
 * @param[in] value Value to set. Will be converted to NiFpga_Bool (0=NiFpga_False, other=NiFpga_True)
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_setDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status);

/**
 * Read an auxiliary digital output
 *
 * Reads the value from the auxiliary digital output port.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary digital output port to read (auxDOn)
 * @param[out] value  Current value of the auxiliary digital output
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getAuxDO(const irioDrv_t *p_DrvPvt, int n, int32_t *value, TStatus *status);

/**
 * Write an auxiliary digital output
 *
 * Writes the given value in an auxiliary digital output port.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the auxiliary digital output to write (auxDOn)
 * @param[in] value Value to set. Will be converted to NiFpga_Bool (0=NiFpga_False, other=NiFpga_True)
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_setAuxDO(irioDrv_t *p_DrvPvt, int n, int32_t value, TStatus *status);

/**
 * Returns the number of digital inputs found in the FPGA
 *
 * @param[in] p_DrvPvt	Pointer to the driver session structure
 * @param[out] value	Pointer to store the number of resources found
 * @param[out] status   Warning and error messages produced during the execution of
 * 						this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getNumDI(const irioDrv_t *p_DrvPvt, size_t *value, TStatus *status);

/**
 * Returns the number of digital outputs found in the FPGA
 *
 * @param[in] p_DrvPvt	Pointer to the driver session structure
 * @param[out] value	Pointer to store the number of resources found
 * @param[out] status   Warning and error messages produced during the execution of
 * 						this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getNumDO(const irioDrv_t *p_DrvPvt, size_t *value, TStatus *status);

/**
 * Returns the number of aux digital inputs found in the FPGA
 *
 * @param[in] p_DrvPvt	Pointer to the driver session structure
 * @param[out] value	Pointer to store the number of resources found
 * @param[out] status   Warning and error messages produced during the execution of
 * 						this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getNumAuxDI(const irioDrv_t *p_DrvPvt, size_t *value, TStatus *status);

/**
 * Returns the number of aux digital outputs found in the FPGA
 *
 * @param[in] p_DrvPvt	Pointer to the driver session structure
 * @param[out] value	Pointer to store the number of resources found
 * @param[out] status   Warning and error messages produced during the execution of
 * 						this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 *
 * @ingroup IrioCoreCompatible 
 */
int irio_getNumAuxDO(const irioDrv_t *p_DrvPvt, size_t *value, TStatus *status);


#ifdef __cplusplus
}
#endif
