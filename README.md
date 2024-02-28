# irio-v2-cpp

# Compilation instructions

This guide will walk you through the process to set the required dependencies on your system to compile irio-v2-cpp.

## Prerequisites

- This guide assumes you are using a RHEL Linux distribution. The process in other distributions may be similar but has not been tested.
- You have sudo privileges.

## Setup Instructions

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

4. **Download NI System Configuration repositories:**
   - Visit [NI System Configuration Downloads](https://www.ni.com/en/support/downloads/drivers/download.system-configuration.html) and download the relevant package.

5. **Unzip the contents (.zip may have different name):**
    ```bash
    unzip NILinux2023Q4DeviceDrivers.zip
    ```

6. **Navigate to the extracted folder (folder may have different name):**
    ```bash
    cd NILinux2023Q4DeviceDrivers
    ```

7. **Install the packages corresponding to your distribution (example for RHEL9):**
    ```bash
    sudo yum install ni-rhel9-drivers-*
    ```

8. **Update the repositories:**
    ```bash
    sudo yum update
    ```

9. **Install NI System Configuration development package:**
    ```bash
    sudo yum install ni-syscfg-devel
    ```

10. **Install niflexrio libs:**
    ```bash
    sudo yum install ni-flexrio-modulario-libs-devel
    ```

11. **Install "rsync":**
    ```bash
    sudo yum install rsync
    ```

12. **Install "pugixml":**
    ```bash
    sudo yum install pugixml-devel
    ```

13. **Install "gtest":**
    ```bash
    sudo yum install gtest-devel
    ```

14. **To generate documentation also install "doxygen" and "graphviz":**
    ```bash
    sudo yum install doxygen graphviz
    ```
    
15. **Generate the packages:**
	 ```bash
    make package
    ```

*WIP: TBC*