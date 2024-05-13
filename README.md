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
- [Run tests](#run-tests)
  - [Automatic execution](#automatic-execution)
  - [Manual execution](#manual-execution)
    - [Unittests](#unittests)
      - [BFP](#bfp)
      - [irioCoreCpp](#iriocorecpp-1)
      - [irioCore (C wrapper)](#iriocore-c-wrapper)
    - [Functional](#functional)
      - [Environment Variables](#environment-variables)
      - [irioCoreCpp](#iriocorecpp-2)
      - [irioCore (C wrapper)](#iriocore-c-wrapper-1)


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
The required packages for compiling the project are:
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

# Run tests
The project contains several tests to try to test irioCoreCpp and its C wrapper. It has unit tests, to check each part of the application, as wll as functional tests, to verify the functionality of the entire application. 

There are two ways to execute these tests... ***TODO***
## Automatic execution

### XML Test automation
Tests can be automatically executed with a Python application. The desired tests must be defined on an XML file following the Schema found on `src/test/testSchema.xsd`. Some recommended tests suites can be found on `src/test/test-suites`, but the serial numbers must be changed to the ones in your installation. 

For example, to create a test that runs all the *FlexRIO* tests of the *irioCore Funcional* binary on a *FlexRIO-7966* that has the serial *0x12345678* and no module attached is:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<testplan
    xmlns="http://www.testLocation.com/test"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.testLocation.com/test ../testSchema.xsd">

    <test>
        <name>Test</name>
        <TestFilter>FlexRIO*</TestFilter>
        <TestType>irioCore Functional</TestType>
        <RIODevice>7966</RIODevice>
        <RIOSerial>0x12345678</RIOSerial>
        <RIOModule>noModule</RIOModule>
        <verbose>false</verbose>                                              <!-- Optional -->
        <coupling>DC</coupling>                                             <!-- Optional -->
        <maxIMAQiterations>65536</maxIMAQiterations>    <!-- Optional -->
    </test>
</testplan>
```

A *testplan* is a collection of *tests* to be run. The different tests inside will be run independently and the results are reported separetely. Each test contains the following fields:
- *name*: Name of the test. Only used for logging.
- *TestFilter*: GTest filter expression to select the desired tests.
- *TestType*: Test family to run. One of:
	- *irioCore Unitary*: Unitary tests of the C library (mocking).
	- *irioCore Functional*: Functional tests of the C library (real hardware).
	- *irioCoreCpp Unitary*: Unitary tests of the C++ library (mocking).
	- *irioCoreCpp Functional*: Functional tests of the C++ library (real hardware).
	- *BFP*: BitFile Parser tests.
- *RIODevice*: Model of the RIO device to test. One of: *7966*, *7965*, *7961* or *9159*.
- *RIOSerial*: Serial number of the device. Format: *0xABCD1234*.        
- *RIOModule*: Module connected to the device. One of: *noModule*, *NI1483*, *NI5761*, *NI5781*, *NI6581*, *NI5734*, *NI92xx*. 
- *verbose*: Boolean controlling the test verbosity. Optional, default is *False*.
- *coupling*: Coupling for signal adquisition. Either *AC* or *DC*. Optional, default is *AC*.
- *maxIMAQiterations*: Maximum counter for the image test. Optional, default to *65536*.
- *results*: Field to store the count of passed tests. Should not be written by the user.
- *summary*: Field to store the string result of the test. Either *PASS* or *FAIL*. Should not be written by the user.

Once the test is defined on the XML file, from the root of the project, run:
```bash
    target/test/run_irioCore.py -i <XML file> [-o <Output file>]
```

If no output file is provided, the tests results will be appended to the input file. If it is provided, the input file is not modified and the input file will be copied to the output file with the results appended.

The test output will be printed as specified on the *verbose* field of the test. If only a final summary is required, use the *-S* flag to supress test output and display a final count only.

### Python test execution

The same python application can be used to execute tests with custom filters. Use the help of the executable to get the possible arguments for the test:
```bash
 run_irioCore.py -h
```
> **_NOTE_**<br>
>
> If the `--input-file` or `-i` flag is present, the custom run arguments will be ignored and the tests from the XML will be run.

For example, in order to run the functional *FlexRIO* tests from the *irioCore* library on a 7966 with the serial number *0x1234ABCD* with full verbosity, use:
```bash
    target/test/run_irioCore.py -d 7966 -s 0x1234ABCD -b target/test/c++/irioCore/test_irioCore -v -f "FlexRIO*"
```

## Manual execution

### Unittests
> **_NOTE_**<br>
>
> These tests have been implemented using mocking and therefore do not require real RIO boards to run them. 

#### BFP
Test are in the following folder. Test should be run from this directory.
```bash
    target/test/c++/bfp
```
To execute all the tests, run:
```bash
    ./test_bfp
```
To execute specific tests use gtest filter functionality
```bash
    ./test_bfp --gtest_filter=<tests to run>
```
> **_NOTE_**<br>
>
> To list available tests use the parameter `--gtest_list_tests`

#### irioCoreCpp
Test are in the following folder. Test should be run from this directory.
```bash
    target/test/c++/unittests/irioCoreCpp
```
To execute all the tests, run:
```bash
    ./test_ut_irioCoreCpp
```
To execute specific tests use gtest filter functionality
```bash
    ./test_ut_irioCoreCpp --gtest_filter=<tests to run>
```
> **_NOTE_**<br>
>
> To list available tests use the parameter `--gtest_list_tests`
>

#### irioCore (C wrapper)
Test are in the following folder. Test should be run from this directory.
```bash
    target/test/c++/unittests/irioCore
```
To execute all the tests, run:
```bash
    ./test_ut_irioCore
```
To execute specific tests use gtest filter functionality
```bash
    ./test_ut_irioCore --gtest_filter=<tests to run>
```

> **_NOTE_**<br>
>
> To list available tests use the parameter `--gtest_list_tests`
> 
### Functional
#### Environment Variables
| Variable Name | Description                                                                                | Example  |
|---------------|--------------------------------------------------------------------------------------------|----------|
| RIODevice     | Type of RIO device                                                                         | 7966     |
| RIOSerial     | Serial of the device. Can be obtained by using "lsni -v -e ni-rio"                         | 0177A2AD |
| VerboseTest   | Print info test messages (not used by irioCoreCpp tests)                                   | 0 or 1   |
| VerboseInit   | Print info of driver initialization (not used by irioCoreCpp tests)                        | 0 or 1   |
| maxCounter    | Maximum counter used when acquiring images (only for IMAQ tests where images are acquired) | 65536    |

#### irioCoreCpp
Test are in the following folder. Test should be run from this folder.
```bash
    target/test/c++/irioCoreCpp
```
To execute all the tests, run:
```bash
    ./test_irioCoreCpp
```
To execute specific tests use gtest filter functionality
```bash
    ./test_irioCoreCpp --gtest_filter=<tests to run>
```

> **_NOTE_**<br>
>
> To list available tests use the parameter `--gtest_list_tests`

#### irioCore (C wrapper)
Test are in the following folder. Test should be run from this folder.
```bash
    target/test/c++/irioCore
```
To execute all the tests, run:
```bash
    ./test_irioCore
```
To execute specific tests use gtest filter functionality
```bash
    ./test_irioCore --gtest_filter=<tests to run>
```

> **_NOTE_**<br>
>
> To list available tests use the parameter `--gtest_list_tests`
