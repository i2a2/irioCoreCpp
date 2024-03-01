# irio-v2-cpp

# Compilation instructions

This guide will walk you through the process to set the required dependencies on your system to compile irio-v2-cpp.

## Prerequisites

- This guide assumes you are using a RHEL or Ubuntu Linux distribution. The process in other distributions may be similar but has not been tested.
- You have sudo privileges.
  
### Prerequisites RHEL
1. **Enable CRB repository:**
    ```bash
    sudo dnf config-manager --set-enabled crb
    ```

2. **Enable EPEL repositories:**
    ```bash
    sudo dnf install epel-release
    ```

3. **Install "Development Tools":**
    ```bash
    sudo dnf group install "Development Tools"
    ```
### Prerequisites Ubuntu
No prerequisites are known

## Setup Instructions
1. **Download NI System Configuration repositories:**
   - Visit [NI System Configuration Downloads](https://www.ni.com/en/support/downloads/drivers/download.system-configuration.html) and download the relevant package.

2. **Unzip the contents (.zip may have different name):**
    ```bash
    unzip NILinux2023Q4DeviceDrivers.zip
    ```

3. **Navigate to the extracted folder (folder may have different name):**
    ```bash
    cd NILinux2023Q4DeviceDrivers
    ```

4. **Install the packages corresponding to your distribution (example for RHEL9):**
    1. **RHEL**
    ```bash
    sudo yum install ni-rhel9-drivers-*
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install ni-ubuntu*
    ```

8. **Update the repositories:**
    1. **RHEL**
    ```bash
    sudo yum update
    ```
    2. **Ubuntu**
    ```bash
    sudo apt update
    ```

10. **Install NI System Configuration development package:**
    1. **RHEL**
    ```bash
    sudo yum install ni-syscfg-devel
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install ni-syscfg-devel
    ```

12. **Install niflexrio libs:**
    1. **RHEL**
    ```bash
    sudo yum install ni-flexrio-modulario-libs-devel
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install ni-flexrio-modulario-libs-devel
    ```

14. **Install "rsync":**
    1. **RHEL**
    ```bash
    sudo yum install rsync
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install rsync
    ```

16. **Install "pugixml":**
    1. **RHEL**
    ```bash
    sudo yum install pugixml-devel
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install libpugixml-dev
    ```

18. **Install "gtest":**
    1. **RHEL**
    ```bash
    sudo yum install gtest-devel
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install libgtest-dev
    ```

20. **To generate documentation also install "doxygen" and "graphviz":**
    1. **RHEL**
    ```bash
    sudo yum install doxygen graphviz
    ```
    2. **Ubuntu**
    ```bash
    sudo apt install doxygen graphviz
    ```
    
22. **Generate the packages:**
	 ```bash
    make package
    ```

*WIP: TBC*
