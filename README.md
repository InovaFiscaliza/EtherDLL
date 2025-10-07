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
    <li><a href="#about-etherdll">About EtherDLL</a></li>
    <li><a href="#installation">Instalation</a></li>
    <li><a href="#detailed-description">Detailed Description</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#additional-references">Additional References</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
# About EtherDLL

<div >
<img style="margin-top: 10px; margin-right: 30px; margin-bottom: 10px;" align="left" width="100" height="100" src=".\doc\images\EtherDLL_Icon.svg">  </div>

The application is a console application that provides a TCP/IP Socket API to access local host services available only through DLL methods. This enables the integration of restricted and legacy Windows services through TCP/IP, allowing the the development solutions for cross platform clients, either locally, such as a x64 application accessing a x86 DLL, or remotely, such as a web application accessing the windows local service through the network.

The executable can be configured using a json file (config.json). The file may use the default name "EtherDLLConfig.json" and be placed in the same folder as the executable or the filename may be set as argument of the executable via command line.

The JSON configuration file is read at startup and stores information such as log format, message format used for the TCP/IP socket communication, and also default parameters that may be used to enable a more friendly operation of the DLL API. The application needs to be restarted for any changes in the configuration file to take effect.

All communication with the clients is done through TCP/IP sockets using JSON formatted messages. The port and IP address are also defined in in the configuration file.

Although the core modules and functions can be reused in different projects, the communication with the DLL API is done through callback functions and specific C++ methods. Thus the program must be compiled with the specific header files and linked with the DLL library files.

The application was originally designed to work with the DLL provided by TCI SPX Inc for their Spectrum Monitoring Stations, thus using Visual Studio 2022 as IDE and Windows as operating system. The solution itself follows the C++17 standard. For testing purposes of this integration, a test client in matlab was also developed. Some of the components used in this example are dependent on the manufacturer's SDK and licensing, thus they are not included in this repository.

Additional examples are intended to be provided in the future.

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>

<!-- Installation -->
# Installation

The DLL to be used as interface must be installed prior to the installation of the service.

The compiled application is composed of the following files:

 - EtherDLL.exe, which is a windows console application
 - EtherDLLConfig.json, which stores the configuration parameters, including the IP address and port of the MIAer Spectrum Monitoring Station

By default, the executable will look for the JSON configuration file in the same folder, but arbitrary configuration files can be used by providing the path to the configuration file as a command line argument.

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>

<!-- Detailed Description -->
# Detailed Description

The application provides the communication to clients through socket connections, simplifying client parsing of the data through the use of JSON formatted messages. This also allows for the isolation between the client application and the specific DLL, allowing the use of cross platform clients, such as web applications running in different architectures (e.g. AMD64 connecting to a x86 DLL).

The following diagram illustrates the module architecture of the application:

![EtherDLL Modules](.\doc\images\modules.svg)

| Core Module | Description                                                  |
|--------------|--------------------------------------------------------------|
| EtherDLL.cpp | Entry point of the application, After parsing the command line arguments, loading the configuration and initializing the log, it initialize the DLL connection and start threads responsible for message forwarding between the client and the DLL |
| EtherDLLClient.hpp |  |
| EtherDLLConstants.hpp |  |
| EtherDLLLog.hpp |  |
| EtherDLLUtils.cpp |  |

| Specific Example Module | Description                                                  |
|--------------|--------------------------------------------------------------|
| etherDLLCodes.hpp |  |
| etherDLLInit.hpp |  |
| etherDLLRequest.hpp |  |
| etherDLLValidation.hpp |  |
| etherDLLDataProcess.hpp |  |
| etherDLLResponse.hpp |  |

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>

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

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>

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

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>

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

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>

# Additional References

<div>
    <a href="#about-etherdll">
        <img align="right" width="40" height="40" src="./doc/images/up-arrow.svg" title="Back to the top of this page">
    </a>
    <br><br>
</div>
