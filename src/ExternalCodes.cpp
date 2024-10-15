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
		default: return "Unknown error Code " + std::to_string(code);
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
		default: return "Unknown message type " + std::to_string(code);
	}
}

/*
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
	*/