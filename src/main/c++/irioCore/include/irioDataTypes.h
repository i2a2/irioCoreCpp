/**************************************************************************
 * \file irioDataTypes.h
 * \authors Mariano Ruiz (Universidad Politécnica de Madrid, UPM)
 * \authors Diego Sanz (Universidad Politécnica de Madrid, UPM)
 * \authors Sergio Esquembri (Universidad Politécnica de Madrid, UPM)
 * \authors Enrique Bernal (Universidad Politécnica de Madrid, UPM)
 * \authors Alvaro Bustos (Universidad Politécnica de Madrid, UPM)
 * \brief Typedefs and Macro definitions for IRIO Driver
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

#include "NiFpga.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @name Size and timeout constants
 * Definitions of maximum size for channel read, dmas and max timeout in reads
 */
///@{
#define SIZE_HOST_DMAS 2048000              //!< DMA size used when configuring FiFo buffer
#define FPGA_READ_BUFFER_TIMEOUT_1ms 1      //!< Maximum timeout for DMA buffer
#define NUMBEROFU64TOREADPERCHANNEL	4096    //!< Number of 8-bytes words per DMA channel
///@}

/**@name FlexRIO Adapter Modules Identifiers
 *
 */
///@{
#define FlexRIO_Module_IO_NI5761 0x109374C6  //!< NI5761: 0x109374C6, supported by ITER in AC version not DC, 4 analog inputs
#define FlexRIO_Module_IO_NI5781 0x1093748A  //!< NI5781: 0x1093748A, not supported by ITER, 2 AI and 2 AO
#define FlexRIO_Module_IO_NI6581 0x10937418  //!< NI6581: 0x10937418, supported by ITER, digital I/O module
#define FlexRIO_Module_IO_NI5734 0x10937595  //!< NI5734: 0x10937595, not supported by ITER, 4AI + digital I/Os
///@}

/** @name Maximum FlexRIO resources
 * Definitions of the maximum number of resources that can be instantiated for FlexRIO platform.
 */
///@{
#define FLEXRIO_MAX_ANALOGS_IN 4        //!< Max number of analog inputs
#define FLEXRIO_MAX_AUXA_IN 16          //!< Max number of auxiliary analog inputs
#define FLEXRIO_MAX_ANALOGS_OUT 2       //!< Max number of analog outputs
#define FLEXRIO_MAX_AUXA_OUT 16         //!< Max number of auxiliary analog outputs
#define FLEXRIO_MAX_DIGITALS 90         //!< Max number of digital inputs and outputs
#define FLEXRIO_MAX_AUXDIGITALS 16      //!< Max number of auxiliary digital inputs and outputs
#define FLEXRIO_MAX_DMAS 16             //!< Max number of DMAs channels
#define FLEXRIO_MAX_SIGNALGENERATOR 2   //!< Max number of signal generators
///@}

/** @name Maximum R-Series resources
 * Definitions of the maximum number of resources that can be instantiated for R Series platform.
 */
///@{
/***
 * NI PXIe 7820R/7821R Digital IO only: 128 I/O
 * NI PXI 7831R/7833R/7841R/7842R/7851R/7852R/7853R/7854R:
 * Specifications document
 * NI 7830R only has 4 analog in channels. IRIO supports it but the LabVIEW design cannot include more than 4 physical ADC channels

 */
#define RSERIES_MAX_ANALOGS_IN 8        //!< Max number of analog inputs
#define RSERIES_MAX_AUXA_IN 16          //!< Max number of auxiliary analog inputs
#define RSERIES_MAX_ANALOGS_OUT 8       //!< Max number of analog outputs
#define RSERIES_MAX_AUXA_OUT 16         //!< Max number of auxiliary analog outputs
#define RSERIES_MAX_DIGITALS 128        //!< Max number of digital inputs and outputs
#define RSERIES_MAX_AUXDIGITALS 16      //!< Max number of auxiliary digital inputs and outputs
#define RSERIES_MAX_DMAS 3              //!< Max number of DMAs channels
#define RSERIES_MAX_SIGNALGENERATOR 2   //!< Max number of signal generators
///@}

/** @name Maximum cRIO resources
 * Definitions of the maximum number of resources that can be instantiated for cRIO platform.
 */
