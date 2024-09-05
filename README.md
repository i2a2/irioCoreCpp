# irioCoreCpp

**Table of contents**
- [irioCoreCpp](#iriocorecpp)
- [Introduction](#introduction)
- [Installation](#installation)
- [Compilation](#compilation)
  - [Prerequisites](#prerequisites)
  - [Instructions](#instructions)
- [Generating packages manually](#generating-packages-manually)
  - [Converting RPM packages to deb](#converting-rpm-packages-to-deb)
    - [Prerequisites](#prerequisites-1)
    - [Instructions](#instructions-1)
- [Configure National Instrument package repository](#configure-national-instrument-package-repository)
- [Run tests](#run-tests)
  - [Python execution](#python-execution)
    - [XML Test automation](#xml-test-automation)
    - [Specific test execution](#specific-test-execution)
  - [Run GoogleTest manually](#run-googletest-manually)
    - [Unittests](#unittests)
      - [BFP](#bfp)
      - [irioCoreCpp](#iriocorecpp-1)
      - [irioCore (C wrapper)](#iriocore-c-wrapper)
    - [Functional](#functional)
      - [Environment Variables](#environment-variables)
      - [irioCoreCpp](#iriocorecpp-2)
      - [irioCore (C wrapper)](#iriocore-c-wrapper-1)
- [Third-Party Libraries](#third-party-libraries)


# Introduction
> **_NOTE_**<br>
> 
>This project is a reimplementation of https://github.com/i2a2/irio-v2 

National Instruments RIO devices are data acquisition devices implemented using field-programmable gate arrays (FPGAs). This project aims to facilitate their use by providing a series of classes and methods to simplify the interface with these devices.

There are three RIO devices: FlexRIO, compactRIO and RSeries. Each of these devices has a XILINX FPGA at its core, which can be reconfigured in virtually an infinite number of implementations. This renders it challenging, if not impossible, to entirely abstract the user from the specific implementation being used. While designing the C++ library, efforts were made to avoid possible problems. However, no all cases were to fully covered.


# Installation
The recommended way is to download the appropiate packages from the [release section](https://github.com/i2a2/irioCoreCpp/releases).

> **_NOTE_**<br>
>  
> If installing through the rpms, it is necessary to have the national instrument package repository configured to resolve the dependencies appropriately (See [Configure National Instrument package repository](#configure-national-instrument-package-repository)). 
>
> Alternatively the following packages could be installed manually:
> - ni-flexrio-modulario-libs
> - ni-syscfg-runtime

# Compilation
## Prerequisites
- The required packages for compiling the project are:
    - pugixml-devel
    - gtest-devel
    - rsync
    - cpplint (python3 module)
- If the generation of the documentation is required, it is also necessary to have installed the following packages:
    - doxygen
    - graphviz
- If the system requires the National Instruments drivers for RIO boards, these packages are also required:
    - ni-syscfg-devel 
    - ni-flexrio-modulario-libs-devel

    > **_NOTE_**<br> 
    > 
    > It is necessary to have the national instrument package repository configured to install the packages related to NI (See [Configure National Instrument package repository](#configure-national-instrument-package-repository)). 



## Instructions
To compile the project go to the root folder and execute:
```
make compile
```
This will compile the libraries, unittests, functional tests and examples.

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

This will generate rpm files in `target/packages/`

> **_NOTE_**<br>
>
> Default installation directories are:
> - `/usr/local/lib` : Libraries
> - `/usr/local/include` : Header files

It is also possible to set the installation directory for the files once the rpm are installed by using the parameter `InstallDir`
```
make package InstallDir=<install directory>
```

Libraries will be placed in `<InstallDir>/lib` and headers in `<InstallDir>/include`.
If `InstallDir` is not set, it defaults to `/usr/local`, unless `CODAC_ROOT` is set, in which case `InstallDir` will use the value of `CODAC_ROOT`

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

There are two ways to execute these tests, thorugh the use of XML and a Python script or running the GoogleTest manually
## Python execution

### XML Test automation
Tests can be automatically executed with a Python application. By default, the `target/test/test-suites` folder contains the unitary tests that do not run on the real hardware.

The functional tests that run on actual hardware must be generated with the `generateFunctional.py` application located in the `target/test/test-suites` folder. This application can be run in two ways:
- Interactive execution: Execute the application with no serial, device or verbose argument to have it ask the user for each parameter.
- Parametric execution: Use **all** the serial, device and verbose arguments to automatize the execution. They can be obtained using the `-h` flag.

This application supports the selection of the input and output folders with the `-i` and `-o` flags, but the default behaviour (input from `test-suites/templates` and output to `test-suites`) is recommended.

Custom test suites can also be created following the schema on `target/test/testSchema.xsd`. For example, to create a test that runs all the *FlexRIO* tests of the *irioCore Funcional* binary on a *FlexRIO-7966* that has the serial *0x12345678* and no module attached is:

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
        <verbose>false</verbose>                        <!-- Optional -->
        <coupling>DC</coupling>                         <!-- Optional -->
        <maxIMAQiterations>65536</maxIMAQiterations>    <!-- Optional -->
    </test>
</testplan>
```

A *testplan* is a collection of *tests* to be run. The different tests inside will be run independently and the results are reported separetely. Each test contains the following fields:
- *name*: Name of the test. Only used for logging.
- *TestFilter*: GTest filter expression to select the desired tests. If not present, all the tests will be run.
- *TestType*: Test family to run. One of:
	- *irioCore Unitary*: Unitary tests of the C library (mocking).
	- *irioCore Functional*: Functional tests of the C library (real hardware).
	- *irioCoreCpp Unitary*: Unitary tests of the C++ library (mocking).
	- *irioCoreCpp Functional*: Functional tests of the C++ library (real hardware).
	- *BFP*: BitFile Parser tests.
    - *Custom binary*: Manually specify the route to the excutable. Requires the optional *Binary* element.
- *RIODevice*: (Only for functional tests) Model of the RIO device to test. One of: *7966*, *7965*, *7961* or *9159*. If not present, a value of `7966` is used.
- *RIOSerial*: (Only for functional tests) Serial number of the device. Format: *0xABCD1234*. If not present, a default value of `0x00000000` is used. 
- *RIOModule*: Module connected to the device. Informative, has no impact on the tests. One of: *noModule*, *NI1483*, *NI5761*, *NI5781*, *NI6581*, *NI5734*, *NI92xx*. 
- *Binary*: Absolute path to the executable to run, if the *TestType* field is set to *CustomBinary*.
- *verbose*: Boolean controlling the test verbosity. Optional, default is *False*.
- *coupling*: (Only for functional tests) Coupling for signal adquisition. Either *AC* or *DC*. Optional, default is *AC*.
- *maxIMAQiterations*: (Only for functional tests) Maximum counter for the image test. Optional, default to *65536*.
- *results*: Field to store the count of passed tests. Should not be written by the user.
- *summary*: Field to store the string result of the test. Either *PASS* or *FAIL*. Should not be written by the user.
- *failedTests*: List of the names of the failed tests. Should not be written by the user.

Once the test is defined on the XML file, from the root of the project, run:
```bash
    target/test/automate_GT.py -i <XML file> [-a | -o <Output file>]
```

Either `-a` or `-o <Output file>` can be used to select the output destination for the tests. `-a` will append the results to the input file and `-o` will create or rewrite the destination file with the results.

The test output will be printed as specified on the *verbose* field of the test. If only a final summary is required, use the *-S* flag to supress test output and display a final count only.

### Specific test execution

The same python application can be used to execute tests with custom filters. Use the help of the executable to get the possible arguments for the test:
```bash
 automate_GT.py -h
```
> **_NOTE_**<br>
>
> If the `--input-file` or `-i` flag is present, the custom run arguments will be ignored and the tests from the XML will be run.

A test report can be generated with the `-o <FILE>` argument, and it will be generated on the same format as the XML files on the previous section.

For example, in order to run the functional *FlexRIO* tests from the *irioCore* library on a 7966 with the serial number *0x1234ABCD* with full verbosity and creating a report on *test.xml*, use:
```bash
    target/test/automate_GT.py -d 7966 -s 0x1234ABCD -b target/test/c++/irioCore/test_irioCore -v -f "FlexRIO*" -o "test.xml"
```

## Run GoogleTest manually

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
Functional test require exporting environment variables.

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

# Third-Party Libraries

This project makes use of third-party libraries. These include:

- **Fake Function Framework (fff)** 
  - Used for unittest mocking
  - https://github.com/meekrosoft/fff
  - License: MIT License (available [here](https://opensource.org/licenses/MIT))
