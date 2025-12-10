// Definition of specific ViCom error codes for the LTE PN-Scanner Measurement

// VICEC is an abbreviation for "ViCom error code"
// See also ViComBasicErrors.h for further information.

// ** SETVICEC macro default implementation

#ifndef SETVICEC
#define SETVICEC(Code, String)
#endif

// ** Error Codes
#define  VICOM_LTE_ERROR_CODE_BASE                          17000

#define  VICEC_LTE_UNKNOWN_ERROR                            (1 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_UNKNOWN_ERROR,                           "Internal error: ViCom LTE unknown error")

#define  VICEC_LTE_INTERNAL                                 (2 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INTERNAL,                                "Internal error: ViCom LTE")

#define  VICEC_LTE_FREQ_COUNT_OUT_OF_RANGE                  (3 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_FREQ_COUNT_OUT_OF_RANGE,                 "Parameter Error: Number of selected frequencies is out of range.")

#define  VICEC_LTE_FREQ_OUT_OF_RANGE                        (4 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_FREQ_OUT_OF_RANGE,                       "Parameter Error: Frequency out of range.")

#define  VICEC_LTE_INVALID_SYMBOLS_PER_SLOT                 (5 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SYMBOLS_PER_SLOT,                "Parameter Error: invalid symbols per slot.")

#define  VICEC_LTE_INVALID_FRAME_STRUCTURE_TYPE             (6 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_FRAME_STRUCTURE_TYPE,            "Parameter Error: invalid frame structure type.")

#define  VICEC_LTE_INVALID_AVG_BLOCK_COUNT                  (7 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_AVG_BLOCK_COUNT,                 "Parameter Error: invalid average block count.")

#define  VICEC_LTE_INVALID_SYNC_RATIO_TYPE                  (8 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SYNC_RATIO_TYPE,                 "Parameter Error: invalid sync ratio type.")

#define  VICEC_LTE_INVALID_SYNC_RATIO_COUNT                 (9 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SYNC_RATIO_COUNT,                "Parameter Error: invalid sync ratio count.")

#define  VICEC_LTE_INVALID_SYNC_RATIO_ORDER                 (10 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SYNC_RATIO_ORDER,                "Parameter Error: invalid sync ratio order.")

#define  VICEC_LTE_SINR                                     (11 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_SINR,                                    "Parameter Error: The SINR threshold for the BCH demodulation trials is out of range.")

#define  VICEC_LTE_BCH_PARAM_DEMOD_MODE_WRONG               (12 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_BCH_PARAM_DEMOD_MODE_WRONG,              "Parameter Error: BCH demodulator wrong demodulation mode.")

#define  VICEC_LTE_CHANNEL_INDEX                            (13 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_CHANNEL_INDEX,                           "Parameter Error: BCH demodulator channel index out of range.")

#define  VICEC_LTE_PDU                                      (14 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_PDU,                                     "Parameter Error: The selected PDU value is out of range.")

#define  VICEC_LTE_L3_DEC_ERROR                             (15 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_L3_DEC_ERROR,                            "Internal Error: Layer 3 decoder error.")

#define  VICEC_LTE_L3_DEC_UNKNOWN_CODE                      (16 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_L3_DEC_UNKNOWN_CODE,                     "Internal Error: The layer 3 decoder returned an unknown code.")

#define  VICEC_LTE_L3_DEC_RESULT_MISSING                    (17 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_L3_DEC_RESULT_MISSING,                   "Internal Error: The layer 3 decoder reported no error but returned no valid result.")

#define  VICEC_LTE_L3_DEC_PDU_EMPTY                         (18 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_L3_DEC_PDU_EMPTY,                        "The layer 3 decoder reported: No PDU bits to decode.")

#define  VICEC_LTE_BCH_PARAM_TIME                           (19 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_BCH_PARAM_TIME,                          "Parameter Error: BCH demodulator Time value must be 0.")

#define  VICEC_LTE_BCH_PARAM_PDUS_MISSING                   (20 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_BCH_PARAM_PDUS_MISSING,                  "Parameter Error: BCH demodulator PDUs missing.")

