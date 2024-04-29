Name:		irioCore
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore Module

License:	GPL-3.0
URL:		https://github.com/i2a2/irioCoreCpp

Requires: bfp irioCoreCpp

%description
C API wrapper of irioCoreCpp

%postun
/sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
