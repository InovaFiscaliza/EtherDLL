/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2021-2021 TCI International, Inc. All rights reserved         *
**************************************************************************/
#pragma once

enum EBistScope { LAST_RESULT, PERFORM_BIST, PERFORM_DIAGNOSTICS };
enum class ETipReason { NO_REASON, DBQUERY, FIRST_TIP, LOC_CHANGED, AZ_CHANGED, TIP_TIMEOUT, SIGNAL_DOWN, FINAL_TIP };

struct SScorpioAPIClient
{
	std::wstring hostName;
	std::wstring port;
	unsigned long sendTimeout;
	//ConfigParams::EAvailable available;
};

struct SCapabilities
{
	bool dynamicAntennalist;
	bool rds;
	bool modrec;
	bool newVCP;
};

// Types
enum ERetCode : unsigned long
{
	API_SUCCESS = 0,
	API_NO_INSTANCE = 1,
	CMD_SENT_ERROR = 2,
	INVALID_REQUEST_ID = 3,
	INVALID_SERVER_ID = 4,
	UNABLE_TO_RESOLVE_HOST = 5,
	PARM_MEMORY_UNINITIALIZED = 6,
	NO_VALID_API_LICENSE = 7,
	API_CONNECTONS_EXCEEDED = 8,
	MEMORY_ALLOC_ERROR = 9
};

enum ECSMSDllMsgType : unsigned long
{
	GET_MSG_VERSION = 0,
	GET_TASK_STATUS = 1,
	GET_TASK_STATE = 2,
	GET_BIST = 3,
	GET_DIAGNOSTICS = 4,
	GET_BIST_RESULT = 5,
	GET_ANT = 6,
	SET_ANT = 7,
	GET_ANT_LIST_INFO = 8,
	TASK_TERMINATE = 9,
	TASK_SUSPEND = 10,
	TASK_RESUME = 11,
	GET_OCCUPANCY = 12,
	VALIDATE_OCCUPANCY = 13,
	GET_OCCUPANCYDF = 14,
	GET_AVD = 15,
	SET_AUDIO_PARAMS = 16,
	OCC_DATA_RESTART_RESPONSE = 17,
	OCC_NODATA_RESTART_RESPONSE = 18,
	EFLD_DATA_RESTART_RESPONSE = 19,
	EFLD_NODATA_RESTART_RESPONSE = 20,
	OCC_TIMEDELAY_DATA_RESTART_RESPONSE = 21,
	OCC_TIMEDELAY_NODATA_RESTART_RESPONSE = 22,
	OCC_MSGLEN_DATA_RESTART_RESPONSE = 23,
	OCC_MSGLEN_NODATA_RESTART_RESPONSE = 24,
	OCC_MSGLEN_DIST_RESPONSE = 25,
	OCC_STATE_RESPONSE = 26,
	OCC_SOLICIT_STATE_RESPONSE = 27,
	OCC_FREQ_VS_CHANNEL = 28,
	OCC_CHANNEL_RESULT = 29,
	OCC_EFLD_CHANNEL_RESULT = 30,
	OCC_TIMEOFDAY_RESULT = 31,
	OCC_MSGLEN_CHANNEL_RESULT = 32,
	OCC_EFLD_TIMEOFDAY_RESULT = 33,
	OCC_SPECTRUM_RESPONSE = 34,
	OCC_STATUS = 35,
	OCCDF_STATE_RESPONSE = 36,
	OCCDF_SOLICIT_STATE_RESPONSE = 37,
	OCCDF_STATUS = 38,
	OCCDF_FREQ_VS_CHANNEL = 39,
	OCCDF_SCANDF_VS_CHANNEL = 40,
	AVD_STATE_RESPONSE = 41,
	AVD_SOLICIT_STATE_RESPONSE = 42,
	AVD_FREQ_VS_CHANNEL = 43,
	AVD_FREQ_MEAS = 44,
	AVD_BW_MEAS = 45,
	AVD_OCC_CHANNEL_RESULT = 46,
	AVD_STATUS = 47,
	DM_STATE_RESPONSE = 51,
	DM_SOLICIT_STATE_RESPONSE = 52,
	DM_FREQ_VS_CHANNEL = 53,
	SET_PAN_PARAMS = 54,
	GET_PAN = 55,
	SET_RCVR = 56,

	RT_SPECTRUM_START = 61,
	RT_SPECTRUM_STOP = 62,
	RT_SPECTRUM_V1RESPONSE = 63,
	RT_SPECTRUM_V2RESPONSE = 64,
	RT_SPECTRUM_RESPONSE = 65,

	RT_IQ_DATA = 69,

	GET_DM = 70,
	RT_DF_START = 71,
	RT_DF_STARTV1 = 72,
	RT_DF_STOP = 73,
	RT_DF_DATA = 74,
	RT_DF_DATAV1 = 75,
	RT_DF_DATAV2 = 76,

