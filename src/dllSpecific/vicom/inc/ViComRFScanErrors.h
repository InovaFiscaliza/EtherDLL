// Definition of specific ViCom error codes for the RFScan and RFPowerScan Measurement

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation
#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE                  11000

#define  VICEC_TO_MANY_RFP_SCANNER_ALREADY_LOADED              (0 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_TO_MANY_RFP_SCANNER_ALREADY_LOADED,             "Call sequence error: To many (32) RF Power Scanner are already loaded.")

#define  VICEC_RF_POWER_SCAN_NOT_FOUND                         (1 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_NOT_FOUND,                        "Parameter Error: RF Power Scanner not found.")

#define  VICEC_RF_POWER_SCAN_FREQ_OUT_OF_RANGE                 (2 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_FREQ_OUT_OF_RANGE,                "Parameter Error: Frequency out of range.")

#define  VICEC_RF_SCAN_RELEASE_TSMX_WHILE_MEAS                 (3 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RELEASE_TSMX_WHILE_MEAS,                "Call sequence error: The device is still in measurement mode during the request to disconnect.")

#define  VICEC_RF_SCAN_MEAS_RATE                               (4 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_MEAS_RATE,                              "Parameter Error: The selected measurement rate is out of range.")

#define  VICEC_RF_SCAN_TSMX_TIME_BASE_CORRECTION               (5 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_TSMX_TIME_BASE_CORRECTION,              "Parameter Error: The selected time base synchronisation is not valid for RF power scannning")

#define  VICEC_RF_SCAN_RF_SWEEP_SPAN                           (6 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RF_SWEEP_SPAN,                          "Parameter Error: Wrong RF sweep span.")

#define  VICEC_RF_SCAN_RF_SWEEP_SECTION_RELATION               (7 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RF_SWEEP_SECTION_RELATION,              "Parameter Error: Wrong RF sweep section relation.")

#define  VICEC_RF_SCAN_RF_SWEEP_ADC_OV                         (8 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RF_SWEEP_ADC_OV,                        "Parameter Error: To many insensitive measurements after AD-converter overflow.")

#define  VICEC_RF_SCAN_RF_SWEEP_RATE                           (9 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RF_SWEEP_RATE,                          "Parameter Error: RF sweep rate too high.")

#define  VICEC_RF_SCAN_RF_SWEEP_COLLECTION                     (10 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RF_SWEEP_COLLECTION,                    "Parameter Error: Wrong RF sweep collection number.")

#define  VICEC_RF_SCAN_RF_SWEEP_BUFFER_TIME                    (11 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_RF_SWEEP_BUFFER_TIME,                   "Parameter Error: RF sweep buffer time is out of range.")

#define  VICEC_RF_SCAN_CHANNEL_FILTER_IN_USE                   (12 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_FILTER_IN_USE,                  "Parameter Error: Channel filter with the given tag is already in use.")

#define  VICEC_RF_SCAN_CHANNEL_FILTER_PARAMETER                (13 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_FILTER_PARAMETER,               "Parameter Error: Channel filter parameter out of range.")

#define  VICEC_RF_SCAN_CHANNEL_FILTER_SCALING                  (14 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_FILTER_SCALING,                 "Parameter Error: Channel filter power scaling values out of range.")

#define  VICEC_RF_SCAN_REQUEST_IDENTITIER                      (15 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_REQUEST_IDENTITIER,                     "Parameter Error: Request identifier out of range.")

#define  VICEC_RF_SCAN_TIME_BETWEEN_REQUESTS                   (16 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_TIME_BETWEEN_REQUESTS,                  "Parameter Error: Time between requests out of RF sweep specific range.")

#define  VICEC_RF_SCAN_FREQUENCY_POSTPROCESS                   (17 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_FREQUENCY_POSTPROCESS,                  "Parameter Error: Wrong frequency post process.")

#define  VICEC_RF_SCAN_FREQUENCY_SUBRANGE                      (18 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_FREQUENCY_SUBRANGE,                     "Parameter Error: Frequency subrange does not fit the RF sweep frequency range.")

#define  VICEC_RF_SCAN_FREQUENCY_DETECTOR                      (19 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_FREQUENCY_DETECTOR,                     "Parameter Error: Wrong frequency detector.")

#define  VICEC_RF_SCAN_DISPLAY_LINES                           (20 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_DISPLAY_LINES,                          "Parameter Error: Count of display lines out of range.")

#define  VICEC_RF_SCAN_CHANNEL_FILTER                          (21 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_FILTER,                         "Parameter Error: Channel filter for given tag undefined.")

#define  VICEC_RF_SCAN_CHANNEL_FILTER_SUB_SEQ_COUNT            (22 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_FILTER_SUB_SEQ_COUNT,           "Parameter Error: Channel subsequence count out of range.")

#define  VICEC_RF_SCAN_CHANNEL_FILTER_FREQ_RANGE               (23 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_FILTER_FREQ_RANGE,              "Parameter Error: Channel filter out of RF sweep frequency range.")

#define  VICEC_RF_SCAN_CHANNEL_COUNT                           (24 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_CHANNEL_COUNT,                          "Parameter Error: Channel count out of range.")

