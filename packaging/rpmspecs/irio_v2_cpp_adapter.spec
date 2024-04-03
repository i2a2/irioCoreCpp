Name:		irio_v2_cpp_adapter
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP Adapter Module

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libbfp

%description
Adapts the IrioV2Cpp library to the prexisting C API or IrioV2

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
