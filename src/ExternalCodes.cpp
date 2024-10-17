#include "stdafx.h"
#include "ScorpioAPIDll.h"
#include "string"

//
// Function to convert ERetCode to string
//
std::string ERetCodeToString(ERetCode code)
{
    switch (code)
    {
		case API_SUCCESS: return "API success. Code " + std::to_string(code);
		case API_NO_INSTANCE: return "No API instance. Code " + std::to_string(code);
		case CMD_SENT_ERROR: return "Command sent error. Code " + std::to_string(code);
		case INVALID_REQUEST_ID: return "Invalid request ID. Code " + std::to_string(code);
		case INVALID_SERVER_ID: return "Invalid server ID. Code " + std::to_string(code);
		case UNABLE_TO_RESOLVE_HOST: return "Unable to resolve host. Code " + std::to_string(code);
		case PARM_MEMORY_UNINITIALIZED: return "Memory parameter uninitialized. Code " + std::to_string(code);
		case NO_VALID_API_LICENSE: return "No valid API license. Code " + std::to_string(code);
		case API_CONNECTONS_EXCEEDED: return "Maximal number of API connections exceeded. Code " + std::to_string(code);
		case MEMORY_ALLOC_ERROR: return "Memory allocation error. Code " + std::to_string(code);
		default: return "Unknown error. Code " + std::to_string(code);
    }
}

