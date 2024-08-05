# MIAerConn

## Description

The application is a Windows Service that may also be accessed as a desktop application for debug and provides a socket interface for the MIAer Spectrum Monitoring Stations through the manufactures specific API.

The API is not included in this repository and must be provided by the manufacturer, being dependent on the manufacturer's SDK and licensing.

## Installation

The application is installed as a Windows Service and must be installed with administrative privileges.

The manufacturer's API must be installed in the system before running the service.

The compiled appplication is composed of the following files:

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




