// Definition of specific ViCom error codes for the CW Measurement

// VICEC is an abbrivation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation
#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

#define  VICOM_CW_ERROR_CODE_BASE                        13000

#define  VICEC_TOO_MANY_CW_INTERFACES_ALREADY_LOADED     (0 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_TOO_MANY_CW_INTERFACES_ALREADY_LOADED,    "Call sequence error: Too many CW receiver interfaces are already loaded.")

#define  VICEC_CW_INTERFACE_NOT_FOUND                    (1 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_INTERFACE_NOT_FOUND,                   "Parameter Error: The pointer to the interface is invalid.")

#define  VICEC_CW_RELEASE_TSMX_WHILE_MEAS                (2 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_RELEASE_TSMX_WHILE_MEAS,               "Call sequence error: The TSMx is still in measurement mode during the request to release the TSMx.")

#define  VICEC_CW_TSMX_TIME_BASE_CORRECTION              (3 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_TSMX_TIME_BASE_CORRECTION,             "Parameter Error: The selected time base synchronisation is not valid for CW measurement")

#define  VICEC_CW_INTERNAL_IBPM_INIT                     (4 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_INTERNAL_IBPM_INIT,                    "Internal Error: IBPM initialization failed")

#define  VICEC_CW_EXCLUSIVE_TSMX_LOCK                    (5 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_EXCLUSIVE_TSMX_LOCK,                   "Parameter Error: The exclusive lock of the TSMx device failed. It's already in use.")

#define  VICEC_CW_WRONG_MEAS_TYPE                        (6 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_WRONG_MEAS_TYPE,                       "Parameter Error: Wrong measurement type")

#define  VICEC_CW_FREQ_COUNT_OUT_OF_RANGE                (7 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_FREQ_COUNT_OUT_OF_RANGE,               "Parameter Error: Number of selected frequencies is out of range for this measurement type.")

#define  VICEC_CW_FREQ_OUT_OF_RANGE                      (8 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_FREQ_OUT_OF_RANGE,                     "Parameter Error: Frequency out of range")

#define  VICEC_CW_FREQ_NOT_IN_ASCENDING_ORDER            (9 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_FREQ_NOT_IN_ASCENDING_ORDER,           "Parameter Error: The list of frequencies is not in ascending order.")

#define  VICEC_CW_BANDWIDTH_OUT_OF_RANGE                 (10 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_BANDWIDTH_OUT_OF_RANGE,                "Parameter Error: Channel Bandwidth out of range for this measurement type.")

#define  VICEC_CW_ATTENUATION_MODE                       (11 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_ATTENUATION_MODE,                      "Parameter Error: Attenuation mode out of range.")

#define  VICEC_CW_MEASUREMENT_TIME                       (12 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_MEASUREMENT_TIME,                      "Parameter Error: Measurement time out of range.")
																						
#define  VICEC_CW_ATTENUATION_SWITCH_MODE                (13 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_ATTENUATION_SWITCH_MODE,               "Parameter Error: Wrong attenuation switch mode.")

#define  VICEC_CW_DETECTOR                               (14 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_DETECTOR,                              "Parameter Error: Wrong detector.")

#define  VICEC_CW_INVALID_TRIGGER_MODE                   (15 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_INVALID_TRIGGER_MODE,                  "Parameter Error: Wrong Trigger mode")

#define  VICEC_CW_TRIGGER_FACTOR_LESS_1                  (16 + VICOM_CW_ERROR_CODE_BASE)
SETVICEC(VICEC_CW_TRIGGER_FACTOR_LESS_1,                 "Parameter Error: The trigger factor is less than 1.")

// ** Error Codes