//
// Function to convert ECSMSDllMsgType to string
//
std::string ECSMSDllMsgTypeToString(ECSMSDllMsgType code)
{
	switch (code)
	{
		case GET_MSG_VERSION: return "Get message version. Code " + std::to_string(code);
		case GET_TASK_STATUS: return "Get task status. Code " + std::to_string(code);
		case GET_TASK_STATE: return "Get task state. Code " + std::to_string(code);
		case GET_BIST: return "Get basic self test (BIST). Code " + std::to_string(code);
		case GET_DIAGNOSTICS: return "Get diagnostics. Code " + std::to_string(code);
		case GET_BIST_RESULT: return "Get basic self test result (BIST). Code " + std::to_string(code);
		case GET_ANT: return "Get antenna. Code " + std::to_string(code);
		case SET_ANT: return "Set antenna. Code " + std::to_string(code);
		case GET_ANT_LIST_INFO: return "Get antenna list info. Code " + std::to_string(code);
		case TASK_TERMINATE: return "Task terminate. Code " + std::to_string(code);
		case TASK_SUSPEND: return "Task suspend. Code " + std::to_string(code);
		case TASK_RESUME: return "Task resume.Code " + std::to_string(code);
		case GET_OCCUPANCY: return "Get occupancy data. Code " + std::to_string(code);
		case VALIDATE_OCCUPANCY: return "Validate occupancy data. Code " + std::to_string(code);
		case GET_OCCUPANCYDF: return "Get occupancy with Direction Finding data. Code " + std::to_string(code);
		case GET_AVD: return "Get Automatic Violation Detection data. Code " + std::to_string(code);
		case SET_AUDIO_PARAMS: return "Set audio parameters. Code " + std::to_string(code);
		case OCC_DATA_RESTART_RESPONSE: return "Response to occupancy (OCC) data restart. Code " + std::to_string(code);
		case OCC_NODATA_RESTART_RESPONSE: return "Response to occupancy (OCC) no data restart. Code " + std::to_string(code);
		case EFLD_DATA_RESTART_RESPONSE: return "Response to with extended Fraunhofer line discriminator (eFLD) data restart. Code " + std::to_string(code);
		case EFLD_NODATA_RESTART_RESPONSE: return "Response to with extended Fraunhofer line discriminator (eFLD) no data restart. Code " + std::to_string(code);
		case OCC_TIMEDELAY_DATA_RESTART_RESPONSE: return "Response to occupancy (OCC) time delay data restart. Code " + std::to_string(code);
		case OCC_TIMEDELAY_NODATA_RESTART_RESPONSE: return "Response to occupancy (OCC) time delay no data restart. Code " + std::to_string(code);
		case OCC_MSGLEN_DATA_RESTART_RESPONSE: return "Response to occupancy (OCC) message length data restart. Code " + std::to_string(code);
		case OCC_MSGLEN_NODATA_RESTART_RESPONSE: return "Response to occupancy (OCC) message length no data restart. Code " + std::to_string(code);
		case OCC_MSGLEN_DIST_RESPONSE: return "Response to occupancy (OCC) message length distribution. Code " + std::to_string(code);
		case OCC_STATE_RESPONSE: return "Response to occupancy (OCC) task state. Code " + std::to_string(code);
		case OCC_SOLICIT_STATE_RESPONSE: return "Response to occupancy (OCC) request task state. Code " + std::to_string(code);
		case OCC_FREQ_VS_CHANNEL: return "Occupancy (OCC) frequency vs channel data. Code " + std::to_string(code);
		case OCC_CHANNEL_RESULT: return "Occupancy (OCC) channel result. Code " + std::to_string(code);
		case OCC_EFLD_CHANNEL_RESULT: return "Occupancy (OCC) with extended Fraunhofer line discriminator channel result. Code " + std::to_string(code);
		case OCC_TIMEOFDAY_RESULT: return "Occupancy (OCC) time of day result. Code " + std::to_string(code);
		case OCC_MSGLEN_CHANNEL_RESULT: return "Occupancy (OCC) message length channel result. Code " + std::to_string(code);
		case OCC_EFLD_TIMEOFDAY_RESULT: return "Occupancy (OCC) with extended Fraunhofer line discriminator time of day result. Code " + std::to_string(code);
		case OCC_SPECTRUM_RESPONSE: return "Occupancy (OCC) spectrum response. Code " + std::to_string(code);
		case OCC_STATUS: return "Occupancy (OCC) task status. Code " + std::to_string(code);
		case OCCDF_STATE_RESPONSE: return "Response to occupancy (OCC) with DF task state. Code " + std::to_string(code);
		case OCCDF_SOLICIT_STATE_RESPONSE: return "Response to occupancy (OCC) with DF request task state. Code " + std::to_string(code);
		case OCCDF_STATUS: return "Occupancy (OCC) with Direction Finding (DF) task status. Code " + std::to_string(code);
		case OCCDF_FREQ_VS_CHANNEL: return "Occupancy (OCC) with Direction Finding (DF) frequency vs channel. Code " + std::to_string(code);
		case OCCDF_SCANDF_VS_CHANNEL: return "Occupancy (OCC) with direction finding (DF) scan vs channel. Code " + std::to_string(code);
		case AVD_STATE_RESPONSE: return "Automatic Violation Detection (AVD) state response. Code " + std::to_string(code);
		case AVD_SOLICIT_STATE_RESPONSE: return "Automatic Violation Detection (AVD) request state response. Code " + std::to_string(code);
		case AVD_FREQ_VS_CHANNEL: return "Automatic Violation Detection (AVD) frequency vs channel. Code " + std::to_string(code);
		case AVD_FREQ_MEAS: return "Automatic Violation Detection (AVD) frequency measurement. Code " + std::to_string(code);
		case AVD_BW_MEAS: return "Automatic Violation Detection (AVD) bandwidth measurement. Code " + std::to_string(code);
		case AVD_OCC_CHANNEL_RESULT: return "Automatic Violation Detection (AVD) occupancy channel result. Code " + std::to_string(code);
		case AVD_STATUS: return "Automatic Violation Detection (AVD) task status. Code " + std::to_string(code);
		case DM_STATE_RESPONSE: return "Digital modulation (DM) response state. Code " + std::to_string(code);
		case DM_SOLICIT_STATE_RESPONSE: return "Digital modulation (DM) request state response. Code " + std::to_string(code);
		case DM_FREQ_VS_CHANNEL: return "Digital modulation (DM) frequency vs channel. Code " + std::to_string(code);
		case SET_PAN_PARAMS: return "Set panoramic (PAN) display parameters. Code " + std::to_string(code);
		case GET_PAN: return "Get panoramic (PAN) display results. Code " + std::to_string(code);
		case SET_RCVR: return "Set receiver. Code " + std::to_string(code);
		case RT_SPECTRUM_START: return "Start real-time spectrum. Code " + std::to_string(code);
		case RT_SPECTRUM_STOP: return "Stop real-time spectrum. Code " + std::to_string(code);
		case RT_SPECTRUM_V1RESPONSE: return "Real-time spectrum response v1. Code " + std::to_string(code);
		case RT_SPECTRUM_V2RESPONSE: return "Real-time spectrum response v2. Code " + std::to_string(code);
		case RT_SPECTRUM_RESPONSE: return "Real-time spectrum response. Code " + std::to_string(code);
		case RT_IQ_DATA: return "Real-time IQ data. Code " + std::to_string(code);
		case GET_DM: return "Get Digital Modulation (DM). Code " + std::to_string(code);
		case RT_DF_START: return "Real-time Direction Finding (DF) start. Code " + std::to_string(code);
		case RT_DF_STARTV1: return "Real-time Direction Finding (DF) start v1. Code " + std::to_string(code);
		case RT_DF_STOP: return "Real-time Direction Finding (DF) stop. Code " + std::to_string(code);
		case RT_DF_DATA: return "Real-time Direction Finding (DF) data. Code " + std::to_string(code);
		case RT_DF_DATAV1: return "Real-time Direction Finding (DF) data v1. Code " + std::to_string(code);
		case RT_DF_DATAV2: return "Real-time Direction Finding (DF) data v2. Code " + std::to_string(code);
		case FREE_AUDIO_CHANNEL: return "Free audio channel. Code " + std::to_string(code);
		case SET_AUDIO_SWITCH: return "Set audio switch. Code " + std::to_string(code);
		case SET_PHONE_HOOK: return "Set phone hook. Code " + std::to_string(code);
		case GET_REAL_TIME_CONNECTION: return "Get real-time connection. Code " + std::to_string(code);
		case GET_MEAS: return "Get measurement. Code " + std::to_string(code);
		case VALIDATE_MEAS: return "Validate measurement. Code " + std::to_string(code);
		case SET_AUDIO_ANSWER_MODE: return "Set audio answer mode. Code " + std::to_string(code);
		case GET_AUDIO_SWITCH_STATUS: return "Get audio switch status. Code " + std::to_string(code);
		case SERVER_READY: return "Server ready. Code " + std::to_string(code);
		default: return "Unknown message. Code " + std::to_string(code);
	}
}

//
// Function to convert BIST EResult values to string
//
std::string BISTResultToString(SEquipCtrlMsg::SGetBistResp::EResult code)
{
	switch (code)
	{
		case SEquipCtrlMsg::SGetBistResp::EResult::PASS: return "BIST passed. Code " + std::to_string(code);
		case SEquipCtrlMsg::SGetBistResp::EResult::FAIL: return "BIST failed. Code " + std::to_string(code);
		case SEquipCtrlMsg::SGetBistResp::EResult::WARNING: return "BIST warning. Code " + std::to_string(code);
		default: return "Unknown BIST result. Code " + std::to_string(code);
	}
}

//
// Function to convert SSmsMsg::EAnt values to string
//
std::string eAntToString(SSmsMsg::EAnt code)

