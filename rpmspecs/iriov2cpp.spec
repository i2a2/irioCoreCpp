%define debug_package %{nil}
Name:		iriov2cpp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

%description
IRIO V2 CPP

%postun
[ $1 == 0 ] && /sbin/ldconfig

%posttrans
/sbin/ldconfig

%files
"{LIB_INSTALL_DIR}/libbfp.so"
"{LIB_INSTALL_DIR}/libirio_v2_cpp.so"