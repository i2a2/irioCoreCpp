Name:		irioCoreCpp_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCoreCpp devel

License:	GPL
URL:		https://github.com/i2a2/irioCore

Requires: bfp irioCoreCpp

%description
irioCoreCpp devel

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
