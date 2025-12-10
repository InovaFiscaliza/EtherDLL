// Definition of specific ViCom error codes for the RS232 Interface

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation
#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_RS232_ERROR_CODE_BASE                        14000

#define  VICEC_TO_MANY_RS232_INTERFACES_ALREADY_LOADED      (0 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_TO_MANY_RS232_INTERFACES_ALREADY_LOADED,     "Call sequence error: To many (32) RS232 Interfaces are already loaded.")

#define  VICEC_RS232_INTERFACE_NOT_FOUND                    (1 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_INTERFACE_NOT_FOUND,                   "Parameter Error: RS232 Interface not found.")

#define  VICEC_RS232_INTERNAL_INIT                          (2 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_INTERNAL_INIT,                         "Internal Error: RS232 initialization failed")

#define  VICEC_RS232_ALREADY_IN_USE                         (3 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_ALREADY_IN_USE,                        "RS232 of the selected TSMx is already in use.")

#define  VICEC_RS232_RELEASE_TSMX_WHILE_MEAS                (4 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_RELEASE_TSMX_WHILE_MEAS,               "Call sequence error: The TSMx is still in measurement mode during the request to release the TSMx.")

#define  VICEC_RS232_TIMEBASESYNCMODE_NOT_AVAILABLE         (5 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_TIMEBASESYNCMODE_NOT_AVAILABLE,        "Call sequence error: SetTimebaseSynchronisationMode not available for RS232 Tunneling")

#define  VICEC_RS232_BAUD_RATE                              (6 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_BAUD_RATE,                             "Parameter Error: invalid baud rate")

#define  VICEC_RS232_SILENT_MODE                            (7 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_SILENT_MODE,                           "Parameter Error: invalid Silent Mode")

#define  VICEC_RS232_DATABUFFER                             (8 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_DATABUFFER,                            "Parameter Error: invalid data buffer")

#define  VICEC_RS232_DATABUFFER_SIZE                        (9 + VICOM_RS232_ERROR_CODE_BASE)
SETVICEC(VICEC_RS232_DATABUFFER_SIZE,                       "Parameter Error: invalid data buffer size")