#define  VICEC_LTE_BCH_PARAM_PDU_INVALID                    (21 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_BCH_PARAM_PDU_INVALID,                   "Parameter Error: BCH demodulator PDU value invalid.")

#define  VICEC_LTE_BCH_PARAM_PDU_OR_TIME_NOT_0              (22 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_BCH_PARAM_PDU_OR_TIME_NOT_0,             "Parameter Error: BCH demodulator PDU or time value not 0.")

#define  VICEC_LTE_BCH_PARAM_PDU_BTS_ID_OR_TIME_NOT_0       (23 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_BCH_PARAM_PDU_BTS_ID_OR_TIME_NOT_0,      "Parameter Error: BCH demodulator PDU, BTS ID or time value not 0.")

#define  VICEC_LTE_INVALID_RS_SIGNAL_MEAS_MODE              (24 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_RS_SIGNAL_MEAS_MODE,             "Parameter Error: invalid reference signal measurement mode.")

#define  VICEC_LTE_INVALID_UP_DOWNLINK_MASK                 (25 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_UP_DOWNLINK_MASK,                "Parameter Error: invalid Up-/Downlink mask.")

#define  VICEC_LTE_INVALID_TRANSMIT_ANTENNA_MASK            (26 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_TRANSMIT_ANTENNA_MASK,           "Parameter Error: invalid transmit antenna selection mask.")

#define  VICEC_LTE_INVALID_BANDWIDTH_CONTROL_MODE           (28 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_BANDWIDTH_CONTROL_MODE,          "Parameter Error: invalid bandwidth control mode.")

#define  VICEC_LTE_INVALID_NUMBER_OF_RBS                    (29 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_NUMBER_OF_RBS,                   "Parameter Error: Number of resource blocks out of rage")

#define  VICEC_LTE_INVALID_DELAY_SPREAD                     (30 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_DELAY_SPREAD,                    "Parameter Error: Delay spread for channel model out of range")

#define  VICEC_LTE_INVALID_MAXIMUM_SPEED                    (31 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MAXIMUM_SPEED,                   "Parameter Error: Maximum speed for channel model out of range")

#define  VICEC_LTE_INVALID_RBS_IN_SUBBAND                   (32 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_RBS_IN_SUBBAND,                  "Parameter Error: Number of resource blocks in subband out of range")

#define  VICEC_LTE_INVALID_FE_SELECTION_MASK                (35 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_FE_SELECTION_MASK,               "Parameter Error: invalid front end selection mask")

#define  VICEC_LTE_INVALID_RSSI_MEAS_MODE                   (36 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_RSSI_MEAS_MODE,                  "Parameter Error: invalid RSSI meas mode")

#define  VICEC_LTE_INVALID_MIMO_MODE_MASK                   (39 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MIMO_MODE_MASK,                  "Parameter Error: invalid MIMO mode mask")

#define  VICEC_LTE_INVALID_MIMO_TIME_RESOLUTION             (40 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MIMO_TIME_RESOLUTION,            "Parameter Error: invalid MIMO time resolution")

#define  VICEC_LTE_INVALID_MIMO_RESULT_MASK                 (41 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MIMO_RESULT_MASK,                "Parameter Error: invalid MIMO result selection mask")

#define  VICEC_LTE_INVALID_SYNC_RATIO_VALUE                 (42 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SYNC_RATIO_VALUE,                "Parameter Error: invalid sync ratio value.")

#define  VICEC_LTE_FREQUENCY_TABLE_NOT_SET                  (43 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_FREQUENCY_TABLE_NOT_SET,                 "Parameter Error: Channel settings missing.")

#define  VICEC_LTE_DEMOD_SETTINGS_NOT_SET                   (44 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_DEMOD_SETTINGS_NOT_SET,                  "Parameter Error: Demodulation settings missing.")

#define  VICEC_LTE_INVALID_TDD_IF_THRESHOLD                 (45 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_TDD_IF_THRESHOLD,                "Parameter Error: invalid TDD interference analysis threshold." )

