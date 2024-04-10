Name:		irioCore
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore Module

License:	GPL
URL:		https://github.com/i2a2/irioCore

Requires: libirioCoreCpp

%description
C API wrapper of irioCoreCpp

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
