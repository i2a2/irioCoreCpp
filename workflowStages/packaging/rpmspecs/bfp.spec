Name:		bfp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	BFP

License:	GPL-3.0
URL:		https://github.com/i2a2/irioCoreCpp

Requires: pugixml

%description
BFP

%postun
/sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
{FILES_TO_INCLUDE}

{BUILD_ID_LINKS}
