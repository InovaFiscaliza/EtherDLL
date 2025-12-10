// Definition of specific ViCom error codes for the TD-SCDMA PN-Scanner Measurement

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_TDSCDMA_ERROR_CODE_BASE                       20000

#define  VICEC_TDSCDMA_UNKNOWN_ERROR                         (1 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_UNKNOWN_ERROR,                        "Internal error: ViCom TD-SCDMA unknown error")

#define  VICEC_TDSCDMA_INTERNAL                              (2 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_INTERNAL,                             "Internal error: ViCom TD-SCDMA")

#define  VICEC_TDSCDMA_FREQ_COUNT_OUT_OF_RANGE               (3 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_FREQ_COUNT_OUT_OF_RANGE,              "Parameter Error: Number of selected frequencies is out of range.")

#define  VICEC_TDSCDMA_FREQ_OUT_OF_RANGE                     (4 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_FREQ_OUT_OF_RANGE,                    "Parameter Error: Frequency out of range.")

#define  VICEC_TDSCDMA_INVALID_MEAS_MODE                     (5 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_INVALID_MEAS_MODE,                    "Parameter Error: Invalid measurement mode.")

#define  VICEC_TDSCDMA_INVALID_ACTIVE_MEAS                   (6 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_INVALID_ACTIVE_MEAS,                  "Parameter Error: Invalid active measurements.")

#define  VICEC_TDSCDMA_BCH_LOAD                              (7 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_LOAD,                             "Parameter Error: Max. load for BCH demodulation is out of range.")

#define  VICEC_TDSCDMA_SINR                                  (8 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_SINR,                                 "Parameter Error: The SINR threshold for the BCH demodulation trials is out of range.")

#define  VICEC_TDSCDMA_BCH_PARAM_DEMOD_MODE_WRONG            (9 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_PARAM_DEMOD_MODE_WRONG,           "Parameter Error: BCH demodulator wrong demodulation mode.")

#define  VICEC_TDSCDMA_CHANNEL_INDEX                         (10 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_CHANNEL_INDEX,                        "Parameter Error: BCH demodulator channel index out of range.")

#define  VICEC_TDSCDMA_PDU                                   (11 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_PDU,                                  "Parameter Error: The selected PDU value is out of range.")

#define  VICEC_TDSCDMA_L3_DEC_ERROR                          (12 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_L3_DEC_ERROR,                         "Internal Error: Layer 3 decoder error.")

#define  VICEC_TDSCDMA_L3_DEC_UNKNOWN_CODE                   (13 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_L3_DEC_UNKNOWN_CODE,                  "Internal Error: The layer 3 decoder returned an unknown code.")

#define  VICEC_TDSCDMA_L3_DEC_RESULT_MISSING                 (14 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_L3_DEC_RESULT_MISSING,                "Internal Error: The layer 3 decoder reported no error but returned no valid result.")

#define  VICEC_TDSCDMA_L3_DEC_PDU_EMPTY                      (15 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_L3_DEC_PDU_EMPTY,                     "The layer 3 decoder reported: No PDU bits to decode.")

#define  VICEC_TDSCDMA_BCH_PARAM_TIME                        (16 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_PARAM_TIME,                       "Parameter Error: BCH demodulator Time value must be 0.")

#define  VICEC_TDSCDMA_BCH_PARAM_PDUS_MISSING                (17 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_PARAM_PDUS_MISSING,               "Parameter Error: BCH demodulator PDUs missing.")

#define  VICEC_TDSCDMA_BCH_PARAM_PDU_INVALID                 (18 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_PARAM_PDU_INVALID,                "Parameter Error: BCH demodulator PDU value invalid.")

#define  VICEC_TDSCDMA_BCH_PARAM_PDU_OR_TIME_NOT_0           (19 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_PARAM_PDU_OR_TIME_NOT_0,          "Parameter Error: BCH demodulator PDU or time value not 0.")

#define  VICEC_TDSCDMA_BCH_PARAM_PDU_BTS_ID_OR_TIME_NOT_0    (20 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_BCH_PARAM_PDU_BTS_ID_OR_TIME_NOT_0,   "Parameter Error: BCH demodulator PDU, BTS ID, SC or time value not 0.")

#define  VICEC_TDSCDMA_FREQUENCY_TABLE_NOT_SET               (21 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_FREQUENCY_TABLE_NOT_SET,              "Parameter Error: Channel settings missing.")

#define  VICEC_TDSCDMA_DEMOD_SETTINGS_NOT_SET                (22 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_DEMOD_SETTINGS_NOT_SET,               "Parameter Error: Demodulation settings missing.")

#define  VICEC_TDSCDMA_INVALID_MEAS_RATE                     (23 + VICOM_TDSCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_TDSCDMA_INVALID_MEAS_RATE,                    "Parameter Error: Invalid measurement rate.")