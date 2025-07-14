<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="indexerd-md-top"></a>

<!-- PROJECT SHIELDS -->
<!--
*** based on https://github.com/othneildrew/Best-README-Template
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about">About MIAerConn</a></li>
    <li><a href="#installation">Instalation</a></li>
    <li><a href="#detailed-description">Detailed Description</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#additional-references">Additional References</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
# About

The application is a Windows Service that may also be accessed as a desktop application for debug and provides a open socket API to access DLL callback methods, allowing the integration of local windows services as if they were a web service, through TCP/IP.

Current version was specifically designed to work with MIAer Spectrum Monitoring Stations and may be modified to work with other DLL or APIs working in the local windows environment.

The API is not included in this repository and must be provided by the manufacturer, being dependent on the manufacturer's SDK and licensing.

The main application is intended to be a single portable application (.exe) that may be installed as a Windows Service and accessed as a console application for debug.

Output from the executable is in the form of log, that may be directed to file or the console.

Configuration is done through a json file (config.json) that must be placed in the same folder as the executable.

The JSON file is read at startup and stores not only the log format but also parameters used to connect to the monitoring station equipment and also that define the services provided.

If not found, the application will create a default configuration file.

Additionally, a test client in matlab is provided, such as to enable testing of the service.

A simulation mode is also provided, such as to enable testing of the service without the need of the manufacturer's equipment, although the execution will require the specific DLL files.

Activation or deactivation of the simulation mode is done through the configuration file.

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>

<!-- Installation -->
# Installation

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

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>

<!-- Detailed Description -->
# Detailed Description

The application provides the communication to clients through socket connections, using multiple ports for different services, simplifying client parsing of the data.

The following diagram shows the main socket connections created by the application and associated DLL functions:

![Client Station Communication Diagram](/doc/diagram.svg)

Command, Data and Error channels use JSON format for data serialization.

Stream channel uses binary format for data serialization using the sequence of bytes '[39 10 13 39]' as the end of each message block, for client synchronization.

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>

<!-- GETTING STARTED -->
# Getting Started

This project was created using Visual Studio Community 2022 and the project files are included in the repository.

This repository includes libraries from other repositories, which are included as submodules. Namely:

- [spdlog](https://github.com/gabime/spdlog) used for logging.
- [nlohmann json](https://github.com/nlohmann/json) used to read configuration data and handle json format.

The solution includes references to header files and libraries from the manufacturer's API, which are not included in this repository and must be provided by the manufacturer under their licensing terms. Namely these are:
- MoreEquipCtrlMsg.h
- OccupSpectConnect.h
- StdAfx.h
- StdAfx.cpp

These files should be copied to the src folder of the project from the demo project provided by the manufacturer.

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
# Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>

<!-- LICENSE -->
# License

Distributed under the GNU General Public License (GPL), version 3. See [`LICENSE.txt`](.\LICENSE) for more information.

For additional information, please check <https://www.gnu.org/licenses/quick-guide-gplv3.html>

This license model was selected with the idea of enabling collaboration of anyone interested in projects listed within this group.

It is in line with the Brazilian Public Software directives, as published at: <https://softwarepublico.gov.br/social/articles/0004/5936/Manual_do_Ofertante_Temporario_04.10.2016.pdf>

Further reading material can be found at:
- <http://copyfree.org/policy/copyleft>
- <https://opensource.stackexchange.com/questions/9805/can-i-license-my-project-with-an-open-source-license-but-disallow-commercial-use>
- <https://opensource.stackexchange.com/questions/21/whats-the-difference-between-permissive-and-copyleft-licenses/42#42>

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>

# Additional References

<p align="right">(<a href="#indexerd-md-top">back to top</a>)</p>