#define  VICEC_LTE_INVALID_SPECIAL_SF_CONFIG                (46 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SPECIAL_SF_CONFIG,               "Parameter Error: invalid special subframe configuration mask." )

#define  VICEC_LTE_INVALID_MBMS_CONFIG_CONTROL_MODE         (47 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MBMS_CONFIG_CONTROL_MODE,        "Parameter Error: invalid MBMS config control mode.")

#define  VICEC_LTE_INVALID_MBSFN_AREA_ID                    (48 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MBSFN_AREA_ID,                   "Parameter Error: MbmsManualConfig: invalid MBSFN area ID.")

#define  VICEC_LTE_INVALID_NON_MBSFN_REGION_LENGTH          (49 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_NON_MBSFN_REGION_LENGTH,         "Parameter Error: MbmsManualConfig: invalid non-MBSFN region length.")

#define  VICEC_LTE_INVALID_NOTIFICATION_INDICATOR           (50 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_NOTIFICATION_INDICATOR,          "Parameter Error: MbmsManualConfig: invalid notification indicator.")

#define  VICEC_LTE_INVALID_MCCH_OFFSET                      (51 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MCCH_OFFSET,                     "Parameter Error: MbmsManualConfig: invalid MCCH offset.")

#define  VICEC_LTE_INVALID_SF_ALLOC_INFO_BIT_MAP            (52 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SF_ALLOC_INFO_BIT_MAP,           "Parameter Error: MbmsManualConfig: invalid subframe allocation info bit map.")

#define  VICEC_LTE_INVALID_SIGNALLING_MCS                   (53 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SIGNALLING_MCS,                  "Parameter Error: MbmsManualConfig: invalid signalling MCS.")

#define  VICEC_LTE_INVALID_MCCH_REPETITION_PERIOD           (54 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MCCH_REPETITION_PERIOD,          "Parameter Error: MbmsManualConfig: invalid MCCH repetition period.")

#define  VICEC_LTE_INVALID_MCCH_MODIFICATION_PERIOD         (55 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MCCH_MODIFICATION_PERIOD,        "Parameter Error: MbmsManualConfig: invalid MCCH modification period.")

#define  VICEC_LTE_INVALID_ONE_FRAME_OR_FOUR_FRAMES         (56 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_ONE_FRAME_OR_FOUR_FRAMES,        "Parameter Error: MbmsManualConfig: bOneFrameOrFourFrames value must be 1 or 4.")

#define  VICEC_LTE_INVALID_SUBFRAME_ALLOCATION_BIT_MAP      (57 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_SUBFRAME_ALLOCATION_BIT_MAP,     "Parameter Error: MbmsManualConfig: invalid subframe allocation bit map.")

#define  VICEC_LTE_INVALID_RADIO_FRAME_ALLOCATION_PERIOD    (58 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_RADIO_FRAME_ALLOCATION_PERIOD,   "Parameter Error: MbmsManualConfig: invalid radio frame allocation period.")

#define  VICEC_LTE_INVALID_RADIO_FRAME_ALLOCATION_OFFSET    (59 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_RADIO_FRAME_ALLOCATION_OFFSET,   "Parameter Error: MbmsManualConfig: invalid radio frame allocation offset.")

#define  VICEC_LTE_RS_CINR_MEAS_REQUIRED_FOR_MBFSN_MEAS     (60 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_RS_CINR_MEAS_REQUIRED_FOR_MBFSN_MEAS,    "Parameter Error: MBFSN measurement requires wideband RS CINR measurement to be enabled.")

#define  VICEC_LTE_INVALID_MBFSN_MAX_SUBFRAME_RATE          (61 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC(VICEC_LTE_INVALID_MBFSN_MAX_SUBFRAME_RATE,         "Parameter Error: invalid maximum subframe rate for MBFSN measurement.")

#define  VICEC_LTE_WB_DISABLED                              (62 + VICOM_LTE_ERROR_CODE_BASE)
SETVICEC( VICEC_LTE_WB_DISABLED,                            "Parameter Error: Wideband measurements disabled, required for MIMO." )