///@{
#define CRIO_MAX_ANALOGS_IN 256         //!< Max number of analog inputs
#define CRIO_MAX_AUXA_IN 256            //!< Max number of auxiliary analog inputs
#define CRIO_MAX_ANALOGS_OUT 256        //!< Max number of analog outputs
#define CRIO_MAX_AUXA_OUT 256           //!< Max number of auxiliary analog outputs
#define CRIO_MAX_DIGITALS 256           //!< Max number of digital inputs and outputs
#define CRIO_MAX_AUXDIGITALS 256        //!< Max number of auxiliary digital inputs and outputs
#define CRIO_MAX_DMAS 3                 //!< Max number of DMAs channels
#define CRIO_MAX_SIGNALGENERATOR 256    //!< Max number of signal generators
#define CRIO_MAX_MODULES 16             //!< Max number of CompactRIO modules
///@}

/** @name Maximum cRIO resources
 * Sizes of several ports
 */
///@{
#define U16BIT 65535           //!< Limit for unsigned 16-bits variables
#define SHORT_CHAR_STRING 10   //!< Size used to IRIO versions like register FPGAVIversion or IRIO number version
///@}

/**
 * Enum Type for IRIO supported platforms
 *
 * Specify the result of a method call
 */
typedef enum {
	IRIO_FlexRIO = 0, 	//!< FlexRIO Devices
	IRIO_cRIO,       	//!< CompactRIO Devices
	IRIO_RSeries     	//!< RSeries Devices
} TIRIOPlatforms;

/**
 * Enum Type for driver operation state
 *
 * Specify the result of a method call
 */
typedef enum {
	IRIO_success = 0,   	//!< success The driver is working without problems
	IRIO_warning,  //!< warning Some problem happened. Operation might not be completed, but no need to shutdown or restart
	IRIO_error  //!< error Critical error happened and IRIO should not be used
} TIRIOStatusCode;

#define NUM_ERRORS 18  //!< Generic error defined in IRIO error messages enumeration

/**
 * Enum Type for IRIO error messages
 *
 * Specify the result of a method call
 */
typedef enum {
	Generic_Error = -NUM_ERRORS,  //!< To be used by upper layers for custom errors in irio_mergeStatus
	HardwareNotFound_Error, 			//!< Given serial number was not found
	BitfileDownload_Error,				//!< Error downloading bitfile
	InitDone_Error, 					//!< Could not configure adapter module
	IOModule_Error,						//!< IO module/s check error
	NIRIO_API_Error, 		//!< Any error given as result of a call to NIRIO
	ListRIODevicesCommand_Error,		//!< Call to lsrio failed
	ListRIODevicesParsing_Error,		//!< Parse lsrio output failed
	SignatureNotFound_Error,	 //!< Bitfile signature not found in header file
	ResourceNotFound_Error, 	//!< Mandatory resource not found in header file
	SignatureValueNotValid_Error,		//!< Parse bitfile signature failed
	ResourceValueNotValid_Error,		//!< Parse resource value failed
	MemoryAllocation_Error,		//!< System call for memory allocation failed
	BitfileNotFound_Error,				//!< Project bitfile was not found
	HeaderNotFound_Error,				//!< Project header file was not found
	FileAccess_Error,					//!< Access to a file failed
	FileNotFound_Error,					//!< Search for a file failed
	FeatureNotImplemented_Error,   //!< Try to use RSeries devices or undefined profiles
	Success = 0,						//!< Everything is OK
	TemporaryFileDelete_Warning,  //!< Could not delete file in tmp generated by lsrio
	ResourceNotFound_Warning,  //!< Part of optional resource not found (e.g. AO0 Present but AOEnable0 not present)
	FPGAAlreadyRunning_Warning,  //!< Tried to change FPGAStart after FPGA was started/FPGA was started (bitfile downloaded was already in the FPGA)
	Read_NIRIO_Warning,					//!< Resource read failed in NIRIO call
	Read_Resource_Warning,			//!< Tried to access a non-present resource
	Write_NIRIO_Warning,				//!< Resource write failed in NIRIO call
	Write_Resource_Warning,		//!< Tried to write in a non-present resource
	ConfigDMA_Warning,		//!< Attempt to Configure, Start or Stop DMA failed
	ConfigUART_Warning,					//!< Timeout setting baudrate
	ValueOOB_Warning,		//!< Value given in configuration is out of bounds
	Generic_Warning,  //!< To be used by upper layers for custom warnings in irio_mergeStatus
	DAQtimeout_Warning,	 //!< Timeout when waiting for data in data acquisition
	CLSLinetimeout_Warning,	 //!< Timeout in cameralink reception of serial line data
	ResourceRelease_Warning		//!< NIRIO error while releasing Fifo elements
} TErrorDetailCode;

