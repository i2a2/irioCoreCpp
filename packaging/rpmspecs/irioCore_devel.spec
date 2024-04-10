Name:		irioCore_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore Module devel

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libirioCoreCpp

%description
C API wrapper of irioCoreCpp
Includes header files and the static library for developing applications with it.

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