	FREE_AUDIO_CHANNEL = 80,
	SET_AUDIO_SWITCH = 81,
	SET_PHONE_HOOK = 82,
	GET_REAL_TIME_CONNECTION = 83,
	GET_MEAS = 84,
	VALIDATE_MEAS = 85,

	SET_AUDIO_ANSWER_MODE = 101,
	GET_AUDIO_SWITCH_STATUS = 102,
	/*SOUNDER_TEST = 194,
	SOUNDER_START = 195,
	SOUNDER_STOP = 196,
	SOUNDER_SWEEP_SETUP = 197,
	SOUNDER_BLANKER_FREQ = 198,
	SOUNDER_IONOREQUEST = 199,*/
	SERVER_READY = 200
	//FAST_MODE_STOP_PAUSE = 103
};

struct SOccupReqData
{
	SEquipCtrlMsg::EDurationMethod durationMethod;
	SEquipCtrlMsg::EThresholdMethod thresholdMethod;
	unsigned long storageTime;		// seconds
	unsigned long measurementTime;	// seconds
	unsigned char confidenceLevel;	// percent
	unsigned char desiredAccuracy;	// percent
	//bool useSecondaryThreshold;  // not implemented
	//bool saveIntermediateData;   // not implmeneted
	// following two parameters
	//  the FIXED threshold (dBuV/m) is always in index 0
	//  the NOISE threshold (dB) is always in index 1 
	//  which thresholds used are based upon thresholdMethod
	//  any unused thresholds do not need to be set (won't be validated)
	short occPrimaryThreshold[2];
	//short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0, not implemented
	unsigned long occupancyMinGap;	// seconds
	SEquipCtrlMsg::SOccupancyOutputFlagsV1 output;
	SEquipCtrlMsg::EAnt ant;
	unsigned long numBands;		// number of frequency bands
	SSmsMsg::SBandV4 band[1];
};

struct SOccDFReqData
{
	unsigned long storageTime;		// seconds
	unsigned long measurementTime;	// seconds
	unsigned long numAzimuths;
	unsigned long confidence;       // 0 to 100
	unsigned long recordHoldoff;	// record holdoff time in sec (RA3 function)
	unsigned char scanDfThreshold;	// dB above NOISE
	bool recordAudioDf;	// 0 = don't record; >0 = record (RA3 function)
	unsigned short numBands;		// number of frequency bands
	SEquipCtrlMsg::SRcvrCtrlCmd rcvrCtrl;		// for recording (RA3 function)
	SSmsMsg::SGetScanDfCmdV1::SBand band[1]; // Variable length
};

struct SAVDReqData
{
	unsigned long storageTime;		// seconds
	unsigned long measurementTime;	// seconds
	SEquipCtrlMsg::EAvdRate measurementRate;
	SEquipCtrlMsg::EAnt ant;
	unsigned char avdThreshold;		// dB above NOISE
	unsigned short numBands;		// number of frequency bands
	SSmsMsg::SBandV4 band[1];	// Variable length. @note: Array size 1 must be
};

struct SMeasReqData
{
	Units::Frequency::Raw freq;
	Units::Frequency::Raw bandwidth;
	SSmsMsg::EAnt ant;
	SSmsMsg::SGetBwCmd bwCmd;
	SSmsMsg::SGetFreqCmd freqCmd;
	SSmsMsg::SGetModulationCmd modulationCmd;
	SSmsMsg::SGetFieldStrengthCmd fieldStrengthCmd;
	SSmsMsg::SGetMeasCmdV5::SGetDfCmd dfCmd;
	SSmsMsg::SGetMeasCmdV5::SGetIQCmd iqCmd;
};


struct SPanParams
{
	SEquipCtrlMsg::EAnt antenna;                       // Antenna choice.
	SEquipCtrlMsg::SRcvrCtrlCmd rcvr;                // Has the SET_RCVR value.
};

struct SGetPanParams
{
	Units::Frequency::Raw	freq;
	Units::Frequency::Raw	bandwidth;
	unsigned char	rcvrAtten;
};

struct SAudioParams
{
	bool						anyChannel;		// true -> server will allocate a channel, false -> use specified channel
	unsigned long				channel;		// audio (demod) channel
	unsigned long 				streamID;
	Units::Frequency::Raw		freq;			// frequency in Hertz
	Units::Frequency::Raw		bandwidth;		// demod bandwidth in Hertz
	SSmsMsg::SRcvrCtrlCmd::EDetMode	detMode;		// OFF, AM, CW, FM, USB, LSB or EXTERNAL
	Units::Frequency::Raw		bfo;			// BFO in Hertz (+/- 8000)
	bool                        doRDS;
	bool 						doModRec;
	bool 						doAudio;
	bool						doSigRec;
	char                        ipAddressRDSRadio[SSmsMsg::IP_ADDRESS_LEN];
};

struct SScorpioAPIText
{
	enum ESeverity : unsigned char { INFO = 0, PASS = 0, WARNING = 1, ERROR_ = 2, FAIL = 2 };

	ESeverity	severity{INFO};
	std::wstring topic;
	std::wstring details;
};