/**************************************************************************
 * \file irioDriver.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Initialization and common resources access methods for IRIO Driver
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

#define IRIOVERSION "{IRIOVERSIONVALUE}"   //!< Actual IRIO version
#include "irioDataTypes.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Method to initialize the NIRIO driver
 *
 * At the beginning of this method, the memory pointed by p_DrvPvt will be set to zero.
 *
 * This method tries to find the RIO device given as parameter and start a session with the target,
 * loading NiFpga library (if needed) and downloading the specified bitfile.
 * The RIO target will be identified by its model and s/n.
 *
 * If the target is found, will identify the resources present in the bitfile and initialize the
 * proper structures to handle them. The resources allocated in this call are freed with a call
 * to irio_closeDriver().
 *
 * The bitfile won't be running after this call, user must call to irio_setFPGAStart() to run the bitfile.
 * This allows the user to make some configurations prior to the start of the execution, specially
 * needed for some FlexRIO adapter modules.
 *
 * In case of error, status structure will be written properly, showing an error message with the
 * error codes (if any) in status->msg. The error codes shown this way correspond to NiFpga
 * library error codes.
 *
 * If any of the mandatory resources are missing the method will return error but its irioDrv_t->session parameter
 * will remain open, so user should close the session manually because IRIO allows to use its methods
 * if the session is open.
 *
 *	Some important fields modified in this call are:
 *		\n irioDrv_t::fpgaRIO
 *		\n irioDrv_t::session
 *		\n irioDrv_t::NiFpgaDriverType
 *		\n irioDrv_t::platform
 *		\n irioDrv_t::devProfile
 *		\n irioDrv_t::Fref
 *		\n ... and struct fields corresponding to found resources.
 *
 * @param[in] appCallID			Name for this irioDriver session.
 * @param[in] DeviceSerialNumber S/n of the RIO target device
 * @param[in] RIODeviceModel	Model number of the RIO target device
 * @param[in] projectName		Part of the name of the bitfile to be downloaded into the FPGA
 * @param[in] FPGAversion 		Version of the bitfile (e.g. [1,0] for v1.0). Must match with the value of the FPGA register
 * @param[in] verbosity			Indicates whether or not should the driver print trace messages.
 * @param[in] headerDir			Path where to search for the header file corresponding to the bitfile to be downloaded
 * @param[in] bitfileDir		Path where to search for the bitfile to be downloaded
 * @param[out] p_DrvPvt		Pointer to the driver structure. Will be initialized with the resources found.
 * @param[out] status		Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_initDriver(const char *appCallID, const char *DeviceSerialNumber,
		const char *RIODeviceModel, const char *projectName,
		const char *FPGAversion, int verbosity, const char *headerDir,
		const char *bitfileDir, irioDrv_t *p_DrvPvt, TStatus *status);

/**
 * Close the driver session and free allocated resources
 *
 * Safely free all the dynamic memory that might be allocated inside driver struct,
 * closes FPGA session, and unload NiFgpa library (if needed).
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure to be closed.
 * @param[in] mode 	0 reset the FPGA. 1 non reset .
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_closeDriver(irioDrv_t *p_DrvPvt, uint32_t mode, TStatus *status);

/**
 * Set FlexRIO Adapter module Analog input coupling mode
 *
 * Depending on the NI 5761 FlexRIO adapter module (AC or DC) used, this method
 * must be called according to. NI5761 coupling mode is not configurable and NI provides different part numbers for this
 * 781287-01 for AC coupled and 781287-02 for DC coupled. ITER HW catalog only has AC coupled version.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] value coupling mode:IRIO_coupling_AC(0) AC, IRIO_coupling_DC(1) DC
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setAICoupling(irioDrv_t *p_DrvPvt, TIRIOCouplingMode value,
		TStatus *status);

/**
 * Get Adapter module Analog input coupling mode value
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value coupling mode: IRIO_coupling_AC(0) AC, IRIO_coupling_DC(1) DC, IRIO_coupling_NULL (2)
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getAICoupling(const irioDrv_t *p_DrvPvt, TIRIOCouplingMode *value,
		TStatus *status);

/**
 * Method to retrieve the version number of IRIO library
 *
 * @param[out] version Current library version
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getVersion(char *version, const TStatus *status);

/**
 * Run the bitfile downloaded in the FPGA and perform initialization check
 *
 * The initialization check consist of:
 *		\n Waits until the target signals the end of its initialization (max. 5s).
 *		\n Checks the IO modules connected to the target.
 *		\n Stops data acquisition
 *
 * This method can not be used to stop the FPGA. The FPGA can only be started once.
 *
 * Some important fields modified in this call are:
 *		\n irioDrv_t::initDone
 *		\n irioDrv_t::moduleValue
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] value	0=Do nothing, 1=Start FPGA if not already started
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setFPGAStart(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status);

/**
 * Read FPGA start status
 *
 * Reads the value from FPGAStart port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value 	Current FPGA status. 0=Not started, 1=Started
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getFPGAStart(const irioDrv_t *p_DrvPvt, int32_t *value, const TStatus *status);

/**
 * Read the FPGA VI Version
 *
 * Reads the FPGA VI Version String from the value stored in the driver structure and
 * copy it in the given pointer. If the version string is bigger than the maximum size
 * specified it will be truncated.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value FPGA VI version value
 * @param[in] maxLength Maximum possible length of the string
 * @param[out] valueLength Length of the copied string
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getFPGAVIVersion(const irioDrv_t *p_DrvPvt, char *value, size_t maxLength,
		size_t *valueLength, TStatus *status);

/**
 * Read the Device Quality Status
 *
 * Reads the value from device quality status port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current value of DevQualityStatus
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDevQualityStatus(const irioDrv_t *p_DrvPvt, int32_t *value,
		TStatus *status);

/**
 * Read Device Temperature
 *
 * Reads the value from device temperature port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current device temperature
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDevTemp(const irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status);

/**
 * Read Device Acquisition Profile
 *
 * Reads the value from device profile port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Device profile value
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDevProfile(const irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status);

/**
 * Set Debug Mode
 *
 * Sets if the device should work in debug mode or not.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] value 0=Normal mode, 1=Debug Mode
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setDebugMode(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status);

/**
 * Read Debug Mode
 *
 * Reads the value from device operation mode port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current value of debug mode
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDebugMode(const irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status);

/**
 * Set Acquisition status
 *
 * Starts or stops the acquisition process.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] value 0=Stop acquisition, 1=Start acquisition
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setDAQStartStop(irioDrv_t *p_DrvPvt, int32_t value, TStatus *status);

/**
 * Read Acquisition status
 *
 * Reads the value of acquisition process status port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[out] value Current acquisition status
 * @param[out] status	Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getDAQStartStop(const irioDrv_t *p_DrvPvt, int32_t *value, TStatus *status);

/**
 * Set Sampling Rate
 *
 * Sets the value of Sampling Rate (only used in cRIO IO profile).
 * Errors may occur if the port was not found or while writing to the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the sampling rate to set (SamplingRaten)
 * @param[in] value Value to set
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_setSamplingRate(irioDrv_t *p_DrvPvt, int n, int32_t value,
		TStatus *status);

/**
 * Read Sampling rate
 *
 * Reads the value from the Sampling Rate port (only used in cRIO IO profile).
 * Errors may occur if the port was not found or while reading from the port.
 *
 * @param[in] p_DrvPvt 	Pointer to the driver session structure
 * @param[in] n Number of the sampling rate to read (SamplingRaten)
 * @param[out] value Current value of sampling rate
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getSamplingRate(const irioDrv_t *p_DrvPvt, int n, int32_t *value,
		TStatus *status);


/**
 * Reads the platform type
 *
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] value    Pointer to store the platform type value.
 * @param[out] status   Warning and error messages produced during the execution of this call will be added here.
 * @return         \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getPlatformType(const irioDrv_t *p_DrvPvt, uint8_t *value, TStatus *status);

/**
 * Indicates whether the FPGA has been initialized properly or not
 *
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] value Pointer to a variable where the initDone will be stored.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getInitDone(const irioDrv_t *p_DrvPvt, uint8_t *value, TStatus *status);

/**
 * Reads module connected to FlexRIO board.
 * 
 * If the board is not a FlexRIO, a warning will be returned
 *
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] value Pointer to store the module connected
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getFlexRIOModule(const irioDrv_t *p_DrvPvt, uint32_t *value,
						  TStatus *status);

/**
 * Reads an array of the modules connected to cRIO board.
 * 
 * If the board is not a cRIO, a warning will be returned
 *
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] modules Pointer to an array on which the modules connected are. Do not free the memory.
 * @param[out] numModules Number of modules connected to the cRIO board
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getcRIOModules(const irioDrv_t *p_DrvPvt, const uint16_t **modules,
						size_t *numModules, TStatus *status);

/**
 * Reads the maximum number of DMAs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxNumDMAs Pointer to store the maximum number of DMAs supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfDMAs(const irioDrv_t *p_DrvPvt, uint16_t *maxNumDMAs,
							TStatus *status);
/**
 * Reads the maximum number of GPU DMAs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxNumGPUDMAs Pointer to store the maximum number of GPU DMAs supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfGPUDMAs(const irioDrv_t *p_DrvPvt, uint16_t *maxNumGPUDMAs,
							   TStatus *status);

/**
 * Reads the maximum number of analog inputs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxAI Pointer to store the maximum number of analog input supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfAI(const irioDrv_t *p_DrvPvt, uint16_t *maxAI,
						  TStatus *status);
/**
 * Reads the maximum number of analog output supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxAO Pointer to store the maximum number of analog output supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfAO(const irioDrv_t *p_DrvPvt, uint16_t *maxAO,
						  TStatus *status);
/**
 * Reads the maximum number of digital inputs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxDI Pointer to store the maximum number of digital input supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfDI(const irioDrv_t *p_DrvPvt, uint16_t *maxDI,
						  TStatus *status);
/**
 * Reads the maximum number of digital output supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxDO Pointer to store the maximum number of digital output supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfDO(const irioDrv_t *p_DrvPvt, uint16_t *maxDO,
						  TStatus *status);

/**
 * Reads the maximum number of aux analog inputs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxAuxAI Pointer to store the maximum number of aux analog input supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfAuxAI(const irioDrv_t *p_DrvPvt, uint16_t *maxAuxAI,
							 TStatus *status);

/**
 * Reads the maximum number of aux analog outputs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxAuxAO Pointer to store the maximum number of aux analog outputs supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfAuxAO(const irioDrv_t *p_DrvPvt, uint16_t *maxAuxAO,
							 TStatus *status);

/**
 * Reads the maximum number of aux digital inputs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxAuxDI Pointer to store the maximum number of aux digital input supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfAuxDI(const irioDrv_t *p_DrvPvt, uint16_t *maxAuxDI,
							 TStatus *status);

/**
 * Reads the maximum number of aux digital outputs supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxAuxDO Pointer to store the maximum number of aux digital outputs supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfAuxDO(const irioDrv_t *p_DrvPvt, uint16_t *maxAuxDO,
							 TStatus *status);

/**
 * Reads the maximum number of aux signal generators supported by the platform
 * 
 * @param[in] p_DrvPvt Pointer to the driver session structure
 * @param[out] maxSG Pointer to store the maximum number of signal generators supported by the platform
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxNumberOfSG(const irioDrv_t *p_DrvPvt, uint16_t *maxSG,
						  TStatus *status);


/**
 * Reads the conversion to Volts of analog inputs
 *
 * @param[in] p_DrvPvt Pointer to the irioDrv_t object.
 * @param[out] cvadc Pointer to store the CVADC value.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getCVADC(const irioDrv_t *p_DrvPvt, double *cvadc, TStatus *status);

/**
 * Reads the conversion from Volts for analog outputs
 *
 * @param[in] p_DrvPvt Pointer to the irioDrv_t object.
 * @param[out] cvdac Pointer to store the CVDAC value.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getCVDAC(const irioDrv_t *p_DrvPvt, double *cvdac, TStatus *status);

/**
 * Reads the maximum value to be written in an analog output
 *
 * @param[in] p_DrvPvt Pointer to the irioDrv_t object.
 * @param[out] maxAOVal Pointer to store the maximum value to be written in an analog output.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxAOValue(const irioDrv_t *p_DrvPvt, float *maxAOVal, TStatus *status);

/**
 * Reads the minimum value to be written in an analog output
 *
 * @param[in] p_DrvPvt Pointer to the irioDrv_t object.
 * @param[out] minAOVal Pointer to store the minimum value to be written in an analog output.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMinAOValue(const irioDrv_t *p_DrvPvt, float *minAOVal, TStatus *status);

/**
 * Reads the maximum sampling rate supported by the FPGA
 *
 * @param[in] p_DrvPvt Pointer to the irioDrv_t object.
 * @param[out] maxSamplingRate Pointer to store the maximum value to be written in an analog output.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMaxSamplingRate(const irioDrv_t *p_DrvPvt, float *maxSamplingRate, TStatus *status);

/**
 * Reads the minimum sampling rate supported by the FPGA
 *
 * @param[in] p_DrvPvt Pointer to the irioDrv_t object.
 * @param[out] minSamplingRate Pointer to store the minimum value to be written in an analog output.
 * @param[out] status Warning and error messages produced during the execution of this call will be added here.
 * @return \ref TIRIOStatusCode result of the execution of this call.
 */
int irio_getMinSamplingRate(const irioDrv_t *p_DrvPvt, float *minSamplingRate, TStatus *status);

#ifdef __cplusplus
}
#endif
