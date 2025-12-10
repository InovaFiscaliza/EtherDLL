// **** Definition of ViCom error codes valid for all measurements and basic functions

// VICEC is an abbreviation for "ViCom error code"

// ** SETVICEC macro:
// The default implementation of SETVICEC(Code, String) is that the error text line for each error will be ignored by the compiler.
// A macro implementation SETVICEC(Code, String) allows to generate a table
// {{ErrorCode1, ErrorString1}, {ErrorCode2, ErrorString2},...} using this header file. For example:

#if 0

#define SETVICEC(Code, String) {Code, String},

struct SVicomError
{
   DWORD                dwCode;
   const char           *pcszString
} ViComErrors[] = {

   #include "ViComBasicErrors.h"
   #include "ViComWcdmaErrors.h"
   #include "ViComGsmErrors.h"
   {0, "No Error"}
};

#undef SETVICEC

#endif

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif


// ** Error Codes
#define  VICEC_NO_ERROR                                  0
SETVICEC(VICEC_NO_ERROR,                                 "No Error.")

#define  VICEC_INTERFACE_VERSION                         101
SETVICEC(VICEC_INTERFACE_VERSION,                        "The interface versions of the ViCom library and the interface header file do not match.")

#define  VICEC_INTERFACE_DLL_NOT_LOADED                  102
#define  VICEC_INTERFACE_DLL_NOT_LOADED_TEXT             "ViCom interface DLL not loaded. Check availability of this DLL and other DLLs this DLL depends from."
SETVICEC(VICEC_INTERFACE_DLL_NOT_LOADED,                 VICEC_INTERFACE_DLL_NOT_LOADED_TEXT)

#define  VICEC_INTERFACE_FUNCTION_NOT_FOUND              103
#define  VICEC_INTERFACE_FUNCTION_NOT_FOUND_TEXT         "Internal error: Interface function not found."
SETVICEC(VICEC_INTERFACE_FUNCTION_NOT_FOUND,             VICEC_INTERFACE_FUNCTION_NOT_FOUND_TEXT)

#define  VICEC_TSMX_DRIVER_TIMEOUT                       104
SETVICEC(VICEC_TSMX_DRIVER_TIMEOUT,                      "Timeout: The device driver has not responded within 45 seconds.")

#define  VICEC_TSMX_DRIVER_NO_AVAILABILITY               105
SETVICEC(VICEC_TSMX_DRIVER_NO_AVAILABILITY,              "Internal error: The TSMx driver has responded but no TSMx availability was found.")

#define  VICEC_NO_IEEE1394_DRIVER                        106
SETVICEC(VICEC_NO_IEEE1394_DRIVER,                       "IEEE1394 driver not found or not active.")

#define  VICEC_DEVICE_TIMEOUT                            107
SETVICEC(VICEC_DEVICE_TIMEOUT,                           "Timeout: Device has not responded within the given time.")

#define  VICEC_INSUFFICIENT_PHYLIS_FRAME                 108
SETVICEC(VICEC_INSUFFICIENT_PHYLIS_FRAME,                "Internal error: Phylis frame with insufficient context received.")

#define  VICEC_RECEIVER_INDEX_OUT_OF_RANGE               109
SETVICEC(VICEC_RECEIVER_INDEX_OUT_OF_RANGE,              "Parameter error: A receiver for the the specified index is not available.")

#define  VICEC_NO_INTERFACE                              110
#define  VICEC_NO_INTERFACE_TEXT                         "Call sequence error: Device and ViCom DLL not loaded."
SETVICEC(VICEC_NO_INTERFACE,                             VICEC_NO_INTERFACE_TEXT)

#define  VICEC_START_OF_MEASUREMENT                      111
SETVICEC(VICEC_START_OF_MEASUREMENT,                     "Failed to start measurement.")

#define  VICEC_NO_DEVICE                                 112
SETVICEC(VICEC_NO_DEVICE,                                "Hardware: No device available.")