{
	switch (code)
	{
		case 1: return "Ref in. Code " + std::to_string(code);
		case 2: return "Aux in. Code " + std::to_string(code);
		case 3: return "Aux2 in. Code " + std::to_string(code);
		case 4: return "Ref Horizontal in. Code " + std::to_string(code);
		case 11: return "DF Element 1V. Code " + std::to_string(code);
		case 12: return "DF Element 2V. Code " + std::to_string(code);
		case 13: return "DF Element 3V. Code " + std::to_string(code);
		case 14: return "DF Element 4V. Code " + std::to_string(code);
		case 15: return "DF Element 5V. Code " + std::to_string(code);
		case 16: return "DF Element 6V. Code " + std::to_string(code);
		case 17: return "DF Element 7V. Code " + std::to_string(code);
		case 18: return "DF Element 8V. Code " + std::to_string(code);
		case 19: return "DF Element 9V. Code " + std::to_string(code);
		case 20: return "DF Element 10V. Code " + std::to_string(code);
		case 21: return "DF Element 1H. Code " + std::to_string(code);
		case 22: return "DF Element 2H. Code " + std::to_string(code);
		case 23: return "DF Element 3H. Code " + std::to_string(code);
		case 24: return "DF Element 4H. Code " + std::to_string(code);
		case 25: return "DF Element 5H. Code " + std::to_string(code);
		case 26: return "DF Element 6H. Code " + std::to_string(code);
		case 27: return "DF Element 7H. Code " + std::to_string(code);
		case 28: return "DF Element 8H. Code " + std::to_string(code);
		case 29: return "DF Element 9H. Code " + std::to_string(code);
		case 31: return "SMPL RF2. Code " + std::to_string(code);
		case 32: return "SMPL RF3. Code " + std::to_string(code);
		case 33: return "Radio termination (on master, 2630). Code " + std::to_string(code);
		case 34: return "Radio termination (on slave 1, 2630). Code " + std::to_string(code);
		case 41: return "SHF extension Analyzer. Code " + std::to_string(code);
		case 50: return "Non auto-switch RF HF input. Code " + std::to_string(code);
		case 51: return "Log periodic antenna (30 MHz - 6 GHz) sit on rotator. Code " + std::to_string(code);
		case 52: return "647 Vertical monitor output (30 MHz - 6 GHz). Code " + std::to_string(code);
		case 53: return "647 Horizontal monitor output (30 MHz - 3 GHz). Code " + std::to_string(code);
		case 79: return "Vertical polarization output. Code " + std::to_string(code);
		case 80: return "Horizontal polarization monitoring output. Code " + std::to_string(code);
		case 81: return "Omni antenna: 1 - 26 GHz. Code " + std::to_string(code);
		case 82: return "LPA antenna: 20 MHz - 4 GHz. Code " + std::to_string(code);
		case 83: return "Horn antenna: 1 GHz - 18 GHz. Code " + std::to_string(code);
		case 84: return "Horn antenna: 18 GHz - 26.5 GHz. Code " + std::to_string(code);
		case 85: return "Vertical Reference/HF. Code " + std::to_string(code);
		case 86: return "LPA antenna: 20 MHz - 7 GHz. Code " + std::to_string(code);
		case 87: return "Horn antenna: 2 GHz - 18 GHz (Horizontal & Vertical). Code " + std::to_string(code);
		case 100: return "Used to rename the 'Aux 1' antenna. Code " + std::to_string(code);
		case 101: return "Used to rename the 'Sample RF 2' antenna. Code " + std::to_string(code);
		default:
			if (code >= 1000 && code <= 1999)
			{
				return "Dynamically assigned antenna. Code " + std::to_string(code);
			}
			else
			{
				return "Unknown antenna. Code " + std::to_string(code);
			}
	}
}

//
// Function to convert SSmsMsg::EAntPol values to string
//

std::string eAntPolToString(SSmsMsg::EAntPol code)
{
	switch (code)
	{
	case SSmsMsg::EAntPol::NA: return "not defined. Code " + std::to_string(code);
	case SSmsMsg::EAntPol::VERTICAL: return "Vertical. Code " + std::to_string(code);
	case SSmsMsg::EAntPol::HORIZONTAL: return "Horizontal. Code " + std::to_string(code);
	default: return "Unknown polarization. Code" + std::to_string(code);
	}
}

//
// Function to convert SEquipCtrlMsg::SStateResp::EState values to string
//
std::string eStateRespToString(SEquipCtrlMsg::SStateResp::EState code)
{
	switch (code)
	{
		case SEquipCtrlMsg::SStateResp::EState::IDLE: return "Idle. Code " + std::to_string(code);
		case SEquipCtrlMsg::SStateResp::EState::RUNNING: return "Running. Code " + std::to_string(code);
		case SEquipCtrlMsg::SStateResp::EState::SUSPENDED: return "Suspended. Code " + std::to_string(code);
		default: return "Unknown state. Code " + std::to_string(code);
	}
}