#define  VICEC_RF_SCAN_TIME_DETECTOR                           (25 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_TIME_DETECTOR,                          "Parameter Error: Wrong time detector.")

#define  VICEC_RF_SCAN_MAX_POWER_INTEGER                       (26 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_MAX_POWER_INTEGER,                      "Parameter Error: Maximum interger value for power results is to small.")

#define  VICEC_RF_SCAN_POWER_INTERVAL                          (27 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_POWER_INTERVAL,                         "Parameter Error: Power interval out of range.")

#define  VICEC_RF_SCAN_ATTENUATION_MODE                        (28 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_ATTENUATION_MODE,                       "Parameter Error: Attenuation mode out of range.")

#define  VICEC_RF_SCAN_MAX_POWER_INTEGER_OV                    (29 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_MAX_POWER_INTEGER_OV,                   "Parameter Error: Maximum interger value for power conflicts with overflow bit0.")

#define  VICEC_RF_SCAN_POWER_WITH_OVERFLOW_FORMAT              (30 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_SCAN_POWER_WITH_OVERFLOW_FORMAT,             "Parameter Error: Wrong power with overflow format value.")

#define  VICEC_RF_POWER_SCAN_ALREADY_LOADED				         (50 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_ALREADY_LOADED,				         "Call sequence error: RF Power Scan is already loaded.")

#define  VICEC_RF_POWER_SCAN_STATE_NOTIDLE				         (52 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_STATE_NOTIDLE,				         "Call sequence error: The device is not in idle state")

#define  VICEC_RF_POWER_SCAN_STATE_NOTMEAS				         (53 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_STATE_NOTMEAS,				         "Call sequence error: The device is not in measurement state")

#define  VICEC_RF_POWER_SCAN_INVALID_SETTINGS			         (54 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_SETTINGS,			         "Parameter error: Invalid sweep settings")

#define  VICEC_RF_POWER_SCAN_INVALID_REPORTINGRATE		         (55 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_REPORTINGRATE,	         "Parameter error: The selected reporting rate is out of range.")

#define  VICEC_RF_POWER_SCAN_INVALID_MEASRATE			         (56 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_MEASRATE,			         "Parameter error: The selected measurement rate is out of range")

#define  VICEC_RF_POWER_SCAN_INVALID_WINDOWTYPE			         (57 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_WINDOWTYPE,		         "Parameter error: The selected window type is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_FFTSIZE				         (58 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_FFTSIZE,			         "Parameter error: The selected FFT size is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_BUFFERAGE			         (59 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_BUFFERAGE,			         "Parameter error: The selected max age of buffered measurements is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_BANDWIDTH			         (60 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_BANDWIDTH,			         "Parameter error: The selected bandwidth is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_LEVELTHRESHOLD	         (61 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_LEVELTHRESHOLD,	         "Parameter error: The selected level threshold is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_ATTENUATION		         (62 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_ATTENUATION,		         "Parameter error: The selected attenuation is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_COUNTOFLINES		         (63 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_COUNTOFLINES,		         "Parameter error: The selected count of lines for the frequency detector is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_FREQDETECTORTYPE	         (64 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_FREQDETECTORTYPE,         "Parameter error: The selected frequency detector type is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_INTERVALTYPE		         (65 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_INTERVALTYPE,		         "Parameter error: The selected interval type for the time detector is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_TIMEDETECTORTYPE	         (66 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_TIMEDETECTORTYPE,         "Parameter error: The selected time detector type is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_RESULTBUFFERDEPTH         (67 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_RESULTBUFFERDEPTH,        "Parameter error: The selected result buffer depth is out of range")

#define  VICEC_RF_POWER_SCAN_INVALID_STARTFREQ			         (68 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_STARTFREQ,			         "Parameter error: The selected start frequency is out of range")

#define  VICEC_RF_POWER_SCAN_INVALID_STOPFREQ			         (69 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_STOPFREQ,			         "Parameter error: The selected stop frequency is out of range")

#define  VICEC_RF_POWER_SCAN_INVALID_FREQRANGE			         (70 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_FREQRANGE,			         "Parameter error: The selected frequency range is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_FREQSPACING		         (71 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_FREQSPACING,		         "Parameter error: The selected frequency spacing is not valid")

#define  VICEC_RF_POWER_SCAN_INVALID_MEASTIMEDETECTORTYPE		(72 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_MEASTIMEDETECTORTYPE,		"Parameter error: The selected measurement time detector type is not valid")
 
#define  VICEC_RF_POWER_SCAN_INVALID_MEASTIME			         (73 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_MEASTIME,			         "Parameter error: The selected measurement time is out of range")

#define  VICEC_RF_POWER_SCAN_INVALID_TIME_PARAMETER	         (74 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_INVALID_TIME_PARAMETER,	         "Parameter error: The selected time detector time paramter is out of range")

#define  VICEC_RF_POWER_SCAN_SWEEP_SETTINGS_NOT_SET            (75 + VICOM_RFSCAN_SCANNER_ERROR_CODE_BASE)
SETVICEC(VICEC_RF_POWER_SCAN_SWEEP_SETTINGS_NOT_SET,           "Parameter Error: Sweep settings missing.")