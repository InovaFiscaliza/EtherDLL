// Definition of specific ViCom error codes for the GSM Network-Scanner Measurement

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define VICOM_GSM_ERROR_CODE_BASE                                             12000

#define  VICEC_GSM_ALREADY_LOADED                                             (0 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_ALREADY_LOADED,                                            "Call sequence error: GSM Network-Scanner is already loaded.")

#define  VICEC_GSM_NOT_FOUND                                                  (1 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_NOT_FOUND,                                                 "Parameter Error: The pointer to the interface is invalid.")

#define  VICEC_GSM_FREQ_OUT_OF_RANGE                                          (2 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_FREQ_OUT_OF_RANGE,                                         "Parameter Error: Frequency out of range.")

#define  VICEC_GSM_FREQ_COUNT_OUT_OF_RANGE                                    (3 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_FREQ_COUNT_OUT_OF_RANGE,                                   "Parameter Error: Number of selected frequencies is out of range.")

#define  VICEC_GSM_RELEASE_TSMX_WHILE_MEAS                                    (4 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_RELEASE_TSMX_WHILE_MEAS,                                   "Call sequence error: The TSMx is still in measurement mode during the request to release the TSMx.")

#define  VICEC_GSM_TSMX_TIME_BASE_CORRECTION                                  (5 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_TSMX_TIME_BASE_CORRECTION,                                 "Parameter Error: The selected time base synchronisation is not valid for GSN NWS")

#define  VICEC_GSM_INVALID_BTS_PARAMETERS                                     (6 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_INVALID_BTS_PARAMETERS,                                    "Parameter Error: Invalid BTS parameters.")

#define  VICEC_GSM_MEAS_RATE                                                  (7 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_MEAS_RATE,                                                 "Parameter Error: The selected measurement rate is out of range.")

#define  VICEC_GSM_FREQ_NOT_IN_ASCENDING_ORDER                                (8 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_FREQ_NOT_IN_ASCENDING_ORDER,                               "Parameter Error: The list of frequencies is not in ascending order.")

#define  VICEC_GSM_MEASUREMENT_DETAILS_DWCOUNT_OUTOFRANGE                     (9 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_MEASUREMENT_DETAILS_DWCOUNT_OUTOFRANGE,                    "Parameter Error: Measurement details: The parameter dwCount is out of range.")

#define  VICEC_GSM_MEASUREMENT_DETAILS_FREQUNCY_INDEX_OUTOFRANGE              (11 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_MEASUREMENT_DETAILS_FREQUNCY_INDEX_OUTOFRANGE,             "Parameter Error: Measurement details: A frequency index is out of range.")

#define  VICEC_GSM_MEASUREMENT_DETAILS_FREQUNCY_INDEX_OCURRS_DOUBLE           (12 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_MEASUREMENT_DETAILS_FREQUNCY_INDEX_OCURRS_DOUBLE,          "Parameter Error: Measurement details: A frequency index ocurrs double in the list.")

#define  VICEC_GSM_SPECTRUM_COUNTOFPOWERVALUESPERCHANNEL_OUTOFRANGE           (13 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SPECTRUM_COUNTOFPOWERVALUESPERCHANNEL_OUTOFRANGE,          "Parameter Error: Spectrum: wCountOfPowerValuesPerChannel is out of range.")

#define  VICEC_GSM_SPECTRUM_COLLECTIONTIMEIN100US_OUTOFRANGE                  (14 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SPECTRUM_COLLECTIONTIMEIN100US_OUTOFRANGE,                 "Parameter Error: Spectrum: wCollectionTimeIn100us is out of range.")

#define  VICEC_GSM_SPECTRUM_FREQ_DETECTOR_INVALID_VALUE                       (15 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SPECTRUM_FREQ_DETECTOR_INVALID_VALUE,                      "Parameter Error: Spectrum: eFreqDetector has invalid value.")

#define  VICEC_GSM_SPECTRUM_TIME_DETECTOR_INVALID_VALUE                       (16 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SPECTRUM_TIME_DETECTOR_INVALID_VALUE,                      "Parameter Error: Spectrum: eTimeDetector has invalid value.")

#define  VICEC_GSM_CHANNELPOWERSPEC_RMSLENGTHIN40NS_OUTOFRANGE                (17 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_CHANNELPOWERSPEC_RMSLENGTHIN40NS_OUTOFRANGE,               "Parameter Error: ChannelPowerSpec: wRMSLengthIn40ns is out of range.")

