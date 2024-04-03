Name:		irio_v2_cpp_adapter_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP Adapter Module devel

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

%description
Adapts the IrioV2Cpp library to the prexisting C API or IrioV2.
Includes header files and the static library for developing applications with it.

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
