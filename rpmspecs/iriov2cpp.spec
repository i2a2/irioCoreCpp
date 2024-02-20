%define debug_package %{nil}
Name:		iriov2cpp
Version:	{VERSION}
Release:	1%{?dist}
Summary:	IRIO V2 CPP

License:	GPL
URL:		https://github.com/i2a2/irio-v2-cpp

%description
TEST

%postun
%beginlog_postun
[ $1 == 0 ] && /sbin/ldconfig
%endlog

%posttrans
%beginlog_posttrans
/sbin/ldconfig
%endlog

%files
"{LIB_INSTALL_DIR}/libbfp.so"
"{LIB_INSTALL_DIR}/libirio_v2_cpp.so"