/*
namespace ErrorCodes
{
	enum EErrorCode
#ifdef _MSC_VER
#if _MSC_VER >= 1400
	: long
#endif
#else
	 : long
#endif
	{
		SUCCESS =				0,
		ERROR_PRIORITY_BUMP =	1,
		ERROR_REJECTED_CONNECT =	2,
		ERROR_NO_CONNECTION =	3,
		ERROR_INVALID_KEY =		4,
		INVALIDMSGTYPE =		5,
		INVALIDSUBTYPE =		6,
		INVALIDTASKID =			7,
		INVALIDTASKKEY =		8,
		TASKALREADYEXISTS =		9,
		NOERROR_SIMULATE =		10,
		NOERROR_OUTPUT_NONE =	11,
		NOERROR_DFNOTINIT =		12,
		INVALIDVERSION =		13,
		DUPLICATETASKID =		101,
		EQUIPCONTROLTIMEOUT =	102,
		EQUIPMENTTIMEOUT =		103,
		INVALIDFREQUENCY =		104,
		INVALIDBANDWIDTH =		105,
		HARDWARENOTPRESENT =	106,
		HARDWAREDOWN =			107,
		HARDWAREBUSY =			108,
		INVALIDDWELLTIME =		109,
		INVALIDBETAPARAM =		110,
		INVALIDYPARAM =			111,
		INVALIDX1PARAM =		112,
		INVALIDX2PARAM =		113,
		INVALIDREPEATCOUNT =	114,
		INVALIDAVEMETHOD =		115,
		INVALIDFREQMETHOD =		116,
		INVALIDSAMPLESIZE =		117,
		INVALIDFIELDMETHOD =	118,
		INVALIDCONFTHRESHOLD =	119,
		INVALIDDFBANDWIDTH =	120,
		INVALIDDETMODE =		121,
		INVALIDAGCTIME =		122,
		INVALIDBFO =			123,
		INVALIDRFATTEN =		124,
		OCCUPANCYFAILURE =		130,
		INVALIDFREQUENCYBAND =	131,
		INVALIDSWEEPWIDTH =		132,
		OCCUPANCYTOOMANYCHANS =	133,
		OCCUPANCYTOOMANYBANDS =	134,
		OCCUPANCYNOOUTPUTREQUEST =	135,
		OCCUPANCYTOOMANYACTIONS =	136,
		INVALIDOCCDATA =		137,
		OCCUPANCYNOBANDS =		138,
		OCCINVALIDCONFLEVEL =	139,
		OCCINVALIDACCURACY =	140,
		INVALIDDURATIONMETHOD =	141,
		INVALIDSTORAGETIME =	142,
		INVALIDTHRESHOLDMETHOD =	143,
		OCCNORESPONSEDEFINED =	144,
		OCCNORESULTDEFINED =	145,
		OCCNORESTARTDEFINED =	146,
		INVALIDGETDWELLDATA =	147,
		MEASUREMENTNOTACTIVE =	148,
		STARTUPINPROGRESS =		149,
		SCANDFACTIVE =			150,
		SCANDFTOOMANYBANDS =	151,
		SCANDFNOBANDS =			152,
		SCANDFTOOMANYCHANS =	153,
		UNABLETOGETDATA =		154,
		AUDIOSWCMDERROR =		155,
		CMDINVALIDSTDMODE =		156,
		CMDINVALIDFASTMODE =	157,
		CMDINVALIDFASTRUNNING =	158,
		CMDINVALIDFASTSTOPPED =	159,
		INVALIDANTENNAID =		160,
		INVALIDSIGNALTYPE =		161,
		INVALIDBWFACTOR =		162,
		FREQUENCYRESTRICTED =   163,
		PRECISIONTIMEUNAVAILABLE = 164,
		INVALIDAUDIOCHANNEL =	165,
		NOFREEAUDIOCHANNEL = 	166,
		INVALIDAUDIOFREQUENCY =	167,
		AUDIOCHANNELNOTFOUND =	168,
		CMDINVALIDVCPMODE 	=	169,
		INVALIDVCPPARAMETER = 	170,
		RXTUNEERROR =			171,
		RXATTENERROR =			172,
		RXCALGENERROR = 		173,
		RXCALGENFREQERROR =		174,
		RXOCXOERROR = 			175,
		RXTEMPSERROR =			176,
		DIGITEMPSERROR =		177,
		DIGIVOLTSERROR =		178,
		NOTDFSYSTEMERROR =		179,
		VMEBUSPROBLEM =			200,
		DSPBUSY =				202,
		DSPNOTINITIALIZED =		204,
		DSPMALLOCFAILED =		216,
		DSPHFSAMPTIMEOUT =		217,
		DSPAGCTIMEOUT =			218,
		DSPVHFSAMPERR =			219,
		DSPILLEGALBINS =		220,
		DSPILLEGALBANDWIDTH =	221,
		DSPBadDFAlgorFreq =		222,
		DSPDFAlgorNoInit =		223,
		DSPTooManyFewAnts =		224,
		DSPTooManyPhases =		225,
		DSPDivideByZero =		226,
		DSPZeroVoltageVector =	227,
		DSPPatternNotFound =	228,
		DSPVMEReadWriteError =	229,
		DSPCmdDataBoundsErr =	230,
		DSPUartTimeout =		231,
		DSPSENDINITFAILURE =	232,
		DSPNoPBCal =			233,
		DSPFieldStrengthNoInit =	234,
		DSPWrongUHFSwitch =		235,
		DSPVHFAntCalBadPower =	236,
		VMEOPENERROR =			251,
		VMENOTMAPPED =			252,
		NETWRITEFAILED =		301,
		NETWORKNOTENABLED =		302,
		PAT_MASTERFILEOPENERR =	331,
		PAT_MASTERFILEREADERR =	332,
		PAT_CTRLFILEOPENERR =	333,
		PAT_CTRLFILEREADERR =	334,
		PAT_PATFILEOPENERR =	335,
		PAT_PATFILEREADERR =	336,
		MALLOCERROR =			337,
		SHAREDRAMWRITEERROR =	338,
		CAL_NOERROR =			400,
		CAL_ACKNOWLEDGE =		401,
		CAL_CORRECTIONFACTOR =	402,
		UNCALIBRATED =			410,
		CAL_NORESPONSE =		411,
		CAL_NOTACKNOWLEDGE =	412,
		CAL_BADRESPONSE =		413,
		CAL_PLLERROR =			414,
		CAL_10MHZERROR =		415,
		CAL_PLL_10MHZERROR =	416,
		FLUXGATEWRITEERROR =	431,
		FLUXGATEREADERROR =		432,
		FLUXGATEDATAERROR =		433,
		REQUEST_TIME_OVERLAP =	500,	// Overlap with scheduled event.
		MEASURE_REQUEST_ERROR =	501,	// Incorrect request format or values
		REQUEST_INVALID_TIME =	502,	// Time(s) in invalid format
		REQUEST_INVALID_TIMESPAN = 503,	// Timespan exceeds limits (min or max)
		REQUEST_PRIORITY_FAIL =	504,	// Timespan scheduled by higher priority request
		MEASURE_ALLOC_FAIL =	505,	// No allocation equipment not available
		REQUEST_FAIL_DATABASE =	506,	// Unable to open/append to db
		REQUEST_UNKNOWN =		507,	// Unknown failure
		REQUEST_FAIL_DELETE =	508,	// Unable to delete requested record.
		REQUEST_FAIL_NOT_FOUND = 509,	// ID of record not found in db.
		MEASURE_REQUEST_PAST_TIME = 510,	// Requested time in the past.
		MEASURE_DATA_NOT_STORED = 511,	// Results not stored in database, fail on store.
		MEASURE_NOT_ONTIME_FINI = 512,	// The equip did not complete measurement by time scheduled.
		REQUEST_FAIL_NETWORK =	513,	// The request fail due to network not enabled.
		REQUEST_COLLIDE =		514,	// Overlap with concurrent request for same time slot.(try again)
		MEASURE_CANCELLED =		515,	// Measurement cancelled.
		REQUEST_FAIL_CALENDAR =	516,	// Calendar service not running
		SERVER_NOT_SYNC =		517,	// Server not syncd with client by over one minute.
		SERVER_FAIL_WORKLOAD =	518,	// Server Workload cannot be determined
		HIGHER_PRIORITY_RUNNING = 519,	// A Client has 'Taken Priority" unable to retrieve measurement.
		AVDTOOMANYCHANS =		533,
		AVDTOOMANYBANDS =		534,
		AVDNOOUTPUTREQUEST =	535,
		AVDTOOMANYACTIONS =		536,
		INVALIDAVDDATA =		537,
		AVDNOBANDS =			538,
		AVDINVALIDCONFLEVEL =	539,
		AVDINVALIDACCURACY =	540,
		AVDNORESPONSEDEFINED =	544,
		AVDNORESULTDEFINED =	545,
		AVDNORESTARTDEFINED =	546,
		PRIORITY_NOT_TAKEN	= 	547,
		PRIORITY_TAKEN_MANUAL = 548,
		PRIORITY_TASK_RUNNING = 549,
		INVALIDDATA			=	550,
		SU_ALREADY_IN_USE   =   551,
		SU_SIGN_IN_FAILED_UNKNOWN = 552,
		SU_SIGN_OUT_NO_USER   = 553,
		SU_SIGN_OUT_BY_IMPOSTER = 554,
		SU_WHO_NONE =           555,
		PRIORITY_REQ_BY_NON_SU= 556,
		COMMAND_FROM_NON_SU   = 560,
		TASK_COMPLETED =		600,	//  request completed successfully, no errors.
		MEASURE_SCHEDULED =		601,	//	measurement req scheduled OK.
		MEASURE_ALLOC_PARTIAL =	602,	//	request filled for one or more requests
		MEASURE_TIME_ADJUST =	603,	//	timespan adjusted for one or more requested timespans
		MEASURE_TIME_REDUCT =	604,	//  timespan reduced for one or more requested timespan
		MEASURE_IN_PROGRESS =	605,	//	request for results which are not complete
		CALENDAR_FREE =			606,	//  calendar is free for request
		CMDINVALIDDDRMODE = 	607,	//
		INVALID_DM_CMD=         608,
		ANT_ROTATOR_CONTENTION= 609,
		INVALID_DF_ANTENNA    = 610,
		BANDWIDTH_EXCEED_SHFEXT	= 611,	//Bandwidth specified exceeded Hardware capability.
		DATABASE_IS_FULL	  = 612,
		ERROR_SIMULATE =		1001,
		NOFLUXGATE =			0x01000000
	};
}
*/

