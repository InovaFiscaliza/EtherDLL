# MIAerConn

## Description

The application is a Windows Service that may also be accessed as a desktop application for debug and provides a socket interface for the MIAer Spectrum Monitoring Stations through the manufactures specific API.

The API is not included in this repository and must be provided by the manufacturer, being dependent on the manufacturer's SDK and licensing.

The main application is intended to be a single portable application (.exe) that may be installed as a Windows Service and accessed as a console application for debug.

Output from the executable is in the form of log, that may be directed to file or the console.

Configuration is done through a json file (config.json) that must be placed in the same folder as the executable.

The JSON file is read at startup and stores not only the log format but also parameters used to connect to the monitoring station equipment and also that define the services provided.

If not found, the application will create a default configuration file.

Additionally, a test client in matlab is provided, such as to enable testing of the service.

A simulation mode is also provided, such as to enable testing of the service without the need of the manufacturer's equipment, although the execution will require the specific DLL files.

Activation or deactivation of the simulation mode is done through the configuration file.

## Installation

The application is installed as a Windows Service and must be installed with administrative privileges.

The manufacturer's API must be installed in the system before running the service.

The compiled application is composed of the following files:

 - MIAerConn.exe, which is a windows console application
 - config.json, which stores the configuration parameters, including the IP address and port of the MIAer Spectrum Monitoring Station

Both files should be placed within the same folder.

The installation is done by running the following command in the command prompt with administrative privileges:

```cmd
sc create MIAerConn binPath= "C:\path\to\MIAerConn.exe"
```

The service may be started and stopped using the following commands:

```cmd
sc start MIAerConn
sc stop MIAerConn
```

The service may be removed using the following command:

```cmd
sc delete MIAerConn
```

To completely remove the service, the executable and configuration file must be deleted.

## Dev Environment

This project was created using Visual Studio Community 2022 and the project files are included in the repository.

This repository includes libraries from other repositories, which are included as submodules. Namely: 

- [spdlog](https://github.com/gabime/spdlog) used for logging.
- [nlohmann json](https://github.com/nlohmann/json) used to read configuration data and handle json format.

The solution includes references to header files and libraries from the manufacturer's API, which are not included in this repository and must be provided by the manufacturer under their licensing terms.

## Detailed Description

The application provides the communication to clients through socket connections, using multiple ports for different services, simplifying client parsing of the data.

The following diagram shows the main socket connections created by the application and associated DLL functions:

![Client Station Communication Diagram](/doc/diagram.svg)

Command, Data and Error channels use JSON format for data serialization.

Stream channel uses binary format for data serialization using the sequence of bytes '[39 10 13 39]' as the end of each message block, for client synchronization.

