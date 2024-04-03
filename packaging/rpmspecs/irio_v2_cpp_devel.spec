Name:		irioCoreCpp_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCoreCpp devel

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libbfp

%description
irioCoreCpp devel

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