#define  VICEC_MEASUREMENT                               113
SETVICEC(VICEC_MEASUREMENT,                              "Hardware: Selected device does not support desired measurement.")

#define  VICEC_STATE_IDLE                                114
SETVICEC(VICEC_STATE_IDLE,                               "Call sequence error: Device is not in idle state.")

#define  VICEC_STATE_PREPARE_MEASUREMENT                 115
SETVICEC(VICEC_STATE_PREPARE_MEASUREMENT,                "Call sequence error: Device is not in prepare measurement state.")

#define  VICEC_STATE_MEASUREMENT                         116
SETVICEC(VICEC_STATE_MEASUREMENT,                        "Call sequence error: Device is not in measurement state.")

#define  VICEC_STATE_STOP_MEASUREMENT                    117
SETVICEC(VICEC_STATE_STOP_MEASUREMENT,                   "Call sequence error: Device is not in stop measurement state.")

#define  VICEC_STATE                                     118
SETVICEC(VICEC_STATE,                                    "Internal error: Device is in unknown state.")

#define  VICEC_TIME_OUT                                  119
SETVICEC(VICEC_TIME_OUT,                                 "The function did not finish within the time given.")

#define  VICEC_RESULT_BUFFER_DEPTH_OUT_OF_RANGE          120
SETVICEC(VICEC_RESULT_BUFFER_DEPTH_OUT_OF_RANGE,         "Parameter error: Result buffer depth out of range [1..1024].")

#define  VICEC_VERSION_MISMATCH                          121
SETVICEC(VICEC_VERSION_MISMATCH,                         "Internal error: Version numbers and version strings mismatch.")

#define  VICEC_IEEE1394_DRIVER_IN_USE                    122
SETVICEC(VICEC_IEEE1394_DRIVER_IN_USE,                   "IEEE1394 driver is already in use by another application.")

#define  VICEC_INTERFACE_ALREADY_LOADED                  123
#define  VICEC_INTERFACE_ALREADY_LOADED_TEXT             "Call sequence error: ViCom interface already loaded."
SETVICEC(VICEC_INTERFACE_ALREADY_LOADED,                 VICEC_INTERFACE_ALREADY_LOADED_TEXT)

#define  VICEC_INTERFACE_MAXIMUM_REACHED                 124
SETVICEC(VICEC_INTERFACE_MAXIMUM_REACHED,                "Call sequence error: Too many receiver interfaces are already loaded.")

#define  VICEC_INTERFACE_NOT_FOUND                       125
SETVICEC(VICEC_INTERFACE_NOT_FOUND,                      "Parameter Error: The pointer to the interface is invalid.")

#define  VICEC_RELEASE_WHILE_MEAS                        127
SETVICEC(VICEC_RELEASE_WHILE_MEAS,                       "Call sequence error: The device is still in measurement mode during the request to release.")

#define  VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE          129
SETVICEC(VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE,         "Call sequence error: The device does not support the called interface function.")

#define  VICEC_CONNECT_FAILED                            131
SETVICEC(VICEC_CONNECT_FAILED,                           "No device connection established")

#define  VICEC_NOT_IMPLEMENTED                           132
SETVICEC(VICEC_NOT_IMPLEMENTED,                          "The called function is not implemented.")

#define  VICEC_FRONTEND_OUT_OF_RANGE                     134
SETVICEC(VICEC_FRONTEND_OUT_OF_RANGE,                    "Parameter Error: Invalid frontend selection.")

#define  VICEC_LOAD_EXCEEDED                             135
SETVICEC(VICEC_LOAD_EXCEEDED,                            "Parameter Error: Load exceeded.")

#define  VICEC_INSUFFICIENT_RESOURCES                    136
SETVICEC(VICEC_INSUFFICIENT_RESOURCES,                   "Parameter Error: Insufficient resources.")

#define  VICEC_INVALID_RESOURCE                          137
SETVICEC(VICEC_INVALID_RESOURCE,                         "Internal: Invalid resource.")

#define  VICEC_FREE_RESOURCE_FAILED                      139
SETVICEC(VICEC_FREE_RESOURCE_FAILED,                     "Internal error: Could not free the specified resource.")

