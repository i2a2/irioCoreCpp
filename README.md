# irioCoreCpp

**Table of contents**
- [irioCoreCpp](#iriocorecpp)
- [Introduction](#introduction)
- [Installation](#installation)
- [Compilation](#compilation)
  - [Prerequisites](#prerequisites)
  - [Instructions](#instructions)
- [Manual installation](#manual-installation)
  - [Prerequisites](#prerequisites-1)
  - [Instructions](#instructions-1)
- [Generating packages manually](#generating-packages-manually)
  - [Converting RPM packages to deb](#converting-rpm-packages-to-deb)
    - [Prerequisites](#prerequisites-2)
    - [Instructions](#instructions-2)
- [Configure National Instrument package repository](#configure-national-instrument-package-repository)


# Introduction
> **_NOTE_**<br>
> 
>This project is a reimplementation of https://github.com/i2a2/irio-v2 

National Instruments RIO devices are data acquisition devices implemented using field-programmable gate arrays (FPGAs). This project aims to facilitate their use by providing a series of classes and methods to simplify the interface with these devices.

There are three RIO devices: FlexRIO, compactRIO and RSeries. Each of these devices has a XILINX FPGA at its core, which can be reconfigured in virtually an infinite number of implementations. This renders it challenging, if not impossible, to entirely abstract the user from the specific implementation being used. While designing the C++ library, efforts were made to avoid possible problems. However, no all cases were to fully covered.


# Installation
The recommended way is to download the appropiate packages from the [release section](https://github.com/i2a2/irioCoreCpp/releases). However, it is also possible to install them [manually](#manual-installation).

> **_NOTE_**<br>
>  
> It is necessary to have the national instrument package repository configured to resolve the dependencies appropriately (See [Configure National Instrument package repository](#configure-national-instrument-package-repository)). 
>
> Alternatively the following packages could be installed manually:
> - ni-flexrio-modulario-libs
> - ni-syscfg-runtime

# Compilation
## Prerequisites
The requires packages for compiling the project are:
- pugixml-devel
- gtest-devel
- rsync
- cpplint (python3 module)
- ni-syscfg-devel 
- ni-flexrio-modulario-libs-devel

> **_NOTE_**<br> 
> 
> It is necessary to have the national instrument package repository configured to install the packages related to NI (See [Configure National Instrument package repository](#configure-national-instrument-package-repository)). 

If the generation of the documentation is required, it is also necessary to have installed the following packages:
- doxygen
- graphviz

## Instructions
To compile the project go to the root folder and execute:
```
make compile
```
This will compile the libraries, unittests, functional tests and examples.

# Manual installation
## Prerequisites
- Compile succesfully the libraries (See [compilation](#compilation))
## Instructions
> **_NOTE_**<br> 
> 
> This section covers how to install the libraries by copying using make install, it is recommended installing it as a package if possible. See [Generating packages manually](#generating-packages-manually) 

To install the project go to the root folder and execute:
```
make install INSTALL_DIR=<INSTALL DIR>
```
> **_NOTE_**<br> 
> 
> `INSTALL_DIR` is the install location. Libraries will be placed in `INSTALL_DIR/lib` and headers in `INSTALL_DIR/include`.

# Generating packages manually
> **_NOTE_**<br> 
> 
> Package generation will only generate rpm for RHEL distributions. In other distributions it is necessary to install tools for managing rpm (in Debian it is usually the package `rpm`).

> **_NOTE_**<br>
>
> It is also possible to convert these rpms into deb with `alien` (See [Converting RPM packages to deb](#converting-rpm-packages-to-deb)), but this has not been tested throughly. 

To genearte the rpm go to the root folder and execute:
```
make package
```

This will generate rpm files in `target/packages/x86_64`

> **_NOTE_**<br>
>
> Default installation directories are:
> - `/usr/local/lib` : Libraries
> - `/usr/local/include` : Header files

It is also possible to set the installation directory for the files once the rpm are installed by using the parameter `INSTALL_DIR`
```
make package INSTALL_DIR=<install direcotry>
```

Libraries will be placed in `INSTALL_DIR/lib` and headers in `INSTALL_DIR/include`.
If `INSTALL_DIR` is not set, it defaults to `/usr/local`, unless `CODAC_ROOT` is set, in which case `INSTALL_DIR` will use the value of `CODAC_ROOT`

## Converting RPM packages to deb
### Prerequisites
- [Packaging](#generating-packages-manually) is done succesfully.
- Install `alien` package.
### Instructions
1. Go to `target/packages/x86_64`
2. Run the `alien` command:
    ```
    sudo alien -c *.rpm
    ```
3. The `.deb` packages will be generated in the same folder

# Configure National Instrument package repository
In order toaccess and install the requiredpackages ofNationalInstrument,it is recommended to add the pacakge repository. The following list explains how to do this:
1. **Download the NI System Configuration repositories:**
    - Visit [NI System Configuration Downloads](https://www.ni.com/en/support/downloads/drivers/download.system-configuration.html) and download the relevant package.
2. **Unzip the contents (`.zip` may have different name):**
    ```bash
    unzip -o NILinuxDeviceDrivers NILinux2023Q4DeviceDrivers.zip
    ```

3. **Navigate to the extracted folder:**
    ```bash
    cd NILinuxDeviceDrivers
    ```

4. **Install the package corresponding to your distribution**
    - ni-\<distribution\>-drivers-stream.rpm
    > **_NOTE_**<br>
    >
    > After this step, it may be required to update the repositories to find the NI packages

