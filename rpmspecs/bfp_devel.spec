Name:		bfp_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	BFP devel

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

%description
BFP devel

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