#define  VICEC_CONNECTION_LOST                      		140
SETVICEC(VICEC_CONNECTION_LOST,                     		"Device connection lost.")

#define  VICEC_INVALID_RESOURCE_HANDLE                   141
SETVICEC(VICEC_INVALID_RESOURCE_HANDLE,                  "Internal error: Invalid resource handle used.")

#define  VICEC_INVALID_FREQUENCY                         142
SETVICEC(VICEC_INVALID_FREQUENCY,                        "Parameter error: Insufficient options for selected frequency / sub band.")

#define  VICEC_DISCONNECT_FAILED                         143
SETVICEC(VICEC_DISCONNECT_FAILED,                        "Internal: Disconnecting the device failed!")

#define  VICEC_L3_DEC_INIT_FAILED                        144
SETVICEC(VICEC_L3_DEC_INIT_FAILED,                       "Internal: L3 decoder initialization failed.")

#define VICEC_DEVICE_RESPONSE_MISSING                    145
SETVICEC( VICEC_DEVICE_RESPONSE_MISSING,                 "Internal error: Did not get any device response." )

#define VICEC_INSUFFICIENT_MTU_SIZE                      146
SETVICEC( VICEC_INSUFFICIENT_MTU_SIZE,                   "Device connection not possible. Configured MTU size (jumbo frame setting) is too small." )

#define VICEC_INSUFFICIENT_NETMASK                       147
SETVICEC( VICEC_INSUFFICIENT_NETMASK,                    "Device connection not possible. Configured netmask does not match." )

#define VICEC_INSUFFICIENT_PATH_MTU_SIZE                 148
SETVICEC( VICEC_INSUFFICIENT_PATH_MTU_SIZE,              "Device connection not possible. Supported MTU size by network path to receiver too small." )

#define VICEC_INCONSISTENT_HW                            149
SETVICEC( VICEC_INCONSISTENT_HW,                         "Device connection not possible. Inconsistent hardware found." )

// **Errors specific to CViComBasicInterface::LoadTransducerFile() call

#define  VICEC_TRANSDUCER_FILE_NOT_FOUND                 200
SETVICEC(VICEC_TRANSDUCER_FILE_NOT_FOUND,                "Could not open transducer file.")

#define  VICEC_TRANSDUCER_FILE_INVALID_XML               201
SETVICEC(VICEC_TRANSDUCER_FILE_INVALID_XML,              "Invalid transducer file structure. Not a valid XML format.")

#define  VICEC_TRANSDUCER_FILE_NOT_PARSED                202
SETVICEC(VICEC_TRANSDUCER_FILE_NOT_PARSED,               "Internal Error: Could not parse transducer file.")

#define  VICEC_TRANSDUCER_FILE_OUT_OF_MEMORY             203
SETVICEC(VICEC_TRANSDUCER_FILE_OUT_OF_MEMORY,            "Internal Error: Parser is out of memory.")

#define  VICEC_TRANSDUCER_FILE_INVALID_NODES             204
SETVICEC(VICEC_TRANSDUCER_FILE_INVALID_NODES,            "Transducer file does not contain nodes valid for the connected device.")

#define  VICEC_TRANSDUCER_FILE_FE_OUT_OF_RANGE           205
SETVICEC(VICEC_TRANSDUCER_FILE_FE_OUT_OF_RANGE,          "Parameter Error: Invalid frontend selection")

#define  VICEC_TRANSDUCER_FILE_INVLAID_FREQ_ORDER        206
SETVICEC(VICEC_TRANSDUCER_FILE_INVLAID_FREQ_ORDER,       "Parameter Error: Frequencies are not in ascending order.")

#define  VICEC_TRANSDUCER_FILE_INVLAID_FREQ_SPACING      207
SETVICEC(VICEC_TRANSDUCER_FILE_INVLAID_FREQ_SPACING,     "Parameter Error: Frequencies spacing between nodes must be at least 10 MHz.")

