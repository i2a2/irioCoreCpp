Name:		bfp_devel
Version:	{VERSION}
Release:	1%{?dist}
Summary:	BFP devel

License:	GPL
URL:		https://github.com/i2a2/irioCore

Requires: bfp pugixml-devel

%description
BFP devel

%postun
/sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}
