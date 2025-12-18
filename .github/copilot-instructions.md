# EtherDLL AI Coding Guidelines

## Project Overview

**EtherDLL** is a Windows console application providing a **TCP/IP Socket API** for legacy or restricted DLL services. It bridges incompatible environments—such as x64 code accessing x86 DLLs—via JSON-based messaging over TCP/IP, enabling cross-platform integration locally or remotely.

## Architecture & Key Components

### Core Modules (Reusable Foundation)
Located in `src/`, these are DLL-agnostic and reusable across projects:

- **EtherDLL.cpp** – Entry point; parses CLI args, loads config, initializes logging, spawns client/DLL threads
- **EtherDLLConfig.hpp** – Configuration management; namespace `edll::` holds app constants (version, interruption codes, frequency multipliers)
- **EtherDLLLog.hpp** – Logging via [spdlog](https://github.com/gabime/spdlog) library
- **EtherDLLClient.hpp** – Client TCP/IP communication, message queuing, socket handling (Winsock2)
- **EtherDLLUtils.hpp** – Data processing utilities, JSON validation, base64 encoding, hash functions

### Specific Modules (DLL Integration)
Located in `src/dllSpecific/<DLLName>/`, these **must** implement required functions/types for a DLL API:

- **etherDLLInit.hpp** – `connectAPI()`, `disconnectAPI()`, `newDefaultConfigFile()`, `validDLLConfigParams()`, and `DLLConnectionData` struct
- **etherDLLRequest.hpp** – `processRequestQueue()`, JSON → DLL struct conversion (e.g., `jsonToSAudioParams()`)
- **etherDLLValidation.hpp** – JSON schema validation before DLL calls
- **etherDLLResponse.hpp** – DLL callback registration and response handling
- **etherDLLCodes.hpp** – DLL error/enum translation to human-readable format
- **etherDLLDataProcess.hpp** – Post-DLL data processing

**Critical contract:** Required functions are called by core modules; signatures and behaviors must remain unchanged. Implementation is DLL-specific; refactor internals freely.

### Communication Flow
```
Client (TCP) → EtherDLLClient (queue) → EtherDLLRequest (validate & convert)
            → DLL API (callback) → EtherDLLResponse (callback handler)
            → Response queue → Client (TCP)
```

## Technical Constraints

- **Language:** C++17 (Visual Studio 2022, MSVC v143)
- **Architecture:** x86 (Win32 platform)
- **OS:** Windows only (Winsock2, WinAPI dependencies)
- **JSON Library:** [nlohmann/json](https://github.com/nlohmann/json) via `#include <nlohmann/json.hpp>`
- **Logging:** spdlog with console and file sinks
- **Configuration:** JSON file (`EtherDLLConfig.json`) with DLL settings under `dll_default` key, service config, logging levels

## Code Patterns & Conventions

### Include Order
1. DLL-specific headers (`#include "etherDLL*.h"`)
2. Core EtherDLL headers (`#include "EtherDLL*.hpp"`)
3. Third-party libraries (`#include <nlohmann/json.hpp>`, `#include <spdlog/spdlog.h>`)
4. Standard C++ headers (`#include <string>`, `#include <iostream>`)

### JSON Handling
- Use global alias: `using json = nlohmann::json;`
- **Null checks:** `jsonObj["key"].is_null() == true` before `.get<Type>()`
- **Default values:** Use ternary: `jsonObj["key"].is_null() ? defaultValue : jsonObj["key"].get<Type>()`
- **Config keys:** Defined in `edll::DefaultConfig::Service::TaskKeys` (e.g., `"CODE"`, `"ID"`, `"QID"`)

### Error Handling
- **No exceptions** – Functions use return values (bool/struct)
- **Logging:** Always use `loggerPtr->error()`, `loggerPtr->info()`, `loggerPtr->trace()` (global logger pointer)
- **Validation:** Validate JSON before DLL calls in `etherDLLValidation.hpp`; send error responses to clients instead of crashing

### Global Variables
- `spdlog::logger* loggerPtr` – Initialized in main; used by all modules for logging
- Thread-safe message queues in `EtherDLLClient.hpp` for request/response handling

### Frequency Handling
- Use constants: `MHZ_MULTIPLIER (1e6)`, `KHZ_FROM_MHZ (1000.0)`, `GHZ_FROM_MHZ (0.001)`
- Example: Scorpio's `Units::Frequency(value).GetRaw()` converts frequency units

## Build & Deployment

- **IDE:** Visual Studio 2022 Community
- **Build Config:** Debug|Win32 and Release|Win32 supported
- **Output:** Console executable in `src/EtherDLL/Release/`
- **Installer:** WiX Toolset v3.11 builds MSI from `install/Installer.wxs` → `MIAerConn.msi`
- **Build task:** `Wix Build Installer.wxs` runs `install/wix build Installer.wxs -o MIAerConn.msi`

## Testing & Examples

- **Scorpio Example** (`src/dllSpecific/scorpio/`) – Integrates TCI Spectrum Monitoring Station DLL
- **MATLAB Test Client** (`test/Scorpio/EtherDLLTestClient_exported.m`) – Sends TCP requests to service; requires manufacturer SDK
- **Config:** `test/Scorpio/appConfig.json` and `test/Scorpio/occdfscan_process.json` for test scenarios

## When Adding New DLL Support

1. **Create folder:** `src/dllSpecific/<NewDLLName>/`
2. **Implement required modules** with exact function signatures (see README.md table)
3. **Update** `src/EtherDLL.cpp` includes to reference new specific modules
4. **Add DLL config** to `EtherDLLConfig.json` under `dll_default` section
5. **Create validation rules** in `etherDLLValidation.hpp` for new DLL parameters
6. **Register callbacks** in `connectAPI()` matching EtherDLL's expectations

## Key Files to Reference

- [src/EtherDLLConfig.hpp](src/EtherDLLConfig.hpp) – Constants and namespaces (`edll::`)
- [src/EtherDLLClient.hpp](src/EtherDLLClient.hpp) – Message queue and socket structures
- [README.md](README.md) – Required function signatures and data types
- [src/dllSpecific/scorpio/](src/dllSpecific/scorpio/) – Best practice example implementation
