Name:		irio_v2_cpp_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP devel

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libbfp

%description
IRIO V2 CPP devel

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