//
// Function to convert EErrorCode to string
//
std::string eErrorCodeToString(ErrorCodes::EErrorCode code)
{
	switch (code)
	{
		case ErrorCodes::SUCCESS: return "Success. Code " + std::to_string(code);
		case ErrorCodes::ERROR_PRIORITY_BUMP: return "Priority bump error. Code " + std::to_string(code);
		case ErrorCodes::ERROR_REJECTED_CONNECT: return "Rejected connection error. Code " + std::to_string(code);
		case ErrorCodes::ERROR_NO_CONNECTION: return "No connection error. Code " + std::to_string(code);
		case ErrorCodes::ERROR_INVALID_KEY: return "Invalid key error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDMSGTYPE: return "Invalid message type error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDSUBTYPE: return "Invalid subtype error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDTASKID: return "Invalid task ID error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDTASKKEY: return "Invalid task key error. Code " + std::to_string(code);
		case ErrorCodes::TASKALREADYEXISTS: return "Task already exists error. Code " + std::to_string(code);
		case ErrorCodes::NOERROR_SIMULATE: return "No error simulate. Code " + std::to_string(code);
		case ErrorCodes::NOERROR_OUTPUT_NONE: return "No error output none. Code " + std::to_string(code);
		case ErrorCodes::NOERROR_DFNOTINIT: return "No error DF not initialized. Code " + std::to_string(code);
		case ErrorCodes::INVALIDVERSION: return "Invalid version error. Code " + std::to_string(code);
		case ErrorCodes::DUPLICATETASKID: return "Duplicate task ID error. Code " + std::to_string(code);
		case ErrorCodes::EQUIPCONTROLTIMEOUT: return "Equipment control timeout error. Code " + std::to_string(code);
		case ErrorCodes::EQUIPMENTTIMEOUT: return "Equipment timeout error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDFREQUENCY: return "Invalid frequency error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDBANDWIDTH: return "Invalid bandwidth error. Code " + std::to_string(code);
		case ErrorCodes::HARDWARENOTPRESENT: return "Hardware not present error. Code " + std::to_string(code);
		case ErrorCodes::HARDWAREDOWN: return "Hardware down error. Code " + std::to_string(code);
		case ErrorCodes::HARDWAREBUSY: return "Hardware busy error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDDWELLTIME: return "Invalid dwell time error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDBETAPARAM: return "Invalid beta parameter error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDYPARAM: return "Invalid Y parameter error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDX1PARAM: return "Invalid X1 parameter error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDX2PARAM: return "Invalid X2 parameter error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDREPEATCOUNT: return "Invalid repeat count error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDAVEMETHOD: return "Invalid average method error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDFREQMETHOD: return "Invalid frequency method error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDSAMPLESIZE: return "Invalid sample size error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDFIELDMETHOD: return "Invalid field method error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDCONFTHRESHOLD: return "Invalid confidence threshold error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDDFBANDWIDTH: return "Invalid DF bandwidth error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDDETMODE: return "Invalid detection mode error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDAGCTIME: return "Invalid AGC time error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDBFO: return "Invalid BFO error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDRFATTEN: return "Invalid RF attenuation error. Code " + std::to_string(code);
		case ErrorCodes::OCCUPANCYFAILURE: return "Occupancy failure error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDFREQUENCYBAND: return "Invalid frequency band error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDSWEEPWIDTH: return "Invalid sweep width error. Code " + std::to_string(code);
		case ErrorCodes::OCCUPANCYTOOMANYCHANS: return "Occupancy too many channels error. Code " + std::to_string(code);
		case ErrorCodes::OCCUPANCYTOOMANYBANDS: return "Occupancy too many bands error. Code " + std::to_string(code);
		case ErrorCodes::OCCUPANCYNOOUTPUTREQUEST: return "Occupancy no output request error. Code " + std::to_string(code);
		case ErrorCodes::OCCUPANCYTOOMANYACTIONS: return "Occupancy too many actions error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDOCCDATA: return "Invalid occupancy data error. Code " + std::to_string(code);
		case ErrorCodes::OCCUPANCYNOBANDS: return "Occupancy no bands error. Code " + std::to_string(code);
		case ErrorCodes::OCCINVALIDCONFLEVEL: return "Occupancy invalid confidence level error. Code " + std::to_string(code);
		case ErrorCodes::OCCINVALIDACCURACY: return "Occupancy invalid accuracy error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDDURATIONMETHOD: return "Invalid duration method error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDSTORAGETIME: return "Invalid storage time error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDTHRESHOLDMETHOD: return "Invalid threshold method error. Code " + std::to_string(code);
		case ErrorCodes::OCCNORESPONSEDEFINED: return "Occupancy no response defined error. Code " + std::to_string(code);
		case ErrorCodes::OCCNORESULTDEFINED: return "Occupancy no result defined error. Code " + std::to_string(code);
		case ErrorCodes::OCCNORESTARTDEFINED: return "Occupancy no restart defined error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDGETDWELLDATA: return "Invalid get dwell data error. Code " + std::to_string(code);
		case ErrorCodes::MEASUREMENTNOTACTIVE: return "Measurement not active error. Code " + std::to_string(code);
		case ErrorCodes::STARTUPINPROGRESS: return "Startup in progress error. Code " + std::to_string(code);
		case ErrorCodes::SCANDFACTIVE: return "Scan DF active error. Code " + std::to_string(code);
		case ErrorCodes::SCANDFTOOMANYBANDS: return "Scan DF too many bands error. Code " + std::to_string(code);
		case ErrorCodes::SCANDFNOBANDS: return "Scan DF no bands error. Code " + std::to_string(code);
		case ErrorCodes::SCANDFTOOMANYCHANS: return "Scan DF too many channels error. Code " + std::to_string(code);
		case ErrorCodes::UNABLETOGETDATA: return "Unable to get data error. Code " + std::to_string(code);
		case ErrorCodes::AUDIOSWCMDERROR: return "Audio switch command error. Code " + std::to_string(code);
		case ErrorCodes::CMDINVALIDSTDMODE: return "Command invalid STD mode error. Code " + std::to_string(code);
		case ErrorCodes::CMDINVALIDFASTMODE: return "Command invalid fast mode error. Code " + std::to_string(code);
		case ErrorCodes::CMDINVALIDFASTRUNNING: return "Command invalid fast running error. Code " + std::to_string(code);
		case ErrorCodes::CMDINVALIDFASTSTOPPED: return "Command invalid fast stopped error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDANTENNAID: return "Invalid antenna ID error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDSIGNALTYPE: return "Invalid signal type error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDBWFACTOR: return "Invalid bandwidth factor error. Code " + std::to_string(code);
		case ErrorCodes::FREQUENCYRESTRICTED: return "Frequency restricted error. Code " + std::to_string(code);
		case ErrorCodes::PRECISIONTIMEUNAVAILABLE: return "Precision time unavailable error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDAUDIOCHANNEL: return "Invalid audio channel error. Code " + std::to_string(code);
		case ErrorCodes::NOFREEAUDIOCHANNEL: return "No free audio channel error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDAUDIOFREQUENCY: return "Invalid audio frequency error. Code " + std::to_string(code);
		case ErrorCodes::AUDIOCHANNELNOTFOUND: return "Audio channel not found error. Code " + std::to_string(code);
		case ErrorCodes::CMDINVALIDVCPMODE: return "Command invalid VCP mode error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDVCPPARAMETER: return "Invalid VCP parameter error. Code " + std::to_string(code);
		case ErrorCodes::RXTUNEERROR: return "RX tune error. Code " + std::to_string(code);
		case ErrorCodes::RXATTENERROR: return "RX attenuation error. Code " + std::to_string(code);
		case ErrorCodes::RXCALGENERROR: return "RX calibration generator error. Code " + std::to_string(code);
		case ErrorCodes::RXCALGENFREQERROR: return "RX calibration generator frequency error. Code " + std::to_string(code);
		case ErrorCodes::RXOCXOERROR: return "RX OCXO error. Code " + std::to_string(code);
		case ErrorCodes::RXTEMPSERROR: return "RX temperature sensor error. Code " + std::to_string(code);
		case ErrorCodes::DIGITEMPSERROR: return "Digital temperature sensor error. Code " + std::to_string(code);
		case ErrorCodes::DIGIVOLTSERROR: return "Digital volts error. Code " + std::to_string(code);
		case ErrorCodes::NOTDFSYSTEMERROR: return "Not DF system error. Code " + std::to_string(code);
		case ErrorCodes::VMEBUSPROBLEM: return "VME bus problem error. Code " + std::to_string(code);
		case ErrorCodes::DSPBUSY: return "DSP busy error. Code " + std::to_string(code);
		case ErrorCodes::DSPNOTINITIALIZED: return "DSP not initialized error. Code " + std::to_string(code);
		case ErrorCodes::DSPMALLOCFAILED: return "DSP malloc failed error. Code " + std::to_string(code);
		case ErrorCodes::DSPHFSAMPTIMEOUT: return "DSP HF sample timeout error. Code " + std::to_string(code);
		case ErrorCodes::DSPAGCTIMEOUT: return "DSP AGC timeout error. Code " + std::to_string(code);
		case ErrorCodes::DSPVHFSAMPERR: return "DSP VHF sample error. Code " + std::to_string(code);
		case ErrorCodes::DSPILLEGALBINS: return "DSP illegal bins error. Code " + std::to_string(code);
		case ErrorCodes::DSPILLEGALBANDWIDTH: return "DSP illegal bandwidth error. Code " + std::to_string(code);
		case ErrorCodes::DSPBadDFAlgorFreq: return "DSP bad DF algorithm frequency error. Code " + std::to_string(code);
		case ErrorCodes::DSPDFAlgorNoInit: return "DSP DF algorithm not initialized error. Code " + std::to_string(code);
		case ErrorCodes::DSPTooManyFewAnts: return "DSP too many few antennas error. Code " + std::to_string(code);
		case ErrorCodes::DSPTooManyPhases: return "DSP too many phases error. Code " + std::to_string(code);
		case ErrorCodes::DSPDivideByZero: return "DSP divide by zero error. Code " + std::to_string(code);
		case ErrorCodes::DSPZeroVoltageVector: return "DSP zero voltage vector error. Code " + std::to_string(code);
		case ErrorCodes::DSPPatternNotFound: return "DSP pattern not found error. Code " + std::to_string(code);
		case ErrorCodes::DSPVMEReadWriteError: return "DSP VME read write error. Code " + std::to_string(code);
		case ErrorCodes::DSPCmdDataBoundsErr: return "DSP command data bounds error. Code " + std::to_string(code);
		case ErrorCodes::DSPUartTimeout: return "DSP UART timeout error. Code " + std::to_string(code);
		case ErrorCodes::DSPSENDINITFAILURE: return "DSP send init failure error. Code " + std::to_string(code);
		case ErrorCodes::DSPNoPBCal: return "DSP no PB calibration error. Code " + std::to_string(code);
		case ErrorCodes::DSPFieldStrengthNoInit: return "DSP field strength not initialized error. Code " + std::to_string(code);
		case ErrorCodes::DSPWrongUHFSwitch: return "DSP wrong UHF switch error. Code " + std::to_string(code);
		case ErrorCodes::DSPVHFAntCalBadPower: return "DSP VHF antenna calibration bad power error. Code " + std::to_string(code);
		case ErrorCodes::VMEOPENERROR: return "VME open error. Code " + std::to_string(code);
		case ErrorCodes::VMENOTMAPPED: return "VME not mapped error. Code " + std::to_string(code);
		case ErrorCodes::NETWRITEFAILED: return "Network write failed error. Code " + std::to_string(code);
		case ErrorCodes::NETWORKNOTENABLED: return "Network not enabled error. Code " + std::to_string(code);
		case ErrorCodes::PAT_MASTERFILEOPENERR: return "Pattern master file open error. Code " + std::to_string(code);
		case ErrorCodes::PAT_MASTERFILEREADERR: return "Pattern master file read error. Code " + std::to_string(code);
		case ErrorCodes::PAT_CTRLFILEOPENERR: return "Pattern control file open error. Code " + std::to_string(code);
		case ErrorCodes::PAT_CTRLFILEREADERR: return "Pattern control file read error. Code " + std::to_string(code);
		case ErrorCodes::PAT_PATFILEOPENERR: return "Pattern file open error. Code " + std::to_string(code);
		case ErrorCodes::PAT_PATFILEREADERR: return "Pattern file read error. Code " + std::to_string(code);
		case ErrorCodes::MALLOCERROR: return "Malloc error. Code " + std::to_string(code);
		case ErrorCodes::SHAREDRAMWRITEERROR: return "Shared RAM write error. Code " + std::to_string(code);
		case ErrorCodes::CAL_NOERROR: return "Calibration no error. Code " + std::to_string(code);
		case ErrorCodes::CAL_ACKNOWLEDGE: return "Calibration acknowledge error. Code " + std::to_string(code);
		case ErrorCodes::CAL_CORRECTIONFACTOR: return "Calibration correction factor error. Code " + std::to_string(code);
		case ErrorCodes::UNCALIBRATED: return "Uncalibrated error. Code " + std::to_string(code);
		case ErrorCodes::CAL_NORESPONSE: return "Calibration no response error. Code " + std::to_string(code);
		case ErrorCodes::CAL_NOTACKNOWLEDGE: return "Calibration not acknowledge error. Code " + std::to_string(code);
		case ErrorCodes::CAL_BADRESPONSE: return "Calibration bad response error. Code " + std::to_string(code);
		case ErrorCodes::CAL_PLLERROR: return "Calibration PLL error. Code " + std::to_string(code);
		case ErrorCodes::CAL_10MHZERROR: return "Calibration 10 MHz error. Code " + std::to_string(code);
		case ErrorCodes::CAL_PLL_10MHZERROR: return "Calibration PLL 10 MHz error. Code " + std::to_string(code);
		case ErrorCodes::FLUXGATEWRITEERROR: return "Fluxgate write error. Code " + std::to_string(code);
		case ErrorCodes::FLUXGATEREADERROR: return "Fluxgate read error. Code " + std::to_string(code);
		case ErrorCodes::FLUXGATEDATAERROR: return "Fluxgate data error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_TIME_OVERLAP: return "Request time overlap error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_REQUEST_ERROR: return "Measurement request error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_INVALID_TIME: return "Request invalid time error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_INVALID_TIMESPAN: return "Request invalid timespan error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_PRIORITY_FAIL: return "Request priority fail error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_ALLOC_FAIL: return "Measurement allocation fail error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_FAIL_DATABASE: return "Request fail database error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_UNKNOWN: return "Request unknown error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_FAIL_DELETE: return "Request fail delete error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_FAIL_NOT_FOUND: return "Request fail not found error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_REQUEST_PAST_TIME: return "Measurement request past time error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_DATA_NOT_STORED: return "Measurement data not stored error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_NOT_ONTIME_FINI: return "Measurement not on time finish error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_FAIL_NETWORK: return "Request fail network error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_COLLIDE: return "Request collide error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_CANCELLED: return "Measurement cancelled error. Code " + std::to_string(code);
		case ErrorCodes::REQUEST_FAIL_CALENDAR: return "Request fail calendar error. Code " + std::to_string(code);
		case ErrorCodes::SERVER_NOT_SYNC: return "Server not sync error. Code " + std::to_string(code);
		case ErrorCodes::SERVER_FAIL_WORKLOAD: return "Server fail workload error. Code " + std::to_string(code);
		case ErrorCodes::HIGHER_PRIORITY_RUNNING: return "Higher priority running error. Code " + std::to_string(code);
		case ErrorCodes::AVDTOOMANYCHANS: return "AVD too many channels error. Code " + std::to_string(code);
		case ErrorCodes::AVDTOOMANYBANDS: return "AVD too many bands error. Code " + std::to_string(code);
		case ErrorCodes::AVDNOOUTPUTREQUEST: return "AVD no output request error. Code " + std::to_string(code);
		case ErrorCodes::AVDTOOMANYACTIONS: return "AVD too many actions error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDAVDDATA: return "Invalid AVD data error. Code " + std::to_string(code);
		case ErrorCodes::AVDNOBANDS: return "AVD no bands error. Code " + std::to_string(code);
		case ErrorCodes::AVDINVALIDCONFLEVEL: return "AVD invalid confidence level error. Code " + std::to_string(code);
		case ErrorCodes::AVDINVALIDACCURACY: return "AVD invalid accuracy error. Code " + std::to_string(code);
		case ErrorCodes::AVDNORESPONSEDEFINED: return "AVD no response defined error. Code " + std::to_string(code);
		case ErrorCodes::AVDNORESULTDEFINED: return "AVD no result defined error. Code " + std::to_string(code);
		case ErrorCodes::AVDNORESTARTDEFINED: return "AVD no restart defined error. Code " + std::to_string(code);
		case ErrorCodes::PRIORITY_NOT_TAKEN: return "Priority not taken error. Code " + std::to_string(code);
		case ErrorCodes::PRIORITY_TAKEN_MANUAL: return "Priority taken manual error. Code " + std::to_string(code);
		case ErrorCodes::PRIORITY_TASK_RUNNING: return "Priority task running error. Code " + std::to_string(code);
		case ErrorCodes::INVALIDDATA: return "Invalid data error. Code " + std::to_string(code);
		case ErrorCodes::SU_ALREADY_IN_USE: return "SU already in use error. Code " + std::to_string(code);
		case ErrorCodes::SU_SIGN_IN_FAILED_UNKNOWN: return "SU sign in failed unknown error. Code " + std::to_string(code);
		case ErrorCodes::SU_SIGN_OUT_NO_USER: return "SU sign out no user error. Code " + std::to_string(code);
		case ErrorCodes::SU_SIGN_OUT_BY_IMPOSTER: return "SU sign out by imposter error. Code " + std::to_string(code);
		case ErrorCodes::SU_WHO_NONE: return "SU who none error. Code " + std::to_string(code);
		case ErrorCodes::PRIORITY_REQ_BY_NON_SU: return "Priority request by non SU error. Code " + std::to_string(code);
		case ErrorCodes::COMMAND_FROM_NON_SU: return "Command from non SU error. Code " + std::to_string(code);
		case ErrorCodes::TASK_COMPLETED: return "Task completed error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_SCHEDULED: return "Measurement scheduled error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_ALLOC_PARTIAL: return "Measurement allocation partial error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_TIME_ADJUST: return "Measurement time adjust error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_TIME_REDUCT: return "Measurement time reduction error. Code " + std::to_string(code);
		case ErrorCodes::MEASURE_IN_PROGRESS: return "Measurement in progress error. Code " + std::to_string(code);
		case ErrorCodes::CALENDAR_FREE: return "Calendar free error. Code " + std::to_string(code);
		case ErrorCodes::CMDINVALIDDDRMODE: return "Command invalid DDR mode error. Code " + std::to_string(code);
		case ErrorCodes::INVALID_DM_CMD: return "Invalid DM command error. Code " + std::to_string(code);
		case ErrorCodes::ANT_ROTATOR_CONTENTION: return "Antenna rotator contention error. Code " + std::to_string(code);
		case ErrorCodes::INVALID_DF_ANTENNA: return "Invalid DF antenna error. Code " + std::to_string(code);
		case ErrorCodes::BANDWIDTH_EXCEED_SHFEXT: return "Bandwidth exceeds SHF extension error. Code " + std::to_string(code);
		case ErrorCodes::DATABASE_IS_FULL: return "Database is full error. Code " + std::to_string(code);
		case ErrorCodes::ERROR_SIMULATE: return "Error simulate error. Code " + std::to_string(code);
		case ErrorCodes::NOFLUXGATE: return "No fluxgate error. Code " + std::to_string(code);
		default: return "Unknown error. Code " + std::to_string(code);
	}
}

			


