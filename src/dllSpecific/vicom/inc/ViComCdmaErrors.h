// Definition of specific ViCom error codes for the CDMA 2000 PN-Scanner Measurement

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define VICOM_CDMA_ERROR_CODE_BASE                             15000

#define  VICEC_CDMA_ALREADY_LOADED                             (0 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_ALREADY_LOADED,                            "Call sequence error: CDMA 2000 PN-Scanner is already loaded.")

#define  VICEC_CDMA_NOT_FOUND                                  (1 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_NOT_FOUND,                                 "Parameter Error: CDMA 2000 PN-Scanner not found.")

#define  VICEC_CDMA_FREQ_OUT_OF_RANGE                          (2 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_FREQ_OUT_OF_RANGE,                         "Parameter Error: Frequency out of range.")

#define  VICEC_CDMA_FREQ_COUNT_OUT_OF_RANGE                    (3 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_FREQ_COUNT_OUT_OF_RANGE,                   "Parameter Error: Number of selected frequencies is out of range.")

#define  VICEC_CDMA_RELEASE_TSMX_WHILE_MEAS                    (4 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_RELEASE_TSMX_WHILE_MEAS,                   "Call sequence error: The TSMx is still in measurement mode during the request to release the TSMx.")

#define  VICEC_CDMA_TSMX_TIME_BASE_CORRECTION                  (5 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_TSMX_TIME_BASE_CORRECTION,                 "Parameter Error: The selected time base synchronisation is not valid for CDMA2000 PN scannning")

#define  VICEC_CDMA_MEAS_MODE                                  (6 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_MEAS_MODE,                                 "Parameter Error: The selected measurement mode is not supported by the TSMx receiver type.")

#define  VICEC_CDMA_MEAS_RATE                                  (7 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_MEAS_RATE,                                 "Parameter Error: The selected measurement rate is out of range.")

#define  VICEC_CDMA_PNOFFS_ARBITR_LIM_INVAL                    (8 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_PNOFFS_ARBITR_LIM_INVAL,                   "Parameter Error: The PN offset aribtitrary limit value is invalid (not 0, 1).")

#define  VICEC_CDMA_MAX_VELOCITY_OUT_OF_RANGE                  (9 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_MAX_VELOCITY_OUT_OF_RANGE,                 "Parameter Error: Max. velocity is out of range.")

#define  VICEC_CDMA_DELAYOFPPSFALLINGEDGE_OUT_OF_RANGE         (10 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_DELAYOFPPSFALLINGEDGE_OUT_OF_RANGE,        "Parameter Error: Delay Of PPS Falling Edge is out of range.")

#define  VICEC_CDMA_SYNCCHANNELDEMODULATIONMODE_INVALID        (11 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_SYNCCHANNELDEMODULATIONMODE_INVALID,       "Parameter Error: Sync Channel Demodulation Mode is invalid.")

#define  VICEC_CDMA_BCH_EC_IO                                  (12 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_EC_IO,                                 "Parameter Error: The Ec/Io threshold for the BCH demodulation trials is out of range.")

#define  VICEC_CDMA_CHANNEL_INDEX                              (13 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_CHANNEL_INDEX,                             "Parameter Error: BCH Demodulator channel index out of range.")

#define  VICEC_CDMA_MSGID_MSGID                                (14 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_MSGID_MSGID,                               "Parameter Error: The selected ePdu value is out of range.")

#define  VICEC_CDMA_BCH_PARAM_TIME                             (15 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_PARAM_TIME,                            "Parameter Error: BCH Demodulator Time value must be 0.")

#define  VICEC_CDMA_BCH_PARAM_DEMOD_MODE_WRONG                 (16 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_PARAM_DEMOD_MODE_WRONG,                "Parameter Error: BCH Demodulator wrong Demodulation Mode.")

#define  VICEC_CDMA_BCH_TSMX_LOAD                              (17 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_TSMX_LOAD,                             "Parameter Error: Max. TSMx load for BCH demodulation is out of range.")

