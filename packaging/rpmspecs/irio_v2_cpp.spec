Name:		irio_v2_cpp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

Requires: libbfp

%description
IRIO V2 CPP

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
