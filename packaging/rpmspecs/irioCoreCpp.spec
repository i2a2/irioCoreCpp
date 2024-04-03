Name:		irioCoreCpp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCoreCpp

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libbfp

%description
irioCoreCpp

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
