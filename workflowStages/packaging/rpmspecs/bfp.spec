Name:		bfp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	BFP

License:	GPL
URL:		https://github.com/i2a2/irioCore

Requires: pugixml

%description
BFP

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
