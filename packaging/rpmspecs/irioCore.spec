Name:		irioCore
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore Module

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libirio_v2_cpp

%description
Adapts the IrioV2Cpp library to the prexisting C API of IrioV2

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
