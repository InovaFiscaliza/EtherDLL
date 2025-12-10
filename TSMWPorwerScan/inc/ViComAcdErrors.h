// Definition of specific ViCom error codes for the Automatic Channel Detection measurement.
// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_ACD_ERROR_CODE_BASE                       30000

#define  VICEC_ACD_UNKNOWN_ERROR                         (1 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC(VICEC_ACD_UNKNOWN_ERROR,                        "Internal error: ViCom ACD unknown error")

#define  VICEC_ACD_INTERNAL                              (2 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC(VICEC_ACD_INTERNAL,                             "Internal error: ViCom ACD")

#define  VICEC_ACD_SETTINGS_NOT_SET                      (3 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC( VICEC_ACD_SETTINGS_NOT_SET,                    "Parameter Error: Channel settings missing." )

#define  VICEC_ACD_TEC_COUNT_OUT_OF_RANGE                (4 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC( VICEC_ACD_TEC_COUNT_OUT_OF_RANGE,              "Parameter Error: Number of selected technologies is out of range." )

#define  VICEC_ACD_MEAS_MODE                             (5 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC( VICEC_ACD_MEAS_MODE,                           "Parameter Error: The selected measurement mode is not supported." )

#define  VICEC_ACD_SENSITIVITY                           (6 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC( VICEC_ACD_SENSITIVITY,                         "Parameter Error: The selected sensitivity is not supported." )

#define  VICEC_ACD_TRIALS_OUT_OF_RANGE                   (7 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC( VICEC_ACD_TRIALS_OUT_OF_RANGE,                 "Parameter Error: The selected number of trials is out of range." )

#define  VICEC_ACD_UNSUPPORTED_TECHNOLOGY                (8 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC( VICEC_ACD_UNSUPPORTED_TECHNOLOGY,              "Parameter Error: Unsupported technology for ACD " )

#define  VICEC_ACD_ROMES_IS_RUNNING                      (9 + VICOM_ACD_ERROR_CODE_BASE)
SETVICEC(VICEC_ACD_ROMES_IS_RUNNING,                    "ViCom ACD cannot be used while Romes is executing")