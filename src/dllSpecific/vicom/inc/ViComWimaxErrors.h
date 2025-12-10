// Definition of specific ViCom error codes for the WIMAX Scanner Measurement

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_WIMAX_ERROR_CODE_BASE                       18000

#define  VICEC_WIMAX_UNKNOWN_ERROR                         (1 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_UNKNOWN_ERROR,                        "Internal error: ViCom WiMAX unknown error.")
 
#define  VICEC_WIMAX_INTERNAL                              (2 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_INTERNAL,                             "Internal error: ViCom WiMAX.")

#define  VICEC_WIMAX_FREQ_COUNT_OUT_OF_RANGE               (3 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_FREQ_COUNT_OUT_OF_RANGE,              "Parameter Error: Number of selected frequencies is out of range.")

#define  VICEC_WIMAX_FRONTEND_OUT_OF_RANGE                 (4 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_FRONTEND_OUT_OF_RANGE,                "Parameter Error: Invalid frontend selection.")

#define  VICEC_WIMAX_FREQ_OUT_OF_RANGE                     (5 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_FREQ_OUT_OF_RANGE,                    "Parameter Error: Frequency out of range.")

#define  VICEC_WIMAX_INVALID_BANDWIDTH_TYPE                (6 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_INVALID_BANDWIDTH_TYPE,               "Parameter Error: Invalid frame structure type.")

#define  VICEC_WIMAX_TRACE_MEAS_OUT_OF_RANGE               (7 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_TRACE_MEAS_OUT_OF_RANGE,              "Parameter Error: Parameter SFrequencySetting::dwTraceMeasPer1000Sec is out of range.")

#define  VICEC_WIMAX_BCH_LOAD                              (11 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_LOAD,                             "Parameter Error: Maximum load for BCH demodulation is out of range.")

#define  VICEC_WIMAX_CINR                                  (12 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_CINR,                                 "Parameter Error: The CINR threshold for the BCH demodulation trials is out of range.")

#define  VICEC_WIMAX_BCH_PARAM_DEMOD_MODE_WRONG            (13 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_PARAM_DEMOD_MODE_WRONG,           "Parameter Error: BCH Demodulator wrong Demodulation Mode.")

#define  VICEC_WIMAX_CHANNEL_INDEX                         (14 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_CHANNEL_INDEX,                        "Parameter Error: BCH Demodulator channel index out of range.")

#define  VICEC_WIMAX_PDU                                   (15 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_PDU,                                  "Parameter Error: The selected PDU value is out of range.")

#define  VICEC_WIMAX_L3_DEC_ERROR                          (16 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_L3_DEC_ERROR,                         "Internal Error: Layer 3 Decoder error.")

#define  VICEC_WIMAX_L3_DEC_UNKNOWN_CODE                   (17 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_L3_DEC_UNKNOWN_CODE,                  "Internal Error: The Layer 3 Decoder returned an unknown code.")

#define  VICEC_WIMAX_L3_DEC_RESULT_MISSING                 (18 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_L3_DEC_RESULT_MISSING,                "Internal Error: The Layer 3 Decoder reported no error but returned no valid result.")

#define  VICEC_WIMAX_L3_DEC_PDU_EMPTY                      (19 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_L3_DEC_PDU_EMPTY,                     "The Layer 3 Decoder reported: No PDU bits to decode.")

#define  VICEC_WIMAX_BCH_PARAM_TIME                        (20 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_PARAM_TIME,                       "Parameter Error: BCH Demodulator Time value must be 0.")

#define  VICEC_WIMAX_BCH_PARAM_PDUS_MISSING                (21 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_PARAM_PDUS_MISSING,               "Parameter Error: BCH Demodulator PDUs missing.")

#define  VICEC_WIMAX_BCH_PARAM_PDU_INVALID                 (22 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_PARAM_PDU_INVALID,                "Parameter Error: BCH Demodulator PDU value invalid.")

#define  VICEC_WIMAX_BCH_PARAM_PDU_OR_TIME_NOT_0           (23 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_PARAM_PDU_OR_TIME_NOT_0,          "Parameter Error: BCH Demodulator PDU or Time value not 0.")

#define  VICEC_WIMAX_BCH_PARAM_PDU_BTS_ID_OR_TIME_NOT_0    (24 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_BCH_PARAM_PDU_BTS_ID_OR_TIME_NOT_0,   "Parameter Error: BCH Demodulator PDU, BTS ID or Time value not 0.")

#define  VICEC_WIMAX_FREQUENCY_TABLE_NOT_SET               (25 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_FREQUENCY_TABLE_NOT_SET,              "Parameter Error: Channel settings missing.")

#define  VICEC_WIMAX_DEMOD_SETTINGS_NOT_SET                (26 + VICOM_WIMAX_ERROR_CODE_BASE)
SETVICEC(VICEC_WIMAX_DEMOD_SETTINGS_NOT_SET,               "Parameter Error: Demodulation settings missing.")