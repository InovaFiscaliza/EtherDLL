# MIAerConn

## Description

The application is a Windows Service that may also be accessed as a desktop application for debug and provides a socket interface for the MIAer Spectrum Monitoring Stations through the manufactures specific API.

The API is not included in this repository and must be provided by the manufacturer, being dependent on the manufacturer's SDK and licensing.

The main application is intended to be a single portable application (.exe) that may be installed as a Windows Service and accessed as a console application for debug.

Output from the executable is in the form of log, that may be directed to file or the console.

Configuration is done through a json file (config.json) that must be placed in the same folder as the executable.

The JSON file is read at startup and stores not only the log format but also parameters used to connect to the monitoring station equipment and also that define the services provided.

Additionally, a test client in matlab is provided, such as to enable testing of the service.

A simulation mode is also provided, such as to enable testing of the service without the need of the manufacturer's equipment, although the execution will require the specific DLL files.

## Installation

The application is installed as a Windows Service and must be installed with administrative privileges.

The manufacturer's API must be installed in the system before running the service.

The compiled application is composed of the following files:

 - MIAerConn.exe, which is a windows console application
 - config.json, which stores the configuration parameteres, including the IP address and port of the MIAer Spectrum Monitoring Station

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
When running as a service, the GUI will not be available, but the service may be stopped and started using the commands above.

To access the GUI, the application may be run as a desktop application, in this case, the service will not be started.

## Dev Environment

This project was created using Visual Studio Community 2022 and the project files are included in the repository.

The following additional files are required to be placed within the same directory as the project files:

	- StdAfx.cpp
	- StdAfx.h
	- OccupSpectConnect.h
	- MoreEquipCtrlMsg.h

These file are provided with the manufacturer's SDK, and are not included in this repository.

This repository includes libraries from other repositories, which are included as submodules. To clone the repository with the submodules, use the following command:

The can be accessed at the following link:

- [spdlog](https://github.com/gabime/spdlog) used for logging.
- [nlohmann json](https://github.com/nlohmann/json) used to read configuration data and handle json format.




