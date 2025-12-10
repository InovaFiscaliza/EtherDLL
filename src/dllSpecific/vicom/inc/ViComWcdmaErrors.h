// Definition of specific ViCom error codes for the WCDMA 3GPP PN-Scanner Measurement

// VICEC is an abbrivation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_WCDMA_ERROR_CODE_BASE                        10000
                                                            
#define  VICEC_WCDMA_ALREADY_LOADED                         (0 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_ALREADY_LOADED,                        "Call sequence error: WCDMA 3GPP PN-Scanner is already loaded.")
                                                            
#define  VICEC_WCDMA_NOT_FOUND                              (1 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_NOT_FOUND,                             "Parameter Error: WCDMA 3GPP PN-Scanner not found.")

#define  VICEC_WCDMA_FREQ_OUT_OF_RANGE                      (2 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_FREQ_OUT_OF_RANGE,                     "Parameter Error: Frequency out of range.")
                                                            
#define  VICEC_WCDMA_FREQ_COUNT_OUT_OF_RANGE                (3 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_FREQ_COUNT_OUT_OF_RANGE,               "Parameter Error: Number of selected frequencies is out of range.")
                                                            
#define  VICEC_WCDMA_RELEASE_TSMX_WHILE_MEAS                (4 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_RELEASE_TSMX_WHILE_MEAS,               "Call sequence error: The TSMx is still in measurement mode during the request to release the TSMx.")
                                                            
#define  VICEC_WCDMA_TSMX_TIME_BASE_CORRECTION              (5 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_TSMX_TIME_BASE_CORRECTION,             "Parameter Error: The selected time base synchronisation is not valid for WCDMA 3GPP PN scannning")
                                                            
#define  VICEC_WCDMA_MEAS_MODE                              (6 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_MEAS_MODE,                             "Parameter Error: The selected measurement mode is not supported by the TSMx receiver type.")
                                                            
#define  VICEC_WCDMA_MEAS_RATE                              (7 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_MEAS_RATE,                             "Parameter Error: The selected measurement rate is out of range.")
                                                            
#define  VICEC_WCDMA_BCH_EC_IO                              (8 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_EC_IO,                             "Parameter Error: The Ec/Io threshold for the BCH demodulation trials is out of range.")
                                                            
#define  VICEC_WCDMA_CHANNEL_INDEX                          (9 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_CHANNEL_INDEX,                         "Parameter Error: BCH Demodulator channel index out of range.")
                                                            
#define  VICEC_WCDMA_PDU                                    (10 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_PDU,                                   "Parameter Error: The selected PDU value is out of range.")

#define  VICEC_WCDMA_L3_DEC_ERROR                           (11 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_L3_DEC_ERROR,                          "Internal Error: Layer 3 Decoder error.")

#define  VICEC_WCDMA_L3_DEC_UNKNOWN_CODE                    (12 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_L3_DEC_UNKNOWN_CODE,                   "Internal Error: The Layer 3 Decoder returned an unknown code.")

#define  VICEC_WCDMA_L3_DEC_RESULT_MISSING                  (13 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_L3_DEC_RESULT_MISSING,                 "Internal Error: The Layer 3 Decoder reported no error but returned no valid result.")

#define  VICEC_WCDMA_L3_DEC_PDU_EMPTY                       (14 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_L3_DEC_PDU_EMPTY,                      "The Layer 3 Decoder reported: No PDU bits to decode.")

#define  VICEC_WCDMA_BCH_PARAM_TIME                         (15 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_PARAM_TIME,                        "Parameter Error: BCH Demodulator Time value must be 0.")
                                                            
#define  VICEC_WCDMA_BCH_PARAM_PDUS_MISSING                 (16 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_PARAM_PDUS_MISSING,                "Parameter Error: BCH Demodulator PDUs missing.")
                                                            
#define  VICEC_WCDMA_BCH_PARAM_PDU_INVALID                  (17 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_PARAM_PDU_INVALID,                 "Parameter Error: BCH Demodulator PDU value invalid.")
                                                            
#define  VICEC_WCDMA_BCH_PARAM_PDU_OR_TIME_NOT_0            (18 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_PARAM_PDU_OR_TIME_NOT_0,           "Parameter Error: BCH Demodulator PDU or Time value not 0.")

#define  VICEC_WCDMA_BCH_PARAM_PDU_SC_SCID_OR_TIME_NOT_0    (19 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_PARAM_PDU_SC_SCID_OR_TIME_NOT_0,   "Parameter Error: BCH Demodulator PDU, SC, SCID or Time value not 0.")

#define  VICEC_WCDMA_BCH_PARAM_DEMOD_MODE_WRONG             (20 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_PARAM_DEMOD_MODE_WRONG,            "Parameter Error: BCH Demodulator wrong Demodulation Mode.")

#define  VICEC_WCDMA_BCH_TSMX_LOAD                          (21 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_TSMX_LOAD,                         "Parameter Error: Max. TSMx load for BCH demodulation is out of range.")

#define  VICEC_WCDMA_FREQUENCY_TABLE_NOT_SET                (22 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_FREQUENCY_TABLE_NOT_SET,               "Parameter Error: Channel settings missing.")
                                                            
#define  VICEC_WCDMA_DEMOD_SETTINGS_NOT_SET                 (23 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_DEMOD_SETTINGS_NOT_SET,                "Parameter Error: Demodulation settings missing.")

#define  VICEC_WCDMA_BCH_NODEB_HOLD_TIME                    (24 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_WCDMA_BCH_NODEB_HOLD_TIME,                   "Parameter Error: Max. NodeB hold time for BCH demodulation is out of range.")

#define  VICEC_WCDMA_TOA_WINDOWN_LENGTH                     (25 + VICOM_WCDMA_ERROR_CODE_BASE)
SETVICEC( VICEC_WCDMA_TOA_WINDOWN_LENGTH,                   "Parameter Error: Window length for ToA averaging is out of range." )