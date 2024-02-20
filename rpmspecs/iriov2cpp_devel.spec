%define debug_package %{nil}
Name:		iriov2cpp_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP devel package

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

%description
IRIO V2 CPP devel package

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
"{LIB_INSTALL_DIR}/libbfp.a"
"{LIB_INSTALL_DIR}/libirio_v2_cpp.a"
"{INC_INSTALL_DIR}/iriov2cpp/bfp.h"
"{INC_INSTALL_DIR}/iriov2cpp/dma.h"
"{INC_INSTALL_DIR}/iriov2cpp/elemtypes.h"
"{INC_INSTALL_DIR}/iriov2cpp/fpgatypes.h"
"{INC_INSTALL_DIR}/iriov2cpp/bfp.h"
"{INC_INSTALL_DIR}/iriov2cpp/dma.h"
"{INC_INSTALL_DIR}/iriov2cpp/elemtypes.h"
"{INC_INSTALL_DIR}/iriov2cpp/fpgatypes.h"
"{INC_INSTALL_DIR}/iriov2cpp/register.h"
"{INC_INSTALL_DIR}/iriov2cpp/resource.h"
"{INC_INSTALL_DIR}/iriov2cpp/irio_v2.h"
"{INC_INSTALL_DIR}/iriov2cpp/platforms.h"
"{INC_INSTALL_DIR}/iriov2cpp/profiles/profileBase.h"
"{INC_INSTALL_DIR}/iriov2cpp/profiles/profileBaseTerminals.h"
"{INC_INSTALL_DIR}/iriov2cpp/profiles/profileDAQ.h"
"{INC_INSTALL_DIR}/iriov2cpp/profiles/profileDAQFlexRIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/profiles/profileFlexRIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/profiles/profiles.h"
"{INC_INSTALL_DIR}/iriov2cpp/register.h"
"{INC_INSTALL_DIR}/iriov2cpp/resource.h"
"{INC_INSTALL_DIR}/iriov2cpp/rioDiscovery.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsAnalog.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsAuxAnalog.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsAuxDigital.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsCommon.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsDMACommon.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsDMADAQ.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsDMAGPUCommon.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsDMAIMAQ.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsDigital.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsFlexRIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsGPUDAQ.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalsSignalGeneration.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/names/namesTerminalscRIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminals.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsAnalog.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsAuxAnalog.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsAuxDigital.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsBase.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsDigital.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsFlexRIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalsSignalGeneration.h"
"{INC_INSTALL_DIR}/iriov2cpp/terminals/terminalscRIO.h"
"{INC_INSTALL_DIR}/iriov2cpp/utils.h"
