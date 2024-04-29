Name:		irioCore_doc
Version:	{VERSION}
Release:	1%{?dist}
Summary:	irioCore documentation

License:	GPL-3.0
URL:		https://github.com/i2a2/irioCoreCpp

Requires: bfp

%description
IrioCore documentation for modules BFP, irioCore and irioCoreCpp

%postun

%posttrans

%files
{FILES_TO_INCLUDE}