/**
 * Enum Type for RIO Device coupling mode
 *
 * Specify the result of a method call
 */
typedef enum {
	IRIO_coupling_AC = 0, IRIO_coupling_DC, IRIO_coupling_NULL
} TIRIOCouplingMode;

/**
 * IRIO status structure
 *
 * Warning and error messages are concatenated in msg in each IRIO API call.
 * code will show the maximum level of message stored (error>warning>success). Code should always contain a valid pointer to allocated memory or NULL.
 */
typedef struct TStatus {
	TIRIOStatusCode code; 	//!< Current level of error
	TErrorDetailCode detailCode;  //!< Status code for a more detailed information of the status. To be used in irio_getErrorString
	char *msg;  //!< Stored log of errors and warnings. Should always contain a valid pointer to allocated memory or NULL.
} TStatus;

/**
 * Type for FPGA Resources. Maintaned for compatibility.
 *
 * Stores whether the resource was found or not and its offset
 */
typedef struct TResourcePort {
	uint8_t found;      //!< Register that indicates if a resource has been found
	uint32_t value;     //!< If a resource has been found this register indicates its value
}TResourcePort;

#define DEVICESERIALNUMBERLENGTH 20
#define RIODEVICEMODELLENGTH 20
#define FPGARIOLENGTH 15

/**
 * Main struct of irioCore
 *
 * Stores all ports, the current session and the status.
 * 
 * @warning Direct access to this structure should be avoided. 
 * Use getters/setters. Variables may be unitialized or null pointers which
 * may lead to unwanted behaviour or even segfaults.
 */
