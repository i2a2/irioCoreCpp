Name:		irioCoreCpp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCoreCpp

License:	GPL-3.0
URL:		https://github.com/i2a2/irioCoreCpp

Requires: bfp ni-flexrio-modulario-libs ni-syscfg-runtime

%description
irioCoreCpp

%postun
/sbin/ldconfig
unset IRIOCORE_MODULES_PATH

%posttrans
/sbin/ldconfig
source /etc/profile.d/irioCore.sh

%files
{FILES_TO_INCLUDE}

{BUILD_ID_LINKS}
