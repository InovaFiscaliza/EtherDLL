// Definition of specific ViCom error codes for the GPS interface

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_GPS_ERROR_CODE_BASE                             19000

#define  VICEC_GPS_UNKNOWN_ERROR                               (1 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_UNKNOWN_ERROR,                              "Internal error: ViCom GPS unknown error")

#define  VICEC_GPS_INTERNAL                                    (2 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_INTERNAL,                                   "Internal error: ViCom GPS")

#define VICEC_GPS_INVALID_MESSAGE_FORMAT                       (3 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_INVALID_MESSAGE_FORMAT,                     "GPS message format is not valid.")

#define VICEC_GPS_NMEA_FORMAT_NOT_ENABLED                      (4 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_NMEA_FORMAT_NOT_ENABLED,                    "NMEA format is not enabled.")

#define VICEC_GPS_UBLOX_FORMAT_NOT_ENABLED                     (5 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_UBLOX_FORMAT_NOT_ENABLED,                   "UBLOX format is not enabled.")

#define VICEC_GPS_NO_MESSAGE_AVAILABLE                         (6 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_NO_MESSAGE_AVAILABLE,                       "No GPS message is available.")

#define VICEC_GPS_INVALID_MESSAGE_SIZE                         (7 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_INVALID_MESSAGE_SIZE,                       "Invalid message size.")

#define VICEC_GPS_SEND_MESSAGE_FAILED                          (8 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_SEND_MESSAGE_FAILED,                        "Send message failed.")

#define VICEC_GPS_INVALID_SAT_NAV_SYSTEM                       (9 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_INVALID_SAT_NAV_SYSTEM,                     "Satellite navigation system type is not valid.")

#define VICEC_GPS_CONFIG_NOT_SET_YET                           (10 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_CONFIG_NOT_SET_YET,                         "The GPS device settings have not been applied yet via SetGPSDeviceSettings().")

#define VICEC_GPS_INIT_FAILED                                  (11 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_INIT_FAILED,                                "Initialization of the GPS device failed.")

#define VICEC_GPS_UBLOX_ADMIN_NOT_FOUND                        (12 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_UBLOX_ADMIN_NOT_FOUND,                      "Could not load UbloxAdmin.dll.")

#define VICEC_GPS_UBLOX_ADMIN_INSTANCE                         (13 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC(VICEC_GPS_UBLOX_ADMIN_INSTANCE,                       "Could not get UbloxAdmin instance.")

#define VICEC_GPS_INVALID_RESET_MODE                           (14 + VICOM_GPS_ERROR_CODE_BASE)
SETVICEC( VICEC_GPS_INVALID_RESET_MODE,                        "Reset mode is not valid." )