#define  VICEC_GSM_CHANNELPOWERSPEC_COUNTOFRESULTSPERGSMTIMESLOT_OUTOFRANGE   (18 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_CHANNELPOWERSPEC_COUNTOFRESULTSPERGSMTIMESLOT_OUTOFRANGE,  "Parameter Error: ChannelPowerSpec: wCountOfResultsPerGSMTimeSlot is out of range.")

#define  VICEC_GSM_L3_DEC_ERROR                                               (19 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_L3_DEC_ERROR,                                              "Internal Error: Layer 3 Decoder error.")

#define  VICEC_GSM_L3_DEC_UNKNOWN_CODE                                        (20 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_L3_DEC_UNKNOWN_CODE,                                       "Internal Error: The Layer 3 Decoder returned an unknown code.")

#define  VICEC_GSM_L3_DEC_RESULT_MISSING                                      (21 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_L3_DEC_RESULT_MISSING,                                     "Internal Error: The Layer 3 Decoder reported no error but returned no valid result.")

#define  VICEC_GSM_L3_DEC_SITYPE_EMPTY                                        (22 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_L3_DEC_SITYPE_EMPTY,                                       "The Layer 3 Decoder reported: No System Information Type bits to decode.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_TOTALPOWEROFFSET_OUTOFRANGE              (23 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_TOTALPOWEROFFSET_OUTOFRANGE,             "Parameter Error: SITypeDemodulation: lTotalPowerOffsetInDB10 is out of range.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_LOADINPERCENT_OUTOFRANGE                 (24 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_LOADINPERCENT_OUTOFRANGE,                "Parameter Error: SITypeDemodulation: wLoadInPercent is out of range.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_COUNTOFSITYPEREQUESTS_OUTOFRANGE         (25 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_COUNTOFSITYPEREQUESTS_OUTOFRANGE,        "Parameter Error: SITypeDemodulation: dwCountOfRequests is out of range.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_CHANNELINDEX_OUTOFRANGE                  (26 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_CHANNELINDEX_OUTOFRANGE,                 "Parameter Error: SITypeDemodulation: dwChannelIndex is out of range.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_SITYPE_OCCURS_DOUBLE_ON_CHANNEL          (27 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_SITYPE_OCCURS_DOUBLE_ON_CHANNEL,         "Parameter Error: SITypeDemodulation: duplicate request for a channel.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_SCHINDICATOR_NOT_NULL                    (28 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_SCHINDICATOR_NOT_NULL,                   "Parameter Error: SITypeDemodulation: dwSCHIndicator is invalid.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_REPETITIONDELAY_OUTOFRANGE               (29 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_REPETITIONDELAY_OUTOFRANGE,              "Parameter Error: SITypeDemodulation: wRepetitionDelayIn100ms is out of range.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_SITYPE_INVALID                           (30 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_SITYPE_INVALID,                           "Parameter Error: SITypeDemodulation: ePDU is invalid.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_MODE_INVALID                             (31 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_MODE_INVALID,                            "Parameter Error: SITypeDemodulation: eDemodulationMode is invalid.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_SCHINDICATOR_INVALID                     (32 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_SCHINDICATOR_INVALID,                    "Parameter Error: SITypeDemodulation: dwSCHIndicator is invalid.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_NOT_ALLOWED_WITHOUT_SETTING              (35 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_NOT_ALLOWED_WITHOUT_SETTING,             "Call sequence error:  SI Type demodulation is not allowed without demodulation settings.")

#define  VICEC_GSM_SITYPE_DEMOD_REQU_SITYPE_NOT_IN_SETTINGS                   (36 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_SITYPE_DEMOD_REQU_SITYPE_NOT_IN_SETTINGS,                  "Parameter Error: SITypeDemodulation: SI Type for channel must be registered in the settings.")

#define  VICEC_GSM_MEASUREMENT_DETAILS_NO_DETAILS_SPECIFIED                   (37 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_MEASUREMENT_DETAILS_NO_DETAILS_SPECIFIED,                  "Parameter Error: No frequencies with details were specified.")

#define  VICEC_GSM_MEASUREMENT_DETAILS_C_TO_I_REQUIRES_SCH_MEAS               (38 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_MEASUREMENT_DETAILS_C_TO_I_REQUIRES_SCH_MEAS,              "Parameter Error: Carrier to Interference measurement requires SCH measurement.")

#define  VICEC_GSM_FREQUENCY_TABLE_NOT_SET                                    (39 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_FREQUENCY_TABLE_NOT_SET,                                   "Parameter Error: Channel settings missing.")

#define  VICEC_GSM_DEMOD_SETTINGS_NOT_SET                                     (40 + VICOM_GSM_ERROR_CODE_BASE)
SETVICEC(VICEC_GSM_DEMOD_SETTINGS_NOT_SET,                                    "Parameter Error: Demodulation settings missing.")