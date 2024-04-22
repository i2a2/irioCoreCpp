Name:		irioCore
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore Module

License:	GPL
URL:		https://github.com/i2a2/irioCore

Requires: bfp irioCoreCpp

%description
C API wrapper of irioCoreCpp

%postun
/sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