typedef struct irioDrv_t {
	/// Part of the name of the bitfile downloaded into the FPGA.
	char* projectName;
	/// Name for this irioDriver session
	char *appCallID;
	/// NiFpga driver port (RIO0,RIO1,...)
	char fpgaRIO[FPGARIOLENGTH];
	/// Board Model (PXI-7965R,...)
	char RIODeviceModel[RIODEVICEMODELLENGTH];
	/// Board serial number
	char DeviceSerialNumber[DEVICESERIALNUMBERLENGTH];
	/// Session obtained by C API to manage a FPGA
	NiFpga_Session session;
	/// Indicates whether or not print trace messages in IRIO API methods
	int verbosity;

	/// Indicates the type of platform being handled (cRIO, FlexRIO, RSeries)
	uint8_t platform;
	/// Indicates the profile of the target
	uint8_t devProfile;
	/// Bitfile's version
	char FPGAVIStringversion[SHORT_CHAR_STRING];

	/// Module identifier connected to FlexRIO board
	uint32_t moduleFlexRIO;
	/// Array with modules identifiers connected to cRIO board
	uint16_t modulescRIO[CRIO_MAX_MODULES];
	/// Number of modules connected to cRIO board
	uint16_t numModulescRIO;

	/// Indicates whether the FPGA has already been started or not
	int fpgaStarted;
	/// Indicates whether the FPGA has been initialized properly or not
	uint8_t initDone;

	/// FPGA's clock reference for signal generation
	uint32_t Fref;

	// DAQ profile
	/// Array that contains the number of Channels per DMA
	uint16_t *DMATtoHOSTNCh;
	/// Array containing the first channel indexed in each DMA
	uint16_t *DMATtoHOSTChIndex;
	/// Array that contains the frame type used by the different DMAs
	uint8_t *DMATtoHOSTFrameType;
	/// Array that contains the sample size used by the different DMAs
	uint8_t *DMATtoHOSTSampleSize;
	/// Array that contains the size of DMA data block in terms of DMA words
	uint16_t *DMATtoHOSTBlockNWords;

	// DAQGPU profile //TODO : Check which ones can be removed from the struct
	/// Array that contains the number of Channels per GPU_DMA
	uint16_t *DMATtoGPUNCh;
	/// Array containing the first channel indexed in each GPU_DMA
	uint16_t *DMATtoGPUChIndex;
	/// Array that contains the frame type used by the different GPU_DMAs
	uint8_t *DMATtoGPUFrameType;
	/// Array that contains the sample size used by the different GPU_DMAs
	uint8_t *DMATtoGPUSampleSize;
	/// Array that contains size of GPU DMA data blocks in terms of DMA words.
	uint16_t *DMATtoGPUBlockNWords;
	/// GPU DMA buffer information
	uint64_t *gpu_buffer;

	// optional resources digital I/O
	/// Conversion to Volts of analog inputs
	double CVADC;
	/// Conversion from Volts for analog outputs
	double CVDAC;
	/// Maximum value to be written in an analog output
	float maxAnalogOut;
	/// Minimum value to be written in an analog output
	float minAnalogOut;
	/// Reference frequency used for signal generation using DDS technique
	uint32_t *SGfref;

	/// The maximum number of analog inputs supported by the platform
	uint16_t max_analoginputs;
	/// The maximum number of analog outputs supported by the platform
	uint16_t max_analogoutputs;
	/// The maximum number of aux analog inputs supported by the platform
	uint16_t max_auxanaloginputs;
	/// The maximum number of aux analog outputs supported by the platform
	uint16_t max_auxanalogoutputs;
	/// The maximum number of digital inputs supported by the platform
	uint16_t max_digitalsinputs;
	/// The maximum number of digital outputs supported by the platform
	uint16_t max_digitalsoutputs;
	/// The maximum number of aux digital inputs supported by the platform
	uint16_t max_auxdigitalsinputs;
	/// The maximum number of aux digital outputs supported by the platform
	uint16_t max_auxdigitalsoutputs;
	/// The maximum number of signal generatos supported by the platform
	uint16_t max_numberofSG;
	/// The maximum number of DMAs supported by the platform
	uint16_t max_dmas;
	/// The maximum number of GPU DMAs supported by the platform
	uint16_t max_dmas_gpu;

	/// Minimum sampling rate supported by the FPGA
	float minSamplingRate;
	/// Maximum sampling rate supported by the FPGA
	float maxSamplingRate;

	/// Number of AI found in FPGA
	size_t numAI;
	/// Number of AuxAI found in FPGA
	size_t numAuxAI;
	/// Number of AO found in FPGA
	size_t numAO;
	/// Number of AuxAO found in FPGA
	size_t numAuxAO;
	/// Number of DI found in FPGA
	size_t numDI;
	/// Number of AuxDI found in FPGA
	size_t numAuxDI;
	/// Number of DO found in FPGA
	size_t numDO;
	/// Number of AuxDO found in FPGA
	size_t numAuxDO;
	/// Number of DMA found in FPGA
	size_t numDMA;
	/// Number of Signal Generators found in FPGA
	size_t numSG;
	/// Number of AuxAI 64 bits found in FPGA
	size_t numAuxAI64;
	/// Number of AuxAO 64 bits found in FPGA
	size_t numAuxAO64;
	/// Number of IO Samplint Rate found in FPGA
	size_t numIOSamplingRate;

	/// Same as \ref numSG. Maintaned for compatibility
	uint8_t NoOfSG;
	/// Same as \ref moduleFlexRIO. Maintaned for compatibility.
	uint32_t moduleValue;
	/// Number of DMAs that has the FPGA for acquiring data in the host, this is
	/// the size of DMATtoHOSTNCh array.Maintaned for compatibility.
	TResourcePort DMATtoHOSTNo;

	/// Coupling mode
	TIRIOCouplingMode couplingMode;
} irioDrv_t;

#define CRIOMODULENAMELENGTH 7

/**
 * Type for cRIO modules on NI9159 chassis
 *
 * Relates cRIO module names and Ids
 */
typedef struct cRIOmodule {
	uint16_t id;     //!< Identificator of each cRIO MODULE
	char cmodule[CRIOMODULENAMELENGTH];  //!< Name of each cRIO module
} cRIOmodule;

#ifdef __cplusplus
}
#endif