#define  VICEC_TRANSDUCER_FILE_INVLAID_NODE_TYPE         208
SETVICEC(VICEC_TRANSDUCER_FILE_INVLAID_NODE_TYPE,        "Parameter Error: Invalid node type.")

#define  VICEC_TRANSDUCER_FILE_INVLAID_NODE_TYPE_ORDER   209
SETVICEC(VICEC_TRANSDUCER_FILE_INVLAID_NODE_TYPE_ORDER,  "Parameter Error: Invalid node type order.")

#define  VICEC_TRANSDUCER_FILE_NOT_READ_FROM_DEVICE      210
SETVICEC(VICEC_TRANSDUCER_FILE_NOT_READ_FROM_DEVICE,     "Transducer file could not be read from device.")

#define  VICEC_TRANSDUCER_FILE_NOT_APPLIED_TO_DEVICE     211
SETVICEC(VICEC_TRANSDUCER_FILE_NOT_APPLIED_TO_DEVICE,    "Transducer file could not be applied to device.")

// **Errors specific to CViComBasicInterface::SetPortConfiguration() call

#define VICEC_INVALID_PORT_TYPE                          212
SETVICEC(VICEC_INVALID_PORT_TYPE,                        "Port type is not valid.")

#define VICEC_INVALID_PORT_MODE                          213
SETVICEC(VICEC_INVALID_PORT_MODE,                        "Port mode is not valid.")

#define VICEC_SET_PORT_CONFIGURATION_FAILED              214
SETVICEC(VICEC_SET_PORT_CONFIGURATION_FAILED,            "Could not apply port configuration to device.")

#define VICEC_SET_PORT_DEVICE_RESPONSE_MISSING           215
SETVICEC(VICEC_SET_PORT_DEVICE_RESPONSE_MISSING,         "Internal error: Did not get any device response.")

// **Errors specific to CViComBasicInterface::GetPortConfiguration() call

#define VICEC_GET_PORT_CONFIGURATION_FAILED              216
SETVICEC(VICEC_GET_PORT_CONFIGURATION_FAILED,            "Could not read port configuration from device.")

#define VICEC_PORT_CONFIGURATION_UNKOWN                  217
SETVICEC(VICEC_PORT_CONFIGURATION_UNKOWN,                "Internal error: Unkown configuration returned from device.")

// **Errors specific to CViComBasicInterface::RegisterRawDataListener() / CViComBasicInterface::UnregisterRawDataListener() call
// **and APP_INTERFACE_CLASS::RegisterResultListener() / APP_INTERFACE_CLASS::UnregisterResultListener()

#define  VICEC_LISTENER_IS_NULL                          126
SETVICEC(VICEC_LISTENER_IS_NULL,                         "The given DataListener object is NULL.")

#define  VICEC_LISTENER_IS_ALREADY_REGISTERED            128
SETVICEC(VICEC_LISTENER_IS_ALREADY_REGISTERED,           "The given DataListener has already been registered to this object.")

#define  VICEC_LISTENER_IS_NOT_REGISTERED                130
SETVICEC(VICEC_LISTENER_IS_NOT_REGISTERED,               "The given DataListener has not been registered to this object.")

#define  VICEC_LISTENER_LICENSE_PROBLEM                  138
SETVICEC(VICEC_LISTENER_LICENSE_PROBLEM,                 "The listener cannot be registered because the scanner does not support the required option.")

#define VICEC_LISTENER_PTR_INVALID                       218
SETVICEC(VICEC_LISTENER_PTR_INVALID,                     "Parameter Error: Invalid listener pointer.")

#define VICEC_LISTENER_MODE                              219
SETVICEC(VICEC_LISTENER_MODE,                            "Call sequence error: GetResultCounters() / GetResult() not allowed in ResultDataListener mode.")


// ** Error Codes for special internal errors

#define  VICEC_INTERNAL                            1        // This error has no specific text, but in the CVicomError the error location is
                                                            // contained to get R&S support. Internal errors produce tolerated memory leaks
                                                            // of 333 byte.
