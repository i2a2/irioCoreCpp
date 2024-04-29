Name:		irioCore_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore Module devel

License:	GPL-3.0
URL:		https://github.com/i2a2/irioCoreCpp

Requires: irioCore

%description
C API wrapper of irioCoreCpp
Includes header files and the static library for developing applications with it.

%postun
/sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
