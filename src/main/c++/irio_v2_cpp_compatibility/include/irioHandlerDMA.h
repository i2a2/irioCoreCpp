/****************************************************************************
 * \file irioHandlerDMA.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief DMA handler methods for IRIO Driver
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
 * Set up DMAs
 *
 * Configure host memory and FPGA registers for DMA transfer. DMA FIFO is started and cleaned.
 * To stop DMAs use irio_closeDMAsTtoHost().
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setUpDMAsTtoHost(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Stops DMAs
 *
 * Stops all DMAs found, freeing the allocated memory for the DMA transfer.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_closeDMAsTtoHost(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Clean DMAs
 *
 * <b>Clean the data from all DMAs</b> implemented in the RIO device. irio_cleanDMATtoHost() will be call to clean each DMA.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_cleanDMAsTtoHost(irioDrv_t* p_DrvPvt, TStatus* status);

/**
 * Clean DMA
 *
 * <b>Clean the data from specified (n) DMA</b>. Several read operations will be done over the selected DMA
 * using the given memory buffer. In order to work properly, DMA writing should be disabled
 * calling irio_setDMATtoHostEnable to set DMATtoHostEnable terminal to false.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA to clean
 * @param[in] cleanbuffer buffer to be used for DMA reads.
 * @param[in] buffersize size of the clean buffer.
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_cleanDMATtoHost(irioDrv_t* p_DrvPvt, int n,uint64_t* cleanbuffer, size_t buffersize, TStatus* status);

/**
 * Get DMA Overflow
 *
 * Reads the value of the FPGA DMA Overflow register.
 * The n'th bit of the register corresponds to the overflow value of the n'th DMA.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Value of the overflow bit from DMAsOverflow register. Non-zero value means overflow in at least one DMA.
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHostOverflow(irioDrv_t* p_DrvPvt,int32_t* value, TStatus* status);

/**
 * Read the sampling rate of a DMA
 *
 * Read the sampling rate value of a DMA from the FPGA.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n  Number of the DMA which sampling rate will be read (DMATtoHOSTSamplingRaten)
 * @param[out] value  Current value of the DMA sampling rate
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHostSamplingRate(irioDrv_t* p_DrvPvt, int n,int32_t *value, TStatus* status);

/**
 * Set the sampling rate of a DMA
 *
 * Write the given value to a DMA Sampling Rate port.
 * No range check or conversion is performed on the given value.
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n  Number of the DMA which sampling rate will be written (DMATtoHOSTSamplingRaten)
 * @param[in] value Sampling rate to set
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setDMATtoHostSamplingRate(irioDrv_t* p_DrvPvt, int n,int32_t value, TStatus* status);

/**
 * Get DMA group enable
 *
 * Reads from the FPGA whether a DMA channel group is enabled or not.
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n  Number of the DMA which enable signal will be read (DMATtoHOSTEnablen)
 * @param[out] value  Current value of the DMA enable
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHostEnable(irioDrv_t* p_DrvPvt, int n,int32_t* value, TStatus* status);

/**
 * Enables or disables a DMA channel group
 *
 * Writes in a FPGA register to enable or disable a DMA channel group.
 * Errors may occur if the port was not found or while writing in the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the DMA to enable/disable (DMATtoHOSTEnablen)
 * @param[in] value 0=Disable DMA, 1=Enable DMA
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setDMATtoHostEnable(irioDrv_t* p_DrvPvt, int n,int32_t value, TStatus* status);

/**
 * Reads data from the DMA
 *
 * Reads data blocks from the specified data DMA if they are available. If there are less blocks
 * than requested nothing is read. The size in DMA words of a data block depends on
 * the frame type and block size given by the FPGA registers DMATtoHOSTFrameType and
 * DMATtoHOSTBlockNWords respectively (read into irioDrv_t::DMATtoHOSTFrameType and
 * irioDrv_t::DMATtoHOSTBlockNWords at irio_findDAQDMAs()).
 * Errors may occur if one of the needed ports were not found or while reading from the ports.
 *
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] NBlocks number of data blocks to read.
 * @param[in] n Number of Number of the DMA where data should be read
 * @param[out] data Previously allocated buffer where data read will be stored
 * @param[out] elementsRead number of elements read. Can be 0 or NBlocks
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHostData(irioDrv_t* p_DrvPvt, int NBlocks, int n, uint64_t *data, int* elementsRead, TStatus* status);

/**
 * Reads data from the DMA using a timeout parameter
 *
 * Reads data blocks from the specified data DMA if they are available inside an window timeout time.
 * If there are less blocks than requested IRIO timeout warning is given. The size in DMA words of a data block depends on
 * the frame type and block size given by the FPGA registers DMATtoHOSTFrameType and
 * DMATtoHOSTBlockNWords respectively (read into irioDrv_t::DMATtoHOSTFrameType and
 * irioDrv_t::DMATtoHOSTBlockNWords at irio_findDAQDMAs()).
 * Errors may occur if one of the needed ports were not found or while reading from the ports.
 *
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] NBlocks number of data blocks to read.
 * @param[in] n Number of Number of the DMA where data should be read
 * @param[in] timeout Time function waits until FPGA fills DMA buffer
 * @param[out] data Previously allocated buffer where data read will be stored
 * @param[out] elementsRead number of elements read. Can be 0 or NBlocks
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHostData_timeout(irioDrv_t* p_DrvPvt, int NBlocks, int n, uint64_t *data, int* elementsRead, uint32_t timeout, TStatus* status);

/**
 * Reads image from the DMA
 *
 * Reads one image from the specified image DMA if it is available.If there is not
 * a complete image ready to be read, nothing is read. The size of an image
 * depends on imageSize (given as parameter) and the pixel deep, specified by the DMATtoHOSTSampleSize FPGA register
 * (read into irioDrv_t::DMATtoHOSTSampleSize at irio_findIMAQDMAs()). Image data
 * (including frame counter and timestamp) will be read if the image frame type is 1
 * (DMATtoHOSTFrameType read into irioDrv_t::DMATtoHOSTFrameType at irio_findIMAQDMAs()).
 * Errors may occur if one of the needed ports were not found or while reading from the ports.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] imageSize Size of the image in pixels
 * @param[in] n Number of Number of the DMA where data should be read
 * @param[out] data Previously allocated buffer where data read will be stored
 * @param[out] elementsRead number of elements read. Can be 0 or imageSize
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHostImage(irioDrv_t* p_DrvPvt, int imageSize, int n, uint64_t *data, int* elementsRead, TStatus* status);

/**
 *
 * Reads size of DMA data blocks in terms of DMA words
 *
 * @param[in]  p_DrvPvt 	Pointer to the driver session structure
 * @param[out] Nwords       Numbers of blocks readed
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHOSTBlockNWords(irioDrv_t* p_DrvPvt, uint16_t* Nwords, TStatus* status);

/**
 * Reads number of Channels per DMA
 *
 * @param[in]  p_DrvPvt 	Pointer to the driver session structure
 * @param[out] NCh          Channel of the FPGA where user wants to read
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDMATtoHOSTNCh(irioDrv_t* p_DrvPvt, uint16_t* NCh, TStatus* status);

#ifdef __cplusplus
}
#endif