#define  VICEC_CDMA_BCH_PARAM_MSGIDS_MISSING                   (18 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_PARAM_MSGIDS_MISSING,                  "Parameter Error: BCH Demodulator ePdu missing.")

#define  VICEC_CDMA_BCH_PARAM_MSGID_INVALID                    (19 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_PARAM_MSGID_INVALID,                   "Parameter Error: BCH Demodulator ePdu value invalid.")

#define  VICEC_CDMA_BCH_PARAM_MSGID_OR_TIME_NOT_0              (20 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_PARAM_MSGID_OR_TIME_NOT_0,             "Parameter Error: BCH Demodulator ePdu or Time value not 0.")

#define  VICEC_CDMA_BCH_PARAM_MSGID_BTSID_OR_TIME_NOT_0        (21 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_BCH_PARAM_MSGID_BTSID_OR_TIME_NOT_0,       "Parameter Error: BCH Demodulator ePdu, BST ID or Time value not 0.")

#define  VICEC_CDMA_NO_C2K_FREQ                                (22 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_NO_C2K_FREQ,                               "Parameter Error: At least one frequency must be a C2K frequency, not all of them can be of EVDO Type")

#define  VICEC_CDMA_FULLSYNCRATE_OUTOFRANGE                    (23 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_FULLSYNCRATE_OUTOFRANGE,                   "Parameter Error: Full sync parameter is not in its valid range.")

#define  VICEC_CDMA_SHORTSYNCRATE_OUTOFRANGE                   (24 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_SHORTSYNCRATE_OUTOFRANGE,                  "Parameter Error: The short sync RATE parameter is not in its valid range.")

#define  VICEC_CDMA_SHORTSYNCRANGE_OUTOFRANGE                  (25 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_SHORTSYNCRANGE_OUTOFRANGE,                 "Parameter Error: The short sync RANGE is not in its valid range.")

#define  VICEC_CDMA_ATLEASTONEC2KFREQ                          (26 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_ATLEASTONEC2KFREQ,                         "Parameter Error: At least one frequency must be a CDMA 2000 frequency.")

#define  VICEC_EVDO_DEMODULATION_NOT_SUPPORTED_YET             (27 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_EVDO_DEMODULATION_NOT_SUPPORTED_YET,            "Currently the demodulation of Ev-DO channels is not supported.")

#define  VICEC_CDMA_L3_DEC_ERROR                               (50 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_L3_DEC_ERROR,                              "Internal Error: Layer 3 Decoder error.")
                                                               
#define  VICEC_CDMA_L3_DEC_UNKNOWN_CODE                        (51 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_L3_DEC_UNKNOWN_CODE,                       "Internal Error: The Layer 3 Decoder returned an unknown code.")
                                                               
#define  VICEC_CDMA_L3_DEC_RESULT_MISSING                      (52 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_L3_DEC_RESULT_MISSING,                     "Internal Error: The Layer 3 Decoder reported no error but returned no valid result.")
                                                               
#define  VICEC_CDMA_L3_DEC_PDU_EMPTY                           (53 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_L3_DEC_PDU_EMPTY,                          "The Layer 3 Decoder reported: No message bits to decode.")
                                                               
#define  VICEC_CDMA_L3_INVALIDCHANNEL                          (54 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_L3_INVALIDCHANNEL,                          "Messages from the channel given in the layer 3 request cannot be decoded.")

#define  VICEC_CDMA_FREQUENCY_TABLE_NOT_SET                    (55 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_FREQUENCY_TABLE_NOT_SET,                   "Parameter Error: Channel settings missing.")

#define  VICEC_CDMA_DEMOD_SETTINGS_NOT_SET                     (56 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC(VICEC_CDMA_DEMOD_SETTINGS_NOT_SET,                    "Parameter Error: Demodulation settings missing.")

#define  VICEC_CDMA_EVDO_SETTINGS_NOT_SET                      (57 + VICOM_CDMA_ERROR_CODE_BASE)
SETVICEC( VICEC_CDMA_EVDO_SETTINGS_NOT_SET,                    "Parameter Error: EvDo settings missing." )