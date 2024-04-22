Name:		irioCoreCpp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCoreCpp

License:	GPL
URL:		https://github.com/i2a2/irioCore

Requires: bfp ni-flexrio-modulario-libs ni-syscfg-runtime

%description
irioCoreCpp

%postun
/sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
