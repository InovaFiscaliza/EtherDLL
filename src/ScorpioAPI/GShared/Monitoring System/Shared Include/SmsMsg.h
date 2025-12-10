/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is pursuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2006-2023 TCI International, Inc. All rights reserved         *
**************************************************************************/

#pragma once

#include <map>
#include <algorithm>

#include "ErrorCodes.h"
#include "Units.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4480)
#include <stdint.h>
#else
#include <cassert>
#include <cstddef>
#include "MSCompat.h"
#endif


static const  float INVALID_VALUE = -9999999.0; // unable to use constexpr and static const won't work as member var for float
//Define bool with pre-defined size.
enum BOOL8 : char
{
	///@note: For Compatibility, it is imperative that the code compare with
	///			FALSE8 to obtain boolean true or false value because C & C++
	///			standard defined false to have a value of 0, while true can be
	///			any non-zero value.
	FALSE8 = 0,
	TRUE8 = 1,
};

// Common structures and definitions for SEquipCtrlMsg and SMetricsMsg
//
// Note: All conversion and assignment operators must be written to be safe when the input and output
// operands overlap in memory (eg they are members of the same union)
//
struct SSmsMsg
{
	// Message types
	enum EMsgType
	{
		NO_MSGTYPE =			0,
		AUDIO_SWITCH =			6,
//		RF_IF_MUX =				7,
//		CLOCK_SYNTH =			8,
//		IO_DIGITAL_IO =			10,
//		NB_ADC_SAMPLE =			19,
		WB_DIGITIZER_SAMPLE =	20,
//		INTER_BITE_CTRL =		24,
		DF_CTRL =				25,
		METRICS_CTRL =			26,
		MEASURE_CTRL =			27,
		DEMOD_CTRL =			28,
		PAN_DISP_CTRL =			29,
		OCCUPANCY_CTRL =		30,
		BIST_CTRL =				31,
//		SCAN_CTRL =				32,
		OCCUPANCYDF_CTRL =		33,
		AUTOVIOLATE_CTRL =		34,
		FAST_MODE_CTRL =		35,
		EQUIPMENT_CTRL =		50,
		DSP_CTRL =				51,
		PRIORITY_CTRL =			52,
		SUPER_USER_CTRL  =      53,
		STATUS_CTRL =			60,
		ANT_CTRL =				61,
		SYSTEM_STATE_CTRL =		62,
		SOUNDER_CTRL      =     63,
		VCP_CTRL		  =		64,
		INTERNAL_DF_CTRL =		65,
		DM_CTRL =               66,
		WBRECORDER_CTRL =		67,
		STREAMING_CTRL =		68,
#ifdef CSMSPLUS_SRV_BUILT
		CSMSPLUS_CTRL =			9900,		// Inter-SoM messages
#endif
		SMS_ERROR_REPORT =		9999,
		MEASUREMENT_BASE =		0x2172,
		MEASUREMENT_PAN_REQ =	MEASUREMENT_BASE + 1,
//		MEASUREMENT_PAN_RES =	MEASUREMENT_BASE + 2,
		MEASUREMENT_GREETING_REQ =	MEASUREMENT_BASE + 3,
		MEASUREMENT_GREETING_RES =	MEASUREMENT_BASE + 4,
		MEASUREMENT_ERROR_REPORT =	MEASUREMENT_BASE + 5,
		MEASUREMENT_ERROR_REPORTA = MEASUREMENT_BASE + 6
	};

	// Message subtypes for message type ANT_CTRL
	enum EAntCtrl
	{
		GET_ANT				= 1,
		SET_ANT				= 2,
		GET_ANT_RESPONSE	= 65537,
		SET_ANT_RESPONSE	= 65538
	};

	// Message subtypes for message type AUDIO_SWITCH
	enum EAudioSwitch
	{
		SET_SWITCH				= 1,
		GET_SWITCH_STATUS		= 2,
		SET_PHONE_HOOK			= 3,
		SET_AUTOANSWER_MODE		= 4,
		RESET_SWITCH			= 5,
		SWITCH_STATUS_RESPONSE  = 65537,
		AUTOANSWER_STATUS		= 65538
	};

	// Message subtypes for message type BIST_CTRL
	enum EBistCtrl
	{
		GET_BIST						= 1,
		GET_DIAGNOSTICS					= 2,
		GET_BIST_RESULT					= 3,
		GET_BIST_RESPONSE_OLD			= 65537,
		GET_DIAGNOSTICS_RESPONSE_OLD	= 65538,
		BIST_RESPONSE					= 65539,
		NOBIST_RESPONSE					= 65540,
		GET_BIST_RESPONSE				= 65541,
		GET_DIAGNOSTICS_RESPONSE		= 65542,
		GET_BIST_RESULT_RESPONSE		= 65543
	};

#ifndef SMS_SRV_BUILT
	// Message subtypes for message type SYSTEM_STATE_CTRL
	enum ESystemStateCtrl
	{
		SET_SYSTEM_STATE			= 1,
		SET_SYSTEM_STATE_RESPONSE	= 65537
	};
#endif

	// Message subtypes for message type DEMOD_CTRL
	enum EDemodCtrl
	{
		SET_RCVR =				1,
		AGC_ONOFF =				2,
		SET_RCVR_MAN =			3,
		SET_LAN_AUDIO =			4,
		SET_PAN_PARA =			5,
		SET_AUDIO_PARAMS = 		6,
		FREE_AUDIO_CHANNEL = 	7,
		GET_AUDIO_PARAMS = 		8,
		SET_RCVR_RESP =			65537,
		AGC_ONOFF_RESP =		65538,
		SET_RCVR_MAN_RESP =		65539,
		SET_LAN_AUDIO_RESP =	65540,
		SET_PAN_PARA_RESP =		65541,
		SET_AUDIO_PARAMS_RESP = 65542,
		FREE_AUDIO_CHANNEL_RESP = 65543,
		GET_AUDIO_PARAMS_RESP = 65544
	};

	enum ESounderCtrl
	{
		SOUNDER_SWEEP_SETUP = 1,
		SOUNDER_BLANKER_FREQ = 2,
		SOUNDER_START_STOP = 3,
		SOUNDER_TEST = 4,
		SOUNDER_IONOREQUEST = 5,
		SOUNDER_STATUS_REQ = 6,
		SOUNDER_SETUP_RESPONSE = 65537,
		SOUNDER_BLANKER_FREQ_RESPONSE = 65538,
		SOUNDER_START_STOP_RESPONSE = 65539,
		SOUNDER_TEST_RESPONSE = 65540,
		SOUNDER_IONO_RESPONSE = 65541,
		SOUNDER_UNSOLICITED_RESPONSE = 65542,
		SOUNDER_STATUS_RESPONSE = 65543
	};

	// Message subtypes for message type DF_CTRL
	enum EDfCtrl
	{
		PASSBAND_CAL			= 1,
		INITIALIZE_DF			= 2,
		DF_CMD					= 6,
		LOAD_IONOGRAM			= 7,
		GET_IONOGRAM			= 8,
		PASSBAND_CAL_RESPONSE	= 65537,
		INITIALIZE_DF_RESPONSE	= 65538,
		DF_RESPONSE				= 65542,
		LOAD_IONOGRAM_RESPONSE	= 65543,
		GET_IONOGRAM_RESPONSE	= 65544
	};

	// Message subtypes for message type METRICS_CTRL
	enum EMetricsCtrl
	{
		GET_MEAS				= 1,
		GET_DWELL				= 2,
		GET_DWELL_RESPONSE		= 3,
		VALIDATE_MEAS			= 4,
		INIT_FIELDSTRENGTH		= 5,
		GET_BAND_REQUEST		= 6,
		GET_GPS					= 7,
		GET_HEADING				= 8,
		GET_ANT_LIST_INFO		= 9,		//0 body size, starts at version 1.
		GET_BAND_RESPONSE		= 65536,
		GET_MEAS_RESPONSE		= 65537,
		VALIDATE_MEAS_RESPONSE	= 65538,
		INIT_FIELDSTRENGTH_RESP	= 65539,
		GET_GPS_RESPONSE		= 65540,
		GET_HEADING_RESPONSE	= 65541,
		GET_MEAS_IQDATA_RESP	= 65542,
		GET_ANT_LIST_RESP		= 65543,
//		GET_CAL_RESP			= 70000,
//		DELETE_CAL_MEAS			= 70001,
//		RESULTS_CAL_MEAS		= 70002,
		CAL_SCHED				= 70003,
//		CAL_DEL					= 70004,
//		CAL_MEAS				= 70005,
//		CAL_RESULTS				= 70006,
//		CAL_STATE				= 70007,
//		GET_NEXT_ID				= 70008,
//		GET_NEXT_MSG			= 70009,
//		GET_NEXT_OCCUP_MSG		= 70010,
//		GET_NEXT_SCANDF_MSG		= 70011
		AVD_RESTART_DATA		= 70012,	// 8068 internal use
		OCCUP_RESTART_DATA		= 70013,	// 8068 internal use
		SCANDF_RESTART_DATA		= 70014		// 8068 internal use
	};

	// Message subtypes for message type PAN_DISP_CTRL
	enum EPanDispCtrl
	{
		GET_PAN =				1,
		GET_PAN_AGC =			2,
		GET_PAN_RESPONSE =		65537
	};

	// Message subtypes for message type PRIORITY_CTRL
	enum EPriorityCtrl
	{
		TAKE_PRIORITY		= 1,		//starts at version 1.
		RELEASE_PRIORITY	= 2,		//0 body size, starts at version 1.
		QUERY_PRIORITY		= 3,		//0 body size, starts at version 1.
		ROTATE				= 4,		//starts at version 1.
		ROTATOR_STATUS		= 5,		//starts at version 1.
		TAKE_PRIORITY_RESP	= 65537,
		RELEASE_PRIORITY_RESP = 65538,
		QUERY_PRIORITY_RESP	= 65539,
		ROTATE_RESP			= 65540,
		ROTATOR_STATUS_RESP	= 65541,
	};

	// Message subtypes for message type SUPER_USER_CTRL
	enum ESuperUserCtrl
	{
		SIGN_IN          = 1,  //starts at version 1.
		SIGN_OUT         = 2,  //0 body size, starts at version 1.
		WHO              = 3,  //0 body size, starts at version 1.
		SIGN_IN_RESP     = 65537,
		SIGN_OUT_RESP    = 65538,
		WHO_RESP         = 65539
	};

	// Message subtypes for message type STATUS_CTRL
#ifndef CSMSPLUS_SRV_BUILT
	enum EStatusCtrl : unsigned long
#else
	enum EStatusCtrl : uint32_t
#endif
	{
		PING_SERVER				= 1,
		PING_SERVER_RMS			= 3,
		GET_MSG_VERSIONS		= 4,

		//Starts at version 1. Reqeust has body size of 0.
		//Version 0 does not exist, therefore GET_FAULT_REQUEST should not be
		// sent to server when version 0 is detected.
		GET_FAULT_REQUEST		= 5,
        METRIC_CONN_INFO_GET    = 6,    //0 body size.
		METRIC_ANT_NAME_CHANGE_REQUEST = 7,	//0 body size, starts at version 1. Same note as GET_FAULT_REQUEST.
		GET_SRV_GUID = 8,				//0 body size, starts at version 1. Same note as GET_FAULT_REQUEST.
		GET_MSG_VERSIONS_RESP	= 65536,

		//Starts at version 1. This is a response to GET_FAULT_REQUEST,
		// no need to do down-conversion to version 0.
		GET_FAULT_RESP			= 65537,
		METRIC_CONN_INFO_RESP = 65538,
		METRIC_ANT_NAME_CHANGE_RESP = 65539,
		GET_CSMS_FAULT_RESP		= 65540,
		GET_SRV_GUID_RESP = 65541,
		GET_CSMSPLUS_FAULT_RESP = 65542
	};

	// Message subtypes for message type WBRECORDER_CTRL
#ifndef CSMSPLUS_SRV_BUILT
	enum EWbRecorderCtrl : unsigned long
#else
	enum EWbRecorderCtrl : uint32_t
#endif
	{
		WBRECORDER_START = 2,
		WBRECORDER_STOP = 3,
		WBRECORDER_CATALOG_REQ = 4,
		WBRECORDER_QUERY_REQ = 5,
		WBRECORDER_IQDATA_REQ = 6,
		WBRECORDER_IQDATA_STOP = 7,
		WBRECORDER_SPECTRUM_REQ = 8,
		WBRECORDER_SIGN_IN = 9,
		WBRECORDER_SIGN_OUT = 10,
		WBRECORDER_STATUS_RESP = 65536,
		WBRECORDER_CATALOG_RESP = 65537,
		WBRECORDER_QUERY_RESP = 65538,
		WBRECORDER_IQDATA_RESP = 65539,
		WBRECORDER_IQDATA_STOP_RESP = 65540,
		WBRECORDER_SPECTRUM_RESP = 65541,
		WBRECORDER_SIGN_IN_RESP = 65542,
		WBRECORDER_SIGN_OUT_RESP = 65543
	};

	// Message subtypes for message type STREAMING_CTRL
#ifndef CSMSPLUS_SRV_BUILT
	enum EStreamingCtrl : unsigned long
#else
	enum EStreamingCtrl : uint32_t
#endif
	{
		STREAMING_INIT = 1,
		STREAMING_RF_PARAMS = 2,
		STREAMING_RF_STOP = 3,
		STREAMING_START_WB = 4,
		STREAMING_STOP_WB = 5,
		STREAMING_START_DDC = 6,
		STREAMING_STOP_DDC = 7,
		STREAMING_INIT_RESP = 65537,
		STREAMING_RF_PARAMS_RESP = 65538,
		STREAMING_RF_STOP_RESP = 65539,
		STREAMING_START_WB_RESP = 65540,
		STREAMING_STOP_WB_RESP = 65541,
		STREAMING_START_DDC_RESP = 65542,
		STREAMING_STOP_DDC_RESP = 65543
	};

	// Common constants
#ifndef CSMSPLUS_SRV_BUILT
	static const unsigned long MAX_OCCBANDS = 100;
	static const unsigned long MAX_OCCBANDS_CSMS_V3p = 250;
	static const unsigned long MAX_OCCCHANNELS = 1000;
	static const unsigned long MAX_OCCRESULTCHANNELS = 6000;
	static const int BWBIN_OFFSET = 100;
	static const unsigned long BWBINS_MAX = 6401;
	static const unsigned long DFCUTS_MAX = 500;
	static const unsigned long DFVOLTS_MAX = 10;
	static const unsigned long NUMHISTBINS_MAX = 100;
	static const DATE MAX_DATE;
	static const unsigned char MAX_AUDIOCHANNELS = 7;
	static const unsigned char MAX_PAN_AUDIOCHANNELS = 8;
#else
	static const uint32_t MAX_OCCBANDS = 100;
	static const uint32_t MAX_OCCBANDS_CSMS_V3p = 250;
	static const uint32_t MAX_OCCCHANNELS = 1000;
	static const uint32_t MAX_OCCRESULTCHANNELS = 6000;
	static const int32_t BWBIN_OFFSET = 100;
	static const uint32_t BWBINS_MAX = 6401;
	static const uint32_t DFCUTS_MAX = 500;
	static const uint32_t DFVOLTS_MAX = 10;
	static const uint32_t NUMHISTBINS_MAX = 100;
	static const DATE MAX_DATE;
	static const uint8_t MAX_AUDIOCHANNELS = 7;
	static const uint8_t MAX_PAN_AUDIOCHANNELS = 8;
#endif
	// Common enums
	//@note: 1. The code assumes ANT1 < ANT2 < ANT3. These are < DF_ANT_1V & DF_ANT_1H.
	//		 2. The code assumes DF_ANT_1V to DF_ANT_10V has consecutive values.
	//		 3. The code assumes DF_ANT_1H to DF_ANT_9H has consecutive values.
#ifndef CSMSPLUS_SRV_BUILT
	enum EAnt : unsigned long
#else
	enum EAnt : uint32_t
#endif
	{
		INVALID_ANT	= 0,

		//@note: The value from ANT1 to SMPL_RF3 must match
		//		 with SMetricsMsg::ECustomAntenna values.
		ANT1		= 1, // Ref in
		ANT2		= 2, // Aux in
		ANT3		= 3, // Aux2 in
		ANT1H		= 4, // Ref Horizontal in.
		DF_ANT_1V	= 11,
		DF_ANT_2V	= 12,
		DF_ANT_3V	= 13,
		DF_ANT_4V	= 14,
		DF_ANT_5V	= 15,
		DF_ANT_6V	= 16,
		DF_ANT_7V	= 17,
		DF_ANT_8V	= 18,
		DF_ANT_9V	= 19,
		///@note: element 1 used as reference is not available for user.
		DF_ANT_10V	= 20,	// Element 10 of HF antenna.
		DF_ANT_1H	= 21,
		DF_ANT_2H	= 22,
		DF_ANT_3H	= 23,
		DF_ANT_4H	= 24,
		DF_ANT_5H	= 25,
		DF_ANT_6H	= 26,
		DF_ANT_7H	= 27,
		DF_ANT_8H	= 28,
		DF_ANT_9H	= 29,
		SMPL_RF2	= 31,
		SMPL_RF3	= 32,
		REF_TERM	= 33,	// Radio termination (on master, 2630)
		SMPL_TERM_1	= 34,	// Radio termination (on slave 1, 2630) - may need to add additional entries for multiple slaves

		//Used internally by the server to identify antenna connected to SHF extension Analyzer.
		SHF_EXT_ANT	= 41,

		///@note: SMS code assumes the antenna values after Horn_571_587_26_5GHz are all
		///		  connected to the RF input of Reference 2614 downconverter or Reference HF.
		///@note: Horn_571_587_26_5GHz & Horn_40GHz only available on EMetricHw::TCI8068_R5550_D4000 in SMS.
		Horn_571_587_26_5GHz = 42,
		Horn_40GHz = 43,

		///@note: The code assumes a consecutive number from ANT1_HF to ANT1_647H_MON.
		////		ANT1_HF to ANT_DYNAMIC_LAST_VAL are only available in csms.
		ANT1_HF			= 50,	//Non auto-switch RF HF input.
		//The following are antenna definitions that are connected to
		// RF1 (Reference) input through RF switch. The selection is controlled
		// by Scorpio through the antenna specification of the request.
		ANT1_LPA		= 51,	//Log periodic antenna (30 MHz - 6 GHz) sit on rotator.
		ANT1_647V_MON	= 52,	//647 Vertical monitor output (30 MHz - 6 GHz).
						// 6 GHz limited by Mini-circuits RF switch.
		ANT1_647H_MON	= 53,	//647 Horizontal monitor output (30 MHz - 3 GHz).

		///@note: ANT640D uses ANT1_VERTICAL_MONITOR &  ANT1_HORIZONTAL_MONITOR
		///		  as its output identifier.
		ANT1_VERTICAL_MONITOR = 79, //Vertical polarization output.
		//**********											**************
		//enum to match with SMetricsMsg::ECustomAntenna (Currently NOT USED, except
		///	ANT1_HORIZONTAL_MONITOR)
		ANT1_HORIZONTAL_MONITOR = 80, //Horizontal polarization monitoring output.
		ANT1_OMNI = 81,			//Omni antenna: 1 - 26 GHz.
		ANT1_LPA_20MHZ = 82,			//LPA antenna: 20 MHz - 4 GHz.
		ANT1_HORN_18GHZ = 83,	//Horn antenna: 1 GHz - 18 GHz.
		ANT1_HORN_26GHZ = 84,	//Horn antenna: 18 GHz - 26.5 GHz.
		ANT1_REF_HAS_HF = 85,  //Vertical Reference/HF.
		ANT1_LPA_7GHZ = 86,	//LPA antenna: 20 MHz - 7 GHz.
		ANT1_HORN_2_18GHZ = 87, //Horn antenna: 2 GHz - 18 GHz (Horizontal & Vertical).
		ANT2_LPA_800MHZ_TO_8GHZ = 100,	// Used to rename the 'Aux 1' antenna
		ANT2_LPA_80MHZ_TO_2GHZ = 101,	// Used to rename the 'Sample RF 2' antenna
		//**********											**************

		//**********											**************
		//enum reserved for ANT dynamically assigned by Server configuration
		// Client shows the antenna name that is passed to it as NULL terminated
		// char16_t string.
		ANT_DYNAMIC_FIRST_VAL	=1000,
		ANT_DYNAMIC_LAST_VAL	=1999,
		//**********											**************
	};

	enum EBand
	{
		ILLEGAL_BAND = 0,
		HF_BAND = 1,
		VHF_BAND = 2
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EFieldMethod : unsigned long
#else
	enum EFieldMethod : uint32_t
#endif
	{
		LINAVERAGE	= 1,
		LOGAVERAGE	= 2,
		RMS			= 3,
		PEAK		= 4
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EAveMethod : unsigned long
#else
	enum EAveMethod : uint32_t
#endif
	{
		MEAN_AVE = 1,
		RMS_AVE = 2,
		MAX_HOLD = 3
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EOutputType : unsigned long
#else
	enum EOutputType : uint32_t
#endif
	{
		NONE = 0,
		SUMMARY = 1,
		DETAILS = 2
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EAvdRate : unsigned long
#else
	enum EAvdRate : uint32_t
#endif
	{
		AUTO = 0,
		ALL = 1
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EDurationMethod : unsigned long
#else
	enum EDurationMethod : uint32_t
#endif
	{
		TIME		= 0,
		CONFIDENCE	= 1
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EThresholdMethod : unsigned long
#else
	enum EThresholdMethod : uint32_t
#endif
	{
		FIXED	= 0,
		NOISE	= 1,
		BOTH	= 2
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EMetricHw : unsigned char
#else
	enum EMetricHw : uint8_t
#endif
	{
		NO_METRICS = false,
		TCI8067 = 1,
		TCI8067A = 2,
		TCI8067B = 3,
		TCI9054 = 4,
		TCI4092 = 5,
		TCI6083 = 6,
		TCI8068 = 7,
		TCI5141 = 8,	//aka: Macedonia Beehive. Downconvert to TCI8068.
		TCI5141_1 = 9, 	//5141: No C-Band, Aux1, Aux2 allowed (China).
		TCI5141_2 = 10,	//5141: 2 independent inputs with C-Band (Italy).
		TCI5141_4 = 11, //5141: 4 inputs (aux1, aux2 with 2 inputs) with C-Band (Italy).

		//5141: 4 inputs (Malaysia Beehive).
		//     (640H, 3 inputs: Vertical 30 -200 MHz Aux1, Vertical 200MHz - 6GHz Ref, Horizontal 20 MHz - 3 GHZ Aux2).
		//	    HF: 9Khz - 30 MHz Vertical.
		TCI5141_640H_HF = 12,

		//5141: 3 inputs (Indonesia Beehive).
		//      3 GHz 640, 2 inputs: Vertical 30 -200 MHz Aux1, Vertical 200MHz - 3GHz Ref.
		//	    HF: 9Khz - 30 MHz Vertical.
		TCI5141_3GHZ_640_HF = 13,

		//5141: 3 inputs (Swaziland Beehive).
		//     (3 GHz 640H, 3 inputs: Vertical 20 -200 MHz Aux1, Vertical 200MHz - 3GHz Ref, Horizontal 20 MHz - 3 GHZ  Aux2).
		TCI5141_3GHZ_640H = 14,

		//8068: 2 element HF(7235: Watson-Watt).
		TCI8068_7235_HF = 15,

		//8068: 11 HF(638-2: 11-Element Array) inputs 
		TCI8068_638_2_HF = 16,

		//8068: has 2 ThinkRf R5550 & D4000 (Horn_571_587_26_5GHz & Horn_40GHz) inputs.
		//		Aux1 and Aux2 selections must not be available.
		//		May also be a DF system.
		TCI8068_R5550_D4000 = 17,

		//CSMS servers
		TCI5143 = 64,

		//Algeria CSMS with Multiple antenna connected to RF1 and
		// selected through Mini-Circuits switch, Horn antenna connected
		// to RF2 through Keysight Spectrum Analyzer, there are:
		// RF1: LPA antenna (20 MHz - 6 GHz).
		//	647 Vertical Monitor output.
		//	647 Horizontal Monitor output.
		// RF2: 6 GHz - 18 GHz Horn antenna.
		//	18 GHz - 40 GHz Horn antenna.
		TCI5143_MULTIPLE_ANT = 65,

		//CSMS with Dual Aux1 antenna capability (Aux1 in Master, SMPL RF2 in Slave)
		TCI5143_DUAL_AUX1 = 66,

		//CSMS servers that has client Gain mode controlled capability
		// through SGetOccupancyCmd::SOccupancyFlag.
		/// @note: 1. Only VUSHF is currently supported.
		///		   2. It is recommended that Client does
		///				>= TCI5143_GAIN_MODE && <= TCI5143_LAST to check
		///				that CSMS servers has Gain mode setting capability.
		TCI5143_GAIN_MODE = 67,

		///@note: TCI5143_LAST is used as marker to denote the last
		/// CSMS server type variant, the number/value can be reused or
		/// changed if needed.
		TCI5143_LAST = 73,
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EDfHw : unsigned char
#else
	enum EDfHw : uint8_t
#endif
	{
		NO_DF = false,
		SINGLE_POL = true,
		DUAL_POL
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EAntPol : unsigned long
#else
	enum EAntPol : uint32_t
#endif
	{
		NA = 0,		//Polarization not defined.
		VERTICAL = 1, //Vertical Polarization
		HORIZONTAL = 2,	//Horizontal polarization.
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum ETdoaHw : unsigned char
#else
	enum ETdoaHw : uint8_t
#endif
	{
		NO_IQDATA = 0,
		IQDATA_PRESENT,	// IQ data without accurate timestamp / location
		TDOA_PRESENT	// IQ data with accurate timestamp / location
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EErrStatus : unsigned long
#else
	enum EErrStatus : uint32_t
#endif
	{
		//@note: The code in CHwControl assumes the following:
		//       PASS_STAT < WARNING_STAT < FAIL_STAT.
		PASS_STAT = 0,
		WARNING_STAT = 1,
		FAIL_STAT = 2,
		BIST_STAT = 3,	//BIST is in progress.
		ON_STAT = 4,
		OFF_STAT = 5,
		NO_VAL_STAT = 6
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum ESiteInfo : unsigned long
#else
	enum ESiteInfo : uint32_t
#endif
	{
		SITE_UNKNOWN = 0,
		FIXED_SITE = 1,
		MOBILE_SITE = 2
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EIonoInfoSource : __int32
#else
	enum EIonoInfoSource : int32_t
#endif
	{
		NOT_APPLICABLE = 0,
		EXTERNAL_SERVER = 1,
		SOUNDER_SYSTEM = 2,
		FLAT_IONOGRAM = 3
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EDFMethod: __int32
#else
	enum EDFMethod: int32_t
#endif
	{
	    DF_ANTENNA =1,
		ANTENNA_ROTATOR =2
	};

#ifndef CSMSPLUS_SRV_BUILT
	enum EOccMode: __int32
#else
	enum EOccMode: int32_t
#endif
	{
		NON_DM =1,
        DM = 2
	};

	// Helper functions
#if defined(_DEBUG) || !defined(NDEBUG)
	static bool IsOnStack(const void* p) // For ASSERT checking of variable-length messages
	{
		void* stackLowCurrent;
		void* stackTop;
#ifdef _MSC_VER
		PNT_TIB ptib = reinterpret_cast<PNT_TIB>(NtCurrentTeb());
		stackTop = ptib->StackBase;
		stackLowCurrent = ptib->StackLimit;
#else
		pthread_attr_t attr;
		pthread_getattr_np(pthread_self(), &attr);
		size_t stacksize;
		pthread_attr_getstack(&attr, &stackLowCurrent, &stacksize);
		stackTop = (void*)((char*)stackLowCurrent + stacksize);
#endif
		return p <= stackTop && p >= stackLowCurrent;
	}
#endif

#ifndef CSMSPLUS_SRV_BUILT
	template<typename T> static size_t SizeOf(const T& msgBody, unsigned long numBands = 0)
#else
	template<typename T> static size_t SizeOf(const T& msgBody, uint32_t numBands = 0)
#endif
	{
#ifdef _MSC_VER
#pragma warning(suppress : 6385) // Variable length structure
#endif
		return offsetof(T, band[numBands == 0 ? msgBody.numBands : numBands]);
	}

	// new and delete operators for variable-length messages longer than the union body
	static void* operator new(size_t size, size_t sizeOverride = 0)
	{
		void* mem = malloc(sizeOverride <= size ? size : sizeOverride);

		if(mem == nullptr)
		{
			ASSERT(FALSE);
			AfxThrowMemoryException();
		}

		return mem;
	}

	static void operator delete(void* ptr) { free(ptr); }
	static void operator delete(void* ptr, size_t) { free(ptr); }

	static void Realloc(SSmsMsg*& msg, size_t size)
	{
		void* newMsg = realloc(msg, size);
	
		if(newMsg == nullptr)
		{
			ASSERT(FALSE);
			AfxThrowMemoryException();
		}

		msg = static_cast<SSmsMsg*>(newMsg);
	}

#ifdef _DEBUG
#ifndef CSMSPLUS_SRV_BUILT
	static void* operator new(size_t size, LPCSTR fileName, int line, size_t sizeOverride = 0)
#else
	static void* operator new(size_t size, LPCSTR fileName, int32_t line, size_t sizeOverride = 0)
#endif
	{
		void* mem = _malloc_dbg(sizeOverride <= size ? size : sizeOverride, _NORMAL_BLOCK, fileName, line);

		if(mem == nullptr)
		{
			ASSERT(FALSE);
			AfxThrowMemoryException();
		}

		return mem;
	}

#ifndef CSMSPLUS_SRV_BUILT
	static void operator delete(void* ptr, LPCSTR, int, size_t) { _free_dbg(ptr, _NORMAL_BLOCK); }
#else
	static void operator delete(void* ptr, LPCSTR, int32_t, size_t) { _free_dbg(ptr, _NORMAL_BLOCK); }
#endif

#ifndef CSMSPLUS_SRV_BUILT
	static void Realloc(SSmsMsg*& msg, size_t size, LPCSTR fileName, int line)
#else
	static void Realloc(SSmsMsg*& msg, size_t size, LPCSTR fileName, int32_t line)
#endif
	{
		void* newMsg = _realloc_dbg(msg, size, _NORMAL_BLOCK, fileName, line);
	
		if(newMsg == nullptr)
		{
			ASSERT(FALSE);
			throw std::bad_alloc();
		}

		msg = static_cast<SSmsMsg*>(newMsg);
	}
#endif
	// Common structures
	struct SAudioSwitchStatusResp // SWITCH_STATUS_RESPONSE
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long psq;
		unsigned long ringDetect;
#else
		uint32_t psq;
		uint32_t ringDetect;
#endif
	};

	struct SIonogramDataMsg
	{
		static const size_t MAX_IONOGRAM_POINTS = 50;

		struct STimestamp
		{
#ifndef CSMSPLUS_SRV_BUILT
			short int sec;
			short int min;
			short int hour;
			short int yday;  // year day (0-365) Jan 1=0
#else
			int16_t sec;
			int16_t min;
			int16_t hour;
			int16_t yday;  // year day (0-365) Jan 1=0
#endif
		} timestamp;

		struct SFinishedIonogram
		{
#ifndef CSMSPLUS_SRV_BUILT
			short int numPoints;
#else
			int16_t numPoints;
#endif
			struct SIonogramPoint
			{
				float freq;
				float trueHeight;
				float virtualHeight;
			} ionData[MAX_IONOGRAM_POINTS];
		} finishedIonogram;
	};

	struct SGetBwCmd
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long dwellTime;
		unsigned long betaParam;
		unsigned long yParam;
		unsigned long x1Param;
		unsigned long x2Param;
		unsigned long repeatCount;
#else
		uint32_t dwellTime;
		uint32_t betaParam;
		uint32_t yParam;
		uint32_t x1Param;
		uint32_t x2Param;
		uint32_t repeatCount;
#endif
		EAveMethod aveMethod;
		EOutputType outputType;
	};

	struct SGetFreqCmd
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum EFreqMethod : unsigned long
#else
		enum EFreqMethod : uint32_t
#endif
		{
			IFM = 1,
			FFT = 2
		};

		EFreqMethod freqMethod;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long dwellTime;
		unsigned long repeatCount;
#else
		uint32_t dwellTime;
		uint32_t repeatCount;
#endif
		EAveMethod aveMethod;
		EOutputType outputType;
	};

	struct SGetModulationCmd
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long dwellTime;
		unsigned long repeatCount;
#else
		uint32_t dwellTime;
		uint32_t repeatCount;
#endif
		EAveMethod aveMethod;
		EOutputType outputType;
	};

	struct SGetFieldStrengthCmd
	{
		EFieldMethod fieldMethod;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long dwellTime;
		unsigned long repeatCount;
#else
		uint32_t dwellTime;
		uint32_t repeatCount;
#endif
		EAveMethod aveMethod;
		EOutputType outputType;
	};

	struct SRcvrCtrlCmdV1; // Forward declaration

	struct SRcvrCtrlCmdV0 // SET_RCVR V0
	{
		inline operator SRcvrCtrlCmdV1(void) const; // Convert to V1; defined below
#ifndef CSMSPLUS_SRV_BUILT
		enum EDetMode : unsigned long
#else
		enum EDetMode : uint32_t
#endif
		{
			OFF = 0,
			AM = 1,
			CW = 2,
			FM = 3,
			USB = 4,
			LSB = 5,
			EXTERNAL = 6
		};
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	freq;		// frequency in Hertz
		unsigned long	bandwidth;	// demod bandwidth in Hertz
		EDetMode		detMode;	// AM, CW, FM, USB, or LSB
		unsigned long	agcTime;	// AGC decay time in ms
		long			bfo;		// BFO in Hertz (+/- 8000)
#else
		uint32_t	freq;		// frequency in Hertz
		uint32_t	bandwidth;	// demod bandwidth in Hertz
		EDetMode		detMode;	// AM, CW, FM, USB, or LSB
		uint32_t	agcTime;	// AGC decay time in ms
		int32_t			bfo;		// BFO in Hertz (+/- 8000)
#endif
	};

	struct SRcvrCtrlCmdV1 // SET_RCVR V1
	{
		operator SRcvrCtrlCmdV0(void) const // Convert to V0
		{
			SRcvrCtrlCmdV0 v0;
#ifndef CSMSPLUS_SRV_BUILT
			v0.freq = Units::Frequency(freq).Hz<unsigned long>(true);
			v0.bandwidth = Units::Frequency(bandwidth).Hz<unsigned long>(true);
			v0.bfo = Units::Frequency(bfo).Hz<unsigned long>(true);
#else
			v0.freq = Units::Frequency(freq).Hz<uint32_t>(true);
			v0.bandwidth = Units::Frequency(bandwidth).Hz<uint32_t>(true);
			v0.bfo = Units::Frequency(bfo).Hz<uint32_t>(true);
#endif
			v0.detMode = static_cast<SRcvrCtrlCmdV0::EDetMode>(detMode);
			v0.agcTime = agcTime;
			return v0;
		}

#ifndef CSMSPLUS_SRV_BUILT
		enum EDetMode : unsigned long
#else
		enum EDetMode : uint32_t
#endif
		{
            ///@note: 1. If EDetMode is changed SPanParaCmd must be changed accordingly.
			///	      2. The code assumes the order and value from OFF to
			///			 WIDE_IQ are preserved.
			OFF = 0,
			AM = 1,
			CW = 2,
			FM = 3,
			USB = 4,
			LSB = 5,
			EXTERNAL = 6,
			WIDE_IQ = 7,
		};

		Units::Frequency::Raw	freq;		// frequency in Hertz
		Units::Frequency::Raw	bandwidth;	// demod bandwidth in Hertz
		EDetMode				detMode;	// AM, CW, FM, USB, or LSB
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			agcTime;	// AGC decay time in ms
#else
		uint32_t			agcTime;	// AGC decay time in ms
#endif
		Units::Frequency::Raw	bfo;		// BFO in Hertz (+/- 8000)
	};

	typedef SRcvrCtrlCmdV1 SRcvrCtrlCmd;

	struct SPanParaCmdV1 // SET_PAN_PARA. There is no version 0.
	{
		EAnt antenna;                       // Antenna choice.
		SRcvrCtrlCmdV1 rcvr;                // Has the SET_RCVR value.
	};

	typedef SPanParaCmdV1 SPanParaCmd;

	struct SAudioParamsCmdV2; // Forward declaration


	struct SAudioParamsCmdV1 // SET_AUDIO_PARAMS. There is no version 0.
	{

		bool						anyChannel;		// true -> server will allocate a channel, false -> use specified channel
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long				channel;		// audio (demod) channel
#else
		uint32_t				channel;		// audio (demod) channel
#endif
		Units::Frequency::Raw		freq;			// frequency in Hertz
		Units::Frequency::Raw		bandwidth;		// demod bandwidth in Hertz
		SRcvrCtrlCmdV1::EDetMode	detMode;		// OFF, AM, CW, FM, USB, LSB or EXTERNAL
		Units::Frequency::Raw		bfo;			// BFO in Hertz (+/- 8000)
	};

	struct SAudioParamsCmdV2 // SET_AUDIO_PARAMS. There is no version 0.
	{
		operator SAudioParamsCmdV1(void) const // Convert to V1
		{
			SAudioParamsCmdV1 v1;
			v1.anyChannel = anyChannel;
			v1.channel = channel;
			v1.freq = freq;
			v1.bandwidth = bandwidth;
			v1.detMode = detMode;
			v1.bfo = bfo;
			return v1;
		}

		SAudioParamsCmdV2& operator =(const SAudioParamsCmdV1 & v1) // Convert from V1
		{
			const SAudioParamsCmdV1* src = &v1;
			anyChannel = src->anyChannel;
			channel = src->channel;
			freq = src->freq;
			bandwidth = src->bandwidth;
			detMode = src->detMode;
			bfo = src->bfo;
			streamID = 0;
			return *this;
		}

		bool						anyChannel;		// true -> server will allocate a channel, false -> use specified channel
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long				channel;		// audio (demod) channel
		unsigned long 				streamID;
#else
		uint32_t				channel;		// audio (demod) channel
		uint32_t 				streamID;
#endif
		Units::Frequency::Raw		freq;			// frequency in Hertz
		Units::Frequency::Raw		bandwidth;		// demod bandwidth in Hertz
		SRcvrCtrlCmdV1::EDetMode	detMode;		// OFF, AM, CW, FM, USB, LSB or EXTERNAL
		Units::Frequency::Raw		bfo;			// BFO in Hertz (+/- 8000)
	};

	static const size_t IP_ADDRESS_LEN = 16;

	struct SAudioParamsCmdV3 // SET_AUDIO_PARAMS. There is no version 0.
	{
		operator SAudioParamsCmdV2(void) const // Convert to V2
		{
			SAudioParamsCmdV2 v2;
			v2.anyChannel = anyChannel;
			v2.channel = channel;
			v2.freq = freq;
			v2.bandwidth = bandwidth;
			v2.detMode = detMode;
			v2.bfo = bfo;
			v2.streamID = streamID;
			return v2;
		}

		SAudioParamsCmdV3& operator =(const SAudioParamsCmdV2 & v2) // Convert from V2
		{
			const SAudioParamsCmdV2* src = &v2;
			anyChannel = src->anyChannel;
			channel = src->channel;
			freq = src->freq;
			bandwidth = src->bandwidth;
			detMode = src->detMode;
			bfo = src->bfo;
			streamID = src->streamID;
			memset(&ipAddressRDSRadio, 0, sizeof(ipAddressRDSRadio));
			doRDS= false;
			return *this;
		}

		bool						anyChannel;		// true -> server will allocate a channel, false -> use specified channel
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long				channel;		// audio (demod) channel
		unsigned long 				streamID;
#else
		uint32_t				channel;		// audio (demod) channel
		uint32_t 				streamID;
#endif
		Units::Frequency::Raw		freq;			// frequency in Hertz
		Units::Frequency::Raw		bandwidth;		// demod bandwidth in Hertz
		SRcvrCtrlCmdV1::EDetMode	detMode;		// OFF, AM, CW, FM, USB, LSB or EXTERNAL
		Units::Frequency::Raw		bfo;			// BFO in Hertz (+/- 8000)
		bool                        doRDS;
		char                        ipAddressRDSRadio[IP_ADDRESS_LEN];
	};

	struct SAudioParamsCmdV4 // SET_AUDIO_PARAMS. There is no version 0.
	{
		operator SAudioParamsCmdV3(void) const // Convert to V3
		{
			SAudioParamsCmdV3 v3;
			v3.anyChannel = anyChannel;
			v3.channel = channel;
			v3.freq = freq;
			v3.bandwidth = bandwidth;
			v3.detMode = detMode;
			v3.bfo = bfo;
			v3.streamID = streamID;
			return v3;
		}

		SAudioParamsCmdV4& operator =(const SAudioParamsCmdV3 & v3) // Convert from V3
		{
			const SAudioParamsCmdV3* src = &v3;
			anyChannel = src->anyChannel;
			channel = src->channel;
			freq = src->freq;
			bandwidth = src->bandwidth;
			detMode = src->detMode;
			bfo = src->bfo;
			streamID = src->streamID;
			memcpy(&ipAddressRDSRadio, src->ipAddressRDSRadio, sizeof(ipAddressRDSRadio));
			doRDS= src->doRDS;
			doModRec = false;
			return *this;
		}

		bool						anyChannel;		// true -> server will allocate a channel, false -> use specified channel
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long				channel;		// audio (demod) channel
		unsigned long 				streamID;
#else
		uint32_t 					channel;		// audio (demod) channel
		uint32_t  					streamID;
#endif
		Units::Frequency::Raw		freq;			// frequency in Hertz
		Units::Frequency::Raw		bandwidth;		// demod bandwidth in Hertz
		SRcvrCtrlCmdV1::EDetMode	detMode;		// OFF, AM, CW, FM, USB, LSB or EXTERNAL
		Units::Frequency::Raw		bfo;			// BFO in Hertz (+/- 8000)
		bool                        doRDS;
		bool 						doModRec;
		char                        ipAddressRDSRadio[IP_ADDRESS_LEN];
	};

	struct SAudioParamsCmdV5 // SET_AUDIO_PARAMS.
	{
		operator SAudioParamsCmdV4(void) const // Convert to V4
		{
			SAudioParamsCmdV4 v4;
			v4.anyChannel = anyChannel;
			v4.channel = channel;
			v4.freq = freq;
			v4.bandwidth = bandwidth;
			v4.detMode = detMode;
			v4.bfo = bfo;
			v4.streamID = streamID;
			return v4;
		}

		SAudioParamsCmdV5& operator =(const SAudioParamsCmdV4 & v4) // Convert from V4
		{
			const SAudioParamsCmdV4* src = &v4;
			anyChannel = src->anyChannel;
			channel = src->channel;
			freq = src->freq;
			bandwidth = src->bandwidth;
			detMode = src->detMode;
			bfo = src->bfo;
			streamID = src->streamID;
			memcpy(&ipAddressRDSRadio, src->ipAddressRDSRadio, sizeof(ipAddressRDSRadio));
			doRDS= src->doRDS;

			doModRec = src->doModRec;
			doAudio = true; // audio is always on in order version for audio cmd
			doSigRec = false;
			return *this;
		}

		bool						anyChannel;		// true -> server will allocate a channel, false -> use specified channel
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long				channel;		// audio (demod) channel
		unsigned long 				streamID;
#else
		uint32_t 					channel;		// audio (demod) channel
		uint32_t  					streamID;
#endif
		Units::Frequency::Raw		freq;			// frequency in Hertz
		Units::Frequency::Raw		bandwidth;		// demod bandwidth in Hertz
		SRcvrCtrlCmdV1::EDetMode	detMode;		// OFF, AM, CW, FM, USB, LSB or EXTERNAL
		Units::Frequency::Raw		bfo;			// BFO in Hertz (+/- 8000)
		bool                        doRDS;
		bool 						doModRec;
		bool 						doAudio;
		bool						doSigRec;
		char                        ipAddressRDSRadio[IP_ADDRESS_LEN];
	};

#if !defined(SMS_SRV_BUILT) && !defined(CSMS_SRV_BUILT)
	typedef SAudioParamsCmdV5 SAudioParamsCmd; // // for client and CSMSPlus
#else
	typedef SAudioParamsCmdV4 SAudioParamsCmd;
#endif

	struct SAudioParamsRespV1 // SET_AUDIO_PARAMS_RESP. There is no version 0.
	{
		ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long channel;			// Channel allocated for this request
#else
		uint32_t channel;			// Channel allocated for this request
#endif
	};



	struct SAudioParamsRespV2 // SET_AUDIO_PARAMS_RESP. V2 adds ip address of master/slave
	{
		operator SAudioParamsRespV1(void) const // Convert to V1
		{
			SAudioParamsRespV1 v1;
			v1.status = status;
			v1.channel = channel;
			return v1;
		}

		SAudioParamsRespV2& operator =(const SAudioParamsRespV1 & v1) // Convert from V1
		{
			const SAudioParamsRespV1* src = &v1;
			status = src->status;
			channel = src->channel;
			ipAddress[0]='\0';
			return *this;
		}

		ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long channel;			// Channel allocated for this request
#else
		uint32_t channel;			// Channel allocated for this request
#endif
		char ipAddress[IP_ADDRESS_LEN];
	};

	typedef SAudioParamsRespV2 SAudioParamsResp;

	struct SFreeAudioChannelCmdV1 // FREE_AUDIO_CHANNEL. There is no version 0.
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long				channel;		// audio (demod) channel
#else
		uint32_t				channel;		// audio (demod) channel
#endif
	};

	typedef SFreeAudioChannelCmdV1 SFreeAudioChannelCmd;

	struct SGetAutoViolateCmdV1; // Forward declaration

	struct SGetAutoViolateCmdV0 // GET_AUTOVIOLATE
	{
		inline operator SGetAutoViolateCmdV1(void) const; // Convert to V1; definition below

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numBands;		// number of frequency bands
		unsigned long lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		unsigned long highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		unsigned long channelBandwidth[MAX_OCCBANDS]; // (Hz)
		unsigned long includeExclude[MAX_OCCBANDS];
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned long avdThreshold;		// dB above NOISE
		EAvdRate measurementRate;
		unsigned long unused[11];		// needed to make this Cmd > getOccupancyCmd
#else
		uint32_t numBands;		// number of frequency bands
		uint32_t lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		uint32_t highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		uint32_t channelBandwidth[MAX_OCCBANDS]; // (Hz)
		uint32_t includeExclude[MAX_OCCBANDS];
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint32_t avdThreshold;		// dB above NOISE
		EAvdRate measurementRate;
		uint32_t unused[11];		// needed to make this Cmd > getOccupancyCmd
#endif
	};

	struct SGetAutoViolateCmdV2; // Forward declaration

	struct SGetAutoViolateCmdV1 // GET_AUTOVIOLATE V1
	{
		operator SGetAutoViolateCmdV0(void) const // Convert to V0
		{
			SGetAutoViolateCmdV0 v0;
			v0.numBands = numBands;
			memcpy(v0.lowFrequency, lowFrequency, sizeof(v0.lowFrequency));
			memcpy(v0.highFrequency, highFrequency, sizeof(v0.highFrequency));
			memcpy(v0.channelBandwidth, channelBandwidth, sizeof(v0.channelBandwidth));
			memcpy(v0.includeExclude, includeExclude, sizeof(v0.includeExclude));
			v0.storageTime = storageTime;
			v0.measurementTime = measurementTime;
			v0.avdThreshold = avdThreshold;
			v0.measurementRate = measurementRate;

			return v0;
		}

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numBands;		// number of frequency bands
		unsigned long lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		unsigned long highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		unsigned long channelBandwidth[MAX_OCCBANDS]; // (Hz)
		unsigned long includeExclude[MAX_OCCBANDS];
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned long avdThreshold;		// dB above NOISE
		EAvdRate measurementRate;
		EAnt ant;						// Antenna to use
		unsigned long unused[11];		// needed to make this Cmd > getOccupancyCmd
#else
		uint32_t numBands;		// number of frequency bands
		uint32_t lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		uint32_t highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		uint32_t channelBandwidth[MAX_OCCBANDS]; // (Hz)
		uint32_t includeExclude[MAX_OCCBANDS];
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint32_t avdThreshold;		// dB above NOISE
		EAvdRate measurementRate;
		EAnt ant;						// Antenna to use
		uint32_t unused[11];		// needed to make this Cmd > getOccupancyCmd
#endif
	private:
		// method that is disallowed due to variable size structure.
		inline operator SGetAutoViolateCmdV2(void) const; // UpConvert to V2; Cannot be implemented due to variable size.
	};

	struct SGetAutoViolateCmdV3; // Forward declaration

	struct SGetAutoViolateCmdV2 // GET_AUTOVIOLATE V2 (variable length)
	{
		operator SGetAutoViolateCmdV1(void) const // Convert to V1
		{
			SGetAutoViolateCmdV1 v1;
			v1.numBands = (numBands <= MAX_OCCBANDS ? numBands : MAX_OCCBANDS);
			v1.storageTime = storageTime;
			v1.measurementTime = measurementTime;
			v1.avdThreshold = avdThreshold;
			v1.measurementRate = measurementRate;
			v1.ant = ant;

			for(size_t i = 0; i < v1.numBands; ++i)
			{
#ifndef CSMSPLUS_SRV_BUILT
				v1.highFrequency[i] = Units::Frequency(band[i].highFrequency).Hz<unsigned long>(true);
				v1.lowFrequency[i] = Units::Frequency(band[i].lowFrequency).Hz<unsigned long>(true);
				v1.channelBandwidth[i] = Units::Frequency(band[i].channelBandwidth).Hz<unsigned long>(true);
#else
				v1.highFrequency[i] = Units::Frequency(band[i].highFrequency).Hz<uint32_t>(true);
				v1.lowFrequency[i] = Units::Frequency(band[i].lowFrequency).Hz<uint32_t>(true);
				v1.channelBandwidth[i] = Units::Frequency(band[i].channelBandwidth).Hz<uint32_t>(true);
#endif
				v1.includeExclude[i] = (band[i].exclude ? 1 : 0);
			}

			return v1;
		}

		SGetAutoViolateCmdV2& operator =(const SGetAutoViolateCmdV1 & v1) // Convert from V1
		{
			const SGetAutoViolateCmdV1* src = &v1;

			if(static_cast<void*>(this) == static_cast<const void*>(&v1))
			{
				// Make a copy. SGetAutoViolateCmdV1 is a fixed size structure.
				if((src = static_cast<const SGetAutoViolateCmdV1*>(malloc(sizeof(v1)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetAutoViolateCmdV1*>(src), &v1, sizeof(v1));
			}

			storageTime = src->storageTime;
			measurementTime = src->measurementTime;
			measurementRate = src->measurementRate;
			ant = src->ant;
#ifndef CSMSPLUS_SRV_BUILT
			avdThreshold = static_cast<unsigned char>(src->avdThreshold);
			numBands = static_cast<unsigned short>(src->numBands <= MAX_OCCBANDS ? src->numBands : 0);
#else
			avdThreshold = static_cast<uint8_t>(src->avdThreshold);
			numBands = static_cast<uint16_t>(src->numBands <= MAX_OCCBANDS ? src->numBands : 0);
#endif
			for(size_t i = 0; i < numBands; ++i)
			{
				band[i].highFrequency = Units::Frequency(src->highFrequency[i]).GetRaw();
				band[i].lowFrequency = Units::Frequency(src->lowFrequency[i]).GetRaw();
				band[i].channelBandwidth = Units::Frequency(src->channelBandwidth[i]).GetRaw();
				band[i].exclude = (src->includeExclude[i] != 0);
			}

			if(src != &v1)
			{
				free(const_cast<SGetAutoViolateCmdV1*>(src));
			}
			return *this;
		}

		SGetAutoViolateCmdV2& operator =(const SGetAutoViolateCmdV3 & v3); // Down-Convert from V3; definition below

		struct SBand
		{
			Units::Frequency::Raw lowFrequency;	// low frequency (Hz)
			Units::Frequency::Raw highFrequency;	// high frequency (Hz), = low frequency for single channel
			Units::Frequency::Raw channelBandwidth; // (Hz)
			bool exclude;
		};

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		EAvdRate measurementRate;
		EAnt ant;
		unsigned char avdThreshold;		// dB above NOISE
		unsigned short numBands;		// number of frequency bands
#else
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		EAvdRate measurementRate;
		EAnt ant;
		uint8_t avdThreshold;		// dB above NOISE
		uint16_t numBands;		// number of frequency bands
#endif
		SBand band[1];	// Variable length. Must be at the end of struct for
						// conversion code to work correctly.
	private:
		// method that is disallowed due to variable size structure.
		operator SGetAutoViolateCmdV3(void) const; // UpConvert to V3; Cannot be implemented due to variable size.
	};

	//@note: SBandV4 is an internal struct of SGetOccupancyCmd & SGetAutoViolateCmdV3.
	struct SBandV4		//@note: Start at v4 as this is used starting with SGetOccupancyCmdV4.
	{
		struct SSignalType
		{
			//@note: Unused fileds (unused0, unused1) must have value of 0
			//       so that new bit fields can be added occupying the
			//       unused bits without doing message versioning.
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long unused0 : 1;
			unsigned long narrow : 1;	//Used in SGetOccupancyCmd not SGetAutoViolateCmd.
			unsigned long horizPol : 1;
			unsigned long unused1 : 29;
#else
			uint32_t unused0 : 1;
			uint32_t narrow : 1;	//Used in SGetOccupancyCmd not SGetAutoViolateCmd.
			uint32_t horizPol : 1;
			uint32_t unused1 : 29;
#endif
		};

		Units::Frequency::Raw lowFrequency;	// low frequency (Hz)
		Units::Frequency::Raw highFrequency;	// high frequency (Hz), = low frequency for single channel
		Units::Frequency::Raw channelBandwidth; // (Hz)
		union USignalType
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	zeroSType;
#else
			uint32_t	zeroSType;
#endif
			SSignalType		signalType;
		} sType;
		bool exclude;
	};

	//@note: SBandV5 is an internal struct used in SGetDmCtrlCmdV2
	struct SBandV5		//@note:
	{
		struct SSignalType
		{
			//@note: Unused fileds (unused0, unused1) must have value of 0
			//       so that new bit fields can be added occupying the
			//       unused bits without doing message versioning.
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long unused0 : 1;
			unsigned long narrow : 1;	//Used in SGetOccupancyCmd not SGetAutoViolateCmd.
			unsigned long horizPol : 1;
			unsigned long unused1 : 29;
#else
			uint32_t unused0 : 1;
			uint32_t narrow : 1;	//Used in SGetOccupancyCmd not SGetAutoViolateCmd.
			uint32_t horizPol : 1;
			uint32_t unused1 : 29;
#endif
		};

		struct StargetAzList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned short nAZs; /* Not to exceed 4 */
			short Az[4];
#else
			uint16_t nAZs; /* Not to exceed 4 */
			int16_t Az[4];
#endif
		};
		struct StargetElList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned short nEls; /* Not to exceed 2 */
			short El[2];
#else
			uint16_t nEls; /* Not to exceed 2 */
			int16_t El[2];
#endif
		};

		Units::Frequency::Raw lowFrequency;	// low frequency (Hz)
		Units::Frequency::Raw highFrequency;	// high frequency (Hz), = low frequency for single channel
		Units::Frequency::Raw channelBandwidth; // (Hz)
		union USignalType
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	zeroSType;
#else
			uint32_t		zeroSType;
#endif
			SSignalType		signalType;
		} sType;
		StargetAzList  targetAzList;
		StargetElList  targetElList;
		BOOL8 exclude;
	};

	typedef SBandV5 SBand;

	struct SGetAutoViolateCmdV3 // GET_AUTOVIOLATE V3 (variable length)
	{
		SGetAutoViolateCmdV3& operator =(const SGetAutoViolateCmdV2 & v2) // Convert from V2
		{
			const SGetAutoViolateCmdV2* src = &v2;

			if(static_cast<void*>(this) == static_cast<const void*>(&v2))
			{
				// Make a copy
				if((src = static_cast<const SGetAutoViolateCmdV2*>(malloc(SizeOf(v2)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetAutoViolateCmdV2*>(src), &v2, SizeOf(v2));
			}

			storageTime = src->storageTime;
			measurementTime = src->measurementTime;
			measurementRate = src->measurementRate;
			ant = src->ant;
			avdThreshold = src->avdThreshold;
			numBands = src->numBands;

			for(size_t i = 0; i < numBands; ++i)
			{
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				//Zero all the SSignalType bit fields.
				band[i].sType.zeroSType = 0;
			}

			if(src != &v2)
			{
				free(const_cast<SGetAutoViolateCmdV2*>(src));
			}
			
			return *this;
		}

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		EAvdRate measurementRate;
		EAnt ant;
		unsigned char avdThreshold;		// dB above NOISE
		unsigned short numBands;		// number of frequency bands
#else
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		EAvdRate measurementRate;
		EAnt ant;
		uint8_t avdThreshold;		// dB above NOISE
		uint16_t numBands;		// number of frequency bands
#endif
		SBandV4 band[1];	// Variable length. @note: Array size 1 must be
						//				specified for the code to work correctly.

	private:
		// method that is disallowed due to variable size structure.
		operator SGetAutoViolateCmdV2(void) const ;// DownConvert to V2; Cannot be implemented due to variable size.
	};

	typedef SGetAutoViolateCmdV3 SGetAutoViolateCmd;

	struct SGetDmCtrlCmdV1
	{
	  struct SAntControl
	  {
#ifndef CSMSPLUS_SRV_BUILT
		  short start;
		  short stop;
		  short step;
#else
		  int16_t start;
		  int16_t stop;
		  int16_t step;
#endif
	  };

      struct SDFControl
	  {
#ifndef CSMSPLUS_SRV_BUILT
        unsigned char confThreshold;
#else
		  uint8_t confThreshold;
#endif
       };

	  struct antennaRotationCtrl
	  {
		  SAntControl az;
		  SAntControl pol;
	  };

#ifndef CSMSPLUS_SRV_BUILT
	  unsigned long storageTime;		// ms
	  unsigned long measurementTime;	// seconds; 0 has a special meaning and implies perform one rotation
 	  EAvdRate measurementRate;
	  unsigned char avdThreshold;		// dB above NOISE
	  EAnt ant;
	  antennaRotationCtrl rotControl;
	  bool takePriority; // Needed if antenna rotation is involved
	  bool doRotate; // Only applicable to an antenna that is sitting on an antenna rotator
	  bool getDF;        // Only applicable to a DF antenna
	  bool getBW;
	  bool getFreq;
	  bool getModulation;
	  bool getFS;
	  EDFMethod  dfMethod;
	  unsigned short numBands;		// number of frequency bands
	  SBandV4 band[1];	// Variable length. @note: Array size 1 must be
						//				specified for the code to work correctly.
#else
	  uint32_t storageTime;		// ms
	  uint32_t measurementTime;	// seconds; 0 has a special meaning and implies perform one rotation
 	  EAvdRate measurementRate;
	  uint8_t avdThreshold;		// dB above NOISE
	  EAnt ant;
	  antennaRotationCtrl rotControl;
	  bool takePriority; // Needed if antenna rotation is involved
	  bool doRotate; // Only applicable to an antenna that is sitting on an antenna rotator
	  bool getDF;        // Only applicable to a DF antenna
	  bool getBW;
	  bool getFreq;
	  bool getModulation;
	  bool getFS;
	  EDFMethod  dfMethod;
	  uint16_t numBands;		// number of frequency bands
	  SBandV4 band[1];	// Variable length. @note: Array size 1 must be
						//				specified for the code to work correctly.
#endif
	};

	struct SGetDmCtrlCmdV2
		{
		  struct SAntControl
		  {
#ifndef CSMSPLUS_SRV_BUILT
			short start;
			short stop;
			short step;
#else
			int16_t start;
			int16_t stop;
			int16_t step;
#endif
		  };

	      struct SDFControl
		  {
#ifndef CSMSPLUS_SRV_BUILT
	        unsigned char confThreshold;
#else
	        uint8_t confThreshold;
#endif
	      };

		  struct antennaRotationCtrl
		  {
			SAntControl az;
			SAntControl pol;
		  };

		struct SMiscFlags
		{
#ifndef CSMSPLUS_SRV_BUILT
			///@note: Unused filed must have value of 0
			///       so that new bit fields can be added occupying the
			///       unused bits without doing message versioning.
			unsigned char unused1:3;
			///@note: gainMode uses 3 bits to specify the csms Gain
			///		mode, with the following definitions:
			///		0: default (used Gain mode as set by Web Config).
			///		1: used Normal Gain mode.
			///		2: used Rural Gain mode.
			///		3: used Urban Gain mode.
			///		4: used Congested Gain mode.
			///		5-7: not defined (reserved for future).
			unsigned char gainMode : 3;
			unsigned char unused : 2; //Unused bits.
#else
			///@note: Unused filed must have value of 0
			///       so that new bit fields can be added occupying the
			///       unused bits without doing message versioning.
			uint8_t unused1:3;
			///@note: gainMode uses 3 bits to specify the csms Gain
			///		mode, with the following definitions:
			///		0: default (used Gain mode as set by Web Config).
			///		1: used Normal Gain mode.
			///		2: used Rural Gain mode.
			///		3: used Urban Gain mode.
			///		4: used Congested Gain mode.
			///		5-7: not defined (reserved for future).
			uint8_t gainMode : 3;
			uint8_t unused : 2; //Unused bits.
#endif
		};

		union MiscFlags
		{
			//@note: 0 must be written to zeroFlag before using the flag field below.
#ifndef CSMSPLUS_SRV_BUILT
			unsigned char	zeroFlag;
#else
			uint8_t zeroFlag;
#endif
			SMiscFlags	flag;
		};

		operator SGetDmCtrlCmdV1(void) const // Convert to V1
		{
			SGetDmCtrlCmdV1 v1;
			v1.numBands = numBands;
			v1.ant = ant;
			v1.avdThreshold = avdThreshold;
			v1.dfMethod = dfMethod;
			v1.doRotate = (doRotate!= FALSE8);
			v1.getBW = (getBW != FALSE8);
			v1.getDF = (getDF != FALSE8);
			v1.getFS = (getFS != FALSE8);
			v1.getFreq = (getFreq != FALSE8);
			v1.getModulation = (getModulation != FALSE8);
			v1.measurementRate = measurementRate;
			v1.measurementTime = measurementTime;
			//v1.rotControl = rotControl;
			v1.storageTime = storageTime;
			v1.takePriority = (takePriority != FALSE8);

			for(size_t i = 0; i < v1.numBands; ++i)
			{
				v1.band[i].channelBandwidth = band[i].channelBandwidth;
				v1.band[i].exclude = (band[i].exclude != FALSE8);
				v1.band[i].highFrequency = band[i].highFrequency;
				v1.band[i].lowFrequency = band[i].lowFrequency;
				v1.band[i].sType.signalType.horizPol = band[i].sType.signalType.horizPol;
				v1.band[i].sType.signalType.narrow = band[i].sType.signalType.narrow;
				v1.band[i].sType.zeroSType = band[i].sType.zeroSType;
			}

			return v1;
		}

		SGetDmCtrlCmdV2& operator =(const SGetDmCtrlCmdV1 & v1) // Convert from V1
		{
			const SGetDmCtrlCmdV1* src = &v1;


			if(static_cast<void*>(this) == static_cast<const void*>(&v1))
			{
				// Make a copy
				if((src = static_cast<const SGetDmCtrlCmdV1*>(malloc(sizeof(v1)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetDmCtrlCmdV1*>(src), &v1, sizeof(v1));
			}
			numBands = src->numBands;
			ant = src->ant;
			avdThreshold = src->avdThreshold;
			dfMethod = src->dfMethod;
			doRotate = (src->doRotate ? TRUE8 : FALSE8);
			getBW = (src->getBW ? TRUE8 : FALSE8);
			getDF = (src->getDF ? TRUE8 : FALSE8);
			getFS = (src->getFS ? TRUE8 : FALSE8);
			getFreq = (src->getFreq ? TRUE8 : FALSE8);
			getModulation = (src->getModulation ? TRUE8 : FALSE8);
			measurementRate = src->measurementRate;
			measurementTime = src->measurementTime;
			rotControl.az.start = src->rotControl.az.start;
			rotControl.az.step = src->rotControl.az.step;
			rotControl.az.stop = src->rotControl.az.stop;
			rotControl.pol.start = src->rotControl.pol.start;
			rotControl.pol.step = src->rotControl.pol.step;
			rotControl.pol.stop = src->rotControl.pol.stop;
			storageTime = src->storageTime;
			takePriority = (src->takePriority ? TRUE8 : FALSE8);

			for(size_t i = 0; i < src->numBands; ++i)
			{
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = (src->band[i].exclude ? TRUE8 : FALSE8);
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].sType.signalType.horizPol = src->band[i].sType.signalType.horizPol;
				band[i].sType.signalType.narrow = src->band[i].sType.signalType.narrow;
				band[i].sType.zeroSType = src->band[i].sType.zeroSType;
			}

			useBandSpecificRotatorControlForAz = FALSE8;
			useBandSpecificRotatorControlForEl = FALSE8;

			// we now use gateDurationByRunCnt to indicate full rotation and meas time as rep count
			if (measurementTime == 0)
			{
				gateDurationByRunCnt = TRUE8;
				measurementTime = 1; // so V1 full rotation should set flag to true with repetition count to 1
			}

			//printf("meas time = %lu -- getduration by run %d  - num of band %u\n",
			//		measurementTime, int(gateDurationByRunCnt), src->numBands);
			if(src != &v1)
			{
				free(const_cast<SGetDmCtrlCmdV1*>(src));
			}

			return *this;
		}

#ifndef CSMSPLUS_SRV_BUILT
		  unsigned long storageTime;	 // ms
		  unsigned long measurementTime; // Seconds or number of runs depending upon the previous flag
#else
		  uint32_t storageTime;	 // ms
		  uint32_t measurementTime; // Seconds or number of runs depending upon the previous flag
#endif
	 	  EAvdRate measurementRate;
		  EAnt ant;
		  antennaRotationCtrl rotControl;
		  EDFMethod  dfMethod;
#ifndef CSMSPLUS_SRV_BUILT
		  unsigned short numBands;		// number of frequency bands
		  unsigned short shortspare1;
#else
		  uint16_t numBands;		// number of frequency bands
		  uint16_t shortspare1;
#endif
		  BOOL8 takePriority; // Needed if antenna rotation is involved
		  BOOL8 doRotate; // Only applicable to an antenna that is sitting on an antenna rotator
		  BOOL8 getDF;        // Only applicable to a DF antenna
		  BOOL8 getBW;
		  BOOL8 getFreq;
		  BOOL8 getModulation;
		  BOOL8 getFS;
		  BOOL8 gateDurationByRunCnt;     // This flag indicates whether measurement time represents number of runs or duration
		  BOOL8 useBandSpecificRotatorControlForAz;  // If true; the following lists are used else Antenna Rotator control spec is used
	      BOOL8 useBandSpecificRotatorControlForEl;  // If true; the following lists are used else Antenna Rotator control spec is used
	      MiscFlags miscFlags;
#ifndef CSMSPLUS_SRV_BUILT
		  unsigned char avdThreshold;	 // dB above NOISE
#else
		  uint8_t avdThreshold;	 // dB above NOISE
#endif
		  SBandV5 band[1];	// Variable length. @note: Array size 1 must be
							//				specified for the code to work correctly.
	      //SBandV4 band[1];
		};

	typedef SGetDmCtrlCmdV2 SGetDmCtrlCmd;

	struct SGetScanDfCmdV0 // GET_SCANDF V0
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_EXCLUDES = 100;
		static const unsigned long MAX_INCLUDES = 10;
#else
		static const uint32_t MAX_EXCLUDES = 100;
		static const uint32_t MAX_INCLUDES = 10;
#endif

		struct SSignalType
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long gsm : 1;
			unsigned long narrow : 1;
			unsigned long horizPol : 1;
			unsigned long unused : 29;
#else
			uint32_t gsm : 1;
			uint32_t narrow : 1;
			uint32_t horizPol : 1;
			uint32_t unused : 29;
#endif
		};

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numBands;		// number of frequency bands
		unsigned long lowFrequency[MAX_INCLUDES + MAX_EXCLUDES];	// low frequency (Hz)
		unsigned long highFrequency[MAX_INCLUDES + MAX_EXCLUDES];	// high frequency (Hz), = low frequency for single channel
		unsigned long channelBandwidth[MAX_INCLUDES + MAX_EXCLUDES]; // (Hz)
		unsigned long includeExclude[MAX_INCLUDES + MAX_EXCLUDES];
		SSignalType signalType[MAX_INCLUDES + MAX_EXCLUDES];
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned long scanDfThreshold;	// dB above NOISE
		unsigned long numAzimuths;
		unsigned long confidence;       // 0 to 100
		unsigned long recordAudioDf;	// 0 = don't record; >0 = record (RA3 function)
		unsigned long recordHoldoff;	// record holdoff time in sec (RA3 function)
#else
		uint32_t numBands;		// number of frequency bands
		uint32_t lowFrequency[MAX_INCLUDES + MAX_EXCLUDES];	// low frequency (Hz)
		uint32_t highFrequency[MAX_INCLUDES + MAX_EXCLUDES];	// high frequency (Hz), = low frequency for single channel
		uint32_t channelBandwidth[MAX_INCLUDES + MAX_EXCLUDES]; // (Hz)
		uint32_t includeExclude[MAX_INCLUDES + MAX_EXCLUDES];
		SSignalType signalType[MAX_INCLUDES + MAX_EXCLUDES];
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint32_t scanDfThreshold;	// dB above NOISE
		uint32_t numAzimuths;
		uint32_t confidence;       // 0 to 100
		uint32_t recordAudioDf;	// 0 = don't record; >0 = record (RA3 function)
		uint32_t recordHoldoff;	// record holdoff time in sec (RA3 function)
#endif
		SRcvrCtrlCmdV0 rcvrCtrl;		// for recording (RA3 function)
	};

	struct SGetScanDfCmdV1 // GET_SCANDF V1 (Variable length)
	{
		operator SGetScanDfCmdV0(void) const // Convert to v0
		{
			SGetScanDfCmdV0 v0;
			v0.numBands = (numBands <= SGetScanDfCmdV0::MAX_INCLUDES + SGetScanDfCmdV0::MAX_EXCLUDES ?
				numBands : SGetScanDfCmdV0::MAX_INCLUDES + SGetScanDfCmdV0::MAX_EXCLUDES);

			for(size_t i = 0; i < v0.numBands; ++i)
			{
#ifndef CSMSPLUS_SRV_BUILT
				v0.lowFrequency[i] = Units::Frequency(band[i].lowFrequency).Hz<unsigned long>(true);
				v0.highFrequency[i] = Units::Frequency(band[i].highFrequency).Hz<unsigned long>(true);
				v0.channelBandwidth[i] = Units::Frequency(band[i].channelBandwidth).Hz<unsigned long>(true);
#else
				v0.lowFrequency[i] = Units::Frequency(band[i].lowFrequency).Hz<uint32_t>(true);
				v0.highFrequency[i] = Units::Frequency(band[i].highFrequency).Hz<uint32_t>(true);
				v0.channelBandwidth[i] = Units::Frequency(band[i].channelBandwidth).Hz<uint32_t>(true);
#endif
				v0.includeExclude[i] = (band[i].exclude ? 1 : 0);
				v0.signalType[i].gsm = band[i].signalType.gsm;
				v0.signalType[i].narrow = band[i].signalType.narrow;
				v0.signalType[i].horizPol = band[i].signalType.horizPol;
				v0.signalType[i].unused = band[i].signalType.unused;
			}

			v0.storageTime = storageTime;
			v0.measurementTime = measurementTime;
			v0.scanDfThreshold = scanDfThreshold;
			v0.numAzimuths = numAzimuths;
			v0.confidence = confidence;
			v0.recordAudioDf = recordAudioDf;
			v0.recordHoldoff = recordHoldoff;
			v0.rcvrCtrl = rcvrCtrl;	

			return v0;
		}

		SGetScanDfCmdV1& operator =(const SGetScanDfCmdV0 & v0) // Convert from V0
		{
			storageTime = v0.storageTime;
			measurementTime = v0.measurementTime;
			numAzimuths = v0.numAzimuths;
			confidence = v0.confidence;
			recordHoldoff = v0.recordHoldoff;
#ifndef CSMSPLUS_SRV_BUILT
			scanDfThreshold = static_cast<unsigned char>(v0.scanDfThreshold);
			recordAudioDf = (v0.recordAudioDf != 0);
			numBands = static_cast<unsigned short>(v0.numBands <= SGetScanDfCmdV0::MAX_INCLUDES + SGetScanDfCmdV0::MAX_EXCLUDES ? v0.numBands : 0);
#else
			scanDfThreshold = static_cast<uint8_t>(v0.scanDfThreshold);
			recordAudioDf = (v0.recordAudioDf != 0);
			numBands = static_cast<uint16_t>(v0.numBands <= SGetScanDfCmdV0::MAX_INCLUDES + SGetScanDfCmdV0::MAX_EXCLUDES ? v0.numBands : 0);
#endif
			rcvrCtrl = v0.rcvrCtrl;

			for(size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = Units::Frequency(v0.lowFrequency[i]).GetRaw();
				band[i].highFrequency = Units::Frequency(v0.highFrequency[i]).GetRaw();
				band[i].channelBandwidth = Units::Frequency(v0.channelBandwidth[i]).GetRaw();
				band[i].signalType.gsm = v0.signalType[i].gsm;
				band[i].signalType.narrow = v0.signalType[i].narrow;
				band[i].signalType.horizPol = v0.signalType[i].horizPol;
				band[i].signalType.unused = v0.signalType[i].unused;
				band[i].exclude = (v0.includeExclude[i] != 0);
			}

			return *this;
		}

		struct SBand
		{
			struct SSignalType
			{
#ifndef CSMSPLUS_SRV_BUILT
				unsigned char gsm : 1;
				unsigned char narrow : 1;
				unsigned char horizPol : 1;
				unsigned char unused : 5;
#else
				uint8_t gsm : 1;
				uint8_t narrow : 1;
				uint8_t horizPol : 1;
				uint8_t unused : 5;
#endif
			};

			Units::Frequency::Raw lowFrequency;	
			Units::Frequency::Raw highFrequency;	// high frequency = low frequency for single channel
			Units::Frequency::Raw channelBandwidth;
			SSignalType signalType;
			bool exclude;
		};

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned long numAzimuths;
		unsigned long confidence;       // 0 to 100
		unsigned long recordHoldoff;	// record holdoff time in sec (RA3 function)
		unsigned char scanDfThreshold;	// dB above NOISE
		bool recordAudioDf;	// 0 = don't record; >0 = record (RA3 function)
		unsigned short numBands;		// number of frequency bands
#else
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint32_t numAzimuths;
		uint32_t confidence;       // 0 to 100
		uint32_t recordHoldoff;	// record holdoff time in sec (RA3 function)
		uint8_t scanDfThreshold;	// dB above NOISE
		bool recordAudioDf;	// 0 = don't record; >0 = record (RA3 function)
		uint16_t numBands;		// number of frequency bands
#endif
		SRcvrCtrlCmdV1 rcvrCtrl;		// for recording (RA3 function)
		SBand band[1]; // Variable length
	};

	typedef SGetScanDfCmdV1 SGetScanDfCmd;

	struct SOccupancyOutputFlagsV1; // Forward declaration

	struct SOccupancyOutputFlagsV0	// All following are true/false flags
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char occupancyVsChannel;
		unsigned char eFieldVsChannel;
		unsigned char occupancyVsTimeOfDay;
		unsigned char msglengthVsChannel;
		unsigned char msglengthDistribution;
		unsigned char spectrogram;
		unsigned char timegram;
#else
		uint8_t occupancyVsChannel;
		uint8_t eFieldVsChannel;
		uint8_t occupancyVsTimeOfDay;
		uint8_t msglengthVsChannel;
		uint8_t msglengthDistribution;
		uint8_t spectrogram;
		uint8_t timegram;
#endif
	};

	struct SOccupancyOutputFlagsV1	// All following are true/false flags
	{
		operator SOccupancyOutputFlagsV0(void) const // Convert to v0
		{
			SOccupancyOutputFlagsV0 v0;
			v0.occupancyVsChannel = occupancyVsChannel;
			v0.eFieldVsChannel = eFieldVsChannel;
			v0.occupancyVsTimeOfDay = occupancyVsTimeOfDay;
			v0.msglengthVsChannel = msglengthVsChannel;
			v0.msglengthDistribution = msglengthDistribution;
			v0.spectrogram = spectrogram;
			v0.timegram = timegram;

			return v0;
		}

		SOccupancyOutputFlagsV1& operator =(const SOccupancyOutputFlagsV0 & v0) // Convert from V0
		{
			occupancyVsChannel = v0.occupancyVsChannel;
			eFieldVsChannel = v0.eFieldVsChannel;
			occupancyVsTimeOfDay = v0.occupancyVsTimeOfDay;
			msglengthVsChannel = v0.msglengthVsChannel;
			msglengthDistribution = v0.msglengthDistribution;
			spectrogram = v0.spectrogram;
			timegram = v0.timegram;
			occupancyFlag.zeroFlag = 0;

			return *this;
		}

		struct SOccupancyFlag
		{
#ifndef CSMSPLUS_SRV_BUILT
			//@note: Unused filed must have value of 0
			//       so that new bit fields can be added occupying the
			//       unused bits without doing message versioning.
			unsigned char eFieldVsTOD : 1;

			//Set this to 1 to get rid-off of Raised Noise Floor in Scorpio's
			// Spectrum Analyzer "Peak Hold" setting. 
			unsigned char adjustAgcThres : 1;

			//Set this to 1 when doing TestClient spur test.
			unsigned char spurTest : 1;

			///@note: gainMode uses 3 bits to specify the csms Gain
			///		mode, with the following definitions:
			///		0: default (used Gain mode as set by Web Config).
			///		1: used Normal Gain mode.
			///		2: used Rural Gain mode.
			///		3: used Urban Gain mode.
			///		4: used Congested Gain mode.
			///		5-7: not defined (reserved for future).
			unsigned char gainMode : 3;
			unsigned char unused : 2; //Unused bits.
#else
			//@note: Unused filed must have value of 0
			//       so that new bit fields can be added occupying the
			//       unused bits without doing message versioning.
			uint8_t eFieldVsTOD : 1;

			//Set this to 1 to get rid-off of Raised Noise Floor in Scorpio's
			// Spectrum Analyzer "Peak Hold" setting. 
			uint8_t adjustAgcThres : 1;

			//Set this to 1 when doing TestClient spur test.
			uint8_t spurTest : 1;

			///@note: gainMode uses 3 bits to specify the csms Gain
			///		mode, with the following definitions:
			///		0: default (used Gain mode as set by Web Config).
			///		1: used Normal Gain mode.
			///		2: used Rural Gain mode.
			///		3: used Urban Gain mode.
			///		4: used Congested Gain mode.
			///		5-7: not defined (reserved for future).
			uint8_t gainMode : 3;
			uint8_t unused : 2; //Unused bits.
#endif
		};

		union OccupancyFlag
		{
			//@note: 0 must be written to zeroFlag before using the flag field below.
#ifndef CSMSPLUS_SRV_BUILT
			unsigned char	zeroFlag;
#else
			uint8_t	zeroFlag;
#endif
			SOccupancyFlag	flag;
		};

#ifndef CSMSPLUS_SRV_BUILT
		unsigned char occupancyVsChannel;
		unsigned char eFieldVsChannel;
		unsigned char occupancyVsTimeOfDay;
		unsigned char msglengthVsChannel;
		unsigned char msglengthDistribution;
		unsigned char spectrogram;
		unsigned char timegram;
#else
		uint8_t occupancyVsChannel;
		uint8_t eFieldVsChannel;
		uint8_t occupancyVsTimeOfDay;
		uint8_t msglengthVsChannel;
		uint8_t msglengthDistribution;
		uint8_t spectrogram;
		uint8_t timegram;
#endif
		OccupancyFlag occupancyFlag;
	};

	typedef SOccupancyOutputFlagsV1 SOccupancyOutputFlags;

	struct SGetOccupancyCmdV1; // Forward declaration

	struct SGetOccupancyCmdV0 // GET_OCCUPANCY V0
	{
		inline operator SGetOccupancyCmdV1(void) const; // Convert to V1; definition below

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numBands;		// number of frequency bands
		unsigned long lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		unsigned long highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		unsigned long channelBandwidth[MAX_OCCBANDS]; // (Hz)
		unsigned long includeExclude[MAX_OCCBANDS];
		unsigned long storageTime;		// seconds
		EDurationMethod durationMethod;
		unsigned long measurementTime;	// seconds
		unsigned long confidenceLevel;	// percent
		unsigned long desiredAccuracy;	// percent
		EThresholdMethod thresholdMethod;
		unsigned long useSecondaryThreshold;  // 0 = don't use  >0 = use (for RA3 intermediate data)
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		long occPrimaryThreshold[2];
		long occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		unsigned long saveIntermediateData;  // 0 = don't save >0 = save (RA3 data)
#else
		uint32_t numBands;		// number of frequency bands
		uint32_t lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		uint32_t highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		uint32_t channelBandwidth[MAX_OCCBANDS]; // (Hz)
		uint32_t includeExclude[MAX_OCCBANDS];
		uint32_t storageTime;		// seconds
		EDurationMethod durationMethod;
		uint32_t measurementTime;	// seconds
		uint32_t confidenceLevel;	// percent
		uint32_t desiredAccuracy;	// percent
		EThresholdMethod thresholdMethod;
		uint32_t useSecondaryThreshold;  // 0 = don't use  >0 = use (for RA3 intermediate data)
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		int32_t occPrimaryThreshold[2];
		int32_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		uint32_t occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		uint32_t saveIntermediateData;  // 0 = don't save >0 = save (RA3 data)
#endif
	};

	struct SGetOccupancyCmdV2; // Forward declaration

	struct SGetOccupancyCmdV1 // GET_OCCUPANCY V1
	{
		operator SGetOccupancyCmdV0(void) const // Convert to V0
		{
			SGetOccupancyCmdV0 v0;
			v0.numBands = numBands;
			memcpy(v0.lowFrequency, lowFrequency, sizeof(v0.lowFrequency));
			memcpy(v0.highFrequency, highFrequency, sizeof(v0.highFrequency));
			memcpy(v0.channelBandwidth, channelBandwidth, sizeof(v0.channelBandwidth));
			memcpy(v0.includeExclude, includeExclude, sizeof(v0.includeExclude));
			v0.storageTime = storageTime;
			v0.durationMethod = durationMethod;
			v0.measurementTime = measurementTime;
			v0.confidenceLevel = confidenceLevel;
			v0.desiredAccuracy = desiredAccuracy;
			v0.thresholdMethod = thresholdMethod;
			v0.useSecondaryThreshold = useSecondaryThreshold;
			memcpy(v0.occPrimaryThreshold, occPrimaryThreshold, sizeof(v0.occPrimaryThreshold));
			memcpy(v0.occSecondaryThreshold, occSecondaryThreshold, sizeof(v0.occSecondaryThreshold));
			v0.occupancyMinGap = occupancyMinGap;
			v0.output = output;
			v0.saveIntermediateData = saveIntermediateData;

			return v0;
		}

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numBands;		// number of frequency bands
		unsigned long lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		unsigned long highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		unsigned long channelBandwidth[MAX_OCCBANDS]; // (Hz)
		unsigned long includeExclude[MAX_OCCBANDS];
		unsigned long storageTime;		// seconds
		EDurationMethod durationMethod;
		unsigned long measurementTime;	// seconds
		unsigned long confidenceLevel;	// percent
		unsigned long desiredAccuracy;	// percent
		EThresholdMethod thresholdMethod;
		unsigned long useSecondaryThreshold;  // 0 = don't use  >0 = use (for RA3 intermediate data)
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		long occPrimaryThreshold[2];
		long occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		unsigned long saveIntermediateData;  // 0 = don't save >0 = save (RA3 data)
#else
		uint32_t numBands;		// number of frequency bands
		uint32_t lowFrequency[MAX_OCCBANDS];	// low frequency (Hz)
		uint32_t highFrequency[MAX_OCCBANDS];	// high frequency (Hz), = low frequency for single channel
		uint32_t channelBandwidth[MAX_OCCBANDS]; // (Hz)
		uint32_t includeExclude[MAX_OCCBANDS];
		uint32_t storageTime;		// seconds
		EDurationMethod durationMethod;
		uint32_t measurementTime;	// seconds
		uint32_t confidenceLevel;	// percent
		uint32_t desiredAccuracy;	// percent
		EThresholdMethod thresholdMethod;
		uint32_t useSecondaryThreshold;  // 0 = don't use  >0 = use (for RA3 intermediate data)
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		int32_t occPrimaryThreshold[2];
		int32_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		uint32_t occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		uint32_t saveIntermediateData;  // 0 = don't save >0 = save (RA3 data)
#endif
		EAnt ant;
		bool forceNarrowBand;
	};

	struct SGetOccupancyCmdV3; // Forward declaration

	struct SGetOccupancyCmdV2 // GET_OCCUPANCY V2 (variable length)
							//Cannot be instantiated as local variable.
	{
		operator SGetOccupancyCmdV1(void) const // Convert to V1
		{
			SGetOccupancyCmdV1 v1;
			v1.numBands = (numBands <= MAX_OCCBANDS ? numBands : MAX_OCCBANDS);
			v1.forceNarrowBand = false;

			for(size_t i = 0; i < v1.numBands; ++i)
			{
				v1.lowFrequency[i] = band[i].lowFrequency;
				v1.highFrequency[i] = band[i].highFrequency;
				v1.channelBandwidth[i] = band[i].channelBandwidth;
				v1.includeExclude[i] = (band[i].exclude ? 1 : 0);

				if(band[i].forceNarrowBand)
				{
					v1.forceNarrowBand = true;
				}
			}

			v1.storageTime = storageTime;
			v1.durationMethod = durationMethod;
			v1.measurementTime = measurementTime;
			v1.confidenceLevel = confidenceLevel;
			v1.desiredAccuracy = desiredAccuracy;
			v1.thresholdMethod = thresholdMethod;
			v1.useSecondaryThreshold = useSecondaryThreshold;
			memcpy(v1.occPrimaryThreshold, occPrimaryThreshold, sizeof(v1.occPrimaryThreshold));
			memcpy(v1.occSecondaryThreshold, occSecondaryThreshold, sizeof(v1.occSecondaryThreshold));
			v1.occupancyMinGap = occupancyMinGap;
			v1.output = output;
			v1.saveIntermediateData = saveIntermediateData;
			v1.ant = ant;

			return v1;
		}

		SGetOccupancyCmdV2& operator =(SGetOccupancyCmdV1 v1) // Convert from V1
		{
			storageTime = v1.storageTime;
			durationMethod = v1.durationMethod;
			measurementTime = v1.measurementTime;
			confidenceLevel = v1.confidenceLevel;
			desiredAccuracy = v1.desiredAccuracy;
			thresholdMethod = v1.thresholdMethod;
			useSecondaryThreshold = (v1.useSecondaryThreshold != 0);
			saveIntermediateData = (v1.saveIntermediateData != 0);
			memcpy(occPrimaryThreshold, v1.occPrimaryThreshold, sizeof(occPrimaryThreshold));
			memcpy(occSecondaryThreshold, v1.occSecondaryThreshold, sizeof(occSecondaryThreshold));
			occupancyMinGap = v1.occupancyMinGap;
			output = v1.output;
			ant = v1.ant;
			numBands = (v1.numBands <= MAX_OCCBANDS ? v1.numBands : 0);

			for(size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = v1.lowFrequency[i];
				band[i].highFrequency = v1.highFrequency[i];
				band[i].channelBandwidth = v1.channelBandwidth[i];
				band[i].exclude = (v1.includeExclude[i] != 0);
				band[i].forceNarrowBand = v1.forceNarrowBand;
			}

			return *this;
		}

		SGetOccupancyCmdV2& operator =(const SGetOccupancyCmdV3& v3); // Convert from V3
		
		struct SBand
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long lowFrequency;	// low frequency (Hz)
			unsigned long highFrequency;	// high frequency (Hz), = low frequency for single channel
			unsigned long channelBandwidth; // (Hz)
#else
			uint32_t lowFrequency;	// low frequency (Hz)
			uint32_t highFrequency;	// high frequency (Hz), = low frequency for single channel
			uint32_t channelBandwidth; // (Hz)
#endif
			bool exclude;
			bool forceNarrowBand;
		};

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		EDurationMethod durationMethod;
		unsigned long measurementTime;	// seconds
		unsigned long confidenceLevel;	// percent
		unsigned long desiredAccuracy;	// percent
		EThresholdMethod thresholdMethod;
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		long occPrimaryThreshold[2];
		long occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		EAnt ant;
		unsigned long numBands;		// number of frequency bands
#else
		uint32_t storageTime;		// seconds
		EDurationMethod durationMethod;
		uint32_t measurementTime;	// seconds
		uint32_t confidenceLevel;	// percent
		uint32_t desiredAccuracy;	// percent
		EThresholdMethod thresholdMethod;
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		int32_t occPrimaryThreshold[2];
		int32_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		uint32_t occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		EAnt ant;
		uint32_t numBands;		// number of frequency bands
#endif
		SBand band[1];	// Variable length. Must be at the end of struct for
						// conversion code to work correctly.

	private:
		// method that is disallowed due to variable size structure.
		inline operator SGetOccupancyCmdV3(void) const; // Convert to V3; Cannot be implemented due to variable len.
	};

	struct SGetOccupancyCmdV4;	//Forward declaration.

	struct SGetOccupancyCmdV3 // GET_OCCUPANCY V3 (variable length)
							//Cannot be instantiated as local variable.
	{
		SGetOccupancyCmdV3& operator =(const SGetOccupancyCmdV2& v2) // Convert from V2
		{
			const SGetOccupancyCmdV2* src = &v2;

			if(static_cast<void*>(this) == static_cast<const void*>(&v2))
			{
				// Make a copy
				if((src = static_cast<const SGetOccupancyCmdV2*>(malloc(SizeOf(v2)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetOccupancyCmdV2*>(src), &v2, SizeOf(v2));
			}

			storageTime = src->storageTime;
			durationMethod = src->durationMethod;
			measurementTime = src->measurementTime;
#ifndef CSMSPLUS_SRV_BUILT
			confidenceLevel = static_cast<unsigned char>(src->confidenceLevel);
			desiredAccuracy = static_cast<unsigned char>(src->desiredAccuracy);
			thresholdMethod = src->thresholdMethod;
			useSecondaryThreshold = (src->useSecondaryThreshold != 0);
			saveIntermediateData = (src->saveIntermediateData != 0);
			occPrimaryThreshold[0] = (src->thresholdMethod == FIXED || src->thresholdMethod == BOTH ? short(src->occPrimaryThreshold[0]) : 0);
			occPrimaryThreshold[1] = (src->thresholdMethod == NOISE || src->thresholdMethod == BOTH ? short(src->occPrimaryThreshold[1]) : 0);
			occSecondaryThreshold[0] = (src->useSecondaryThreshold != 0 && (src->thresholdMethod == FIXED || src->thresholdMethod == BOTH) ?
				short(src->occSecondaryThreshold[0]) : 0);
			occSecondaryThreshold[1] = (src->useSecondaryThreshold != 0 && (src->thresholdMethod == NOISE || src->thresholdMethod == BOTH) ?
				short(src->occSecondaryThreshold[1]) : 0);
#else
			confidenceLevel = static_cast<uint8_t>(src->confidenceLevel);
			desiredAccuracy = static_cast<uint8_t>(src->desiredAccuracy);
			thresholdMethod = src->thresholdMethod;
			useSecondaryThreshold = (src->useSecondaryThreshold != 0);
			saveIntermediateData = (src->saveIntermediateData != 0);
			occPrimaryThreshold[0] = (src->thresholdMethod == FIXED || src->thresholdMethod == BOTH ? int16_t(src->occPrimaryThreshold[0]) : 0);
			occPrimaryThreshold[1] = (src->thresholdMethod == NOISE || src->thresholdMethod == BOTH ? int16_t(src->occPrimaryThreshold[1]) : 0);
			occSecondaryThreshold[0] = (src->useSecondaryThreshold != 0 && (src->thresholdMethod == FIXED || src->thresholdMethod == BOTH) ?
				int16_t(src->occSecondaryThreshold[0]) : 0);
			occSecondaryThreshold[1] = (src->useSecondaryThreshold != 0 && (src->thresholdMethod == NOISE || src->thresholdMethod == BOTH) ?
				int16_t(src->occSecondaryThreshold[1]) : 0);
#endif
			occupancyMinGap = src->occupancyMinGap;
			output = src->output;
			ant = src->ant;
			numBands = src->numBands;

			for(size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = Units::Frequency(src->band[i].lowFrequency).GetRaw();
				band[i].highFrequency = Units::Frequency(src->band[i].highFrequency).GetRaw();
				band[i].channelBandwidth = Units::Frequency(src->band[i].channelBandwidth).GetRaw();
				band[i].exclude = src->band[i].exclude;
				band[i].forceNarrowBand = src->band[i].forceNarrowBand;
			}

			if(src != &v2)
			{
				free(const_cast<SGetOccupancyCmdV2*>(src));
			}

			return *this;
		}

		SGetOccupancyCmdV3& operator =(const SGetOccupancyCmdV4& v4); // Down Convert from V4

		struct SBand
		{
			Units::Frequency::Raw lowFrequency;	// low frequency (Hz)
			Units::Frequency::Raw highFrequency;	// high frequency (Hz), = low frequency for single channel
			Units::Frequency::Raw channelBandwidth; // (Hz)
			bool exclude;
			bool forceNarrowBand;
		};

		EDurationMethod durationMethod;
		EThresholdMethod thresholdMethod;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned char confidenceLevel;	// percent
		unsigned char desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		EAnt ant;
		unsigned long numBands;		// number of frequency bands
#else
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint8_t confidenceLevel;	// percent
		uint8_t desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		int16_t occPrimaryThreshold[2];
		int16_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		uint32_t occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		EAnt ant;
		uint32_t numBands;		// number of frequency bands
#endif
		SBand band[1];	// Variable length. Must be at the end of struct for
						// conversion code to work correctly.
	};

	struct SGetOccupancyCmdV5;	//Forward declaration.

	struct SGetOccupancyCmdV4 // GET_OCCUPANCY V4 (variable length)
							//Cannot be instantiated as local variable.
	{
		SGetOccupancyCmdV4& operator =(const SGetOccupancyCmdV3& v3) // Convert from V3
		{
			const SGetOccupancyCmdV3* src = &v3;

			if(static_cast<void*>(this) == static_cast<const void*>(&v3))
			{
				// Make a copy
				if((src = static_cast<const SGetOccupancyCmdV3*>(malloc(SizeOf(v3)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetOccupancyCmdV3*>(src), &v3, SizeOf(v3));
			}

			storageTime = src->storageTime;
			durationMethod = src->durationMethod;
			measurementTime = src->measurementTime;
#ifndef CSMSPLUS_SRV_BUILT
			confidenceLevel = static_cast<unsigned char>(src->confidenceLevel);
			desiredAccuracy = static_cast<unsigned char>(src->desiredAccuracy);
#else
			confidenceLevel = static_cast<uint8_t>(src->confidenceLevel);
			desiredAccuracy = static_cast<uint8_t>(src->desiredAccuracy);
#endif
			thresholdMethod = src->thresholdMethod;
			useSecondaryThreshold = (src->useSecondaryThreshold != 0);
			saveIntermediateData = (src->saveIntermediateData != 0);
			occPrimaryThreshold[0] = src->occPrimaryThreshold[0];
			occPrimaryThreshold[1] = src->occPrimaryThreshold[1];
			occSecondaryThreshold[0] = src->occSecondaryThreshold[0];
			occSecondaryThreshold[1] = src->occSecondaryThreshold[1];
			occupancyMinGap = src->occupancyMinGap;
			output = src->output;
			ant = src->ant;
			numBands = src->numBands;

			for(size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				//Clear unused fields to default 0.
				band[i].sType.zeroSType = 0;
				if( src->band[i].forceNarrowBand )
				{
					band[i].sType.signalType.narrow = 1;
				}
			}

			if(src != &v3)
			{
				free(const_cast<SGetOccupancyCmdV3*>(src));
			}

			return *this;
		}

		SGetOccupancyCmdV4& operator =(const SGetOccupancyCmdV5& v5); // Down Convert from V5

		EDurationMethod durationMethod;
		EThresholdMethod thresholdMethod;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned char confidenceLevel;	// percent
		unsigned char desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		EAnt ant;
		unsigned long numBands;		// number of frequency bands
#else
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint8_t confidenceLevel;	// percent
		uint8_t desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		int16_t occPrimaryThreshold[2];
		int16_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		uint32_t occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV0 output;
		EAnt ant;
		uint32_t numBands;		// number of frequency bands
#endif
		SBandV4 band[1];	// Variable length. Must be at the end of struct and
						// size of 1 for conversion code & other to work correctly.
	};

	//struct SGetOccupancyCmdV6;	//Forward declaration.

	struct SGetOccupancyCmdV5 // GET_OCCUPANCY V5 (variable length)
		//Cannot be instantiated as local variable.
	{
		SGetOccupancyCmdV5& operator =(const SGetOccupancyCmdV4& v4) // Convert from V4
		{
			const SGetOccupancyCmdV4* src = &v4;

			if (static_cast<void*>(this) == static_cast<const void*>(&v4))
			{
				// Make a copy
				if ((src = static_cast<const SGetOccupancyCmdV4*>(malloc(SizeOf(v4)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetOccupancyCmdV4*>(src), &v4, SizeOf(v4));
			}

			storageTime = src->storageTime;
			durationMethod = src->durationMethod;
			measurementTime = src->measurementTime;
#ifndef CSMSPLUS_SRV_BUILT
			confidenceLevel = static_cast<unsigned char>(src->confidenceLevel);
			desiredAccuracy = static_cast<unsigned char>(src->desiredAccuracy);
#else
			confidenceLevel = static_cast<uint8_t>(src->confidenceLevel);
			desiredAccuracy = static_cast<uint8_t>(src->desiredAccuracy);
#endif
			thresholdMethod = src->thresholdMethod;
			useSecondaryThreshold = (src->useSecondaryThreshold != 0);
			saveIntermediateData = (src->saveIntermediateData != 0);
			occPrimaryThreshold[0] = src->occPrimaryThreshold[0];
			occPrimaryThreshold[1] = src->occPrimaryThreshold[1];
			occSecondaryThreshold[0] = src->occSecondaryThreshold[0];
			occSecondaryThreshold[1] = src->occSecondaryThreshold[1];
			occupancyMinGap = src->occupancyMinGap;
			output = src->output;
			ant = src->ant;
			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i]= src->band[i];
			}

			if (src != &v4)
			{
				free(const_cast<SGetOccupancyCmdV4*>(src));
			}

			return *this;
		}

		SGetOccupancyCmdV5& operator =(const SGetAutoViolateCmdV3& avCmd) // Convert from SGetAutoViolateCmdV3
		{
			const SGetAutoViolateCmdV3* src = &avCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&avCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetAutoViolateCmdV3*>(malloc(SizeOf(avCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetAutoViolateCmdV3*>(src), &avCmd, SizeOf(avCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				band[i].sType.signalType = src->band[i].sType.signalType;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->avdThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 1;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.zeroFlag = 0;
			saveIntermediateData = false;
			ant = src->ant;

			if (src != &avCmd)
			{
				free(const_cast<SGetAutoViolateCmdV3*>(src));
			}

			return *this;
		};

		SGetOccupancyCmdV5& operator =(const SGetDmCtrlCmdV1& dmCmd) // Convert from SGetDmCtrlCmdV1
		{
			const SGetDmCtrlCmdV1* src = &dmCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&dmCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetDmCtrlCmdV1*>(malloc(SizeOf(dmCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetDmCtrlCmdV1*>(src), &dmCmd, SizeOf(dmCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				band[i].sType.signalType = src->band[i].sType.signalType;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->avdThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 1;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.zeroFlag = 0;
			saveIntermediateData = false;
			ant = src->ant;

			if (src != &dmCmd)
			{
				free(const_cast<SGetDmCtrlCmdV1*>(src));
			}

			return *this;
		};

		SGetOccupancyCmdV5& operator =(const SGetDmCtrlCmdV2& dmCmd) // Convert from SGetDmCtrlCmdV1
		{
			const SGetDmCtrlCmdV2* src = &dmCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&dmCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetDmCtrlCmdV2*>(malloc(SizeOf(dmCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetDmCtrlCmdV2*>(src), &dmCmd, SizeOf(dmCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = (src->band[i].exclude != FALSE8);
				band[i].sType.signalType.horizPol = src->band[i].sType.signalType.horizPol;
				band[i].sType.signalType.narrow = src->band[i].sType.signalType.narrow;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->avdThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 1;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.flag.gainMode = src->miscFlags.flag.gainMode;
			saveIntermediateData = false;
			ant = src->ant;

			if (src != &dmCmd)
			{
				free(const_cast<SGetDmCtrlCmdV2*>(src));
			}

			return *this;
		};

		SGetOccupancyCmdV5& operator =(const SGetScanDfCmdV1& dfCmd) // Convert from SGetScanDfCmd
		{
			const SGetScanDfCmdV1* src = &dfCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&dfCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetScanDfCmdV1*>(malloc(SizeOf(dfCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetScanDfCmdV1*>(src), &dfCmd, SizeOf(dfCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;

				//Clear unused fields to 0.
				band[i].sType.zeroSType = 0;
				band[i].sType.signalType.narrow = src->band[i].signalType.narrow;
				band[i].sType.signalType.horizPol = src->band[i].signalType.horizPol;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->scanDfThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 0;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.zeroFlag = 0;
			saveIntermediateData = false;
			ant = ANT1;

			if (src != &dfCmd)
			{
				free(const_cast<SGetScanDfCmdV1*>(src));
			}

			return *this;
		}

		EDurationMethod durationMethod;
		EThresholdMethod thresholdMethod;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned char confidenceLevel;	// percent
		unsigned char desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV1 output;
		EAnt ant;
		unsigned long numBands;		// number of frequency bands
		SBandV4 band[1];	// Variable length. Must be at the end of struct and
		// size of 1 for conversion code & other to work correctly.
#else
		uint32_t storageTime;		// seconds
		uint32_t measurementTime;	// seconds
		uint8_t confidenceLevel;	// percent
		uint8_t desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		int16_t occPrimaryThreshold[2];
		int16_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		uint32_t occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV1 output;
		EAnt ant;
		uint32_t numBands;		// number of frequency bands
		SBandV4 band[1];	// Variable length. Must be at the end of struct and
		// size of 1 for conversion code & other to work correctly.
#endif
	};

	/*struct SGetOccupancyCmdV6 // GET_OCCUPANCY V5 (variable length)
		//Cannot be instantiated as local variable.
	{
		SGetOccupancyCmdV6& operator =(const SGetOccupancyCmdV5& v5) // Convert from V5
		{
			const SGetOccupancyCmdV5* src = &v5;

			if (static_cast<void*>(this) == static_cast<const void*>(&v5))
			{
				// Make a copy
				if ((src = static_cast<const SGetOccupancyCmdV5*>(malloc(SizeOf(v5)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetOccupancyCmdV5*>(src), &v5, SizeOf(v5));
			}

			storageTime = src->storageTime;
			durationMethod = src->durationMethod;
			measurementTime = src->measurementTime;
			confidenceLevel = static_cast<unsigned char>(src->confidenceLevel);
			desiredAccuracy = static_cast<unsigned char>(src->desiredAccuracy);
			thresholdMethod = src->thresholdMethod;
			useSecondaryThreshold = (src->useSecondaryThreshold != 0);
			saveIntermediateData = (src->saveIntermediateData != 0);
			occPrimaryThreshold[0] = src->occPrimaryThreshold[0];
			occPrimaryThreshold[1] = src->occPrimaryThreshold[1];
			occSecondaryThreshold[0] = src->occSecondaryThreshold[0];
			occSecondaryThreshold[1] = src->occSecondaryThreshold[1];
			occupancyMinGap = src->occupancyMinGap;
			output = src->output;
			ant = src->ant;
			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].channelBandwidth= src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].lowFrequency =  src->band[i].lowFrequency;
				band[i].sType.signalType.horizPol = src->band[i].sType.signalType.horizPol;
				band[i].sType.signalType.narrow = src->band[i].sType.signalType.narrow;
				band[i].sType.zeroSType = src->band[i].sType.zeroSType;

			}

			if (src != &v5)
			{
				free(const_cast<SGetOccupancyCmdV5*>(src));
			}

			return *this;
		}

		SGetOccupancyCmdV6& operator =(const SGetAutoViolateCmdV3& avCmd) // Convert from SGetAutoViolateCmdV3
		{
			const SGetAutoViolateCmdV3* src = &avCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&avCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetAutoViolateCmdV3*>(malloc(SizeOf(avCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetAutoViolateCmdV3*>(src), &avCmd, SizeOf(avCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				band[i].sType.signalType.horizPol = src->band[i].sType.signalType.horizPol;
				band[i].sType.signalType.narrow = src->band[i].sType.signalType.narrow;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->avdThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 1;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.zeroFlag = 0;
			saveIntermediateData = false;
			ant = src->ant;

			if (src != &avCmd)
			{
				free(const_cast<SGetAutoViolateCmdV3*>(src));
			}

			return *this;
		};

		SGetOccupancyCmdV6& operator =(const SGetDmCtrlCmdV2& dmCmd) // Convert from SGetDmCtrlCmdV1
		{
			const SGetDmCtrlCmdV2* src = &dmCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&dmCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetDmCtrlCmdV2*>(malloc(SizeOf(dmCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetDmCtrlCmdV2*>(src), &dmCmd, SizeOf(dmCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;
				band[i].sType.signalType = src->band[i].sType.signalType;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->avdThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 1;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.zeroFlag = 0;
			saveIntermediateData = false;
			ant = src->ant;

			if (src != &dmCmd)
			{
				free(const_cast<SGetDmCtrlCmdV2*>(src));
			}

			return *this;
		};


		SGetOccupancyCmdV6& operator =(const SGetScanDfCmdV1& dfCmd) // Convert from SGetScanDfCmd
		{
			const SGetScanDfCmdV1* src = &dfCmd;

			if (static_cast<void*>(this) == static_cast<const void*>(&dfCmd))
			{
				// Make a copy
				if ((src = static_cast<const SGetScanDfCmdV1*>(malloc(SizeOf(dfCmd)))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SGetScanDfCmdV1*>(src), &dfCmd, SizeOf(dfCmd));
			}

			numBands = src->numBands;

			for (size_t i = 0; i < numBands; ++i)
			{
				band[i].lowFrequency = src->band[i].lowFrequency;
				band[i].highFrequency = src->band[i].highFrequency;
				band[i].channelBandwidth = src->band[i].channelBandwidth;
				band[i].exclude = src->band[i].exclude;

				//Clear unused fields to 0.
				band[i].sType.zeroSType = 0;
				band[i].sType.signalType.narrow = src->band[i].signalType.narrow;
				band[i].sType.signalType.horizPol = src->band[i].signalType.horizPol;
			}

			storageTime = src->storageTime;
			durationMethod = TIME;
			measurementTime = src->measurementTime;
			confidenceLevel = 0;
			desiredAccuracy = 0;
			thresholdMethod = NOISE;
			useSecondaryThreshold = false;
			occPrimaryThreshold[1] = src->scanDfThreshold;
			occupancyMinGap = 0;
			output.occupancyVsChannel = 0;
			output.eFieldVsChannel = 0;
			output.occupancyVsTimeOfDay = 0;
			output.msglengthVsChannel = 0;
			output.msglengthDistribution = 0;
			output.spectrogram = 0;
			output.timegram = 0;
			output.occupancyFlag.zeroFlag = 0;
			saveIntermediateData = false;
			ant = ANT1;

			if (src != &dfCmd)
			{
				free(const_cast<SGetScanDfCmdV1*>(src));
			}

			return *this;
		}

		EDurationMethod durationMethod;
		EThresholdMethod thresholdMethod;
		unsigned long storageTime;		// seconds
		unsigned long measurementTime;	// seconds
		unsigned char confidenceLevel;	// percent
		unsigned char desiredAccuracy;	// percent
		bool useSecondaryThreshold;
		bool saveIntermediateData;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1
		//  which thresholds used are based upon thresholdMethod
		//  any unused thresholds do not need to be set (won't be validated)
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		unsigned long occupancyMinGap;	// seconds
		SOccupancyOutputFlagsV1 output;
		EAnt ant;
		unsigned long numBands;		// number of frequency bands
		SBandV5 band[1];	// Variable length. Must be at the end of struct and
		// size of 1 for conversion code & other to work correctly.
	};

*/


	typedef SGetOccupancyCmdV5 SGetOccupancyCmd;
	//typedef SGetOccupancyCmdV6 SGetOccupancyCmd;

	struct SGetDwellCmdV1; // Forward declaration

	struct SGetDwellCmdV0 // GET_DWELL V0
	{
		inline operator SGetDwellCmdV1(void) const; // Convert to V1; defined below

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long freq;
		unsigned long bandwidth;
#else
		uint32_t freq;
		uint32_t bandwidth;
#endif
	};

	struct SGetDwellCmdV1 // GET_DWELL V1
	{
		operator SGetDwellCmdV0(void) const // Convert to V0
		{
			SGetDwellCmdV0 v0;
#ifndef CSMSPLUS_SRV_BUILT
			v0.freq = Units::Frequency(freq).Hz<unsigned long>(true);
			v0.bandwidth = Units::Frequency(bandwidth).Hz<unsigned long>(true);
#else
			v0.freq = Units::Frequency(freq).Hz<uint32_t>(true);
			v0.bandwidth = Units::Frequency(bandwidth).Hz<uint32_t>(true);
#endif
			return v0;
		}

		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
	};

	typedef SGetDwellCmdV1 SGetDwellCmd;

	struct SGetDwellResp // GET_DWELL_RESP
	{
		ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			bwDwellTime;
		unsigned long			freqDwellTime;
		unsigned long			modulationDwellTime;
		unsigned long			fieldStrengthDwellTime;
		unsigned long			dfDwellTime;
#else
		uint32_t			bwDwellTime;
		uint32_t			freqDwellTime;
		uint32_t			modulationDwellTime;
		uint32_t			fieldStrengthDwellTime;
		uint32_t			dfDwellTime;
#endif
	};

	struct SGpsResponse
	{
		struct SGpsStatus
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long satLock	: 1; // = L,U
			unsigned long accuracy	: 2; // = A,B,C,D
			unsigned long mode		: 2; // = T,A,S,D
			unsigned long numSats	: 3; // >7 stored as 7
			unsigned long timSrce	: 2; //= G,F,I,N
			unsigned long pllSynth	: 1; // = -,P
			unsigned long antenna	: 2; //  = -,O,S
			unsigned long receiver	: 1; // = -,R
			unsigned long nvRam		: 1; // = -,N
			unsigned long timErr1	: 1; // = -,U
			unsigned long timErr2	: 1; // = -,U
			unsigned long tracking	: 1; // = -,T
			unsigned long oscVolt	: 1; // = -,X
			unsigned long batVolt	: 1; // = -,B
			unsigned long lockHist	: 2; //  = -,a,A
			unsigned long			: 2;
			unsigned long noGps		: 1;
			unsigned long			: 6;
			unsigned long notTested	: 1;
#else
			uint32_t satLock	: 1; // = L,U
			uint32_t accuracy	: 2; // = A,B,C,D
			uint32_t mode		: 2; // = T,A,S,D
			uint32_t numSats	: 3; // >7 stored as 7
			uint32_t timSrce	: 2; //= G,F,I,N
			uint32_t pllSynth	: 1; // = -,P
			uint32_t antenna	: 2; //  = -,O,S
			uint32_t receiver	: 1; // = -,R
			uint32_t nvRam		: 1; // = -,N
			uint32_t timErr1	: 1; // = -,U
			uint32_t timErr2	: 1; // = -,U
			uint32_t tracking	: 1; // = -,T
			uint32_t oscVolt	: 1; // = -,X
			uint32_t batVolt	: 1; // = -,B
			uint32_t lockHist	: 2; //  = -,a,A
			uint32_t			: 2;
			uint32_t noGps		: 1;
			uint32_t			: 6;
			uint32_t notTested	: 1;
#endif
		};

		SGpsStatus		status;
		double			dateTime;	// using COleTime format
		double			latitude;	// degrees (+ North, - South)
		double			longitude;	// degrees (+ East, - West)
	};

	struct SGetGpsResp // GET_GPS_RESPONSE
	{
		ErrorCodes::EErrorCode	status;
		SGpsResponse			gpsResponse;
	};

	struct SGetHeadingResp // GET_HEADING_RESPONSE
	{
		ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
		long			magDeclination;		// (1/100 degree -> -18000 to +18000)
		bool			hfFluxgateUsed;		// true = fluxgate present & used; false = fluxgate not present or failed
		unsigned long	hfRawMagHeading;	// antenna orientation wrt magnetic north prior to mast up/down correction (1/100 degree -> 0 to 35999)
		unsigned long	hfMagHeading;		// antenna orientation wrt magnetic north (1/100 degree -> 0 to 35999)
		unsigned long	hfTrueHeading;		// antenna orientation wrt true north (1/100 degree -> 0 to 35999)
		bool			vhfFluxgateUsed;	// true = fluxgate present & used; false = fluxgate not present or failed
		unsigned long	vhfRawMagHeading;	// antenna orientation wrt magnetic north prior to mast up/down correction (1/100 degree -> 0 to 35999)
		unsigned long	vhfMagHeading;		// antenna orientation wrt magnetic north (1/100 degree -> 0 to 35999)
		unsigned long	vhfTrueHeading;		// antenna orientation wrt true north (1/100 degree -> 0 to 35999)
#else
		int32_t			magDeclination;		// (1/100 degree -> -18000 to +18000)
		bool			hfFluxgateUsed;		// true = fluxgate present & used; false = fluxgate not present or failed
		uint32_t	hfRawMagHeading;	// antenna orientation wrt magnetic north prior to mast up/down correction (1/100 degree -> 0 to 35999)
		uint32_t	hfMagHeading;		// antenna orientation wrt magnetic north (1/100 degree -> 0 to 35999)
		uint32_t	hfTrueHeading;		// antenna orientation wrt true north (1/100 degree -> 0 to 35999)
		bool			vhfFluxgateUsed;	// true = fluxgate present & used; false = fluxgate not present or failed
		uint32_t	vhfRawMagHeading;	// antenna orientation wrt magnetic north prior to mast up/down correction (1/100 degree -> 0 to 35999)
		uint32_t	vhfMagHeading;		// antenna orientation wrt magnetic north (1/100 degree -> 0 to 35999)
		uint32_t	vhfTrueHeading;		// antenna orientation wrt true north (1/100 degree -> 0 to 35999)
#endif
	};

	struct SGetMeasCmdV1; // Forward declaration

	struct SGetMeasCmdV0 // GET_MEAS V0
	{
		struct SGetDfCmd
		{
#ifndef CSMSPLUS_SRV_BUILT
			enum EOutputType : unsigned long
#else
			enum EOutputType : uint32_t
#endif
			{
				NONE = 0,
				SUMMARY_V = 1,
				CUTS_V = 2, // Implies summary
				VOLTS_V = 3, // Implies summary and cuts
				SUMMARY_H = 4,
				CUTS_H = 5,
				VOLTS_H = 6,
				SUMMARY_VH = 7,
				CUTS_VH = 8,
				VOLTS_VH = 9
			};

#ifndef CSMSPLUS_SRV_BUILT
			enum ESource : unsigned long
#else
			enum ESource : uint32_t
#endif
			{
				TASK_CALENDAR	= 0, // Requesting a DF Measurement from the Task Calendar
				NETTED			= 1, // Requesting a DF Measurement from the Netted DF screen
				HOMING			= 2, // Requesting a DF Measurement from the Homing DF screen
				PUSHBUTTON		= 3  // Requesting a DF Measurement from the Pushbutton DF screen
			};

#ifndef CSMSPLUS_SRV_BUILT
			unsigned long dwellTime;		// maximum DF time for the measurement
			unsigned long repeatCount;		// number of cuts
			unsigned long confThreshold;	// expressed as whole percent
			unsigned long dfBandwidth;		// in Hz
#else
			uint32_t dwellTime;		// maximum DF time for the measurement
			uint32_t repeatCount;		// number of cuts
			uint32_t confThreshold;	// expressed as whole percent
			uint32_t dfBandwidth;		// in Hz
#endif
			EOutputType outputType;
			ESource srcOfRequest;
		};

		inline operator SGetMeasCmdV1(void) const; // Convert to V1; definition below

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long freq;
		unsigned long bandwidth;
#else
		uint32_t freq;
		uint32_t bandwidth;
#endif
		SGetBwCmd bwCmd;
		SGetFreqCmd freqCmd;
		SGetModulationCmd modulationCmd;
		SGetFieldStrengthCmd fieldStrengthCmd;
		SGetDfCmd dfCmd;
	};

	struct SGetMeasCmdV2; // Forward declaration

	struct SGetMeasCmdV1 // GET_MEAS V1
	{
		struct SGetDfCmd
		{
#ifndef CSMSPLUS_SRV_BUILT
			enum EOutputType : unsigned long
#else
			enum EOutputType : uint32_t
#endif
			{
				NONE = 0,
				SUMMARY_V = 1,
				CUTS_V = 2, // Implies summary
				VOLTS_V = 3, // Implies summary and cuts
				SUMMARY_H = 4,
				CUTS_H = 5,
				VOLTS_H = 6,
				SUMMARY_VH = 7,
				CUTS_VH = 8,
				VOLTS_VH = 9
			};

#ifndef CSMSPLUS_SRV_BUILT
			enum ESource : unsigned long
#else
			enum ESource : uint32_t
#endif
			{
				TASK_CALENDAR	= 0, // Requesting a DF Measurement from the Task Calendar
				NETTED			= 1, // Requesting a DF Measurement from the Netted DF screen
				HOMING			= 2, // Requesting a DF Measurement from the Homing DF screen
				PUSHBUTTON		= 3  // Requesting a DF Measurement from the Pushbutton DF screen
			};

#ifndef CSMSPLUS_SRV_BUILT
			unsigned long dwellTime;		// maximum DF time for the measurement
			unsigned long repeatCount;		// number of cuts
			unsigned long confThreshold;	// expressed as whole percent
			unsigned long dfBandwidth;		// in Hz
#else
			uint32_t dwellTime;		// maximum DF time for the measurement
			uint32_t repeatCount;		// number of cuts
			uint32_t confThreshold;	// expressed as whole percent
			uint32_t dfBandwidth;		// in Hz
#endif
			EOutputType outputType;
			ESource srcOfRequest;
		};

		operator SGetMeasCmdV0(void) const // Convert to V0
		{
			SGetMeasCmdV0 v0;
			v0.freq = freq;
			v0.bandwidth = bandwidth;
			v0.bwCmd = bwCmd;
			v0.freqCmd = freqCmd;
			v0.modulationCmd = modulationCmd;
			v0.fieldStrengthCmd = fieldStrengthCmd;
			memcpy(&v0.dfCmd, &dfCmd, sizeof(v0.dfCmd));

			return v0;
		}

		inline operator SGetMeasCmdV2(void) const; // Convert to V2; definition below

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long freq;
		unsigned long bandwidth;
#else
		uint32_t freq;
		uint32_t bandwidth;
#endif
		EAnt ant;
		SGetBwCmd bwCmd;
		SGetFreqCmd freqCmd;
		SGetModulationCmd modulationCmd;
		SGetFieldStrengthCmd fieldStrengthCmd;
		SGetDfCmd dfCmd;
	};

	struct SGetMeasCmdV3; // Forward declaration

	struct SGetMeasCmdV2 // GET_MEAS V2
	{
		struct SGetDfCmd
		{
#ifndef CSMSPLUS_SRV_BUILT
			enum EOutputType : unsigned long
#else
			enum EOutputType : uint32_t
#endif
			{
				NONE = 0,
				SUMMARY_V = 1,
				CUTS_V = 2, // Implies summary
				VOLTS_V = 3, // Implies summary and cuts
				SUMMARY_H = 4,
				CUTS_H = 5,
				VOLTS_H = 6,
				SUMMARY_VH = 7,
				CUTS_VH = 8,
				VOLTS_VH = 9
			};

#ifndef CSMSPLUS_SRV_BUILT
			enum ESource : unsigned long
#else
			enum ESource : uint32_t
#endif
			{
				TASK_CALENDAR	= 0, // Requesting a DF Measurement from the Task Calendar
				NETTED			= 1, // Requesting a DF Measurement from the Netted DF screen
				HOMING			= 2, // Requesting a DF Measurement from the Homing DF screen
				PUSHBUTTON		= 3  // Requesting a DF Measurement from the Pushbutton DF screen
			};

#ifndef CSMSPLUS_SRV_BUILT
			unsigned long dwellTime;		// maximum DF time for the measurement
			unsigned long repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			unsigned char confThreshold;	// expressed as whole percent
#else
			uint32_t dwellTime;		// maximum DF time for the measurement
			uint32_t repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			uint8_t confThreshold;	// expressed as whole percent
#endif
		};

		operator SGetMeasCmdV1(void) const // Convert to V1
		{
			SGetMeasCmdV1 v1;
#ifndef CSMSPLUS_SRV_BUILT
			v1.freq = Units::Frequency(freq).Hz<unsigned long>(true);
			v1.bandwidth = Units::Frequency(bandwidth).Hz<unsigned long>(true);
#else
			v1.freq = Units::Frequency(freq).Hz<uint32_t>(true);
			v1.bandwidth = Units::Frequency(bandwidth).Hz<uint32_t>(true);
#endif
			v1.bwCmd = bwCmd;
			v1.freqCmd = freqCmd;
			v1.ant = ant;
			v1.modulationCmd = modulationCmd;
			v1.fieldStrengthCmd = fieldStrengthCmd;
			v1.dfCmd.dwellTime = dfCmd.dwellTime;
			v1.dfCmd.repeatCount = dfCmd.repeatCount;
			v1.dfCmd.confThreshold = dfCmd.confThreshold;
#ifndef CSMSPLUS_SRV_BUILT
			v1.dfCmd.dfBandwidth = Units::Frequency(dfCmd.dfBandwidth).Hz<unsigned long>(true);
#else
			v1.dfCmd.dfBandwidth = Units::Frequency(dfCmd.dfBandwidth).Hz<uint32_t>(true);
#endif
			v1.dfCmd.outputType = static_cast<SGetMeasCmdV1::SGetDfCmd::EOutputType>(dfCmd.outputType);
			v1.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV1::SGetDfCmd::ESource>(dfCmd.srcOfRequest);

			return v1;
		}

		inline operator SGetMeasCmdV3(void) const; // Convert to V3; definition below

		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
		EAnt ant;
		SGetBwCmd bwCmd;
		SGetFreqCmd freqCmd;
		SGetModulationCmd modulationCmd;
		SGetFieldStrengthCmd fieldStrengthCmd;
		SGetDfCmd dfCmd;
	};

	struct SGetMeasCmdV4; // Forward declaration

	struct SGetMeasCmdV3 // GET_MEAS V3
	{
		struct SGetDfCmd
		{
#ifndef CSMSPLUS_SRV_BUILT
			enum EOutputType : unsigned long
#else
			enum EOutputType : uint32_t
#endif
			{
				NONE = 0,
				SUMMARY_V = 1,
				CUTS_V = 2, // Implies summary
				VOLTS_V = 3, // Implies summary and cuts
				SUMMARY_H = 4,
				CUTS_H = 5,
				VOLTS_H = 6,
				SUMMARY_VH = 7,
				CUTS_VH = 8,
				VOLTS_VH = 9
			};

#ifndef CSMSPLUS_SRV_BUILT
			enum ESource : unsigned long
#else
			enum ESource : uint32_t
#endif
			{
				TASK_CALENDAR	= 0, // Requesting a DF Measurement from the Task Calendar
				NETTED			= 1, // Requesting a DF Measurement from the Netted DF screen
				HOMING			= 2, // Requesting a DF Measurement from the Homing DF screen
				PUSHBUTTON		= 3, // Requesting a DF Measurement from the Pushbutton DF screen
				TDOA			= 4  // Requesting a DF Measurement from the TDOA screen
			};

#ifndef CSMSPLUS_SRV_BUILT
			unsigned long dwellTime;		// maximum DF time for the measurement
			unsigned long repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			unsigned char confThreshold;	// expressed as whole percent
#else
			uint32_t dwellTime;		// maximum DF time for the measurement
			uint32_t repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			uint8_t confThreshold;	// expressed as whole percent
#endif
		};

		struct SGetIQCmd
		{
			bool tdoa;	// set to denote that accurate start time and location is needed
			float bwFactor; // adjusts bandwidth (& sampleRate) of IQ data
							// bandwidth = bwFactor * SGetMeasCmd::bandwidth
							// sampleRate = bwFactor * SGetMeasCmd::bandwidth * 1.25
							// valid range is 0.1 to 16.0
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long numSamples; // valid range is 32 to 1000000
#else
			uint32_t numSamples; // valid range is 32 to 1000000
#endif
			EOutputType outputType; // NONE = no measurement; SUMMARY or DETAILS = measurement
		};

		operator SGetMeasCmdV2(void) const // Convert to V2
		{
			SGetMeasCmdV2 v2;
			v2.freq = freq;
			v2.bandwidth = bandwidth;
			v2.ant = ant;
			v2.bwCmd = bwCmd;
			v2.freqCmd = freqCmd;
			v2.modulationCmd = modulationCmd;
			v2.fieldStrengthCmd = fieldStrengthCmd;
			v2.dfCmd.dwellTime = dfCmd.dwellTime;
			v2.dfCmd.repeatCount = dfCmd.repeatCount;
			v2.dfCmd.dfBandwidth = dfCmd.dfBandwidth;
			v2.dfCmd.confThreshold = dfCmd.confThreshold;
			v2.dfCmd.outputType = static_cast<SGetMeasCmdV2::SGetDfCmd::EOutputType>(dfCmd.outputType);
			if(dfCmd.srcOfRequest == SGetDfCmd::TDOA)
			{
				v2.dfCmd.srcOfRequest = SGetMeasCmdV2::SGetDfCmd::NETTED;
			}
			else
			{
				v2.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV2::SGetDfCmd::ESource>(dfCmd.srcOfRequest);
			}

			return v2;
		}

		inline operator SGetMeasCmdV4(void) const; // Convert to V4; definition below

		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
		EAnt ant;
		SGetBwCmd bwCmd;
		SGetFreqCmd freqCmd;
		SGetModulationCmd modulationCmd;
		SGetFieldStrengthCmd fieldStrengthCmd;
		SGetDfCmd dfCmd;
		SGetIQCmd iqCmd;
	};

	struct SGetMeasCmdV5; // Forward declaration

	struct SGetMeasCmdV4 // GET_MEAS V4
	{
		struct SGetDfCmd
		{
#ifndef CSMSPLUS_SRV_BUILT
			enum EOutputType : unsigned long
#else
			enum EOutputType : uint32_t
#endif
			{
				NONE = 0,
				SUMMARY_V = 1,
				CUTS_V = 2, // Implies summary
				VOLTS_V = 3, // Implies summary and cuts
				SUMMARY_H = 4,
				CUTS_H = 5,
				VOLTS_H = 6,
				SUMMARY_VH = 7,
				CUTS_VH = 8,
				VOLTS_VH = 9
			};

#ifndef CSMSPLUS_SRV_BUILT
			enum ESource : unsigned long
#else
			enum ESource : uint32_t
#endif
			{
				TASK_CALENDAR	= 0, // Requesting a DF Measurement from the Task Calendar
				NETTED			= 1, // Requesting a DF Measurement from the Netted DF screen
				HOMING			= 2, // Requesting a DF Measurement from the Homing DF screen
				PUSHBUTTON		= 3, // Requesting a DF Measurement from the Pushbutton DF screen
				TDOA			= 4  // Requesting a DF Measurement from the TDOA screen
			};

#ifndef CSMSPLUS_SRV_BUILT
			unsigned long dwellTime;		// maximum DF time for the measurement
			unsigned long repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			unsigned char confThreshold;	// expressed as whole percent
#else
			uint32_t dwellTime;		// maximum DF time for the measurement
			uint32_t repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			uint8_t confThreshold;	// expressed as whole percent
#endif
		};

		struct SGetIQCmd
		{
			bool tdoa;	// set to denote that accurate start time and location is needed
			float bwFactor; // adjusts bandwidth (& sampleRate) of IQ data
							// bandwidth = bwFactor * SGetMeasCmd::bandwidth
							// sampleRate = bwFactor * SGetMeasCmd::bandwidth * 1.25
							// valid range is 0.1 to 16.0
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long numSamples; // valid range is 32 to 1000000 (if tdoa set, numSamples s/b power of 2)
#else
			uint32_t numSamples; // valid range is 32 to 1000000 (if tdoa set, numSamples s/b power of 2)
#endif
			DATE startTime; // internal server use only; input ignored (can be anything)
			EOutputType outputType; // NONE = no measurement; SUMMARY or DETAILS = measurement
		};

		operator SGetMeasCmdV3(void) const // Convert to V3
		{
			SGetMeasCmdV3 v3;
			v3.freq = freq;
			v3.bandwidth = bandwidth;
			v3.ant = ant;
			v3.bwCmd = bwCmd;
			v3.freqCmd = freqCmd;
			v3.modulationCmd = modulationCmd;
			v3.fieldStrengthCmd = fieldStrengthCmd;
			v3.dfCmd.dwellTime = dfCmd.dwellTime;
			v3.dfCmd.repeatCount = dfCmd.repeatCount;
			v3.dfCmd.dfBandwidth = dfCmd.dfBandwidth;
			v3.dfCmd.confThreshold = dfCmd.confThreshold;
			v3.dfCmd.outputType = static_cast<SGetMeasCmdV3::SGetDfCmd::EOutputType>(dfCmd.outputType);
			if(dfCmd.srcOfRequest == SGetDfCmd::TDOA)
			{
				v3.dfCmd.srcOfRequest = SGetMeasCmdV3::SGetDfCmd::NETTED;
			}
			else
			{
				v3.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV3::SGetDfCmd::ESource>(dfCmd.srcOfRequest);
			}
			v3.iqCmd.bwFactor = iqCmd.bwFactor;
			v3.iqCmd.numSamples = iqCmd.numSamples;
			v3.iqCmd.outputType = iqCmd.outputType;
			v3.iqCmd.tdoa = iqCmd.tdoa;

			return v3;
		}

		inline operator SGetMeasCmdV5(void) const; // Convert to V5; definition below

		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
		EAnt ant;
		SGetBwCmd bwCmd;
		SGetFreqCmd freqCmd;
		SGetModulationCmd modulationCmd;
		SGetFieldStrengthCmd fieldStrengthCmd;
		SGetDfCmd dfCmd;
		SGetIQCmd iqCmd;
	};

	struct SGetMeasCmdV5 // GET_MEAS V5
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned char AGC = 255;
		static const unsigned char AGC_DEFAULT_GAIN_MODE = 255;
		static const unsigned char AGC_NORMAL_GAIN_MODE = 254;
		static const unsigned char AGC_RURAL_GAIN_MODE = 253;
		static const unsigned char AGC_URBAN_GAIN_MODE = 252;
		static const unsigned char AGC_CONGESTED_GAIN_MODE = 251;
#else
		static const uint8_t AGC = 255;
		static const uint8_t AGC_DEFAULT_GAIN_MODE = 255;
		static const uint8_t AGC_NORMAL_GAIN_MODE = 254;
		static const uint8_t AGC_RURAL_GAIN_MODE = 253;
		static const uint8_t AGC_URBAN_GAIN_MODE = 252;
		static const uint8_t AGC_CONGESTED_GAIN_MODE = 251;
#endif

		struct SGetDfCmd
		{
#ifndef CSMSPLUS_SRV_BUILT
			enum EOutputType : unsigned long
#else
			enum EOutputType : uint32_t
#endif
			{
				NONE = 0,
				SUMMARY_V = 1,
				CUTS_V = 2, // Implies summary
				VOLTS_V = 3, // Implies summary and cuts
				SUMMARY_H = 4,
				CUTS_H = 5,
				VOLTS_H = 6,
				SUMMARY_VH = 7,
				CUTS_VH = 8,
				VOLTS_VH = 9
			};

#ifndef CSMSPLUS_SRV_BUILT
			enum ESource : unsigned long
#else
			enum ESource : uint32_t
#endif
			{
				TASK_CALENDAR	= 0, // Requesting a DF Measurement from the Task Calendar
				NETTED			= 1, // Requesting a DF Measurement from the Netted DF screen
				HOMING			= 2, // Requesting a DF Measurement from the Homing DF screen
				PUSHBUTTON		= 3, // Requesting a DF Measurement from the Pushbutton DF screen
				TDOA			= 4  // Requesting a DF Measurement from the TDOA screen
			};

#ifndef CSMSPLUS_SRV_BUILT
			unsigned long dwellTime;		// maximum DF time for the measurement
			unsigned long repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			unsigned char confThreshold;	// expressed as whole percent
#else
			uint32_t dwellTime;		// maximum DF time for the measurement
			uint32_t repeatCount;		// number of cuts
			Units::Frequency::Raw dfBandwidth;		// in Hz
			EOutputType outputType;
			ESource srcOfRequest;
			uint8_t confThreshold;	// expressed as whole percent
#endif
		};

		struct SGetIQCmd
		{
			bool tdoa;	// set to denote that accurate start time and location is needed
			float bwFactor; // adjusts bandwidth (& sampleRate) of IQ data
							// bandwidth = bwFactor * SGetMeasCmd::bandwidth
							// sampleRate = bwFactor * SGetMeasCmd::bandwidth * 1.25
							// valid range is 0.1 to 16.0
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long numSamples; // valid range is 32 to 1000000 (if tdoa set, numSamples s/b power of 2)
#else
			uint32_t numSamples; // valid range is 32 to 1000000 (if tdoa set, numSamples s/b power of 2)
#endif
			DATE startTime; // internal server use only; input ignored (can be anything)
			EOutputType outputType; // NONE = no measurement; SUMMARY or DETAILS = measurement
		};

		operator SGetMeasCmdV4(void) const // Convert to V4
		{
			SGetMeasCmdV4 v4;
			v4.freq = freq;
			v4.bandwidth = bandwidth;
			v4.ant = ant;
			v4.bwCmd = bwCmd;
			v4.freqCmd = freqCmd;
			v4.modulationCmd = modulationCmd;
			v4.fieldStrengthCmd = fieldStrengthCmd;
			v4.dfCmd.dwellTime = dfCmd.dwellTime;
			v4.dfCmd.repeatCount = dfCmd.repeatCount;
			v4.dfCmd.dfBandwidth = dfCmd.dfBandwidth;
			v4.dfCmd.confThreshold = dfCmd.confThreshold;
			v4.dfCmd.outputType = static_cast<SGetMeasCmdV4::SGetDfCmd::EOutputType>(dfCmd.outputType);

			if(dfCmd.srcOfRequest == SGetDfCmd::TDOA)
			{
				v4.dfCmd.srcOfRequest = SGetMeasCmdV4::SGetDfCmd::NETTED;
			}
			else
			{
				v4.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV4::SGetDfCmd::ESource>(dfCmd.srcOfRequest);
			}

			v4.iqCmd.bwFactor = iqCmd.bwFactor;
			v4.iqCmd.numSamples = iqCmd.numSamples;
			v4.iqCmd.outputType = iqCmd.outputType;
			v4.iqCmd.tdoa = iqCmd.tdoa;

			return v4;
		}

		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
		EAnt ant;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char rcvrAtten; // 255 => AGC
#else
		uint8_t rcvrAtten; // 255 => AGC
#endif
		SGetBwCmd bwCmd;
		SGetFreqCmd freqCmd;
		SGetModulationCmd modulationCmd;
		SGetFieldStrengthCmd fieldStrengthCmd;
		SGetDfCmd dfCmd;
		SGetIQCmd iqCmd;
	};

	typedef SGetMeasCmdV5 SGetMeasCmd;

	struct SGetMeasRespV1; // Forward declaration

	struct SGetMeasRespV0 // GET_MEAS_RESPONSE V0
	{
		struct SBwResponse
		{
			ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			unsigned long	betaBw; // 1/10 Hz
			unsigned long	betaFreqLow;
			unsigned long	betaFreqHigh;
			unsigned long	betaStdDev;
			unsigned long	x1Bw; // 1/10 Hz
			unsigned long	x1FreqLow;
			unsigned long	x1FreqHigh;
			unsigned long	x1StdDev;
			unsigned long	x2Bw; // 1/10 Hz
			unsigned long	x2FreqLow;
			unsigned long	x2FreqHigh; 
			unsigned long	x2StdDev;
			unsigned long	freq;
			unsigned long	binSize; // 1/10000 Hz
			unsigned long	numBins;
#else
			uint32_t	repeatCount;
			uint32_t	betaBw; // 1/10 Hz
			uint32_t	betaFreqLow;
			uint32_t	betaFreqHigh;
			uint32_t	betaStdDev;
			uint32_t	x1Bw; // 1/10 Hz
			uint32_t	x1FreqLow;
			uint32_t	x1FreqHigh;
			uint32_t	x1StdDev;
			uint32_t	x2Bw; // 1/10 Hz
			uint32_t	x2FreqLow;
			uint32_t	x2FreqHigh; 
			uint32_t	x2StdDev;
			uint32_t	freq;
			uint32_t	binSize; // 1/10000 Hz
			uint32_t	numBins;
#endif
		};

		struct SDfCutData
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long azim;
			unsigned long elev;
			long conf;
			unsigned long range;
			unsigned long ms;
#else
			uint32_t azim;
			uint32_t elev;
			int32_t conf;
			uint32_t range;
			uint32_t ms;
#endif
		};

		struct SDfSummaryResponse
		{
			ErrorCodes::EErrorCode status;
			ErrorCodes::EErrorCode fluxgateStatus;		
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long headingCorrection;   // degrees * 100 (0 to 35999)
			unsigned long numSumCuts;
			unsigned long azim;   // degrees * 100 (0 to 35999)
			unsigned long elev;   // degrees * 100 (0 to 9000)
			unsigned long conf;   // * 100 (0 to 100)
			unsigned long range;  // kilometers * 100 (0 to ???)
			unsigned long stdDev; // degrees * 100 (0 to ???)
			long snr;    // dB * 100 (??? to ???)
			long powerDbm; // dBm * 100 (??? to ???)
			unsigned long numAllCuts;
			unsigned long numAnts;	// needed to understand volts output
#else
			uint32_t headingCorrection;   // degrees * 100 (0 to 35999)
			uint32_t numSumCuts;
			uint32_t azim;   // degrees * 100 (0 to 35999)
			uint32_t elev;   // degrees * 100 (0 to 9000)
			uint32_t conf;   // * 100 (0 to 100)
			uint32_t range;  // kilometers * 100 (0 to ???)
			uint32_t stdDev; // degrees * 100 (0 to ???)
			int32_t snr;    // dB * 100 (??? to ???)
			int32_t powerDbm; // dBm * 100 (??? to ???)
			uint32_t numAllCuts;
			uint32_t numAnts;	// needed to understand volts output
#endif
		};

		struct SDfVoltageData
		{
#ifndef CSMSPLUS_SRV_BUILT
			static const unsigned long NUMANTS_MAX = 9;

			long volts[2 * NUMANTS_MAX];
			long powerDbm[2];
			unsigned long antNum[2];
#else
			static const uint32_t NUMANTS_MAX = 9;

			int32_t volts[2 * NUMANTS_MAX];
			int32_t powerDbm[2];
			uint32_t antNum[2];
#endif
		};

		struct SFieldStrengthResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			EFieldMethod	fieldMethod;
			long			eField;
			long			pFlux;
			long			stdDev;
			long			powerDbm;
			long			rxPowerDbm;
#else
			uint32_t	repeatCount;
			EFieldMethod	fieldMethod;
			int32_t			eField;
			int32_t			pFlux;
			int32_t			stdDev;
			int32_t			powerDbm;
			int32_t			rxPowerDbm;
#endif
		};

		struct SFreqResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			long			freqOffset;
			unsigned long	freq;
			unsigned long	stdDev;
			unsigned long	confidence;
#else
			uint32_t	repeatCount;
			int32_t			freqOffset;
			uint32_t	freq;
			uint32_t	stdDev;
			uint32_t	confidence;
#endif
		};

		struct SModulationResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			unsigned long	amModPlus;
			unsigned long	amModMinus;
			unsigned long	amModDepth;
			unsigned long	fmPeakFreq;
			unsigned long	fmRmsFreq;
			unsigned long	pmPeakPhase;
			unsigned long	pmRmsPhase;
			unsigned long	numAmHistBins;
			unsigned long	numFmHistBins;
			unsigned long	numPmHistBins;
			unsigned long	amHistBinsize;
			unsigned long	fmHistBinsize;
			unsigned long	pmHistBinsize;
#else
			uint32_t	repeatCount;
			uint32_t	amModPlus;
			uint32_t	amModMinus;
			uint32_t	amModDepth;
			uint32_t	fmPeakFreq;
			uint32_t	fmRmsFreq;
			uint32_t	pmPeakPhase;
			uint32_t	pmRmsPhase;
			uint32_t	numAmHistBins;
			uint32_t	numFmHistBins;
			uint32_t	numPmHistBins;
			uint32_t	amHistBinsize;
			uint32_t	fmHistBinsize;
			uint32_t	pmHistBinsize;
#endif
		};	

		inline operator SGetMeasRespV1(void) const; // Convert to V1; definition below

		SGpsResponse	gpsResponse;
		SBwResponse		bwResponse;
		SFreqResponse	freqResponse;
		SModulationResponse	modulationResponse;
		SFieldStrengthResponse fieldStrengthResponse;
		SDfSummaryResponse dfResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char	bwBinData[BWBINS_MAX];
		unsigned char	amProbDist[NUMHISTBINS_MAX];
		unsigned char	fmProbDist[NUMHISTBINS_MAX];
		unsigned char	pmProbDist[NUMHISTBINS_MAX];
#else
		uint8_t	bwBinData[BWBINS_MAX];
		uint8_t	amProbDist[NUMHISTBINS_MAX];
		uint8_t	fmProbDist[NUMHISTBINS_MAX];
		uint8_t	pmProbDist[NUMHISTBINS_MAX];
#endif
		SDfCutData		dfCutData[DFCUTS_MAX];
		SDfVoltageData	dfVoltageData[DFVOLTS_MAX];
	};

	struct SGetMeasRespV2; // Forward declaration

	struct SGetMeasRespV1 // GET_MEAS_RESPONSE V1
	{
		struct SBwResponse
		{
			ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			unsigned long	betaBw; // 1/10 Hz
			unsigned long	betaFreqLow;
			unsigned long	betaFreqHigh;
			unsigned long	betaStdDev;
			unsigned long	x1Bw; // 1/10 Hz
			unsigned long	x1FreqLow;
			unsigned long	x1FreqHigh;
			unsigned long	x1StdDev;
			unsigned long	x2Bw; // 1/10 Hz
			unsigned long	x2FreqLow;
			unsigned long	x2FreqHigh; 
			unsigned long	x2StdDev;
			unsigned long	freq;
			unsigned long	binSize; // 1/10000 Hz
			unsigned long	numBins;
#else
			uint32_t	repeatCount;
			uint32_t	betaBw; // 1/10 Hz
			uint32_t	betaFreqLow;
			uint32_t	betaFreqHigh;
			uint32_t	betaStdDev;
			uint32_t	x1Bw; // 1/10 Hz
			uint32_t	x1FreqLow;
			uint32_t	x1FreqHigh;
			uint32_t	x1StdDev;
			uint32_t	x2Bw; // 1/10 Hz
			uint32_t	x2FreqLow;
			uint32_t	x2FreqHigh; 
			uint32_t	x2StdDev;
			uint32_t	freq;
			uint32_t	binSize; // 1/10000 Hz
			uint32_t	numBins;
#endif
		};

		struct SDfResponse
		{
			struct SDfCutData
			{
#ifndef CSMSPLUS_SRV_BUILT
				unsigned long ms;
				unsigned long range;
				unsigned short azim;
				unsigned short elev;
				short conf;
				short refPowerDbm;
				short maxSamplePowerDbm;
				unsigned char maxSamplePowerIdx;
#else
				uint32_t ms;
				uint32_t range;
				uint16_t azim;
				uint16_t elev;
				int16_t conf;
				int16_t refPowerDbm;
				int16_t maxSamplePowerDbm;
				uint8_t maxSamplePowerIdx;
#endif
			};

			struct SDfSummaryResponse
			{
				ErrorCodes::EErrorCode status;
				ErrorCodes::EErrorCode fluxgateStatus;		
#ifndef CSMSPLUS_SRV_BUILT
				unsigned long headingCorrection;   // degrees * 100 (0 to 35999)
				unsigned long numSumCuts;
				unsigned long azim;   // degrees * 100 (0 to 35999)
				unsigned long elev;   // degrees * 100 (0 to 9000)
				unsigned long conf;   // * 100 (0 to 100)
				unsigned long range;  // kilometers * 100 (0 to ???)
				unsigned long stdDev; // degrees * 100 (0 to ???)
				long snr;    // dB * 100 (??? to ???)
				long powerDbm; // dBm * 100 (??? to ???)
				unsigned long numAllCuts;
				///@note: numAnts specifies the valid volts valued in
				///			SDfVoltageData below.
				unsigned long numAnts;	// needed to understand volts output
#else
				uint32_t headingCorrection;   // degrees * 100 (0 to 35999)
				uint32_t numSumCuts;
				uint32_t azim;   // degrees * 100 (0 to 35999)
				uint32_t elev;   // degrees * 100 (0 to 9000)
				uint32_t conf;   // * 100 (0 to 100)
				uint32_t range;  // kilometers * 100 (0 to ???)
				uint32_t stdDev; // degrees * 100 (0 to ???)
				int32_t snr;    // dB * 100 (??? to ???)
				int32_t powerDbm; // dBm * 100 (??? to ???)
				uint32_t numAllCuts;
				///@note: numAnts specifies the valid volts valued in
				///			SDfVoltageData below.
				uint32_t numAnts;	// needed to understand volts output
#endif
			};

			struct SDfVoltageData
			{
#ifndef CSMSPLUS_SRV_BUILT
				static const unsigned long NUMANTS_MAX = 9;

				long volts[2 * NUMANTS_MAX];
				long powerDbm[2];
				unsigned long antNum[2];
#else
				static const uint32_t NUMANTS_MAX = 9;

				int32_t volts[2 * NUMANTS_MAX];
				int32_t powerDbm[2];
				uint32_t antNum[2];
#endif
			};

			SDfSummaryResponse dfResponse;
			SDfCutData		dfCutData[DFCUTS_MAX];
			SDfVoltageData	dfVoltageData[DFVOLTS_MAX];
		};

		struct SFieldStrengthResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			EFieldMethod	fieldMethod;
			long			eField;
			long			pFlux;
			long			stdDev;
			long			powerDbm;
			long			rxPowerDbm;
#else
			uint32_t	repeatCount;
			EFieldMethod	fieldMethod;
			int32_t			eField;
			int32_t			pFlux;
			int32_t			stdDev;
			int32_t			powerDbm;
			int32_t			rxPowerDbm;
#endif
		};

		struct SFreqResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			long			freqOffset;
			unsigned long	freq;
			unsigned long	stdDev;
			unsigned long	confidence;
#else
			uint32_t	repeatCount;
			int32_t			freqOffset;
			uint32_t	freq;
			uint32_t	stdDev;
			uint32_t	confidence;
#endif
		};

		struct SModulationResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
			unsigned long	amModPlus;
			unsigned long	amModMinus;
			unsigned long	amModDepth;
			unsigned long	fmPeakFreq;
			unsigned long	fmRmsFreq;
			unsigned long	pmPeakPhase;
			unsigned long	pmRmsPhase;
			unsigned long	numAmHistBins;
			unsigned long	numFmHistBins;
			unsigned long	numPmHistBins;
			unsigned long	amHistBinsize;
			unsigned long	fmHistBinsize;
			unsigned long	pmHistBinsize;
#else
			uint32_t	repeatCount;
			uint32_t	amModPlus;
			uint32_t	amModMinus;
			uint32_t	amModDepth;
			uint32_t	fmPeakFreq;
			uint32_t	fmRmsFreq;
			uint32_t	pmPeakPhase;
			uint32_t	pmRmsPhase;
			uint32_t	numAmHistBins;
			uint32_t	numFmHistBins;
			uint32_t	numPmHistBins;
			uint32_t	amHistBinsize;
			uint32_t	fmHistBinsize;
			uint32_t	pmHistBinsize;
#endif
		};	

		operator SGetMeasRespV0(void) const // Convert to V0
		{
			SGetMeasRespV0 v0;
			v0.gpsResponse = gpsResponse;
			memcpy(&v0.bwResponse, &bwResponse, sizeof(v0.bwResponse));
			memcpy(&v0.freqResponse, &freqResponse, sizeof(v0.freqResponse));
			memcpy(&v0.modulationResponse, &modulationResponse, sizeof(v0.modulationResponse));
			memcpy(&v0.fieldStrengthResponse, &fieldStrengthResponse, sizeof(v0.fieldStrengthResponse));
			memcpy(&v0.dfResponse, &dfResponse[0].dfResponse, sizeof(v0.dfResponse));
			memcpy(v0.bwBinData, bwBinData, sizeof(v0.bwBinData));
			memcpy(v0.amProbDist, amProbDist, sizeof(v0.amProbDist));
			memcpy(v0.fmProbDist, fmProbDist, sizeof(v0.fmProbDist));
			memcpy(v0.pmProbDist, pmProbDist, sizeof(v0.pmProbDist));
			memcpy(v0.dfVoltageData, dfResponse[0].dfVoltageData, sizeof(v0.dfVoltageData));

			for(size_t cut = 0; cut < DFCUTS_MAX; ++cut)
			{
				v0.dfCutData[cut].azim = dfResponse[0].dfCutData[cut].azim;
				v0.dfCutData[cut].elev = dfResponse[0].dfCutData[cut].elev;
				v0.dfCutData[cut].conf = dfResponse[0].dfCutData[cut].conf;
				v0.dfCutData[cut].range = dfResponse[0].dfCutData[cut].range;
				v0.dfCutData[cut].ms = dfResponse[0].dfCutData[cut].ms;
			}

			return v0;
		}

		inline operator SGetMeasRespV2(void) const; // Convert to V2; definition below

		SGpsResponse	gpsResponse;
		SBwResponse		bwResponse;
		SFreqResponse	freqResponse;
		SModulationResponse	modulationResponse;
		SFieldStrengthResponse fieldStrengthResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char	bwBinData[BWBINS_MAX];
		unsigned char	amProbDist[NUMHISTBINS_MAX];
		unsigned char	fmProbDist[NUMHISTBINS_MAX];
		unsigned char	pmProbDist[NUMHISTBINS_MAX];
#else
		uint8_t	bwBinData[BWBINS_MAX];
		uint8_t	amProbDist[NUMHISTBINS_MAX];
		uint8_t	fmProbDist[NUMHISTBINS_MAX];
		uint8_t	pmProbDist[NUMHISTBINS_MAX];
#endif
		SDfResponse		dfResponse[2]; // V, H
	};

	struct SGetMeasRespV3; // Forward declaration

	struct SGetMeasRespV2 // GET_MEAS_RESPONSE V2
	{
		operator SGetMeasRespV1(void) const // Convert to V1
		{
			SGetMeasRespV1 v1;
			v1.gpsResponse = gpsResponse;
			v1.bwResponse.status = bwResponse.status;
			v1.bwResponse.repeatCount = bwResponse.repeatCount;
#ifndef CSMSPLUS_SRV_BUILT
			v1.bwResponse.betaBw = static_cast<unsigned long>(bwResponse.betaBw * 10 + 0.5);
			v1.bwResponse.betaFreqLow = static_cast<unsigned long>(bwResponse.betaFreqLow + 0.5);
			v1.bwResponse.betaFreqHigh = static_cast<unsigned long>(bwResponse.betaFreqHigh + 0.5);
			v1.bwResponse.betaStdDev = static_cast<unsigned long>(bwResponse.betaStdDev + 0.5f);
			v1.bwResponse.x1Bw = static_cast<unsigned long>(bwResponse.x1Bw * 10 + 0.5);
			v1.bwResponse.x1FreqLow = static_cast<unsigned long>(bwResponse.x1FreqLow + 0.5);
			v1.bwResponse.x1FreqHigh = static_cast<unsigned long>(bwResponse.x1FreqHigh + 0.5);
			v1.bwResponse.x1StdDev = static_cast<unsigned long>(bwResponse.x1StdDev + 0.5f);
			v1.bwResponse.x2Bw = static_cast<unsigned long>(bwResponse.x2Bw * 10 + 0.5);
			v1.bwResponse.x2FreqLow = static_cast<unsigned long>(bwResponse.x2FreqLow + 0.5);
			v1.bwResponse.x2FreqHigh = static_cast<unsigned long>(bwResponse.x2FreqHigh + 0.5);
			v1.bwResponse.x2StdDev = static_cast<unsigned long>(bwResponse.x2StdDev + 0.5f);
			v1.bwResponse.freq = Units::Frequency(bwResponse.freq).Hz<unsigned long>(true);
			v1.bwResponse.binSize = static_cast<unsigned long>(Units::Frequency(bwResponse.binSize).Hz<double>() * 10000 + 0.5);
			v1.bwResponse.numBins = bwResponse.numBins;
			v1.freqResponse.status = freqResponse.status;
			v1.freqResponse.repeatCount = freqResponse.repeatCount;
			v1.freqResponse.freqOffset = long(floor(freqResponse.freqOffset + 0.5f));
			v1.freqResponse.freq = static_cast<unsigned long>(freqResponse.freq + 0.5);
			v1.freqResponse.stdDev = static_cast<unsigned long>(freqResponse.stdDev + 0.5f);
			v1.freqResponse.confidence = 900;
			v1.modulationResponse.status = modulationResponse.status;
			v1.modulationResponse.repeatCount = modulationResponse.repeatCount;
			v1.modulationResponse.amModPlus = static_cast<unsigned long>(modulationResponse.amModPlus * 1000 + 0.5f);
			v1.modulationResponse.amModMinus = static_cast<unsigned long>(modulationResponse.amModMinus * 1000 + 0.5f);
			v1.modulationResponse.amModDepth = static_cast<unsigned long>(modulationResponse.amModDepth * 1000 + 0.5f);
			v1.modulationResponse.fmPeakFreq = static_cast<unsigned long>(modulationResponse.fmPeakFreq + 0.5f);
			v1.modulationResponse.fmRmsFreq = static_cast<unsigned long>(modulationResponse.fmRmsFreq + 0.5f);
			v1.modulationResponse.pmPeakPhase = static_cast<unsigned long>(modulationResponse.pmPeakPhase * 100 + 0.5f);
			v1.modulationResponse.pmRmsPhase = static_cast<unsigned long>(modulationResponse.pmRmsPhase * 100 + 0.5f);
			v1.modulationResponse.numAmHistBins = modulationResponse.numAmHistBins;
			v1.modulationResponse.numFmHistBins = modulationResponse.numFmHistBins;
			v1.modulationResponse.numPmHistBins = modulationResponse.numPmHistBins;
			v1.modulationResponse.amHistBinsize = static_cast<unsigned long>(modulationResponse.amHistBinsize * 1000 + 0.5f);
			v1.modulationResponse.fmHistBinsize = static_cast<unsigned long>(modulationResponse.fmHistBinsize + 0.5f);
			v1.modulationResponse.pmHistBinsize = static_cast<unsigned long>(modulationResponse.pmHistBinsize * 100 + 0.5f);
			v1.fieldStrengthResponse.status = fieldStrengthResponse.status;
			v1.fieldStrengthResponse.repeatCount = fieldStrengthResponse.repeatCount;
			v1.fieldStrengthResponse.fieldMethod = fieldStrengthResponse.fieldMethod;
			v1.fieldStrengthResponse.eField = long(floor(fieldStrengthResponse.eField * 10 + 0.5f));
			v1.fieldStrengthResponse.pFlux = long(floor(fieldStrengthResponse.pFlux * 10 + 0.5f));
			v1.fieldStrengthResponse.stdDev = long(floor(fieldStrengthResponse.stdDev * 10 + 0.5f));
			v1.fieldStrengthResponse.powerDbm = long(floor(fieldStrengthResponse.powerDbm * 10 + 0.5f));
			v1.fieldStrengthResponse.rxPowerDbm = long(floor(fieldStrengthResponse.rxPowerDbm * 10 + 0.5f));

			for(size_t i = 0; i < 2; ++i)
			{
				v1.dfResponse[i].dfResponse.status = dfResponse[i].dfResponse.status;
				v1.dfResponse[i].dfResponse.fluxgateStatus = dfResponse[i].dfResponse.fluxgateStatus;		
				v1.dfResponse[i].dfResponse.headingCorrection = static_cast<unsigned long>(dfResponse[i].dfResponse.headingCorrection * 100 + 0.5f) % 36000;
				v1.dfResponse[i].dfResponse.numSumCuts = dfResponse[i].dfResponse.numSumCuts;
				v1.dfResponse[i].dfResponse.azim = static_cast<unsigned long>(dfResponse[i].dfResponse.azim * 100 + 0.5f) % 36000;
				v1.dfResponse[i].dfResponse.elev = static_cast<unsigned long>(dfResponse[i].dfResponse.elev * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.conf = static_cast<unsigned long>(dfResponse[i].dfResponse.conf * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.range = static_cast<unsigned long>(dfResponse[i].dfResponse.range * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.stdDev = static_cast<unsigned long>(dfResponse[i].dfResponse.stdDev * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.snr = static_cast<unsigned long>(dfResponse[i].dfResponse.snr * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.powerDbm = static_cast<unsigned long>(dfResponse[i].dfResponse.powerDbm * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.numAllCuts = (dfResponse[i].dfResponse.numAllCuts <= DFCUTS_MAX ? dfResponse[i].dfResponse.numAllCuts : 0);
				v1.dfResponse[i].dfResponse.numAnts =
					(dfResponse[i].dfResponse.numAnts <= SDfResponse::SDfVoltageData::NUMANTS_MAX ? dfResponse[i].dfResponse.numAnts : 0);

				for(size_t j = 0; j < v1.dfResponse[i].dfResponse.numAllCuts; ++j)
				{
					v1.dfResponse[i].dfCutData[j].ms = static_cast<unsigned long>(dfResponse[i].dfCutData[j].ms + 0.5f);
					v1.dfResponse[i].dfCutData[j].range = static_cast<unsigned long>(dfResponse[i].dfCutData[j].range * 100 + 0.5f);
					v1.dfResponse[i].dfCutData[j].azim = static_cast<unsigned short>(dfResponse[i].dfCutData[j].azim * 100 + 0.5f) % 36000;
					v1.dfResponse[i].dfCutData[j].elev = static_cast<unsigned short>(dfResponse[i].dfCutData[j].elev * 100 + 0.5f);
					v1.dfResponse[i].dfCutData[j].conf = short(floor(dfResponse[i].dfCutData[j].conf * 100 + 0.5f));
					v1.dfResponse[i].dfCutData[j].refPowerDbm = short(floor(dfResponse[i].dfCutData[j].refPowerDbm * 100 + 0.5f));
					v1.dfResponse[i].dfCutData[j].maxSamplePowerDbm = short(floor(dfResponse[i].dfCutData[j].maxSamplePowerDbm * 100 + 0.5f));
					v1.dfResponse[i].dfCutData[j].maxSamplePowerIdx = dfResponse[i].dfCutData[j].maxSamplePowerIdx;
				}

				for(size_t j = 0; j < DFVOLTS_MAX; ++j)
				{
					for(size_t k = 0; k < v1.dfResponse[i].dfResponse.numAnts; ++k)
					{
						v1.dfResponse[i].dfVoltageData[j].volts[2 * k] = long(floor(dfResponse[i].dfVoltageData[j].volts[k][0] * 1000 + 0.5f));
						v1.dfResponse[i].dfVoltageData[j].volts[2 * k + 1] = long(floor(dfResponse[i].dfVoltageData[j].volts[k][1] * 100 + 0.5f));
					}

					v1.dfResponse[i].dfVoltageData[j].powerDbm[0] = long(floor(dfResponse[i].dfVoltageData[j].powerDbm[0]) * 100 + 0.5f);
					v1.dfResponse[i].dfVoltageData[j].powerDbm[1] = long(floor(dfResponse[i].dfVoltageData[j].powerDbm[1]) * 100 + 0.5f);
					v1.dfResponse[i].dfVoltageData[j].antNum[0] = dfResponse[i].dfVoltageData[j].antNum[0];
					v1.dfResponse[i].dfVoltageData[j].antNum[1] = dfResponse[i].dfVoltageData[j].antNum[1];
				}
			}
#else
			v1.bwResponse.betaBw = static_cast<uint32_t>(bwResponse.betaBw * 10 + 0.5);
			v1.bwResponse.betaFreqLow = static_cast<uint32_t>(bwResponse.betaFreqLow + 0.5);
			v1.bwResponse.betaFreqHigh = static_cast<uint32_t>(bwResponse.betaFreqHigh + 0.5);
			v1.bwResponse.betaStdDev = static_cast<uint32_t>(bwResponse.betaStdDev + 0.5f);
			v1.bwResponse.x1Bw = static_cast<uint32_t>(bwResponse.x1Bw * 10 + 0.5);
			v1.bwResponse.x1FreqLow = static_cast<uint32_t>(bwResponse.x1FreqLow + 0.5);
			v1.bwResponse.x1FreqHigh = static_cast<uint32_t>(bwResponse.x1FreqHigh + 0.5);
			v1.bwResponse.x1StdDev = static_cast<uint32_t>(bwResponse.x1StdDev + 0.5f);
			v1.bwResponse.x2Bw = static_cast<uint32_t>(bwResponse.x2Bw * 10 + 0.5);
			v1.bwResponse.x2FreqLow = static_cast<uint32_t>(bwResponse.x2FreqLow + 0.5);
			v1.bwResponse.x2FreqHigh = static_cast<uint32_t>(bwResponse.x2FreqHigh + 0.5);
			v1.bwResponse.x2StdDev = static_cast<uint32_t>(bwResponse.x2StdDev + 0.5f);
			v1.bwResponse.freq = Units::Frequency(bwResponse.freq).Hz<uint32_t>(true);
			v1.bwResponse.binSize = static_cast<uint32_t>(Units::Frequency(bwResponse.binSize).Hz<double>() * 10000 + 0.5);
			v1.bwResponse.numBins = bwResponse.numBins;
			v1.freqResponse.status = freqResponse.status;
			v1.freqResponse.repeatCount = freqResponse.repeatCount;
			v1.freqResponse.freqOffset = int32_t(floor(freqResponse.freqOffset + 0.5f));
			v1.freqResponse.freq = static_cast<uint32_t>(freqResponse.freq + 0.5);
			v1.freqResponse.stdDev = static_cast<uint32_t>(freqResponse.stdDev + 0.5f);
			v1.freqResponse.confidence = 900;
			v1.modulationResponse.status = modulationResponse.status;
			v1.modulationResponse.repeatCount = modulationResponse.repeatCount;
			v1.modulationResponse.amModPlus = static_cast<uint32_t>(modulationResponse.amModPlus * 1000 + 0.5f);
			v1.modulationResponse.amModMinus = static_cast<uint32_t>(modulationResponse.amModMinus * 1000 + 0.5f);
			v1.modulationResponse.amModDepth = static_cast<uint32_t>(modulationResponse.amModDepth * 1000 + 0.5f);
			v1.modulationResponse.fmPeakFreq = static_cast<uint32_t>(modulationResponse.fmPeakFreq + 0.5f);
			v1.modulationResponse.fmRmsFreq = static_cast<uint32_t>(modulationResponse.fmRmsFreq + 0.5f);
			v1.modulationResponse.pmPeakPhase = static_cast<uint32_t>(modulationResponse.pmPeakPhase * 100 + 0.5f);
			v1.modulationResponse.pmRmsPhase = static_cast<uint32_t>(modulationResponse.pmRmsPhase * 100 + 0.5f);
			v1.modulationResponse.numAmHistBins = modulationResponse.numAmHistBins;
			v1.modulationResponse.numFmHistBins = modulationResponse.numFmHistBins;
			v1.modulationResponse.numPmHistBins = modulationResponse.numPmHistBins;
			v1.modulationResponse.amHistBinsize = static_cast<uint32_t>(modulationResponse.amHistBinsize * 1000 + 0.5f);
			v1.modulationResponse.fmHistBinsize = static_cast<uint32_t>(modulationResponse.fmHistBinsize + 0.5f);
			v1.modulationResponse.pmHistBinsize = static_cast<uint32_t>(modulationResponse.pmHistBinsize * 100 + 0.5f);
			v1.fieldStrengthResponse.status = fieldStrengthResponse.status;
			v1.fieldStrengthResponse.repeatCount = fieldStrengthResponse.repeatCount;
			v1.fieldStrengthResponse.fieldMethod = fieldStrengthResponse.fieldMethod;
			v1.fieldStrengthResponse.eField = int32_t(floor(fieldStrengthResponse.eField * 10 + 0.5f));
			v1.fieldStrengthResponse.pFlux = int32_t(floor(fieldStrengthResponse.pFlux * 10 + 0.5f));
			v1.fieldStrengthResponse.stdDev = int32_t(floor(fieldStrengthResponse.stdDev * 10 + 0.5f));
			v1.fieldStrengthResponse.powerDbm = int32_t(floor(fieldStrengthResponse.powerDbm * 10 + 0.5f));
			v1.fieldStrengthResponse.rxPowerDbm = int32_t(floor(fieldStrengthResponse.rxPowerDbm * 10 + 0.5f));

			for(size_t i = 0; i < 2; ++i)
			{
				v1.dfResponse[i].dfResponse.status = dfResponse[i].dfResponse.status;
				v1.dfResponse[i].dfResponse.fluxgateStatus = dfResponse[i].dfResponse.fluxgateStatus;		
				v1.dfResponse[i].dfResponse.headingCorrection = static_cast<uint32_t>(dfResponse[i].dfResponse.headingCorrection * 100 + 0.5f) % 36000;
				v1.dfResponse[i].dfResponse.numSumCuts = dfResponse[i].dfResponse.numSumCuts;
				v1.dfResponse[i].dfResponse.azim = static_cast<uint32_t>(dfResponse[i].dfResponse.azim * 100 + 0.5f) % 36000;
				v1.dfResponse[i].dfResponse.elev = static_cast<uint32_t>(dfResponse[i].dfResponse.elev * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.conf = static_cast<uint32_t>(dfResponse[i].dfResponse.conf * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.range = static_cast<uint32_t>(dfResponse[i].dfResponse.range * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.stdDev = static_cast<uint32_t>(dfResponse[i].dfResponse.stdDev * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.snr = static_cast<uint32_t>(dfResponse[i].dfResponse.snr * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.powerDbm = static_cast<uint32_t>(dfResponse[i].dfResponse.powerDbm * 100 + 0.5f);
				v1.dfResponse[i].dfResponse.numAllCuts = (dfResponse[i].dfResponse.numAllCuts <= DFCUTS_MAX ? dfResponse[i].dfResponse.numAllCuts : 0);
				v1.dfResponse[i].dfResponse.numAnts =
					(dfResponse[i].dfResponse.numAnts <= SDfResponse::SDfVoltageData::NUMANTS_MAX ? dfResponse[i].dfResponse.numAnts : 0);

				for(size_t j = 0; j < v1.dfResponse[i].dfResponse.numAllCuts; ++j)
				{
					v1.dfResponse[i].dfCutData[j].ms = static_cast<uint32_t>(dfResponse[i].dfCutData[j].ms + 0.5f);
					v1.dfResponse[i].dfCutData[j].range = static_cast<uint32_t>(dfResponse[i].dfCutData[j].range * 100 + 0.5f);
					v1.dfResponse[i].dfCutData[j].azim = static_cast<uint16_t>(dfResponse[i].dfCutData[j].azim * 100 + 0.5f) % 36000;
					v1.dfResponse[i].dfCutData[j].elev = static_cast<uint16_t>(dfResponse[i].dfCutData[j].elev * 100 + 0.5f);
					v1.dfResponse[i].dfCutData[j].conf = int16_t(floor(dfResponse[i].dfCutData[j].conf * 100 + 0.5f));
					v1.dfResponse[i].dfCutData[j].refPowerDbm = int16_t(floor(dfResponse[i].dfCutData[j].refPowerDbm * 100 + 0.5f));
					v1.dfResponse[i].dfCutData[j].maxSamplePowerDbm = int16_t(floor(dfResponse[i].dfCutData[j].maxSamplePowerDbm * 100 + 0.5f));
					v1.dfResponse[i].dfCutData[j].maxSamplePowerIdx = dfResponse[i].dfCutData[j].maxSamplePowerIdx;
				}

				for(size_t j = 0; j < DFVOLTS_MAX; ++j)
				{
					for(size_t k = 0; k < v1.dfResponse[i].dfResponse.numAnts; ++k)
					{
						v1.dfResponse[i].dfVoltageData[j].volts[2 * k] = int32_t(floor(dfResponse[i].dfVoltageData[j].volts[k][0] * 1000 + 0.5f));
						v1.dfResponse[i].dfVoltageData[j].volts[2 * k + 1] = int32_t(floor(dfResponse[i].dfVoltageData[j].volts[k][1] * 100 + 0.5f));
					}

					v1.dfResponse[i].dfVoltageData[j].powerDbm[0] = int32_t(floor(dfResponse[i].dfVoltageData[j].powerDbm[0]) * 100 + 0.5f);
					v1.dfResponse[i].dfVoltageData[j].powerDbm[1] = int32_t(floor(dfResponse[i].dfVoltageData[j].powerDbm[1]) * 100 + 0.5f);
					v1.dfResponse[i].dfVoltageData[j].antNum[0] = dfResponse[i].dfVoltageData[j].antNum[0];
					v1.dfResponse[i].dfVoltageData[j].antNum[1] = dfResponse[i].dfVoltageData[j].antNum[1];
				}
			}
#endif
			memcpy(v1.bwBinData, bwBinData, sizeof(v1.bwBinData));
			memcpy(v1.amProbDist, amProbDist, sizeof(v1.amProbDist));
			memcpy(v1.fmProbDist, fmProbDist, sizeof(v1.fmProbDist));
			memcpy(v1.pmProbDist, pmProbDist, sizeof(v1.pmProbDist));

			return v1;
		}

		struct SBwResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long			repeatCount;
#else
			uint32_t			repeatCount;
#endif
			double					betaBw;
			double					betaFreqLow;
			double					betaFreqHigh;
			float					betaStdDev;
			double					x1Bw;
			double					x1FreqLow;
			double					x1FreqHigh;
			float					x1StdDev;
			double					x2Bw;
			double					x2FreqLow;
			double					x2FreqHigh; 
			float					x2StdDev;
			Units::Frequency::Raw	freq;
			Units::Frequency::Raw	binSize;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long			numBins;
#else
			uint32_t			numBins;
#endif
		};

		struct SDfResponse
		{
			struct SDfCutData
			{
				float			ms;
				float			range;
				float			azim;
				float			elev;
				float			conf;
				float			refPowerDbm;
				float			maxSamplePowerDbm;
#ifndef CSMSPLUS_SRV_BUILT
				unsigned char	maxSamplePowerIdx;
#else
				uint8_t	maxSamplePowerIdx;
#endif
			};

			struct SDfSummaryResponse
			{
				ErrorCodes::EErrorCode status;
				ErrorCodes::EErrorCode fluxgateStatus;		
				float	 headingCorrection;
#ifndef CSMSPLUS_SRV_BUILT
				unsigned long numSumCuts;
#else
				uint32_t numSumCuts;
#endif
				float azim;
				float elev;
				float conf; // 0 - 1.0
				float range;
				float stdDev;
				float snr;
				float powerDbm;
#ifndef CSMSPLUS_SRV_BUILT
				unsigned long numAllCuts;
				///@note: numAnts specifies the valid volts valued in
				///			SDfVoltageData below.
				unsigned char numAnts;	// needed to understand volts output
#else
				uint32_t numAllCuts;
				///@note: numAnts specifies the valid volts valued in
				///			SDfVoltageData below.
				uint8_t numAnts;	// needed to understand volts output
#endif
			};

			struct SDfVoltageData
			{
#ifndef CSMSPLUS_SRV_BUILT
				static const unsigned long NUMANTS_MAX = 9;

				float volts[NUMANTS_MAX][2];
				float powerDbm[2];
				unsigned char antNum[2];
#else
				static const uint32_t NUMANTS_MAX = 9;

				float volts[NUMANTS_MAX][2];
				float powerDbm[2];
				uint8_t antNum[2];
#endif
			};

			SDfSummaryResponse dfResponse;
			SDfCutData		dfCutData[DFCUTS_MAX];
			SDfVoltageData	dfVoltageData[DFVOLTS_MAX];
		};

		struct SFieldStrengthResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	repeatCount;
#else
			uint32_t	repeatCount;
#endif
			EFieldMethod	fieldMethod;
			float			eField;
			float			pFlux;
			float			stdDev;
			float			powerDbm;
			float			rxPowerDbm;
		};

		struct SFreqResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long			repeatCount;
#else
			uint32_t			repeatCount;
#endif
			double					freq;
			float					freqOffset;
			float					stdDev;
		};

		struct SModulationResponse
		{
			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long			repeatCount;
#else
			uint32_t			repeatCount;
#endif
			float					amModPlus;
			float					amModMinus;
			float					amModDepth;
			float					fmPeakFreq;
			float					fmRmsFreq;
			float					pmPeakPhase;
			float					pmRmsPhase;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned char			numAmHistBins;
			unsigned char			numFmHistBins;
			unsigned char			numPmHistBins;
#else
			uint8_t			numAmHistBins;
			uint8_t			numFmHistBins;
			uint8_t			numPmHistBins;
#endif
			float					amHistBinsize;
			float					fmHistBinsize;
			float					pmHistBinsize;
		};	

		inline operator SGetMeasRespV3(void) const; // Convert to V3; definition below

		SGpsResponse	gpsResponse;
		SBwResponse		bwResponse;
		SFreqResponse	freqResponse;
		SModulationResponse	modulationResponse;
		SFieldStrengthResponse fieldStrengthResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char	bwBinData[BWBINS_MAX];
		unsigned char	amProbDist[NUMHISTBINS_MAX];
		unsigned char	fmProbDist[NUMHISTBINS_MAX];
		unsigned char	pmProbDist[NUMHISTBINS_MAX];
#else
		uint8_t	bwBinData[BWBINS_MAX];
		uint8_t	amProbDist[NUMHISTBINS_MAX];
		uint8_t	fmProbDist[NUMHISTBINS_MAX];
		uint8_t	pmProbDist[NUMHISTBINS_MAX];
#endif
		SDfResponse		dfResponse[2]; // V, H
	};

	struct SIqResponse
	{
		ErrorCodes::EErrorCode	status;
		Units::Frequency::Raw	freq;	// requested frequency
		Units::Timestamp::Raw	firstSampleTime;
		double					latitude;	// degrees (+ North, - South)
		double					longitude;	// degrees (+ East, - West)
		float					altitude;	// meters in hae (height above ellipsoid)
		Units::Frequency::Raw	sampleRate;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			numSamplesTotal;
		unsigned long			numDataPackets;
#else
		uint32_t			numSamplesTotal;
		uint32_t			numDataPackets;
#endif
	};

	struct SGetMeasRespV4; // Forward declaration

	struct SGetMeasRespV3 // GET_MEAS_RESPONSE V3
	{
		operator SGetMeasRespV2(void) const // Convert to V2
		{
			SGetMeasRespV2 v2;
			v2.gpsResponse = gpsResponse;
			v2.bwResponse = bwResponse;
			v2.freqResponse = freqResponse;
			v2.modulationResponse = modulationResponse;
			v2.fieldStrengthResponse = fieldStrengthResponse;

			for(size_t i = 0; i < 2; ++i)
			{
				v2.dfResponse[i] = dfResponse[i];
			}

			memcpy(v2.bwBinData, bwBinData, sizeof(v2.bwBinData));
			memcpy(v2.amProbDist, amProbDist, sizeof(v2.amProbDist));
			memcpy(v2.fmProbDist, fmProbDist, sizeof(v2.fmProbDist));
			memcpy(v2.pmProbDist, pmProbDist, sizeof(v2.pmProbDist));

			return v2;
		}

		typedef SGetMeasRespV2::SBwResponse SBwResponse;
		typedef SGetMeasRespV2::SFreqResponse SFreqResponse;
		typedef SGetMeasRespV2::SModulationResponse SModulationResponse;
		typedef SGetMeasRespV2::SFieldStrengthResponse SFieldStrengthResponse;
		typedef SGetMeasRespV2::SDfResponse SDfResponse;

		inline operator SGetMeasRespV4(void) const; // Convert to V4; definition below

		SGpsResponse			gpsResponse;
		SBwResponse				bwResponse;
		SFreqResponse			freqResponse;
		SModulationResponse		modulationResponse;
		SFieldStrengthResponse	fieldStrengthResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char			bwBinData[BWBINS_MAX];
		unsigned char			amProbDist[NUMHISTBINS_MAX];
		unsigned char			fmProbDist[NUMHISTBINS_MAX];
		unsigned char			pmProbDist[NUMHISTBINS_MAX];
#else
		uint8_t			bwBinData[BWBINS_MAX];
		uint8_t			amProbDist[NUMHISTBINS_MAX];
		uint8_t			fmProbDist[NUMHISTBINS_MAX];
		uint8_t			pmProbDist[NUMHISTBINS_MAX];
#endif
		SDfResponse				dfResponse[2]; // V, H
		SIqResponse				iqResponse;
	};


	struct SGetMeasRespV5; // Forward declaration

	struct SGetMeasRespV4 // GET_MEAS_RESPONSE V4
	{
		operator SGetMeasRespV3(void) const // Convert to V3
		{
			SGetMeasRespV3 v3;
			v3.gpsResponse = gpsResponse;
			v3.bwResponse = bwResponse;
			v3.freqResponse = freqResponse;
			v3.modulationResponse.amHistBinsize = modulationResponse.amHistBinsize;
			v3.modulationResponse.amModDepth = modulationResponse.amModDepth;
			v3.modulationResponse.amModMinus = modulationResponse.amModMinus;
			v3.modulationResponse.amModPlus = modulationResponse.amModPlus;
			v3.modulationResponse.fmHistBinsize = modulationResponse.fmHistBinsize;
			v3.modulationResponse.fmPeakFreq = modulationResponse.fmPeakFreq;
			v3.modulationResponse.fmRmsFreq = modulationResponse.fmRmsFreq;
			v3.modulationResponse.numAmHistBins = modulationResponse.numAmHistBins;
			v3.modulationResponse.numFmHistBins = modulationResponse.numFmHistBins;
			v3.modulationResponse.numPmHistBins = modulationResponse.numPmHistBins;
			v3.modulationResponse.pmHistBinsize = modulationResponse.pmHistBinsize;
			v3.modulationResponse.pmPeakPhase = modulationResponse.pmPeakPhase;
			v3.modulationResponse.pmRmsPhase = modulationResponse.pmRmsPhase;
			v3.modulationResponse.repeatCount = modulationResponse.repeatCount;
			v3.modulationResponse.status = modulationResponse.status;

			v3.fieldStrengthResponse = fieldStrengthResponse;

			for(size_t i = 0; i < 2; ++i)
			{
				v3.dfResponse[i] = dfResponse[i];
			}

			v3.iqResponse = iqResponse;

			memcpy(v3.bwBinData, bwBinData, sizeof(v3.bwBinData));
			memcpy(v3.amProbDist, amProbDist, sizeof(v3.amProbDist));
			memcpy(v3.fmProbDist, fmProbDist, sizeof(v3.fmProbDist));
			memcpy(v3.pmProbDist, pmProbDist, sizeof(v3.pmProbDist));

			return v3;
		}

		typedef SGetMeasRespV2::SBwResponse SBwResponse;
		typedef SGetMeasRespV2::SFreqResponse SFreqResponse;
		//typedef SGetMeasRespV2::SModulationResponse SModulationResponse;
		typedef SGetMeasRespV2::SFieldStrengthResponse SFieldStrengthResponse;
		typedef SGetMeasRespV2::SDfResponse SDfResponse;

		struct SModulationResponse
		{
			SModulationResponse& operator= (const SGetMeasRespV2::SModulationResponse & v2) // Convert from V2
			{
				status = v2.status;
				repeatCount = v2.repeatCount;
				amModPlus = v2.amModPlus;
				amModMinus = v2.amModMinus;
				amModDepth = v2.amModDepth;
				fmPeakFreq = v2.fmPeakFreq;
				fmRmsFreq = v2.fmRmsFreq;
				pmPeakPhase = v2.pmPeakPhase;
				pmRmsPhase = v2.pmRmsPhase;
				numAmHistBins = v2.numAmHistBins;
				numFmHistBins = v2.numFmHistBins;
				numPmHistBins = v2.numPmHistBins;
				amHistBinsize = v2.amHistBinsize;
				fmHistBinsize = v2.fmHistBinsize;
				pmHistBinsize = v2.pmHistBinsize;
				fmModPwr = fmAmModDepth = INVALID_VALUE; // new field
				return *this;
			}

			ErrorCodes::EErrorCode	status;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long			repeatCount;
#else
			uint32_t				repeatCount;
#endif
			float					amModPlus;
			float					amModMinus;
			float					amModDepth;
			float					fmPeakFreq;
			float					fmRmsFreq;
			float					pmPeakPhase;
			float					pmRmsPhase;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned char			numAmHistBins;
			unsigned char			numFmHistBins;
			unsigned char			numPmHistBins;
#else
			uint8_t					numAmHistBins;
			uint8_t 				numFmHistBins;
			uint8_t 				numPmHistBins;
#endif
			float					amHistBinsize;
			float					fmHistBinsize;
			float					pmHistBinsize;
			float					fmModPwr;
			float					fmAmModDepth;
		};

		SGetMeasRespV4 & operator= (const SGetMeasRespV3 & v3) // Equal assignment from V3
		{
			gpsResponse = v3.gpsResponse;
			bwResponse = v3.bwResponse;
			freqResponse = v3.freqResponse;
			modulationResponse = v3.modulationResponse;
			fieldStrengthResponse = v3.fieldStrengthResponse;

			for (size_t i = 0; i < 2; ++i)
			{
				dfResponse[i] = v3.dfResponse[i];
			}

			iqResponse = v3.iqResponse;

			memcpy(bwBinData, v3.bwBinData, sizeof(v3.bwBinData));
			memcpy(amProbDist, v3.amProbDist, sizeof(v3.amProbDist));
			memcpy(fmProbDist, v3.fmProbDist, sizeof(v3.fmProbDist));
			memcpy(pmProbDist, v3.pmProbDist, sizeof(v3.pmProbDist));
			return *this;
		}

		inline operator SGetMeasRespV5(void) const; // Convert to V5; definition below

		SGpsResponse			gpsResponse;
		SBwResponse				bwResponse;
		SFreqResponse			freqResponse;
		SModulationResponse		modulationResponse;
		SFieldStrengthResponse	fieldStrengthResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char			bwBinData[BWBINS_MAX];
		unsigned char			amProbDist[NUMHISTBINS_MAX];
		unsigned char			fmProbDist[NUMHISTBINS_MAX];
		unsigned char			pmProbDist[NUMHISTBINS_MAX];
#else
		uint8_t 				bwBinData[BWBINS_MAX];
		uint8_t 				amProbDist[NUMHISTBINS_MAX];
		uint8_t 				fmProbDist[NUMHISTBINS_MAX];
		uint8_t 				pmProbDist[NUMHISTBINS_MAX];
#endif
		SDfResponse				dfResponse[2]; // V, H
		SIqResponse				iqResponse;
	};

	struct SGetMeasRespV5 // GET_MEAS_RESPONSE V5
	{
		operator SGetMeasRespV4(void) const // Convert to V4
		{
			SGetMeasRespV4 v4;
			v4.gpsResponse = gpsResponse;
			v4.bwResponse = bwResponse;
			v4.freqResponse = freqResponse;
			v4.modulationResponse = modulationResponse;
			v4.fieldStrengthResponse = fieldStrengthResponse;
			v4.iqResponse = iqResponse;
			memcpy(v4.bwBinData, bwBinData, sizeof(v4.bwBinData));
			memcpy(v4.amProbDist, amProbDist, sizeof(v4.amProbDist));
			memcpy(v4.fmProbDist, fmProbDist, sizeof(v4.fmProbDist));
			memcpy(v4.pmProbDist, pmProbDist, sizeof(v4.pmProbDist));

			//Change made in V5 is in SDfResponse
			for (size_t i = 0; i < 2; ++i)
			{
				v4.dfResponse[i].dfResponse = dfResponse[i].dfResponse;
				for (size_t j = 0; j < DFCUTS_MAX; ++j)
				{
					v4.dfResponse[i].dfCutData[j] = dfResponse[i].dfCutData[j];
				}
				for (size_t j = 0; j < DFVOLTS_MAX; ++j)
				{
					//v4 has array of 9 voltages vs. v5 that has 11 voltages.
					for (size_t k = 0; k < SGetMeasRespV2::SDfResponse::SDfVoltageData::NUMANTS_MAX; ++k)
					{
						for (size_t m = 0; m < 2; ++m)
						{
							v4.dfResponse[i].dfVoltageData[j].volts[k][m] = dfResponse[i].dfVoltageData[j].volts[k][m];
						}
					}
					for (size_t k = 0; k < 2; ++k)
					{
						v4.dfResponse[i].dfVoltageData[j].powerDbm[k] = dfResponse[i].dfVoltageData[j].powerDbm[k];
						v4.dfResponse[i].dfVoltageData[j].antNum[k] = dfResponse[i].dfVoltageData[j].antNum[k];
					}
				}
			}

			return v4;
		}
		SGetMeasRespV5 & operator= (const SGetMeasRespV4 & v4) // Equal assignment from v4
		{
			gpsResponse = v4.gpsResponse;
			bwResponse = v4.bwResponse;
			freqResponse = v4.freqResponse;
			modulationResponse = v4.modulationResponse;
			fieldStrengthResponse = v4.fieldStrengthResponse;
			iqResponse = v4.iqResponse;
			memcpy(bwBinData, v4.bwBinData, sizeof(v4.bwBinData));
			memcpy(amProbDist, v4.amProbDist, sizeof(v4.amProbDist));
			memcpy(fmProbDist, v4.fmProbDist, sizeof(v4.fmProbDist));
			memcpy(pmProbDist, v4.pmProbDist, sizeof(v4.pmProbDist));

			//Change made in V5 is in SDfResponse
			for (size_t i = 0; i < 2; ++i)
			{
				dfResponse[i].dfResponse = v4.dfResponse[i].dfResponse;
				for (size_t j = 0; j < DFCUTS_MAX; ++j)
				{
					dfResponse[i].dfCutData[j] = v4.dfResponse[i].dfCutData[j];
				}
				for (size_t j = 0; j < DFVOLTS_MAX; ++j)
				{
					//v4 has array of 9 voltages vs. v5 that has 11 voltages.
					for (size_t k = 0; k < SGetMeasRespV2::SDfResponse::SDfVoltageData::NUMANTS_MAX; ++k)
					{
						for (size_t m = 0; m < 2; ++m)
						{
							dfResponse[i].dfVoltageData[j].volts[k][m] = v4.dfResponse[i].dfVoltageData[j].volts[k][m];
						}
					}

					//Zero out voltages that are not there.
					for (size_t k = SGetMeasRespV2::SDfResponse::SDfVoltageData::NUMANTS_MAX; k < SGetMeasRespV5::SDfResponse::SDfVoltageData::MAX_ANT_EL; ++k)
					{
						for (size_t m = 0; m < 2; ++m)
						{
							dfResponse[i].dfVoltageData[j].volts[k][m] = 0;
						}
					}
					for (size_t k = 0; k < 2; ++k)
					{
						dfResponse[i].dfVoltageData[j].powerDbm[k] = v4.dfResponse[i].dfVoltageData[j].powerDbm[k];
						dfResponse[i].dfVoltageData[j].antNum[k] = v4.dfResponse[i].dfVoltageData[j].antNum[k];
					}
				}
			}
			return	*this;
		}

		typedef SGetMeasRespV2::SBwResponse SBwResponse;
		typedef SGetMeasRespV2::SFreqResponse SFreqResponse;
		typedef SGetMeasRespV4::SModulationResponse SModulationResponse;
		typedef SGetMeasRespV2::SFieldStrengthResponse SFieldStrengthResponse;

		struct SDfResponse
		{
			typedef SGetMeasRespV2::SDfResponse::SDfCutData SDfCutData;
			typedef SGetMeasRespV2::SDfResponse::SDfSummaryResponse SDfSummaryResponse;
			struct SDfVoltageData
			{
#ifndef CSMSPLUS_SRV_BUILT
				static const unsigned long MAX_ANT_EL = 11; //Maximum number of antenna elements.
#else
				static const uint32_t MAX_ANT_EL = 11; //Maximum number of antenna elements.
#endif
				float volts[MAX_ANT_EL][2];
				float powerDbm[2];
#ifndef CSMSPLUS_SRV_BUILT
				unsigned char antNum[2];
#else
				uint8_t antNum[2];
#endif
			};

			SDfSummaryResponse dfResponse;
			SDfCutData		dfCutData[DFCUTS_MAX];
			SDfVoltageData	dfVoltageData[DFVOLTS_MAX];
		};

		SGpsResponse			gpsResponse;
		SBwResponse				bwResponse;
		SFreqResponse			freqResponse;
		SModulationResponse		modulationResponse;
		SFieldStrengthResponse	fieldStrengthResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char			bwBinData[BWBINS_MAX];
		unsigned char			amProbDist[NUMHISTBINS_MAX];
		unsigned char			fmProbDist[NUMHISTBINS_MAX];
		unsigned char			pmProbDist[NUMHISTBINS_MAX];
#else
		uint8_t 				bwBinData[BWBINS_MAX];
		uint8_t 				amProbDist[NUMHISTBINS_MAX];
		uint8_t 				fmProbDist[NUMHISTBINS_MAX];
		uint8_t 				pmProbDist[NUMHISTBINS_MAX];
#endif
		SDfResponse				dfResponse[2]; // V, H
		SIqResponse				iqResponse;
	};

	typedef SGetMeasRespV5 SGetMeasResp;

	struct SIqDataRespV1;	// Forward declaration

	struct SIqDataRespV0	// GET_MEAS_IQDATA_RESP V0
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short IQDATA_SIZE = 4096;
#else
		static const uint16_t IQDATA_SIZE = 4096;
#endif
		SIqDataRespV0& operator=(const SIqDataRespV1& v1)		// Convert from V1 to V0
		{
			const SIqDataRespV1* src = &v1;
			if (static_cast<void*>(this) == static_cast<const void*>(&v1))
			{
				// Make a copy
				if ((src = static_cast<const SIqDataRespV1*>(malloc(sizeof(v1)))) == nullptr)
				{
					AfxThrowMemoryException();
				}
				memcpy(const_cast<SIqDataRespV1*>(src), &v1, sizeof(v1));
			}
			packetIndex = src->packetIndex;
			numIqData = src->numIqData;
#ifndef CSMSPLUS_SRV_BUILT
			for (unsigned short i = 0; i < numIqData; ++i)		// memcpy gives alignment errors in GCC.
#else
			for (uint16_t i = 0; i < numIqData; ++i)		// memcpy gives alignment errors in GCC.
#endif
			{
				iqData[i].re = src->iqData[i].re;
				iqData[i].im = src->iqData[i].im;
			}

			if (src != &v1)
			{
				free(const_cast<SIqDataRespV1*>(src));
			}

			return *this;
		}

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long packetIndex;		// 0 based
		unsigned short numIqData;		// Number of samples
		struct SIq
		{
			short re;
			short im;
#else
		uint32_t packetIndex;		// 0 based
		uint16_t numIqData;		// Number of samples
		struct SIq
		{
			int16_t re;
			int16_t im;
#endif
		} iqData[IQDATA_SIZE];
	};

	struct SIqDataRespV1	// GET_MEAS_IQDATA_RESP V1
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short IQDATA_SIZE = 4096;
#else
		static const uint16_t IQDATA_SIZE = 4096;
#endif
		SIqDataRespV1& operator=(const SIqDataRespV0& v0)		// Convert from V0 to V1
		{
			const SIqDataRespV0* src = &v0;
			if (static_cast<void*>(this) == static_cast<const void*>(&v0))
			{
				// Make a copy
				if ((src = static_cast<const SIqDataRespV0*>(malloc(sizeof(v0)))) == nullptr)
				{
					AfxThrowMemoryException();
				}
				memcpy(const_cast<SIqDataRespV0*>(src), &v0, sizeof(v0));
			}
			packetIndex = src->packetIndex;
			numIqData = src->numIqData;
			unused = 0;
#ifndef CSMSPLUS_SRV_BUILT
			for (unsigned short i = 0; i < numIqData; ++i)		// memcpy gives alignment errors in GCC.
#else
			for (uint16_t i = 0; i < numIqData; ++i)		// memcpy gives alignment errors in GCC.
#endif
			{
				iqData[i].re = src->iqData[i].re;
				iqData[i].im = src->iqData[i].im;
			}

			if (src != &v0)
			{
				free(const_cast<SIqDataRespV0*>(src));
			}

			return *this;
		}

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long packetIndex;		// 0 based
		unsigned short numIqData;		// Number of samples
		unsigned short unused;			// Added to align the following array on 32-bit boundary
		struct SIq
		{
			short re;
			short im;
#else
		uint32_t packetIndex;		// 0 based
		uint16_t numIqData;		// Number of samples
		uint16_t unused;			// Added to align the following array on 32-bit boundary
		struct SIq
		{
			int16_t re;
			int16_t im;
#endif
		} iqData[IQDATA_SIZE];
	};

	typedef SIqDataRespV1 SIqDataResp;

	struct SOccupancyHeader
	{
		ErrorCodes::EErrorCode	status;
		SGpsResponse			gpsResponse;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			numTotalChannels;
		unsigned long			firstChannel;
		unsigned long			numChannels;
		unsigned long			numTimeOfDays;
#else
		uint32_t			numTotalChannels;
		uint32_t			firstChannel;
		uint32_t			numChannels;
		uint32_t			numTimeOfDays;
#endif
	};

	struct SOccupancyHeaderForDM
	{
		ErrorCodes::EErrorCode	status;
		SGpsResponse			gpsResponse;
#ifndef CSMSPLUS_SRV_BUILT
		long                    runNumber;
		double                  collectionTime;
		unsigned long           az;  // Az angle
		unsigned long           el;  // El angle
		unsigned long			numTotalChannels;
		unsigned long			firstChannel;
		unsigned long			numChannels;
		unsigned long			numTimeOfDays;
#else
		int32_t                    runNumber;
		double                  collectionTime;
		uint32_t           az;  // Az angle
		uint32_t           el;  // El angle
		uint32_t			numTotalChannels;
		uint32_t			firstChannel;
		uint32_t			numChannels;
		uint32_t			numTimeOfDays;
#endif
	};

	struct SInitializeDfV2;

	struct SInitializeDfV1 // INITIALIZE_DF V1 (V0 has no body)
	{
		inline operator SInitializeDfV2(void) const; // Convert to V2; defined below

		// All zero means perform a default full calibration, otherwise an incremental calibration
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long freq;
		unsigned long rxBw;
		unsigned long numIntegrations;
#else
		uint32_t freq;
		uint32_t rxBw;
		uint32_t numIntegrations;
#endif
		bool hf;
	};

	struct SInitializeDfV2 // INITIALIZE_DF V2
	{
		operator SInitializeDfV1(void) const // Convert to V1
		{
			SInitializeDfV1 v1;
#ifndef CSMSPLUS_SRV_BUILT
			v1.freq = Units::Frequency(freq).Hz<unsigned long>(true);
			v1.rxBw = Units::Frequency(rxBw).Hz<unsigned long>(true);
#else
			v1.freq = Units::Frequency(freq).Hz<uint32_t>(true);
			v1.rxBw = Units::Frequency(rxBw).Hz<uint32_t>(true);
#endif
			v1.numIntegrations = numIntegrations;
			v1.hf = hf;

			return v1;
		}

		// All zero means perform a default full calibration, otherwise an incremental calibration
		Units::Frequency::Raw freq;
		Units::Frequency::Raw rxBw;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numIntegrations;
#else
		uint32_t numIntegrations;
#endif
		bool hf;
	};

	typedef SInitializeDfV2 SInitializeDf;

	struct SInitializeDfRespV0 // INITIALIZE_DF_RESPONSE V0
	{
		ErrorCodes::EErrorCode vhfAntCalStatus;
		ErrorCodes::EErrorCode initIntStatus;
		ErrorCodes::EErrorCode initWwStatus;
		ErrorCodes::EErrorCode initWfaStatus;
		ErrorCodes::EErrorCode initSymStatus;
		ErrorCodes::EErrorCode pbCalStatus[30];
	};

	struct SInitializeDfRespV2; // Forward declaration
	struct SInitializeDfRespV3; // Forward declaration
	struct SInitializeDfRespV4; // Forward declaration
	struct SInitializeDfRespV1 // INITIALIZE_DF_RESPONSE V1 (Variable length)
	{
		operator SInitializeDfRespV0(void) const // Convert to V0
		{
			SInitializeDfRespV0 v0;
			v0.vhfAntCalStatus = vhfAntCalStatus;
			v0.initIntStatus = initIntStatus;
			v0.initWwStatus = initWwStatus;
			v0.initWfaStatus = initWfaStatus;
			v0.initSymStatus = initSymStatus;

			for(size_t i = 0; i < std::extent<decltype(SInitializeDfRespV0::pbCalStatus)>::value; ++i)
			{
				v0.pbCalStatus[i] = i < numPbCalStatus ?
						pbCalStatus[i].status : ErrorCodes::NOERROR_DFNOTINIT;
			}

			return v0;
		}

		SInitializeDfRespV1& operator =(SInitializeDfRespV0 v0) // Convert from V0
		{
			vhfAntCalStatus = v0.vhfAntCalStatus;
			initIntStatus = v0.initIntStatus;
			initWwStatus = v0.initWwStatus;
			initWfaStatus = v0.initWfaStatus;
			initSymStatus = v0.initSymStatus;
			numPbCalStatus = 0;

			for(size_t i = 0; i < std::extent<decltype(SInitializeDfRespV0::pbCalStatus)>::value; ++i)
			{
				if(v0.pbCalStatus[i] != ErrorCodes::NOERROR_DFNOTINIT)
				{
					pbCalStatus[i].freq = 0;
					pbCalStatus[i].rxBw = 0;
					pbCalStatus[i].avgRelAmp = 0;
					pbCalStatus[i].stdDevRelAmp = 0;
					pbCalStatus[i].avgRelPhase = 0;
					pbCalStatus[i].stdDevRelPhase = 0;
					pbCalStatus[i].numIntegrations = 1;
					pbCalStatus[i].hf = false;
					pbCalStatus[i].status = v0.pbCalStatus[i];
					++numPbCalStatus;
				}
			}

			return *this;
		}

		SInitializeDfRespV1& operator =(const SInitializeDfRespV2& v2); // Convert from V2

		struct SPbCalStatus
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long freq;
			unsigned long rxBw;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			unsigned long numIntegrations;
#else
			uint32_t freq;
			uint32_t rxBw;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			uint32_t numIntegrations;
#endif
			bool hf;
			ErrorCodes::EErrorCode status;
		};

		ErrorCodes::EErrorCode vhfAntCalStatus;
		ErrorCodes::EErrorCode initIntStatus;
		ErrorCodes::EErrorCode initWwStatus;
		ErrorCodes::EErrorCode initWfaStatus;
		ErrorCodes::EErrorCode initSymStatus;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned int numPbCalStatus;
#else
		uint32_t numPbCalStatus;
#endif
		SPbCalStatus pbCalStatus[1]; // Variable length
	};

	struct SInitializeDfRespV2 // INITIALIZE_DF_RESPONSE V2 (Variable length)
	{
		SInitializeDfRespV2& operator =(const SInitializeDfRespV1& v1) // Convert from V1
		{
			const SInitializeDfRespV1* src = &v1;

			if(static_cast<void*>(this) == static_cast<const void*>(&v1))
			{
				// Make a copy
				if((src = static_cast<const SInitializeDfRespV1*>(malloc(offsetof(SInitializeDfRespV1, pbCalStatus[v1.numPbCalStatus])))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SInitializeDfRespV1*>(src), &v1, offsetof(SInitializeDfRespV1, pbCalStatus[v1.numPbCalStatus]));
			}

			vhfAntCalStatus = src->vhfAntCalStatus;
			initIntStatus = src->initIntStatus;
			initWwStatus = src->initWwStatus;
			initWfaStatus = src->initWfaStatus;
			initSymStatus = src->initSymStatus;
			numPbCalStatus = src->numPbCalStatus;

			for(size_t i = 0; i < numPbCalStatus; ++i)
			{
				pbCalStatus[i].freq = src->pbCalStatus[i].freq;
				pbCalStatus[i].rxBw = src->pbCalStatus[i].rxBw;
				pbCalStatus[i].band = 0;
				pbCalStatus[i].avgRelAmp = src->pbCalStatus[i].avgRelAmp;
				pbCalStatus[i].stdDevRelAmp = src->pbCalStatus[i].stdDevRelAmp;
				pbCalStatus[i].avgRelPhase = src->pbCalStatus[i].avgRelPhase;
				pbCalStatus[i].stdDevRelPhase = src->pbCalStatus[i].stdDevRelPhase;
				pbCalStatus[i].numIntegrations = src->pbCalStatus[i].numIntegrations;
				pbCalStatus[i].hf = src->pbCalStatus[i].hf;
				pbCalStatus[i].status = src->pbCalStatus[i].status;
			}

			if(src != &v1)
			{
				free(const_cast<SInitializeDfRespV1*>(src));
			}

			return *this;
		}

		SInitializeDfRespV2& operator =(const SInitializeDfRespV3& v3); // Convert from V3

		struct SPbCalStatus
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long freq;
			unsigned long rxBw;
			unsigned long band;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			unsigned long numIntegrations;
#else
			uint32_t freq;
			uint32_t rxBw;
			uint32_t band;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			uint32_t numIntegrations;
#endif
			bool hf;
			ErrorCodes::EErrorCode status;
		};

		ErrorCodes::EErrorCode vhfAntCalStatus;
		ErrorCodes::EErrorCode initIntStatus;
		ErrorCodes::EErrorCode initWwStatus;
		ErrorCodes::EErrorCode initWfaStatus;
		ErrorCodes::EErrorCode initSymStatus;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned int numPbCalStatus;
#else
		uint32_t numPbCalStatus;
#endif
		SPbCalStatus pbCalStatus[1]; // Variable length
	};

	struct SInitializeDfRespV3 // INITIALIZE_DF_RESPONSE V3 (Variable length)
	{
		SInitializeDfRespV3& operator =(const SInitializeDfRespV2& v2) // Convert from V2
		{
			const SInitializeDfRespV2* src = &v2;

			if(static_cast<void*>(this) == static_cast<const void*>(&v2))
			{
				// Make a copy
				if((src = static_cast<const SInitializeDfRespV2*>(malloc(offsetof(SInitializeDfRespV2, pbCalStatus[v2.numPbCalStatus])))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SInitializeDfRespV2*>(src), &v2, offsetof(SInitializeDfRespV2, pbCalStatus[v2.numPbCalStatus]));
			}

			vhfAntCalStatus = src->vhfAntCalStatus;
			initIntStatus = src->initIntStatus;
			initWwStatus = src->initWwStatus;
			initWfaStatus = src->initWfaStatus;
			initSymStatus = src->initSymStatus;
			numPbCalStatus = src->numPbCalStatus;

			for(size_t i = 0; i < numPbCalStatus; ++i)
			{
				pbCalStatus[i].freq = Units::Frequency(src->pbCalStatus[i].freq).GetRaw();
				pbCalStatus[i].rxBw = Units::Frequency(src->pbCalStatus[i].rxBw).GetRaw();
				pbCalStatus[i].band = 0;
				pbCalStatus[i].avgRelAmp = src->pbCalStatus[i].avgRelAmp;
				pbCalStatus[i].stdDevRelAmp = src->pbCalStatus[i].stdDevRelAmp;
				pbCalStatus[i].avgRelPhase = src->pbCalStatus[i].avgRelPhase;
				pbCalStatus[i].stdDevRelPhase = src->pbCalStatus[i].stdDevRelPhase;
				pbCalStatus[i].numIntegrations = src->pbCalStatus[i].numIntegrations;
				pbCalStatus[i].hf = src->pbCalStatus[i].hf;
				pbCalStatus[i].status = src->pbCalStatus[i].status;
			}

			if(src != &v2)
			{
				free(const_cast<SInitializeDfRespV2*>(src));
			}

			return *this;
		}

		SInitializeDfRespV3& operator =(const SInitializeDfRespV4& v4); // Convert from V4

		struct SPbCalStatus
		{
			Units::Frequency::Raw freq;
			Units::Frequency::Raw rxBw;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			unsigned long numIntegrations;
#else
		uint32_t band;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			uint32_t numIntegrations;
#endif
			bool hf;
			ErrorCodes::EErrorCode status;
		};

		ErrorCodes::EErrorCode vhfAntCalStatus;
		ErrorCodes::EErrorCode initIntStatus;
		ErrorCodes::EErrorCode initWwStatus;
		ErrorCodes::EErrorCode initWfaStatus;
		ErrorCodes::EErrorCode initSymStatus;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned int numPbCalStatus;
#else
		uint32_t numPbCalStatus;
#endif
		SPbCalStatus pbCalStatus[1]; // Variable length
	};

	struct SInitializeDfRespV4 // INITIALIZE_DF_RESPONSE V4 (Variable length)
	{
		SInitializeDfRespV4& operator =(const SInitializeDfRespV3& v3) // Convert from V3
		{
			const SInitializeDfRespV3* src = &v3;

			if(static_cast<void*>(this) == static_cast<const void*>(&v3))
			{
				// Make a copy
				if((src = static_cast<const SInitializeDfRespV3*>(malloc(offsetof(SInitializeDfRespV3, pbCalStatus[v3.numPbCalStatus])))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SInitializeDfRespV3*>(src), &v3, offsetof(SInitializeDfRespV3, pbCalStatus[v3.numPbCalStatus]));
			}

			vhfAntCalStatus = src->vhfAntCalStatus;
			initIntStatus = src->initIntStatus;
			initWwStatus = src->initWwStatus;
			initWfaStatus = src->initWfaStatus;
			initSymStatus = src->initSymStatus;
			numPbCalStatus = src->numPbCalStatus;

			for(size_t i = 0; i < numPbCalStatus; ++i)
			{
				pbCalStatus[i].freq = Units::Frequency(src->pbCalStatus[i].freq).GetRaw();
				pbCalStatus[i].rxBw = Units::Frequency(src->pbCalStatus[i].rxBw).GetRaw();
				pbCalStatus[i].band = 0;
				pbCalStatus[i].avgRelAmp = src->pbCalStatus[i].avgRelAmp;
				pbCalStatus[i].stdDevRelAmp = src->pbCalStatus[i].stdDevRelAmp;
				pbCalStatus[i].avgRelPhase = src->pbCalStatus[i].avgRelPhase;
				pbCalStatus[i].stdDevRelPhase = src->pbCalStatus[i].stdDevRelPhase;
				pbCalStatus[i].avgRelDelayNsec = 0;
				pbCalStatus[i].numIntegrations = src->pbCalStatus[i].numIntegrations;
				pbCalStatus[i].hf = src->pbCalStatus[i].hf;
				pbCalStatus[i].status = src->pbCalStatus[i].status;
			}

			if(src != &v3)
			{
				free(const_cast<SInitializeDfRespV3*>(src));
			}

			return *this;
		}

		struct SPbCalStatus
		{
			Units::Frequency::Raw freq;
			Units::Frequency::Raw rxBw;
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			float avgRelDelayNsec;
			unsigned long numIntegrations;
#else
			uint32_t band;
			float avgRelAmp;
			float stdDevRelAmp;
			float avgRelPhase;
			float stdDevRelPhase;
			float avgRelDelayNsec;
			uint32_t numIntegrations;
#endif
			bool hf;
			ErrorCodes::EErrorCode status;
		};

		ErrorCodes::EErrorCode vhfAntCalStatus;
		ErrorCodes::EErrorCode initIntStatus;
		ErrorCodes::EErrorCode initWwStatus;
		ErrorCodes::EErrorCode initWfaStatus;
		ErrorCodes::EErrorCode initSymStatus;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned int numPbCalStatus;
#else
		uint32_t numPbCalStatus;
#endif
		SPbCalStatus pbCalStatus[1]; // Variable length
	};

	typedef SInitializeDfRespV4 SInitializeDfResp;

	struct SRcvrManCmd // SET_RCVR_MAN 
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	freq;		// frequency in Hertz
		unsigned long	bandwidth;	// rcvr bandwidth in Hertz
		unsigned long	atten;		// rcvr attenuation
#else
		uint32_t	freq;		// frequency in Hertz
		uint32_t	bandwidth;	// rcvr bandwidth in Hertz
		uint32_t	atten;		// rcvr attenuation
#endif
	};

	struct SLoadIonogram // LOAD_IONOGRAM
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
#else
		uint32_t taskId;
#endif
		SIonogramDataMsg ionoData;
	};

	struct SLoadIonogramResp // LOAD_IONOGRAM_RESPONSE
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
#else
		uint32_t taskId;
#endif
		ErrorCodes::EErrorCode status;
	};

	struct SSounderSetupCmdV1
	{
#ifndef CSMSPLUS_SRV_BUILT
		long startFreq;
		long endFreq;
		long interval;
		int  sweepRate;
		char powerLevel;
		char blankerOn;
#else
		int32_t startFreq;
		int32_t endFreq;
		int32_t interval;
		int32_t sweepRate;
		int8_t powerLevel;
		int8_t blankerOn;
#endif
	};

	typedef SSounderSetupCmdV1 SSounderSetupCmd;

		struct SBlankerListCmdV1
	{
#ifndef CSMSPLUS_SRV_BUILT
		long freqs[96];
#else
		int32_t freqs[96];
#endif
	};

	typedef SBlankerListCmdV1 SBlankerListCmd;

	struct SSounderSweepCtrlCmdV1
	{
		BOOL Run;
	};

	typedef SSounderSweepCtrlCmdV1 SSounderSweepCtrlCmd;

	struct SSounderSelfTestCmdV1
	{
		BOOL Start;
	};

	typedef SSounderSelfTestCmdV1 SSounderSelfTestCmd;

	struct SSounderStatusInfo
	{
		SSounderSetupCmd SetupInfo;
		SBlankerListCmd  BlankingInfo;
		bool Has1310SampleRate;
		bool Running;
	};

	struct SSounderGenericRespV1   // MSG_SOUNDER_BLANKER_FREQ_RESP, MSG_SOUNDER_SETUP_RESP, MSG_SOUNDER_START_RESP, MSG_SOUNDER_STOP_RESP
						           // MSG_SOUNDER_TEST_RESP, 
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long status; // 0 = bad, 1 = OK
#else
		uint32_t status; // 0 = bad, 1 = OK
#endif
	};

	typedef SSounderGenericRespV1 SSounderGenericResp;

	struct SGetSounderStatusRespV1 // SOUNDER_STATUS_RESPONSE
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
#else
		uint32_t taskId;
#endif
		ErrorCodes::EErrorCode status;
		SSounderStatusInfo sounderStatus;
	};

	typedef SGetSounderStatusRespV1 SGetSounderStatusResp;

	struct SAvdMeasureResultV1; // Forward declaration

	struct SAvdMeasureResultV0 // AUTOVIOLATE_BWMEAS_RESULT, AUTOVIOLATE_FREQMEAS_RESULT V0
	{
		operator SAvdMeasureResultV1(void) const; // Convert to V1; defined below

		struct SMeasureData
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long result;
			unsigned long stdDev;
#else
			uint32_t result;
			uint32_t stdDev;
#endif
		};

		SOccupancyHeader occHdr;
		SMeasureData measData[MAX_OCCCHANNELS];
	};

	struct SAvdMeasureResultV1 // AUTOVIOLATE_BWMEAS_RESULT, AUTOVIOLATE_FREQMEAS_RESULT V1 (variable length)
	{
		operator SAvdMeasureResultV0(void) const // Convert to V0
		{
#ifndef CSMSPLUS_SRV_BUILT
			static const unsigned long maxOccChannels = MAX_OCCCHANNELS;
#else
			static const uint32_t maxOccChannels = MAX_OCCCHANNELS;
#endif
			SAvdMeasureResultV0 v0;
			v0.occHdr = occHdr;
			v0.occHdr.numChannels = (occHdr.numChannels <= std::min(maxOccChannels, occHdr.numTotalChannels) ? occHdr.numChannels : 0);
			for(size_t i = 0; i < v0.occHdr.numChannels; ++i)
			{
#ifndef CSMSPLUS_SRV_BUILT
				v0.measData[i].result = static_cast<unsigned long>(measData[i].result + 0.5);
				v0.measData[i].stdDev = static_cast<unsigned long>(measData[i].stdDev + 0.5);
#else
				v0.measData[i].result = static_cast<uint32_t>(measData[i].result + 0.5);
				v0.measData[i].stdDev = static_cast<uint32_t>(measData[i].stdDev + 0.5);
#endif
			}

			return v0;
		}

		struct SMeasureData
		{
			double result;
			float stdDev;
		};

		SOccupancyHeader occHdr;
		SMeasureData measData[MAX_OCCCHANNELS]; // Variable length up to MAX_OCCCHANNELS
	};

	typedef SAvdMeasureResultV1 SAvdMeasureResult;

	struct SDmMeasureResultV1 // (variable length)
	{
			struct SMeasureData
		{
			double result;
			float stdDev;
		};

		SOccupancyHeaderForDM occHdr;
		SMeasureData measData[MAX_OCCCHANNELS]; // Variable length up to MAX_OCCCHANNELS
	};

	typedef SDmMeasureResultV1 SDmMeasureResult;

	struct SMsgLengthDistributionResp // MSGLEN_DIST_RESPONSE
	{
		struct SMsgLenHistData
		{
#ifndef CSMSPLUS_SRV_BUILT
			long channel;
#else
			int32_t channel;
#endif
			float length;
		};

		SOccupancyHeader occHdr;
		SMsgLenHistData	histData[MAX_OCCCHANNELS];
	};

	// OCC_CHANNEL_RESULT, EFLD_CHANNEL_RESULT, OCC_TIMEOFDAY_RESULT, MSGLEN_CHANNEL_RESULT,
    // OCC_SPECGRAM_RESULT, OCC_EFLD_TIMEOFDAY_RESULT
	struct SOccResult 
	{
		struct SResultData
		{
#ifndef CSMSPLUS_SRV_BUILT
			signed char avg;
			signed char max;
#else
			int8_t avg;
			int8_t max;
#endif
		};

		SOccupancyHeader occHdr;
		SResultData resultData[MAX_OCCRESULTCHANNELS];
	};

	struct SOccResultForDM
	{
		struct SResultData
		{
#ifndef CSMSPLUS_SRV_BUILT
			signed char avg;
			signed char max;
#else
			int8_t avg;
			int8_t max;
#endif
		};

		SOccupancyHeaderForDM occHdr;
		SResultData resultData[MAX_OCCRESULTCHANNELS];
	};

	struct SFrequencyVsChannelRespV1; // Forward declaration

	struct SFrequencyVsChannelRespV0 // OCC_FREQUENCY_RESULT V0
	{
		inline operator SFrequencyVsChannelRespV1(void) const; // Convert to V1; defined below

		SOccupancyHeader occHdr;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long frequencies[MAX_OCCCHANNELS];
		int numBands;
		unsigned long numChannels[MAX_OCCBANDS];
		unsigned long saveIntermediateData;  // 0 - don't save; > 0 - save (RA3 data)
		unsigned long useSecondaryThreshold;  // 0 = don't use  >0 = use
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod in command
		long occPrimaryThreshold[2];
		long occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		EAnt selectedAntenna;  //uses ANT1 & ANT2 defines in dsp_msg.h
		unsigned long chanBandwidth;    // in Hz -- reserved -- currently not used
#else
		uint32_t frequencies[MAX_OCCCHANNELS];
		int32_t numBands;
		uint32_t numChannels[MAX_OCCBANDS];
		uint32_t saveIntermediateData;  // 0 - don't save; > 0 - save (RA3 data)
		uint32_t useSecondaryThreshold;  // 0 = don't use  >0 = use
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod in command
		int32_t occPrimaryThreshold[2];
		int32_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		EAnt selectedAntenna;  //uses ANT1 & ANT2 defines in dsp_msg.h
		uint32_t chanBandwidth;    // in Hz -- reserved -- currently not used
#endif
		char hostName[32];
	};

	struct SFrequencyVsChannelRespV2; // Forward declaration

	struct SFrequencyVsChannelRespV1 // OCC_FREQUENCY_RESULT V1 (variable length)
	{
		inline operator SFrequencyVsChannelRespV2(void) const; // Convert to V2; defined below

		operator SFrequencyVsChannelRespV0(void) const // Convert to V0
		{
#ifndef CSMSPLUS_SRV_BUILT
			static const unsigned long maxOccChannels = MAX_OCCCHANNELS;
#else
			static const uint32_t maxOccChannels = MAX_OCCCHANNELS;
#endif
			SFrequencyVsChannelRespV0 v0;
			v0.occHdr = occHdr;
			v0.occHdr.numChannels = (occHdr.numChannels <= std::min(maxOccChannels, occHdr.numTotalChannels) ? occHdr.numChannels : 0);

			for(size_t i = 0; i < v0.occHdr.numChannels; ++i)
			{
#ifndef CSMSPLUS_SRV_BUILT
				v0.frequencies[i] = Units::Frequency(frequencies[i]).Hz<unsigned long>(true);
#else
				v0.frequencies[i] = Units::Frequency(frequencies[i]).Hz<uint32_t>(true);
#endif
			}

			v0.numBands = numBands;
			memcpy(v0.numChannels, numChannels, sizeof(v0.numChannels));
			v0.saveIntermediateData = (saveIntermediateData ? 1 : 0);
			v0.useSecondaryThreshold = (useSecondaryThreshold ? 1 : 0);
			v0.occPrimaryThreshold[0] = occPrimaryThreshold[0];
			v0.occPrimaryThreshold[1] = occPrimaryThreshold[1];
			v0.occSecondaryThreshold[0] = occSecondaryThreshold[0];
			v0.occSecondaryThreshold[1] = occSecondaryThreshold[1];
			v0.selectedAntenna = selectedAntenna;
			v0.chanBandwidth = 0;
			strcpy_s(v0.hostName, hostName);

			return v0;
		}

		SOccupancyHeader occHdr;
		EAnt selectedAntenna;  //uses ANT1 & ANT2 defines in dsp_msg.h
		bool saveIntermediateData; 
		bool useSecondaryThreshold;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1 
		//  which thresholds used are based upon thresholdMethod in command
#ifndef CSMSPLUS_SRV_BUILT
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		char hostName[32];
		int numBands;
		unsigned long numChannels[MAX_OCCBANDS]; // Fixed length
#else
		int16_t occPrimaryThreshold[2];
		int16_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		char hostName[32];
		int32_t numBands;
		uint32_t numChannels[MAX_OCCBANDS]; // Fixed length
#endif
		Units::Frequency::Raw frequencies[MAX_OCCCHANNELS]; // Variable length up to MAX_OCCCHANNELS
	};

	struct SFrequencyVsChannelRespV2 // OCC_FREQUENCY_RESULT V2 (variable length)
	{
		operator SFrequencyVsChannelRespV1(void) const // Convert to V1
		{
#ifndef CSMSPLUS_SRV_BUILT
			static const unsigned long maxOccChannels = MAX_OCCCHANNELS;
#else
			static const uint32_t maxOccChannels = MAX_OCCCHANNELS;
#endif
			SFrequencyVsChannelRespV1 v1;
			v1.occHdr = occHdr;
			v1.occHdr.numChannels = (occHdr.numChannels <= std::min(maxOccChannels, occHdr.numTotalChannels) ? occHdr.numChannels : 0);

			for(size_t i = 0; i < v1.occHdr.numChannels; ++i)
			{
				v1.frequencies[i] = frequencies[i];
			}
#ifndef CSMSPLUS_SRV_BUILT
			v1.numBands = (numBands <= static_cast<int> (MAX_OCCBANDS) ? numBands : static_cast<int> (MAX_OCCBANDS));
#else
			v1.numBands = (numBands <= static_cast<int32_t> (MAX_OCCBANDS) ? numBands : static_cast<int32_t> (MAX_OCCBANDS));
#endif
			memcpy(v1.numChannels, numChannels, sizeof(v1.numChannels));
			v1.saveIntermediateData = (saveIntermediateData ? 1 : 0);
			v1.useSecondaryThreshold = (useSecondaryThreshold ? 1 : 0);
			v1.occPrimaryThreshold[0] = occPrimaryThreshold[0];
			v1.occPrimaryThreshold[1] = occPrimaryThreshold[1];
			v1.occSecondaryThreshold[0] = occSecondaryThreshold[0];
			v1.occSecondaryThreshold[1] = occSecondaryThreshold[1];
			v1.selectedAntenna = selectedAntenna;
			strcpy_s(v1.hostName, hostName);

			return v1;
		}

		SOccupancyHeader occHdr;
		EAnt selectedAntenna;  //uses ANT1 & ANT2 defines in dsp_msg.h
		bool saveIntermediateData;
		bool useSecondaryThreshold;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1
		//  which thresholds used are based upon thresholdMethod in command
#ifndef CSMSPLUS_SRV_BUILT
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		char hostName[32];
		int numBands;
		unsigned long numChannels[MAX_OCCBANDS_CSMS_V3p]; // Fixed length
#else
		int16_t occPrimaryThreshold[2];
		int16_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		char hostName[32];
		int32_t numBands;
		uint32_t numChannels[MAX_OCCBANDS_CSMS_V3p]; // Fixed length
#endif
		Units::Frequency::Raw frequencies[MAX_OCCCHANNELS]; // Variable length up to MAX_OCCCHANNELS
	};

#ifndef SMS_SRV_BUILT
	typedef SFrequencyVsChannelRespV2 SFrequencyVsChannelResp;
#else
	typedef SFrequencyVsChannelRespV1 SFrequencyVsChannelResp;
#endif

	struct SFrequencyVsChannelRespForDmV1 // DM_OCC_FREQUENCY_RESULT V1 (variable length)
	{

		SOccupancyHeaderForDM occHdr;
		EAnt selectedAntenna;  //uses ANT1 & ANT2 defines in dsp_msg.h
		bool saveIntermediateData;
		bool useSecondaryThreshold;
		// following two parameters
		//  the FIXED threshold (dBuV/m) is always in index 0
		//  the NOISE threshold (dB) is always in index 1
		//  which thresholds used are based upon thresholdMethod in command
#ifndef CSMSPLUS_SRV_BUILT
		short occPrimaryThreshold[2];
		short occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		char hostName[32];
		int numBands;
		unsigned long numChannels[MAX_OCCBANDS_CSMS_V3p]; // Fixed length
#else
		int16_t occPrimaryThreshold[2];
		int16_t occSecondaryThreshold[2]; // used only if useSecondaryThreshold >0
		char hostName[32];
		int32_t numBands;
		uint32_t numChannels[MAX_OCCBANDS_CSMS_V3p]; // Fixed length
#endif
		Units::Frequency::Raw frequencies[MAX_OCCCHANNELS]; // Variable length up to MAX_OCCCHANNELS
	};

	typedef SFrequencyVsChannelRespForDmV1 SFrequencyVsChannelRespForDm;

	struct SScanDfVsChannelResp // SCANDF_DATA_RESULT
	{
		SOccupancyHeader occHdr;
#ifndef CSMSPLUS_SRV_BUILT
		int numAzimuths;
		int numChannels;
		int scanDfData[MAX_OCCCHANNELS];
		int aveRange[MAX_OCCCHANNELS];	// 1/100ths kilometers
		int aveFldStr[MAX_OCCCHANNELS];	// dBuV/m
#else
		int32_t numAzimuths;
		int32_t numChannels;
		int32_t scanDfData[MAX_OCCCHANNELS];
		int32_t aveRange[MAX_OCCCHANNELS];	// 1/100ths kilometers
		int32_t aveFldStr[MAX_OCCCHANNELS];	// dBuV/m
#endif
	};

	struct SSetAudioSwitchCmd // SET_SWITCH
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum EInput : unsigned long
#else
		enum EInput : uint32_t
#endif
		{
			DAC1		= 0,
			DAC2		= 1,
			DAC3		= 2,
			DAC4		= 3,
			SOUND_LEFT	= 4,
			SOUND_RIGHT	= 5,
			MIC			= 6,
			PHONE		= 7
		};

		EInput speaker;
		EInput soundBoardLeft;
		EInput soundBoardRight;
		EInput fskDemod;
		EInput lineAudio;
		EInput phone;
	};

	struct SSetAutoAnswerCmd // SET_AUTOANSWER_MODE, AUTOANSWER_STATUS
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum EMode : unsigned long
#else
		enum EMode : uint32_t
#endif
		{
			DATA	= 0,
			VOICE	= 1
		};

#ifndef CSMSPLUS_SRV_BUILT
		enum EHookState : unsigned long
#else
		enum EHookState : uint32_t
#endif
		{
			ONHOOK	= 0,
			OFFHOOK	= 1
		};

		EHookState onOff;
		EMode voiceData;
	};

	struct SRcvrLanCmd // SET_LAN_AUDIO
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum ECmd : unsigned long
#else
		enum ECmd : uint32_t
#endif
		{
			AUDIO_OFF =	0,
			AUDIO_ON =	1
		};

		ECmd			cmd;			// use above defines (may be others in future)
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	port;			// port ID
		unsigned long	timeout;		// seconds -- audio stays on this long
#else
		uint32_t	port;			// port ID
		uint32_t	timeout;		// seconds -- audio stays on this long
#endif
		char			ipAddr[20];		// IP address -> 123.123.123.123
	};

	struct SSetPhoneHookCmd // SET_PHONE_HOOL
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long voiceData;
		unsigned long offHook;
#else
		uint32_t voiceData;
		uint32_t offHook;
#endif
	};


	// Message versioning
	struct SVersionKey
	{
		bool operator<(const SVersionKey& b) const
			{ return msgType < b.msgType || (msgType == b.msgType && msgSubType < b.msgSubType); }
		bool operator==(const SVersionKey& b) const
		{
			return (msgType == b.msgType && msgSubType == b.msgSubType);
		}
#ifndef CSMSPLUS_SRV_BUILT
		unsigned short msgType;
		unsigned long msgSubType;
#else
		uint16_t msgType;
		uint32_t msgSubType;
#endif
	};

	struct SVersionValue
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char cmdVersion;
		unsigned char respVersion;
#else
		uint8_t cmdVersion;
		uint8_t respVersion;
#endif
	};

	struct SVersionData
	{
		SVersionKey msgId;
		SVersionValue version;
	};

	typedef std::map<SVersionKey, SVersionValue> VersionMap;


	// Message body structures
	struct SAntGetSetCtrlResp // GET_ANT_RESPONSE, SET_ANT_RESPONSE
	{
		ErrorCodes::EErrorCode	status;
		EAnt	antenna; // Antenna state
	};

	typedef SAntGetSetCtrlResp SAntGetSetCtrlRespV0;
	typedef SAntGetSetCtrlResp SAntGetSetCtrlRespV1;

	struct SAntSetCtrlCmdV0 // SET_ANT
	{
		EAnt antenna; // Antenna choice
	};

	typedef SAntSetCtrlCmdV0 SAntSetCtrlCmdV1;

	typedef SAntSetCtrlCmdV1 SAntSetCtrlCmd;

	struct SGenericRespV0 // SET_RCVR_RESP, SET_PAN_PARA_RESP, DO_DSP_INIT_RESPONSE, CLEAR_HISTORY_RESPONSE,
						// GOTO_FAST_MODE_RESPONSE, GOTO_STANDARD_MODE_RESPONSE, ROTATE_RESP,
						// START_RESUME_RESPONSE, STOP_PAUSE_RESPONSE, INIT_FIELDSTRENGTH_RESP
	{
		ErrorCodes::EErrorCode status;
	};

	typedef SGenericRespV0 SGenericRespV1;

	typedef SGenericRespV1 SGenericResp;

	struct SGetBandRespV1; // Forward declaration

	struct SGetBandRespV0 // GET_BAND_RESPONSE V0
	{
		inline operator SGetBandRespV1(void) const; // Convert to V1; definition below

#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_BANDWIDTHS = 23;

		struct SBandwidthList
		{
			unsigned long	numBandwidths;
			unsigned long	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#else
		static const uint32_t MAX_BANDWIDTHS = 23;

		struct SBandwidthList
		{
			uint32_t	numBandwidths;
			uint32_t	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#endif
		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequency[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidth[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulation[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	fieldStrength[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFinding[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequency[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidth[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulation[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	fieldStrength[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFinding[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		EMetricHw		hfMetricHardwarePresent;
		EDfHw			hfDfHardwarePresent;
		EMetricHw		vuhfMetricHardwarePresent;
		EDfHw			vuhfDfHardwarePresent;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	hfMetricLowFreq;
		unsigned long	hfMetricHighFreq;
		unsigned long	hfDfLowFreq;
		unsigned long	hfDfHighFreq;
		unsigned long	vuhfMetricLowFreq;
		unsigned long	vuhfMetricHighFreq;
		unsigned long	vuhfDfLowFreq;
		unsigned long	vuhfDfHighFreq;
#else
		uint32_t	hfMetricLowFreq;
		uint32_t	hfMetricHighFreq;
		uint32_t	hfDfLowFreq;
		uint32_t	hfDfHighFreq;
		uint32_t	vuhfMetricLowFreq;
		uint32_t	vuhfMetricHighFreq;
		uint32_t	vuhfDfLowFreq;
		uint32_t	vuhfDfHighFreq;
#endif
		SBandwidthList	hfMeasurementBw;
		SBandwidthList	hfPanDisplayBw;
		SBandwidthList	hfOccupancyBw;
		SBandwidthList	hfDfScanBw;
		SBandwidthList	hfReceiverBw;
		SBandwidthList	vuhfMeasurementBw;
		SBandwidthList	vuhfPanDisplayBw;
		SBandwidthList	vuhfOccupancyBw;
		SBandwidthList	vuhfDfScanBw;
		SBandwidthList	vuhfReceiverBw;
		SDwellTimeList	hfMeasurementDwell;
		SDwellTimeList	vuhfMeasurementDwell;
	};

	struct SGetBandRespV2; // Forward declaration

	struct SGetBandRespV1 // GET_BAND_RESPONSE V1
	{
		operator SGetBandRespV0(void) const // Convert to V0
		{
			SGetBandRespV0 v0;
			v0.status = status;
			v0.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v0.hfDfHardwarePresent = hfDfHardwarePresent;
			v0.vuhfMetricHardwarePresent = vuhfMetricHardwarePresent;
			v0.vuhfDfHardwarePresent = vuhfDfHardwarePresent;
			v0.hfMetricLowFreq = hfMetricLowFreq;
			v0.hfMetricHighFreq = hfMetricHighFreq;
			v0.hfDfLowFreq = hfDfLowFreq;
			v0.hfDfHighFreq = hfDfHighFreq;
			v0.vuhfMetricLowFreq = vuhfMetricLowFreq;
			v0.vuhfMetricHighFreq = vuhfMetricHighFreq;
			v0.vuhfDfLowFreq = vuhfDfLowFreq;
			v0.vuhfDfHighFreq = vuhfDfHighFreq;
			v0.hfMeasurementBw.numBandwidths = (hfMeasurementBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : hfMeasurementBw.numBandwidths);
			v0.hfPanDisplayBw.numBandwidths = (hfPanDisplayBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : hfPanDisplayBw.numBandwidths);
			v0.hfOccupancyBw.numBandwidths = (hfOccupancyBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : hfOccupancyBw.numBandwidths);
			v0.hfDfScanBw.numBandwidths = (hfDfScanBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : hfDfScanBw.numBandwidths);
			v0.hfReceiverBw.numBandwidths = (hfReceiverBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : hfReceiverBw.numBandwidths);
			v0.vuhfMeasurementBw.numBandwidths = (vuhfMeasurementBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : vuhfMeasurementBw.numBandwidths);
			v0.vuhfPanDisplayBw.numBandwidths = (vuhfPanDisplayBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : vuhfPanDisplayBw.numBandwidths);
			v0.vuhfOccupancyBw.numBandwidths = (vuhfOccupancyBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : vuhfOccupancyBw.numBandwidths);
			v0.vuhfDfScanBw.numBandwidths = (vuhfDfScanBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : vuhfDfScanBw.numBandwidths);
			v0.vuhfReceiverBw.numBandwidths = (vuhfReceiverBw.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : vuhfReceiverBw.numBandwidths);
			v0.hfMeasurementDwell.numBandwidths = (hfMeasurementDwell.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : hfMeasurementDwell.numBandwidths);
			v0.vuhfMeasurementDwell.numBandwidths = (vuhfMeasurementDwell.numBandwidths > SGetBandRespV0::MAX_BANDWIDTHS ?
				SGetBandRespV0::MAX_BANDWIDTHS : vuhfMeasurementDwell.numBandwidths);
	
			for(size_t bw = 0; bw < SGetBandRespV0::MAX_BANDWIDTHS; ++bw)
			{
				v0.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
				v0.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
				v0.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
				v0.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
				v0.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
				v0.vuhfMeasurementBw.bandwidth[bw] = vuhfMeasurementBw.bandwidth[bw];
				v0.vuhfPanDisplayBw.bandwidth[bw] = vuhfPanDisplayBw.bandwidth[bw];
				v0.vuhfOccupancyBw.bandwidth[bw] = vuhfOccupancyBw.bandwidth[bw];
				v0.vuhfDfScanBw.bandwidth[bw] = vuhfDfScanBw.bandwidth[bw];
				v0.vuhfReceiverBw.bandwidth[bw] = vuhfReceiverBw.bandwidth[bw];
				v0.hfMeasurementDwell.frequency[bw] = hfMeasurementDwell.frequency[bw];
				v0.hfMeasurementDwell.bandwidth[bw] = hfMeasurementDwell.bandwidth[bw];
				v0.hfMeasurementDwell.modulation[bw] = hfMeasurementDwell.modulation[bw];
				v0.hfMeasurementDwell.fieldStrength[bw] = hfMeasurementDwell.fieldStrength[bw];
				v0.hfMeasurementDwell.directionFinding[bw] = hfMeasurementDwell.directionFinding[bw];
				v0.vuhfMeasurementDwell.frequency[bw] = vuhfMeasurementDwell.frequency[bw];
				v0.vuhfMeasurementDwell.bandwidth[bw] = vuhfMeasurementDwell.bandwidth[bw];
				v0.vuhfMeasurementDwell.modulation[bw] = vuhfMeasurementDwell.modulation[bw];
				v0.vuhfMeasurementDwell.fieldStrength[bw] = vuhfMeasurementDwell.fieldStrength[bw];
				v0.vuhfMeasurementDwell.directionFinding[bw] = vuhfMeasurementDwell.directionFinding[bw];
			}

			return v0;
		}

		inline operator SGetBandRespV2(void) const; // Convert to V2; definition below

#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			unsigned long	numBandwidths;
			unsigned long	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Units::Frequency internal representation
#else
		static const uint32_t MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			uint32_t	numBandwidths;
			uint32_t	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Units::Frequency internal representation
#endif
		};

		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequency[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidth[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulation[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	fieldStrength[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFinding[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequency[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidth[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulation[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	fieldStrength[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFinding[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		EMetricHw		hfMetricHardwarePresent;
		EDfHw			hfDfHardwarePresent;
		EMetricHw		vuhfMetricHardwarePresent;
		EDfHw			vuhfDfHardwarePresent;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	hfMetricLowFreq;
		unsigned long	hfMetricHighFreq;
		unsigned long	hfDfLowFreq;
		unsigned long	hfDfHighFreq;
		unsigned long	vuhfMetricLowFreq;
		unsigned long	vuhfMetricHighFreq;
		unsigned long	vuhfDfLowFreq;
		unsigned long	vuhfDfHighFreq;
#else
		uint32_t	hfMetricLowFreq;
		uint32_t	hfMetricHighFreq;
		uint32_t	hfDfLowFreq;
		uint32_t	hfDfHighFreq;
		uint32_t	vuhfMetricLowFreq;
		uint32_t	vuhfMetricHighFreq;
		uint32_t	vuhfDfLowFreq;
		uint32_t	vuhfDfHighFreq;
#endif
		SBandwidthList	hfMeasurementBw;
		SBandwidthList	hfPanDisplayBw;
		SBandwidthList	hfOccupancyBw;
		SBandwidthList	hfDfScanBw;
		SBandwidthList	hfReceiverBw;
		SBandwidthList	vuhfMeasurementBw;
		SBandwidthList	vuhfPanDisplayBw;
		SBandwidthList	vuhfOccupancyBw;
		SBandwidthList	vuhfDfScanBw;
		SBandwidthList	vuhfReceiverBw;
		SDwellTimeList	hfMeasurementDwell;
		SDwellTimeList	vuhfMeasurementDwell;
	};

	struct SGetBandRespV3; // Forward declaration

	struct SGetBandRespV2 // GET_BAND_RESPONSE V2
	{
		operator SGetBandRespV1(void) const // Convert to V1
		{
			SGetBandRespV1 v1;
			v1.status = status;
			v1.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v1.hfDfHardwarePresent = hfDfHardwarePresent;
			v1.vuhfMetricHardwarePresent = vushfMetricHardwarePresent;
			v1.vuhfDfHardwarePresent = vushfDfHardwarePresent;
#ifndef CSMSPLUS_SRV_BUILT
			v1.hfMetricLowFreq = Units::Frequency(hfMetricLowFreq).Hz<unsigned long>(true);
			v1.hfMetricHighFreq = Units::Frequency(hfMetricHighFreq).Hz<unsigned long>(true);
			v1.hfDfLowFreq = Units::Frequency(hfDfLowFreq).Hz<unsigned long>(true);
			v1.hfDfHighFreq = Units::Frequency(hfDfHighFreq).Hz<unsigned long>(true);
			v1.vuhfMetricLowFreq = Units::Frequency(vushfMetricLowFreq).Hz<unsigned long>(true);
			v1.vuhfMetricHighFreq = Units::Frequency(vushfMetricHighFreq).Hz<unsigned long>(true);
			v1.vuhfDfLowFreq = Units::Frequency(vushfDfLowFreq).Hz<unsigned long>(true);
			v1.vuhfDfHighFreq = Units::Frequency(vushfDfHighFreq).Hz<unsigned long>(true);
#else
			v1.hfMetricLowFreq = Units::Frequency(hfMetricLowFreq).Hz<uint32_t>(true);
			v1.hfMetricHighFreq = Units::Frequency(hfMetricHighFreq).Hz<uint32_t>(true);
			v1.hfDfLowFreq = Units::Frequency(hfDfLowFreq).Hz<uint32_t>(true);
			v1.hfDfHighFreq = Units::Frequency(hfDfHighFreq).Hz<uint32_t>(true);
			v1.vuhfMetricLowFreq = Units::Frequency(vushfMetricLowFreq).Hz<uint32_t>(true);
			v1.vuhfMetricHighFreq = Units::Frequency(vushfMetricHighFreq).Hz<uint32_t>(true);
			v1.vuhfDfLowFreq = Units::Frequency(vushfDfLowFreq).Hz<uint32_t>(true);
			v1.vuhfDfHighFreq = Units::Frequency(vushfDfHighFreq).Hz<uint32_t>(true);
#endif
			v1.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
			v1.hfPanDisplayBw.numBandwidths =hfPanDisplayBw.numBandwidths;
			v1.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
			v1.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
			v1.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
			v1.vuhfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
			v1.vuhfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
			v1.vuhfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
			v1.vuhfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
			v1.vuhfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
			v1.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
			v1.vuhfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

			for(size_t bw = 0; bw < SGetBandRespV1::MAX_BANDWIDTHS; ++bw)
			{
#ifndef CSMSPLUS_SRV_BUILT
				v1.hfMeasurementBw.bandwidth[bw] = Units::Frequency(hfMeasurementBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.hfPanDisplayBw.bandwidth[bw] = Units::Frequency(hfPanDisplayBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.hfOccupancyBw.bandwidth[bw] = Units::Frequency(hfOccupancyBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.hfDfScanBw.bandwidth[bw] = Units::Frequency(hfDfScanBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.hfReceiverBw.bandwidth[bw] = Units::Frequency(hfReceiverBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.vuhfMeasurementBw.bandwidth[bw] = Units::Frequency(vushfMeasurementBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.vuhfPanDisplayBw.bandwidth[bw] = Units::Frequency(vushfPanDisplayBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.vuhfOccupancyBw.bandwidth[bw] = Units::Frequency(vushfOccupancyBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.vuhfDfScanBw.bandwidth[bw] = Units::Frequency(vushfDfScanBw.bandwidth[bw]).Hz<unsigned long>(true);
				v1.vuhfReceiverBw.bandwidth[bw] = Units::Frequency(vushfReceiverBw.bandwidth[bw]).Hz<unsigned long>(true);
#else
				v1.hfMeasurementBw.bandwidth[bw] = Units::Frequency(hfMeasurementBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.hfPanDisplayBw.bandwidth[bw] = Units::Frequency(hfPanDisplayBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.hfOccupancyBw.bandwidth[bw] = Units::Frequency(hfOccupancyBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.hfDfScanBw.bandwidth[bw] = Units::Frequency(hfDfScanBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.hfReceiverBw.bandwidth[bw] = Units::Frequency(hfReceiverBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.vuhfMeasurementBw.bandwidth[bw] = Units::Frequency(vushfMeasurementBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.vuhfPanDisplayBw.bandwidth[bw] = Units::Frequency(vushfPanDisplayBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.vuhfOccupancyBw.bandwidth[bw] = Units::Frequency(vushfOccupancyBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.vuhfDfScanBw.bandwidth[bw] = Units::Frequency(vushfDfScanBw.bandwidth[bw]).Hz<uint32_t>(true);
				v1.vuhfReceiverBw.bandwidth[bw] = Units::Frequency(vushfReceiverBw.bandwidth[bw]).Hz<uint32_t>(true);
#endif
				v1.hfMeasurementDwell.frequency[bw] = hfMeasurementDwell.frequencyDwell[bw];
				v1.hfMeasurementDwell.bandwidth[bw] = hfMeasurementDwell.bandwidthDwell[bw];
				v1.hfMeasurementDwell.modulation[bw] = hfMeasurementDwell.modulationDwell[bw];
				v1.hfMeasurementDwell.fieldStrength[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
				v1.hfMeasurementDwell.directionFinding[bw] = hfMeasurementDwell.directionFindingDwell[bw];
				v1.vuhfMeasurementDwell.frequency[bw] = vushfMeasurementDwell.frequencyDwell[bw];
				v1.vuhfMeasurementDwell.bandwidth[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
				v1.vuhfMeasurementDwell.modulation[bw] = vushfMeasurementDwell.modulationDwell[bw];
				v1.vuhfMeasurementDwell.fieldStrength[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
				v1.vuhfMeasurementDwell.directionFinding[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
			}

			return v1;
		}

		inline operator SGetBandRespV3(void) const; // Convert to V3; definition below
		
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			unsigned long			numBandwidths;
#else
		static const uint32_t MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			uint32_t			numBandwidths;
#endif
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};

		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		EMetricHw				hfMetricHardwarePresent;
		EDfHw					hfDfHardwarePresent;
		EMetricHw				vushfMetricHardwarePresent;
		EDfHw					vushfDfHardwarePresent;
		Units::Frequency::Raw	hfMetricLowFreq;
		Units::Frequency::Raw	hfMetricHighFreq;
		Units::Frequency::Raw	hfDfLowFreq;
		Units::Frequency::Raw	hfDfHighFreq;
		Units::Frequency::Raw	vushfMetricLowFreq;
		Units::Frequency::Raw	vushfMetricHighFreq;
		Units::Frequency::Raw	vushfDfLowFreq;
		Units::Frequency::Raw	vushfDfHighFreq;
		SBandwidthList			hfMeasurementBw;
		SBandwidthList			hfPanDisplayBw;
		SBandwidthList			hfOccupancyBw;
		SBandwidthList			hfDfScanBw;
		SBandwidthList			hfReceiverBw;
		SBandwidthList			vushfMeasurementBw;
		SBandwidthList			vushfPanDisplayBw;
		SBandwidthList			vushfOccupancyBw;
		SBandwidthList			vushfDfScanBw;
		SBandwidthList			vushfReceiverBw;
		SDwellTimeList			hfMeasurementDwell;
		SDwellTimeList			vushfMeasurementDwell;
	};

	static const Units::Frequency	NO_HORIZON_LOW_FREQ;
	static const Units::Frequency	NO_HORIZON_HIGH_FREQ;
	static const Units::Frequency	NO_VERTICAL_LOW_FREQ;
	static const Units::Frequency	NO_VERTICAL_HIGH_FREQ;
	
	struct SGetBandRespV4; // Forward declaration

	struct SGetBandRespV3 // GET_BAND_RESPONSE V3
	{
		operator SGetBandRespV2(void) const // Convert to V2
		{
			SGetBandRespV2 v2;
			v2.status = status;
			v2.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v2.hfDfHardwarePresent = hfDfHardwarePresent;
			v2.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
			v2.vushfDfHardwarePresent = vushfDfHardwarePresent;
			v2.hfMetricLowFreq = hfMetricLowFreq;
			v2.hfMetricHighFreq = hfMetricHighFreq;
			v2.hfDfLowFreq = hfDfLowFreq;
			v2.hfDfHighFreq = hfDfHighFreq;
			v2.vushfMetricLowFreq = vushfMetricVLowFreq;
			v2.vushfMetricHighFreq = vushfMetricVHighFreq;
			v2.vushfDfLowFreq = vushfDfVLowFreq;
			v2.vushfDfHighFreq = vushfDfVHighFreq;
			v2.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
			v2.hfPanDisplayBw.numBandwidths =hfPanDisplayBw.numBandwidths;
			v2.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
			v2.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
			v2.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
			v2.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
			v2.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
			v2.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
			v2.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
			v2.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
			v2.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
			v2.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

			for(size_t bw = 0; bw < SGetBandRespV2::MAX_BANDWIDTHS; ++bw)
			{
				v2.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
				v2.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
				v2.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
				v2.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
				v2.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
				v2.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
				v2.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
				v2.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
				v2.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
				v2.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
				v2.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
				v2.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
				v2.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.modulationDwell[bw];
				v2.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
				v2.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
				v2.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
				v2.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
				v2.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
				v2.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
				v2.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
			}

			return v2;
		}

		inline operator SGetBandRespV4(void) const; // Convert to V4; definition below

#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			unsigned long			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#else
		static const uint32_t MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			uint32_t			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#endif
		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		EMetricHw				hfMetricHardwarePresent;
		EDfHw					hfDfHardwarePresent;
		EMetricHw				vushfMetricHardwarePresent;
		EDfHw					vushfDfHardwarePresent;
		Units::Frequency::Raw	hfMetricLowFreq;
		Units::Frequency::Raw	hfMetricHighFreq;
		Units::Frequency::Raw	hfDfLowFreq;
		Units::Frequency::Raw	hfDfHighFreq;
		Units::Frequency::Raw	vushfMetricHLowFreq;
		Units::Frequency::Raw	vushfMetricHHighFreq;
		Units::Frequency::Raw	vushfMetricVLowFreq;
		Units::Frequency::Raw	vushfMetricVHighFreq;
		Units::Frequency::Raw	vushfDfHLowFreq;
		Units::Frequency::Raw	vushfDfHHighFreq;
		Units::Frequency::Raw	vushfDfVLowFreq;
		Units::Frequency::Raw	vushfDfVHighFreq;
		SBandwidthList			hfMeasurementBw;
		SBandwidthList			hfPanDisplayBw;
		SBandwidthList			hfOccupancyBw;
		SBandwidthList			hfDfScanBw;
		SBandwidthList			hfReceiverBw;
		SBandwidthList			vushfMeasurementBw;
		SBandwidthList			vushfPanDisplayBw;
		SBandwidthList			vushfOccupancyBw;
		SBandwidthList			vushfDfScanBw;
		SBandwidthList			vushfReceiverBw;
		SDwellTimeList			hfMeasurementDwell;
		SDwellTimeList			vushfMeasurementDwell;
	};
	
	struct SGetBandRespV5; // Forward declaration

	struct SGetBandRespV4 // GET_BAND_RESPONSE V4
	{
		operator SGetBandRespV3(void) const // Convert to V3
		{
			SGetBandRespV3 v3;
			v3.status = status;
			v3.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v3.hfDfHardwarePresent = hfDfHardwarePresent;
			v3.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
			v3.vushfDfHardwarePresent = vushfDfHardwarePresent;
			v3.hfMetricLowFreq = hfMetricLowFreq;
			v3.hfMetricHighFreq = hfMetricHighFreq;
			v3.hfDfLowFreq = hfDfLowFreq;
			v3.hfDfHighFreq = hfDfHighFreq;
			v3.vushfMetricHLowFreq = vushfMetricHLowFreq;
			v3.vushfMetricHHighFreq = vushfMetricHHighFreq;
			v3.vushfMetricVLowFreq = vushfMetricVLowFreq;
			v3.vushfMetricVHighFreq = vushfMetricVHighFreq;
			v3.vushfDfHLowFreq = vushfDfHLowFreq;
			v3.vushfDfHHighFreq = vushfDfHHighFreq;
			v3.vushfDfVLowFreq = vushfDfVLowFreq;
			v3.vushfDfVHighFreq = vushfDfVHighFreq;
			v3.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
			v3.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
			v3.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
			v3.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
			v3.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
			v3.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
			v3.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
			v3.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
			v3.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
			v3.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
			v3.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
			v3.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

			for(size_t bw = 0; bw < SGetBandRespV3::MAX_BANDWIDTHS; ++bw)
			{
				v3.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
				v3.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
				v3.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
				v3.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
				v3.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
				v3.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
				v3.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
				v3.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
				v3.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
				v3.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
				v3.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
				v3.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
				v3.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.modulationDwell[bw];
				v3.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
				v3.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
				v3.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
				v3.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
				v3.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
				v3.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
				v3.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
			}

			return v3;
		}

		inline operator SGetBandRespV5(void) const; // Convert to V5; definition below

#ifndef CSMSPLUS_SRV_BUILT
		//static const unsigned long MAX_BANDWIDTHS = 33;
		static const unsigned long MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			unsigned long			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#else
		//static const unsigned long MAX_BANDWIDTHS = 33;
		static const uint32_t MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			uint32_t			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#endif
		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		EMetricHw				hfMetricHardwarePresent;
		EDfHw					hfDfHardwarePresent;
		EMetricHw				vushfMetricHardwarePresent;
		EDfHw					vushfDfHardwarePresent;
		ETdoaHw					iqDataCapability;
		Units::Frequency::Raw	hfMetricLowFreq;
		Units::Frequency::Raw	hfMetricHighFreq;
		Units::Frequency::Raw	hfDfLowFreq;
		Units::Frequency::Raw	hfDfHighFreq;
		Units::Frequency::Raw	vushfMetricHLowFreq;
		Units::Frequency::Raw	vushfMetricHHighFreq;
		Units::Frequency::Raw	vushfMetricVLowFreq;
		Units::Frequency::Raw	vushfMetricVHighFreq;
		Units::Frequency::Raw	vushfDfHLowFreq;
		Units::Frequency::Raw	vushfDfHHighFreq;
		Units::Frequency::Raw	vushfDfVLowFreq;
		Units::Frequency::Raw	vushfDfVHighFreq;
		SBandwidthList			hfMeasurementBw;
		SBandwidthList			hfPanDisplayBw;
		SBandwidthList			hfOccupancyBw;
		SBandwidthList			hfDfScanBw;
		SBandwidthList			hfReceiverBw;
		SBandwidthList			vushfMeasurementBw;
		SBandwidthList			vushfPanDisplayBw;
		SBandwidthList			vushfOccupancyBw;
		SBandwidthList			vushfDfScanBw;
		SBandwidthList			vushfReceiverBw;
		SDwellTimeList			hfMeasurementDwell;
		SDwellTimeList			vushfMeasurementDwell;
	};
	
	struct SGetBandRespV6; // Forward declaration

	struct SGetBandRespV5 // GET_BAND_RESPONSE V5
	{
		operator SGetBandRespV4(void) const // Convert to V4
		{
			SGetBandRespV4 v4;
			v4.status = status;
			v4.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v4.hfDfHardwarePresent = hfDfHardwarePresent;

			if( SSmsMsg::Is5141Type(vushfMetricHardwarePresent) )
			{
				//v4 does not recognize TCI5141 value.
				v4.vushfMetricHardwarePresent = TCI8068;
			}
			else
			{
				v4.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
			}

			v4.vushfDfHardwarePresent = vushfDfHardwarePresent;
			v4.iqDataCapability = iqDataCapability;
			v4.hfMetricLowFreq = hfMetricLowFreq;
			v4.hfMetricHighFreq = hfMetricHighFreq;
			v4.hfDfLowFreq = hfDfLowFreq;
			v4.hfDfHighFreq = hfDfHighFreq;
			v4.vushfMetricHLowFreq = vushfMetricHLowFreq;
			v4.vushfMetricHHighFreq = vushfMetricHHighFreq;
			v4.vushfMetricVLowFreq = vushfMetricVLowFreq;
			v4.vushfMetricVHighFreq = vushfMetricVHighFreq;
			v4.vushfDfHLowFreq = vushfDfHLowFreq;
			v4.vushfDfHHighFreq = vushfDfHHighFreq;
			v4.vushfDfVLowFreq = vushfDfVLowFreq;
			v4.vushfDfVHighFreq = vushfDfVHighFreq;
			v4.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
			v4.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
			v4.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
			v4.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
			v4.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
			v4.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
			v4.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
			v4.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
			v4.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
			v4.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
			v4.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
			v4.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

			for(size_t bw = 0; bw < SGetBandRespV4::MAX_BANDWIDTHS; ++bw)
			{
				v4.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
				v4.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
				v4.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
				v4.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
				v4.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
				v4.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
				v4.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
				v4.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
				v4.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
				v4.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
				v4.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
				v4.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
				v4.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.modulationDwell[bw];
				v4.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
				v4.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
				v4.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
				v4.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
				v4.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
				v4.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
				v4.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
			}

			return v4;
		}

		inline operator SGetBandRespV6(void) const; // Convert to V6; definition below

#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			unsigned long			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#else
		static const uint32_t MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			uint32_t			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#endif
		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		ESiteInfo				siteInfo;
		EMetricHw				hfMetricHardwarePresent;
		EDfHw					hfDfHardwarePresent;
		EMetricHw				vushfMetricHardwarePresent;
		EDfHw					vushfDfHardwarePresent;
		ETdoaHw					iqDataCapability;
		Units::Frequency::Raw	hfMetricLowFreq;
		Units::Frequency::Raw	hfMetricHighFreq;
		Units::Frequency::Raw	hfDfLowFreq;
		Units::Frequency::Raw	hfDfHighFreq;
		Units::Frequency::Raw	vushfMetricHLowFreq;
		Units::Frequency::Raw	vushfMetricHHighFreq;
		Units::Frequency::Raw	vushfMetricVLowFreq;
		Units::Frequency::Raw	vushfMetricVHighFreq;
		Units::Frequency::Raw	vushfDfHLowFreq;
		Units::Frequency::Raw	vushfDfHHighFreq;
		Units::Frequency::Raw	vushfDfVLowFreq;
		Units::Frequency::Raw	vushfDfVHighFreq;
		SBandwidthList			hfMeasurementBw;
		SBandwidthList			hfPanDisplayBw;
		SBandwidthList			hfOccupancyBw;
		SBandwidthList			hfDfScanBw;
		SBandwidthList			hfReceiverBw;
		SBandwidthList			vushfMeasurementBw;
		SBandwidthList			vushfPanDisplayBw;
		SBandwidthList			vushfOccupancyBw;
		SBandwidthList			vushfDfScanBw;
		SBandwidthList			vushfReceiverBw;
		SDwellTimeList			hfMeasurementDwell;
		SDwellTimeList			vushfMeasurementDwell;
	};
	
	struct SGetBandRespV7;	// Forward declaration

	struct SGetBandRespV6 // GET_BAND_RESPONSE V6
	{
		operator SGetBandRespV5(void) const // Convert to V5
		{
			SGetBandRespV5 v5;
			v5.status = status;
			v5.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v5.hfDfHardwarePresent = hfDfHardwarePresent;
			v5.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
			v5.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
			v5.vushfDfHardwarePresent = vushfDfHardwarePresent;
			v5.iqDataCapability = iqDataCapability;
			v5.hfMetricLowFreq = hfMetricLowFreq;
			v5.hfMetricHighFreq = hfMetricHighFreq;
			v5.hfDfLowFreq = hfDfLowFreq;
			v5.hfDfHighFreq = hfDfHighFreq;
			v5.vushfMetricHLowFreq = vushfMetricHLowFreq;
			v5.vushfMetricHHighFreq = vushfMetricHHighFreq;
			v5.vushfMetricVLowFreq = vushfMetricVLowFreq;
			v5.vushfMetricVHighFreq = vushfMetricVHighFreq;
			v5.vushfDfHLowFreq = vushfDfHLowFreq;
			v5.vushfDfHHighFreq = vushfDfHHighFreq;
			v5.vushfDfVLowFreq = vushfDfVLowFreq;
			v5.vushfDfVHighFreq = vushfDfVHighFreq;
			v5.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
			v5.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
			v5.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
			v5.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
			v5.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
			v5.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
			v5.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
			v5.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
			v5.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
			v5.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
			v5.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
			v5.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

			for (size_t bw = 0; bw < SGetBandRespV4::MAX_BANDWIDTHS; ++bw)
			{
				v5.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
				v5.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
				v5.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
				v5.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
				v5.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
				v5.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
				v5.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
				v5.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
				v5.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
				v5.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
				v5.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
				v5.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
				v5.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.modulationDwell[bw];
				v5.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
				v5.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
				v5.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
				v5.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
				v5.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
				v5.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
				v5.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
			}

			return v5;
		}

		inline operator SGetBandRespV7(void) const;	// Convert to V7; definition below

#ifndef CSMSPLUS_SRV_BUILT
		//static const unsigned long MAX_BANDWIDTHS = 33;
		static const unsigned long MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			unsigned long			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#else
		//static const unsigned long MAX_BANDWIDTHS = 33;
		static const uint32_t MAX_BANDWIDTHS = 32;

		struct SBandwidthList
		{
			uint32_t			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
#endif
		struct SDwellTimeList
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#else
			uint32_t	numBandwidths;
			uint32_t	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
#endif
		};

		ErrorCodes::EErrorCode	status;
		ESiteInfo				siteInfo;
		EMetricHw				hfMetricHardwarePresent;
		EDfHw					hfDfHardwarePresent;
		EIonoInfoSource         hfIonoInfoSourceType;
		EMetricHw				vushfMetricHardwarePresent;
		EDfHw					vushfDfHardwarePresent;
		ETdoaHw					iqDataCapability;
		Units::Frequency::Raw	hfMetricLowFreq;
		Units::Frequency::Raw	hfMetricHighFreq;
		Units::Frequency::Raw	hfDfLowFreq;
		Units::Frequency::Raw	hfDfHighFreq;
		Units::Frequency::Raw	vushfMetricHLowFreq;
		Units::Frequency::Raw	vushfMetricHHighFreq;
		Units::Frequency::Raw	vushfMetricVLowFreq;
		Units::Frequency::Raw	vushfMetricVHighFreq;
		Units::Frequency::Raw	vushfDfHLowFreq;
		Units::Frequency::Raw	vushfDfHHighFreq;
		Units::Frequency::Raw	vushfDfVLowFreq;
		Units::Frequency::Raw	vushfDfVHighFreq;
		SBandwidthList			hfMeasurementBw;
		SBandwidthList			hfPanDisplayBw;
		SBandwidthList			hfOccupancyBw;
		SBandwidthList			hfDfScanBw;
		SBandwidthList			hfReceiverBw;
		SBandwidthList			vushfMeasurementBw;
		SBandwidthList			vushfPanDisplayBw;
		SBandwidthList			vushfOccupancyBw;
		SBandwidthList			vushfDfScanBw;
		SBandwidthList			vushfReceiverBw;
		SDwellTimeList			hfMeasurementDwell;
		SDwellTimeList			vushfMeasurementDwell;
	};

	struct SGetBandRespV7 // GET_BAND_RESPONSE V7
	{
		operator SGetBandRespV6(void) const // Convert to V6
		{
			SGetBandRespV6 v6;
			v6.status = status;
			v6.siteInfo = siteInfo;
			v6.hfMetricHardwarePresent = hfMetricHardwarePresent;
			v6.hfDfHardwarePresent = hfDfHardwarePresent;
			v6.hfIonoInfoSourceType = hfIonoInfoSourceType;
			v6.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
			v6.vushfDfHardwarePresent = vushfDfHardwarePresent;
			v6.iqDataCapability = iqDataCapability;
			v6.hfMetricLowFreq = hfMetricLowFreq;
			v6.hfMetricHighFreq = hfMetricHighFreq;
			v6.hfDfLowFreq = hfDfLowFreq;
			v6.hfDfHighFreq = hfDfHighFreq;
			v6.vushfMetricHLowFreq = vushfMetricHLowFreq;
			v6.vushfMetricHHighFreq = vushfMetricHHighFreq;
			v6.vushfMetricVLowFreq = vushfMetricVLowFreq;
			v6.vushfMetricVHighFreq = vushfMetricVHighFreq;
			v6.vushfDfHLowFreq = vushfDfHLowFreq;
			v6.vushfDfHHighFreq = vushfDfHHighFreq;
			v6.vushfDfVLowFreq = vushfDfVLowFreq;
			v6.vushfDfVHighFreq = vushfDfVHighFreq;
			v6.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
			v6.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
			v6.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
			v6.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
			v6.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
			v6.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
			v6.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
			v6.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
			v6.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
			v6.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
			v6.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
			v6.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

			for (size_t bw = 0; bw < SGetBandRespV6::MAX_BANDWIDTHS; ++bw)
			{
				v6.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
				v6.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
				v6.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
				v6.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
				v6.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
				v6.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
				v6.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
				v6.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
				v6.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
				v6.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
				v6.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
				v6.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
				v6.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.modulationDwell[bw];
				v6.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
				v6.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
				v6.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
				v6.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
				v6.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
				v6.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
				v6.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
			}

			return v6;
		}

#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long MAX_BANDWIDTHS = 32;
		static const unsigned long MAX_DDCBANDWIDTHS = 80;

		struct SBandwidthList
		{
			unsigned long			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
		struct SDdcBandwidthList
		{
			unsigned long			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_DDCBANDWIDTHS];  // bandwidth in Hz
		};
#else
		//static const unsigned long MAX_BANDWIDTHS = 33;
		static const uint32_t MAX_BANDWIDTHS = 32;
		static const uint32_t MAX_DDCBANDWIDTHS = 80;

		struct SBandwidthList
		{
			uint32_t			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_BANDWIDTHS];  // bandwidth in Hz
		};
		struct SDdcBandwidthList
		{
			uint32_t			numBandwidths;
			Units::Frequency::Raw	bandwidth[MAX_DDCBANDWIDTHS];  // bandwidth in Hz
		};
#endif

		struct SDwellTimeList
		{
	#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBandwidths;
			unsigned long	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			unsigned long	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			unsigned long	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
	#else
			uint32_t	numBandwidths;
			uint32_t	frequencyDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	bandwidthDwell[MAX_BANDWIDTHS];			// dwell time in ms
			uint32_t	modulationDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	fieldStrengthDwell[MAX_BANDWIDTHS];		// dwell time in ms
			uint32_t	directionFindingDwell[MAX_BANDWIDTHS];	// dwell time in ms
	#endif
		};

		ErrorCodes::EErrorCode	status;
		ESiteInfo				siteInfo;
		EMetricHw				hfMetricHardwarePresent;
		EDfHw					hfDfHardwarePresent;
		EIonoInfoSource         hfIonoInfoSourceType;
		EMetricHw				vushfMetricHardwarePresent;
		EDfHw					vushfDfHardwarePresent;
		ETdoaHw					iqDataCapability;
		Units::Frequency::Raw	hfMetricLowFreq;
		Units::Frequency::Raw	hfMetricHighFreq;
		Units::Frequency::Raw	hfDfLowFreq;
		Units::Frequency::Raw	hfDfHighFreq;
		Units::Frequency::Raw	vushfMetricHLowFreq;
		Units::Frequency::Raw	vushfMetricHHighFreq;
		Units::Frequency::Raw	vushfMetricVLowFreq;
		Units::Frequency::Raw	vushfMetricVHighFreq;
		Units::Frequency::Raw	vushfDfHLowFreq;
		Units::Frequency::Raw	vushfDfHHighFreq;
		Units::Frequency::Raw	vushfDfVLowFreq;
		Units::Frequency::Raw	vushfDfVHighFreq;
		SBandwidthList			hfMeasurementBw;
		SBandwidthList			hfPanDisplayBw;
		SBandwidthList			hfOccupancyBw;
		SBandwidthList			hfDfScanBw;
		SBandwidthList			hfReceiverBw;
		SDdcBandwidthList		hfDdcBw;
		SBandwidthList			vushfMeasurementBw;
		SBandwidthList			vushfPanDisplayBw;
		SBandwidthList			vushfOccupancyBw;
		SBandwidthList			vushfDfScanBw;
		SBandwidthList			vushfReceiverBw;
		SDdcBandwidthList		vushfDdcBw;
		SDwellTimeList			hfMeasurementDwell;
		SDwellTimeList			vushfMeasurementDwell;
	};

	typedef SGetBandRespV7 SGetBandResp;

	struct SGetBistResp // BIST_RESPONSE, GET_BIST_RESPONSE, GET_DIAGNOSTICS_RESPONSE, GET_BIST_RESULT_RESPONSE (variable length)
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum EResult : unsigned long
#else
		enum EResult : uint32_t
#endif
		{
			PASS,
			FAIL,
			WARNING
		} result;

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long textLen;
#else
		uint32_t textLen;
#endif
		bool last; // Last message of complete BIST result
#ifdef _MSC_VER
		wchar_t text[1]; // Variable length (textLen); "test name\tresult string"
#else
		char16_t text[1];
#endif
	};

	struct SGetBistRespOld
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	status;			// Used for go/nogo
		double			startDateTime;	// Using COleTime format
		double			completionDateTime;
		unsigned long	pcStatus;
		unsigned long	gpsStatus;
		unsigned long	fluxGateStatus;
		unsigned long	cv8Status;
		unsigned long	clockSynthStatus;
		unsigned long	access256Status;
		unsigned long	audioCardStatus;
		unsigned long	hfDfInterfaceCardStatus;
		unsigned long	hfChassisStatus;
		unsigned long	hfDfReceiverStatus;
		unsigned long	uhfDfMuxIfStatus;
		unsigned long	hfReceiverStatus;
		unsigned long	hfDfSwitchStatus;
		unsigned long	hfAntennaStatus;
		unsigned long	vhfReceiverStatus;
		unsigned long	uhfDfSwitchStatus;
		unsigned long	uhfDfMuxRfStatus;
		unsigned long	uhfAntennaStatus;
		unsigned long	vhfDfSwitchStatus;
		unsigned long	vhfAntennaStatus;
#else
		uint32_t	status;			// Used for go/nogo
		double			startDateTime;	// Using COleTime format
		double			completionDateTime;
		uint32_t	pcStatus;
		uint32_t	gpsStatus;
		uint32_t	fluxGateStatus;
		uint32_t	cv8Status;
		uint32_t	clockSynthStatus;
		uint32_t	access256Status;
		uint32_t	audioCardStatus;
		uint32_t	hfDfInterfaceCardStatus;
		uint32_t	hfChassisStatus;
		uint32_t	hfDfReceiverStatus;
		uint32_t	uhfDfMuxIfStatus;
		uint32_t	hfReceiverStatus;
		uint32_t	hfDfSwitchStatus;
		uint32_t	hfAntennaStatus;
		uint32_t	vhfReceiverStatus;
		uint32_t	uhfDfSwitchStatus;
		uint32_t	uhfDfMuxRfStatus;
		uint32_t	uhfAntennaStatus;
		uint32_t	vhfDfSwitchStatus;
		uint32_t	vhfAntennaStatus;
#endif
	};

	struct SGetMsgVersionsDataV0 // GET_MSG_VERSIONS, GET_MSG_VERSIONS_RESP (variable length)
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long count;
#else
		uint32_t count;
#endif
		SVersionData versionData[1]; // Variable length
	};

	typedef SGetMsgVersionsDataV0 SGetMsgVersionsDataV1; // GET_MSG_VERSIONS, GET_MSG_VERSIONS_RESP V1 (variable length, same as V0 except body is not compressed)

	typedef SGetMsgVersionsDataV1 SGetMsgVersionsData; // GET_MSG_VERSIONS, GET_MSG_VERSIONS_RESP V0 (variable length)

	struct SGetFaultRespV1	 // GET_FAULT_RESP. There is no version 0.
	{
		EErrStatus	cond;	//Overall system fault condition.
							//Valid value: PASS_STAT, FAIL_STAT, WARNING_STAT.

		struct SFaultDetails
		{
			EErrStatus	bistCond; //BIST result: 5093 Processor front panel CPU Green LED off when software not running.
								//FAIL_STAT: 5093 Processor front panel CPU Green LED flash fast.
								//PASS_STAT/WARNING_STAT: 5093 Processor front panel CPU Green LED on solid.
								//BIST_STAT: 5093 Processor front panel CPU Green LED flash slow.
			EErrStatus	tenMHzCond; //10 MHz status:
									//PASS_STAT: 5093 Processor front panel 10 MHz Green LED on solid.
									//WARNING_STAT: 5093 Processor front panel 10 MHz Green LED toggle on & off.
									//FAIL_STAT: 5093 Processor front panel 10 MHz Green LED off.
			EErrStatus	adcCond;	//A/D clock status:
									//PASS_STAT: 5093 Processor front panel A/D CLK Green LED on.
									//FAIL_STAT: 5093 Processor front panel A/D CLK Green LED off.
			EErrStatus	dspACond;	//Reference DSP sampling activity status:
									//ON_STAT: 5093 Processor front panel DSP A Yellow LED on.
									//OFF_STAT: 5093 Processor front panel DSP A Yellow LED off.
			EErrStatus	dspBCond;	//Sample DSP sampling activity status:
									//ON_STAT: 5093 Processor front panel DSP B Yellow LED on.
									//OFF_STAT: 5093 Processor front panel DSP B Yellow LED off.
			EErrStatus	synthCond;	//Synthesizer fault (Fail = Oscillator unlock):
									//PASS_STAT: 5093 Processor front panel Synth Fault Red LED off.
									//FAIL_STAT: 5093 Processor front panel Synth Fault Red LED on.
			EErrStatus	v5Cond;		//5V power status:
									//PASS_STAT: 5093 Processor front panel +5 VDC Green LED on.
									//WARNING_STAT: 5093 Processor front panel +5 VDC Green LED on.
			float		v5Val;		//Measured 5V value.
			EErrStatus	v12Cond;	//12V power status:
									//PASS_STAT: 5093 Processor front panel +12 VDC Green LED on.
									//WARNING_STAT: 5093 Processor front panel +12 VDC Green LED on.
			float		v12Val;		//Measured 12V value.
			EErrStatus	vNeg12Cond;	//-12V power status:
									//PASS_STAT: 5093 Processor front panel -12 VDC Green LED on.
									//WARNING_STAT: 5093 Processor front panel -12 VDC Green LED on.
			float		vNeg12Val;	//Measured -12V value.
			EErrStatus	v24Cond;	//24V power status:
									//PASS_STAT: 5093 Processor front panel +24 VDC Green LED on.
									//WARNING_STAT: 5093 Processor front panel +24 VDC Green LED on.
									//NO_VAL_STAT: 24V value not available(5141/Beehive system).
			float		v24Val;		//Measured 24V value.
									//NO_VAL_STAT: Has standard default value.
			EErrStatus	vNeg48Cond;	//-48V power status:
									//PASS_STAT: 5093 Processor front panel -48 VDC Green LED on.
									//WARNING_STAT: 5093 Processor front panel -48 VDC Green LED on.
									//NO_VAL_STAT: -48V value not available(5141/Beehive system).
			float		vNeg48Val;	//Measured -48V value.
									//NO_VAL_STAT: Has standard default value.
			EErrStatus	overTempCond; //Over temperature status:
									//PASS_STAT: 5093 Processor front panel OVER TEMP Red LED off.
									//FAIL_STAT: 5093 Processor front panel OVER TEMP Red LED on.
									//WARNING_STAT: 5141 either T1 or T2 temperature over 50 celsius.
			float		tempVal;	//Measured temperature value in celsius.
			EErrStatus	fanCond;	//Fan Speed status(available in 5141 only):
									//PASS_STAT: Normal Fan speed.
									//FAIL_STAT: Fan speed slower than expected.
									//NO_VAL_STAT: Fan speed not available(non 5141).
			float		fanVal;		//Measured fan speed in rpm.
									//NO_VAL_STAT: Has standard default value.
		} env;	//Specific Environmental fault condition.

	};

	typedef SGetFaultRespV1 SGetFaultResp;

	struct SGetCsmsFaultRespV1	 // GET_CSMS_FAULT_RESP. There is no version 0.
	{
		struct SfItem
		{
			float fval;
			EErrStatus cond;
		};
		struct SulItem
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long ulval;
#else
			uint32_t ulval;
#endif
			EErrStatus cond;
		};

		EErrStatus	cond;	//Overall system fault condition.
							//Valid value: PASS_STAT, FAIL_STAT, WARNING_STAT.
		struct SFaultDetails
		{
			EErrStatus bistCond;	// added 8/18/16
			SfItem ZYNQ_INTERNAL_TEMP;
			SfItem BOARD_TEMP;
			SfItem ZYNQ_INTERNAL_VCCINT;
			SfItem ZYNQ_INTERNAL_VCCAUX;
			SfItem ZYNQ_INTERNAL_VCCBRAM;
			SfItem ZYNQ_INTERNAL_VCCPINT;
			SfItem ZYNQ_INTERNAL_VCCPAUX;
			SfItem ZYNQ_INTERNAL_VCCODDR;
			SfItem BOARD_5p3V_INPUT;
			SfItem DIGITAL_3p3V;
			SfItem DIGITAL_2p5V;
			SfItem MGT_1p0V;
			SfItem MGT_1p2V;
			SfItem MGT_1p8V;
			SfItem PS_DDR3_VTT;
			SfItem PL_DDR3_VTT;
			SfItem BOARD_TOTAL_CURR_5p3V;
			SfItem ADC_3p0V_ANALOG;
			SfItem ADC_1p8V_ANALOG;
			SfItem ADC_1p2V_ANALOG;
			SfItem CLOCK_3p3_ANALOG;
			SfItem CLOCK_3p3_DIGITAL;
			SfItem TEMP_REF_2p5V;
			SulItem clock368MHz;
			SulItem clock100MHz;
			SulItem timeStatus;
			SfItem radioTemp;
			SfItem radioTemp1;		// added 6/2/16
			SfItem radioTemp2;		// added 6/2/16
			SulItem ocxoLock;
		};
		SFaultDetails env;	//Specific Environmental fault condition.
	};

	struct SGetCsmsFaultRespV2	 // GET_CSMS_FAULT_RESP.
	{
		operator SGetCsmsFaultRespV1(void) const	// Convert to V1
		{
			SGetCsmsFaultRespV1 v1;
			v1.cond = cond;
			v1.env = env;
			return v1;
		}

		SGetCsmsFaultRespV2& operator=(SGetCsmsFaultRespV1& v1)	// Convert from V1
		{
			cond = v1.cond;
			env = v1.env;
			numSlaves = 0;
			return *this;
		}
#ifndef CSMSPLUS_SRV_BUILT
		static const int MAX_SLAVES = 3;
#else
		static const int32_t MAX_SLAVES = 3;
#endif
		typedef SGetCsmsFaultRespV1::SfItem SfItem;
		typedef SGetCsmsFaultRespV1::SulItem SulItem;
		typedef SGetCsmsFaultRespV1::SFaultDetails SFaultDetails;

		EErrStatus	cond;	//Overall system fault condition.
							//Valid value: PASS_STAT, FAIL_STAT, WARNING_STAT.
		SFaultDetails env;	//Specific Environmental fault conditions.
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numSlaves;
#else
		uint32_t numSlaves;
#endif
		SFaultDetails slaves[MAX_SLAVES];	//Specific Environmental fault conditions.
	};

	typedef SGetCsmsFaultRespV2 SGetCsmsFaultResp;

	struct SGetCsmsplusFaultRespV1	 // GET_CSMSPLUS_FAULT_RESP.
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const int MAX_SLAVES = 3;
#else
		static const int32_t MAX_SLAVES = 3;
#endif
		typedef SGetCsmsFaultRespV1::SfItem SfItem;
		typedef SGetCsmsFaultRespV1::SulItem SulItem;
		struct SFaultDetails
		{
			EErrStatus bistCond;
			SfItem temps323X[3];
			SulItem clock368MHz;
			SulItem timeStatus;
			SfItem temps2630[3];
			SulItem ocxoLock;
		};

		EErrStatus	cond;	//Overall system fault condition.
							//Valid value: PASS_STAT, FAIL_STAT, WARNING_STAT.
		SFaultDetails env;	//Specific Environmental fault conditions.
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numSlaves;
#else
		uint32_t numSlaves;
#endif
		SFaultDetails slaves[MAX_SLAVES];	//Specific Environmental fault conditions.
	};

	typedef SGetCsmsplusFaultRespV1 SGetCsmsplusFaultResp;


#ifndef _MSC_VER
	struct SGetSrvGuidRespV1	 // GET_SRV_GUID_RESP. There is no version 0.
	{
		static const size_t C_DATA4_SIZE = 8;
		//Create the same field as Windows GUID.
#ifndef CSMSPLUS_SRV_BUILT
	    unsigned long  Data1;
	    unsigned short Data2;
	    unsigned short Data3;
	    unsigned char  Data4[C_DATA4_SIZE];
#else
	    uint32_t Data1;
	    uint16_t Data2;
	    uint16_t Data3;
	    uint8_t  Data4[C_DATA4_SIZE];
#endif
	};

#else
	typedef GUID SGetSrvGuidRespV1;
#endif
	typedef SGetSrvGuidRespV1 SGetSrvGuidResp;

	static const size_t C_MAX_ANT_NAME_LEN = 128;	//Maximum antenna description length.

	// Common structures
	struct SAntInfoV1
	{

		//Valid antenna id:
		// ANT1 - ANT3; ANT1H
		// DF_ANT_1V -- DF_ANT_10V
		// DF_ANT_1H -- DF_ANT_9H
		// SMPL_RF2 -- SMPL_RF3
		// ANT_DYNAMIC_FIRST_VAL -- ANT_DYNAMIC_LAST_VAL
		EAnt ant;
		char16_t antName[C_MAX_ANT_NAME_LEN]; //NULL terminated String desription of this antenna.
		EAntPol			polarization;
		Units::Frequency::Raw	lowFreq; //Lowest frequency supported by the antenna.
		Units::Frequency::Raw	highFreq;//Highest frequency supported by the antenna.
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	rotatorId; //0: Antenna not mounted on rotator.
								  //!0: Rotator id in which the antenna is mounted.

		//***********						******
		//The following only meaningful if rotatorId !0
		short			azimuthStart; //Start of Azimuth angle, usually is 0.
		short			azimuthStop; //Azimuth stop angle, usually 360 if stop == start means no Azimuth rotator.
		unsigned short	azimuthStep; //Step size in degree, usually 1°, 0  means no Azimuth rotator.
		unsigned short	azimuthTolerance; //Azimuth tolerance in °, Rotator stops if the angle is +- tolerance.
		short			aziOrientation; //Azimuth orientation;
		short			elevationStart; //Start of elevation angle, usually is -90.
		short			elevationStop; //Elevation stop angle, usually is 90.
		unsigned short	elevationStep; //Step size in degree, usually 90° could be 1°, 0  means no elevation rotator.
		unsigned short	elevationTolerance; //Elevation tolerance in °, Rotator stops if the angle is +- tolerance.
#else
		uint32_t	rotatorId; //0: Antenna not mounted on rotator.
								  //!0: Rotator id in which the antenna is mounted.

		//***********						******
		//The following only meaningful if rotatorId !0
		int16_t			azimuthStart; //Start of Azimuth angle, usually is 0.
		int16_t			azimuthStop; //Azimuth stop angle, usually 360 if stop == start means no Azimuth rotator.
		uint16_t	azimuthStep; //Step size in degree, usually 1°, 0  means no Azimuth rotator.
		uint16_t	azimuthTolerance; //Azimuth tolerance in °, Rotator stops if the angle is +- tolerance.
		int16_t			aziOrientation; //Azimuth orientation;
		int16_t			elevationStart; //Start of elevation angle, usually is -90.
		int16_t			elevationStop; //Elevation stop angle, usually is 90.
		uint16_t	elevationStep; //Step size in degree, usually 90° could be 1°, 0  means no elevation rotator.
		uint16_t	elevationTolerance; //Elevation tolerance in °, Rotator stops if the angle is +- tolerance.
#endif
		//***********						******
	};

	typedef SAntInfoV1 SAntInfo;

	struct SAntInfoListRespV1		 	// GET_ANT_LIST_RESP. There is no version 0.
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	numAnt;		//Has the number of antenna that followed.
#else
		uint32_t	numAnt;		//Has the number of antenna that followed.
#endif
		SAntInfoV1 		ant[1];	    //Variable size as specified by numAnt.
	};

	typedef SAntInfoListRespV1 SAntInfoListResp;
#ifndef CSMSPLUS_SRV_BUILT
	static const unsigned char CURR_ANT_LIST_INFO_VERS = 1;
	static const unsigned char CURR_ANT_LIST_RESP_VERS = 1;
#else
	static const uint8_t CURR_ANT_LIST_INFO_VERS = 1;
	static const uint8_t CURR_ANT_LIST_RESP_VERS = 1;
#endif
	struct SGetPanCmdV1; // Forward declaration

	struct SGetPanCmdV0 // GET_PAN, GET_PAN_AGC V0
	{
		inline operator SGetPanCmdV1(void) const; // Convert to V1; definition below

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	freq;
		unsigned long	bandwidth;
		long			rcvrAtten;
#else
		uint32_t	freq;
		uint32_t	bandwidth;
		int32_t		rcvrAtten;
#endif
	};

	struct SGetPanCmdV2; // Forward declaration

	struct SGetPanCmdV1 // GET_PAN, GET_PAN_AGC V1
	{
		operator SGetPanCmdV0(void) const // Convert to V0
		{
			SGetPanCmdV0 v0;
#ifndef CSMSPLUS_SRV_BUILT
			v0.freq = Units::Frequency(freq).Hz<unsigned long>(true);
			v0.bandwidth = Units::Frequency(bandwidth).Hz<unsigned long>(true);
#else
			v0.freq = Units::Frequency(freq).Hz<uint32_t>(true);
			v0.bandwidth = Units::Frequency(bandwidth).Hz<uint32_t>(true);
#endif
			v0.rcvrAtten = rcvrAtten;
			return v0;
		}

		inline operator SGetPanCmdV2(void) const; // Convert to V2; definition below

		Units::Frequency::Raw	freq;
		Units::Frequency::Raw	bandwidth;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char	rcvrAtten;
#else
		uint8_t	rcvrAtten;
#endif
	};

	struct SGetPanCmdV2 // GET_PAN, GET_PAN_AGC V2
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned char AGC = 255;
		static const unsigned char AGC_DEFAULT_GAIN_MODE = 255;
		static const unsigned char AGC_NORMAL_GAIN_MODE = 254;
		static const unsigned char AGC_RURAL_GAIN_MODE = 253;
		static const unsigned char AGC_URBAN_GAIN_MODE = 252;
		static const unsigned char AGC_CONGESTED_GAIN_MODE = 251;
#else
		static const uint8_t AGC = 255;
		static const uint8_t AGC_DEFAULT_GAIN_MODE = 255;
		static const uint8_t AGC_NORMAL_GAIN_MODE = 254;
		static const uint8_t AGC_RURAL_GAIN_MODE = 253;
		static const uint8_t AGC_URBAN_GAIN_MODE = 252;
		static const uint8_t AGC_CONGESTED_GAIN_MODE = 251;

#endif
		operator SGetPanCmdV1(void) const // Convert to V1
		{
			SGetPanCmdV1 v1;
			v1.freq = freq;
			v1.bandwidth = bandwidth;
			v1.rcvrAtten = rcvrAtten;
			return v1;
		}

		Units::Frequency::Raw	freq;
		Units::Frequency::Raw	bandwidth;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char	rcvrAtten;
#else
		uint8_t	rcvrAtten;
#endif
	};

	typedef SGetPanCmdV2 SGetPanCmd;

	struct SGetPanRespV0 // GET_PAN_RESP V0
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned long NUMBINS_MAX = 481;

		ErrorCodes::EErrorCode status;
		double			dateTime;	// using COleTime format
		unsigned long	freq;
		long			powerDbm; // dBm
		unsigned long	binSize; // 1/10000 Hz
		unsigned long	numBins;
		long			rcvrAtten; // dB
		unsigned char	binData[NUMBINS_MAX]; // dBm + 192
#else
		static const uint32_t NUMBINS_MAX = 481;

		ErrorCodes::EErrorCode status;
		double			dateTime;	// using COleTime format
		uint32_t	freq;
		int32_t			powerDbm; // dBm
		uint32_t	binSize; // 1/10000 Hz
		uint32_t	numBins;
		int32_t			rcvrAtten; // dB
		uint8_t	binData[NUMBINS_MAX]; // dBm + 192
#endif
	};

	struct SGetPanRespV2; // Forwrad declaration

	struct SGetPanRespV1 // GET_PAN_RESP V1 (variable length)
	{
		operator SGetPanRespV0(void) const // Convert to V0
		{
			SGetPanRespV0 v0;
			v0.status = status;
			v0.dateTime = dateTime;
#ifndef CSMSPLUS_SRV_BUILT
			v0.freq = Units::Frequency(freq).Hz<unsigned long>(true);
			v0.powerDbm = powerDbm - 192l;
			v0.binSize = static_cast<unsigned long>(Units::Frequency(binSize).Hz<double>(true) * 10000);
#else
			v0.freq = Units::Frequency(freq).Hz<uint32_t>(true);
			v0.powerDbm = powerDbm - 192l;
			v0.binSize = static_cast<uint32_t>(Units::Frequency(binSize).Hz<double>(true) * 10000);
#endif
			v0.numBins = (numBins > SGetPanRespV0::NUMBINS_MAX ? SGetPanRespV0::NUMBINS_MAX : numBins);
			v0.rcvrAtten = rcvrAtten;
			memcpy(v0.binData, binData, v0.numBins);

			return v0;
		}

		SGetPanRespV1& operator =(const SGetPanRespV0& v0) // Convert from V0
		{
			dateTime = v0.dateTime;
			freq = Units::Frequency(v0.freq).GetRaw();
			binSize = Units::Frequency(v0.binSize / 10000.0).GetRaw();
			status = v0.status;
#ifndef CSMSPLUS_SRV_BUILT
			numBins = static_cast<unsigned short>(v0.numBins);
			powerDbm = static_cast<unsigned char>(v0.powerDbm + 192);
			rcvrAtten = static_cast<unsigned char>(v0.rcvrAtten);
#else
			numBins = static_cast<uint16_t>(v0.numBins);
			powerDbm = static_cast<uint8_t>(v0.powerDbm + 192);
			rcvrAtten = static_cast<uint8_t>(v0.rcvrAtten);
#endif
			memcpy(binData, v0.binData, numBins);

			return *this;
		}

		SGetPanRespV1& operator =(const SGetPanRespV2& v2); // Convert from V2

		double					dateTime;	// using COleTime format
		Units::Frequency::Raw	freq;
		Units::Frequency::Raw	binSize;
		ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned short			numBins;
		unsigned char			powerDbm; // dBm + 192
		unsigned char			rcvrAtten; // dB
		unsigned char			binData[1]; // dBm + 192 (variable length)
#else
		uint16_t			numBins;
		uint8_t			powerDbm; // dBm + 192
		uint8_t			rcvrAtten; // dB
		uint8_t			binData[1]; // dBm + 192 (variable length)
#endif
	};

	struct SAudioChannelPower
	{
		bool active;
		double powerdBm;  // Audio channel Power
	};

	struct SGetPanRespV3; // Forward declaration

	struct SGetPanRespV2 // GET_PAN_RESP V2 (variable length)
	{
		SGetPanRespV2& operator =(const SGetPanRespV1& v1) // Convert from V1
		{
			dateTime = v1.dateTime;
			freq = v1.freq;
			binSize = v1.binSize;
			status = v1.status;
			numBins = v1.numBins;
			powerDbm = v1.powerDbm;
			rcvrAtten = v1.rcvrAtten;
			conversionFactorForFS= 0.0f;
			nActiveAudioChannels =0;
			memcpy(binData, v1.binData, numBins);

			for (size_t i = 0; i < MAX_AUDIOCHANNELS; i++)
			{
				audioPower[i].active = false;
				audioPower[i].powerdBm = -200.0f;
			}

			return *this;
		}

		SGetPanRespV2& operator =(const SGetPanRespV3& v3); // Convert from V3

		double					dateTime;	// using COleTime format
		Units::Frequency::Raw	freq;
		Units::Frequency::Raw	binSize;
		ErrorCodes::EErrorCode  status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned short			numBins;
		unsigned short          nActiveAudioChannels;
		unsigned char			powerDbm; // dBm + 192
		unsigned char			rcvrAtten; // dB
		double                  conversionFactorForFS;
		SAudioChannelPower      audioPower[MAX_AUDIOCHANNELS];
		unsigned char			binData[1]; // dBm + 192 (variable length)
#else
		uint16_t				numBins;
		uint16_t          		nActiveAudioChannels;
		uint8_t					powerDbm; // dBm + 192
		uint8_t					rcvrAtten; // dB
		double                  conversionFactorForFS;
		SAudioChannelPower      audioPower[MAX_AUDIOCHANNELS];
		uint8_t					binData[1]; // dBm + 192 (variable length)
#endif
	};

	struct SGetPanRespV3 // GET_PAN_RESP V3 (variable length)
	{
		SGetPanRespV3& operator =(const SGetPanRespV2& v2) // Convert from V2
		{
			dateTime = v2.dateTime;
			freq = v2.freq;
			binSize = v2.binSize;
			status = v2.status;
			numBins = v2.numBins;
			nActiveAudioChannels = v2.nActiveAudioChannels;
			powerDbm = v2.powerDbm;
			rcvrAtten = v2.rcvrAtten;
			conversionFactorForFS = v2.conversionFactorForFS;
			memcpy(binData, v2.binData, numBins);

			for (size_t i = 0; i < MAX_PAN_AUDIOCHANNELS; i++)
			{
				if (i < MAX_AUDIOCHANNELS)
				{
					audioPower[i].active = v2.audioPower[i].active;
					audioPower[i].powerdBm = v2.audioPower[i].powerdBm;
				}
				else
				{
					audioPower[i].active = false;
					audioPower[i].powerdBm = -200.0f;
				}
			}

			return *this;
		}

		double					dateTime;	// using COleTime format
		Units::Frequency::Raw	freq;
		Units::Frequency::Raw	binSize;
		ErrorCodes::EErrorCode  status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned short			numBins;
		unsigned short          nActiveAudioChannels;
		unsigned char			powerDbm; // dBm + 192
		unsigned char			rcvrAtten; // dB
		double                  conversionFactorForFS;
		SAudioChannelPower      audioPower[MAX_PAN_AUDIOCHANNELS];
		unsigned char			binData[1]; // dBm + 192 (variable length)
#else
		uint16_t				numBins;
		uint16_t          		nActiveAudioChannels;
		uint8_t					powerDbm; // dBm + 192
		uint8_t					rcvrAtten; // dB
		double                  conversionFactorForFS;
		SAudioChannelPower      audioPower[MAX_PAN_AUDIOCHANNELS];
		uint8_t					binData[1]; // dBm + 192 (variable length)
#endif
	};

#ifndef SMS_SRV_BUILT
	typedef SGetPanRespV3 SGetPanResp;
#else
	typedef SGetPanRespV1 SGetPanResp;
#endif

	struct SWbRecorderStartV1			// WBRECORDER_START
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned char AGC = 255;
		static const unsigned char AGC_DEFAULT_GAIN_MODE = 255;
		static const unsigned char AGC_NORMAL_GAIN_MODE = 254;
		static const unsigned char AGC_RURAL_GAIN_MODE = 253;
		static const unsigned char AGC_URBAN_GAIN_MODE = 252;
		static const unsigned char AGC_CONGESTED_GAIN_MODE = 251;
#else
		static const uint8_t AGC = 255;
		static const uint8_t AGC_DEFAULT_GAIN_MODE = 255;
		static const uint8_t AGC_NORMAL_GAIN_MODE = 254;
		static const uint8_t AGC_RURAL_GAIN_MODE = 253;
		static const uint8_t AGC_URBAN_GAIN_MODE = 252;
		static const uint8_t AGC_CONGESTED_GAIN_MODE = 251;
#endif
		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;		// sample rate = 1.28 * bandwidth
		EAnt antenna;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long numSamples;	// Should be a power of 2 but will be adjusted if necessary
		unsigned char rcvrAtten;
#else
		uint32_t numSamples;
		uint8_t	rcvrAtten;
#endif
	};
	typedef SWbRecorderStartV1 SWbRecorderStart;

#ifndef CSMSPLUS_SRV_BUILT
	enum EIqFormat : unsigned char { DEFAULT, INT16, FLOAT32 };	// default is float32 except when bandwidth == recorded rate
#else
	enum EIqFormat : uint8_t { DEFAULT, INT16, FLOAT32 };	// default is float32 except when bandwidth == recorded rate
#endif
	struct SWbRecorderQueryReqV1 // WBRECORDER_QUERY_REQ
	{
		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
		Units::Timestamp::Raw start; // Must be valid
		Units::Timestamp::Raw stop; // Must be valid
		EIqFormat dataFormat;
	};
	typedef SWbRecorderQueryReqV1 SWbRecorderQueryReq;

	struct SWbRecorderIqdataReqV1 // WBRECORDER_IQDATA_REQ
	{
		Units::Frequency::Raw freq;
		Units::Frequency::Raw bandwidth;
		Units::Timestamp::Raw start; // Must be valid
		Units::Timestamp::Raw stop; // Must be valid
		EIqFormat dataFormat;
	};
	typedef SWbRecorderIqdataReqV1 SWbRecorderIqdataReq;

	struct SWbRecorderStatusRespV1		// WBRECORDER_STATUS_RESP
	{
		bool isRecording;
	};
	typedef SWbRecorderStatusRespV1 SWbRecorderStatusResp;

	struct SWbRecorderCatalogRespV1 // WBRECORDER_CATALOG_RESP
	{
		// Note: All fields = 0 is end-of-data indicator
		Units::Frequency::Raw f1;
		Units::Frequency::Raw f2;
		Units::Timestamp::Raw start;
		Units::Timestamp::Raw stop;
		Units::Frequency::Raw sampleRate;
		ErrorCodes::EErrorCode status;
	};
	typedef SWbRecorderCatalogRespV1 SWbRecorderCatalogResp;

	struct SWbRecorderQueryRespV1 // WBRECORDER_QUERY_RESP
	{
		// Note: All fields = 0 is end-of-data indicator
		Units::Frequency::Raw f1;
		Units::Frequency::Raw f2;
		Units::Timestamp::Raw start;
		Units::Timestamp::Raw stop;
		Units::Frequency::Raw sampleRate;
		float minScaleFactor;
		float maxScaleFactor;
		ErrorCodes::EErrorCode status;
	};
	typedef SWbRecorderQueryRespV1 SWbRecorderQueryResp;

	struct SWbRecorderIqdataRespV1 // WBRECORDER_IQDATA_RESP
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short MAX_SAMPLES_BYTES = 8192;
		static const unsigned short MAX_SAMPLES_INT16 = MAX_SAMPLES_BYTES / 4;
		static const unsigned short MAX_SAMPLES_FLOAT32 = MAX_SAMPLES_BYTES / 8;

		struct SIqInt16
		{
			short re;
			short im;
		};
		struct SIqFloat32
		{
			float re;
			float im;
		};

		Units::Frequency::Raw freq;
		Units::Frequency::Raw sampleRate;
		Units::Timestamp::Raw firstSampleTime;
		double latitude;
		double longitude;
		float altitude;
		float scaleFactor; // Factor to convert to ???; 0 = Null Segment
		ErrorCodes::EErrorCode status;
		unsigned long packetIndex;		// 0 based
		unsigned short numIqData;		// Number of samples
		EIqFormat dataFormat;
		union
		{
			unsigned long long samplesAlign;	// force alignment to 64 bit boundary
			unsigned char samplesBuf[MAX_SAMPLES_BYTES];
			SIqInt16 iqData16[MAX_SAMPLES_INT16];
			SIqFloat32 iqData32[MAX_SAMPLES_FLOAT32];
		};
#else
		static const uint16_t MAX_SAMPLES_BYTES = 16384;
		static const uint16_t MAX_SAMPLES_INT16 = MAX_SAMPLES_BYTES / 4;
		static const uint16_t MAX_SAMPLES_FLOAT32 = MAX_SAMPLES_BYTES / 8;

		struct SIqInt16
		{
			int16_t re;
			int16_t im;
		};
		struct SIqFloat32
		{
			float re;
			float im;
		};

		Units::Frequency::Raw freq;
		Units::Frequency::Raw sampleRate;
		Units::Timestamp::Raw firstSampleTime;
		double latitude;
		double longitude;
		float altitude;
		float scaleFactor; // Factor to convert to ???; 0 = Null Segment
		ErrorCodes::EErrorCode status;
		uint32_t packetIndex;		// 0 based
		uint16_t numIqData;		// Number of samples
		EIqFormat dataFormat;
		union
		{
			uint64_t samplesAlign;	// force alignment to 64 bit boundary
			uint8_t samplesBuf[MAX_SAMPLES_BYTES];
			SIqInt16 iqData16[MAX_SAMPLES_INT16];
			SIqFloat32 iqData32[MAX_SAMPLES_FLOAT32];
		};
#endif
	};
	typedef SWbRecorderIqdataRespV1 SWbRecorderIqdataResp;

	struct SWbRecorderIqdataStopRespV1		// WBRECORDER_IQDATA_STOP_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SWbRecorderIqdataStopRespV1 SWbRecorderIqdataStopResp;

	struct SWbRecorderSpectrumRespV1 // WBRECORDER_SPECTRUM_RESP
	{
		static const int MAX_PSD_BINS = 1024;

		Units::Timestamp::Raw dateTime;
		Units::Frequency::Raw freq;
		Units::Frequency::Raw binSize;
		Units::TimeSpan::Raw duration;
		ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned short numBins;
		unsigned short blockNum;		// 0, 1, ... numBlocks - 1
		unsigned short numBlocks;
		unsigned short numFrames;
		unsigned char psd[MAX_PSD_BINS];	// dBm + 192
#else
		uint16_t numBins;
		uint16_t blockNum;		// 0, 1, ... numBlocks - 1
		uint16_t numBlocks;
		uint16_t numFrames;
		uint8_t psd[MAX_PSD_BINS];	// dBm + 192
#endif
	};
	typedef SWbRecorderSpectrumRespV1 SWbRecorderSpectrumResp;

	struct SStreamingInitV1				// STREAMING_INIT
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char destMacAddress[6];
		unsigned char destIpAddress[4];
		unsigned short destPort;
#else
		uint8_t destMacAddress[6];
		uint8_t destIpAddress[4];
		uint16_t destPort;
#endif
	};
	typedef SStreamingInitV1 SStreamingInit;

	struct SStreamingRfParamsV1			// STREAMING_RF_PARAMS
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned char AGC = 255;
		static const unsigned char AGC_DEFAULT_GAIN_MODE = 255;
		static const unsigned char AGC_NORMAL_GAIN_MODE = 254;
		static const unsigned char AGC_RURAL_GAIN_MODE = 253;
		static const unsigned char AGC_URBAN_GAIN_MODE = 252;
		static const unsigned char AGC_CONGESTED_GAIN_MODE = 251;

		unsigned char rcvrAtten;
#else
		static const uint8_t AGC = 255;
		static const uint8_t AGC_DEFAULT_GAIN_MODE = 255;
		static const uint8_t AGC_NORMAL_GAIN_MODE = 254;
		static const uint8_t AGC_RURAL_GAIN_MODE = 253;
		static const uint8_t AGC_URBAN_GAIN_MODE = 252;
		static const uint8_t AGC_CONGESTED_GAIN_MODE = 251;

		uint8_t	rcvrAtten;
#endif
		EAnt antenna;
		Units::Frequency::Raw freq;		// center frequency
		Units::Frequency::Raw bandwidth;	// 80,40,20,10,5,2.5,1.25 MHz for wideband.
	};
	typedef SStreamingRfParamsV1 SStreamingRfParams;

	struct SStreamingStartWbV1			// STREAMING_START_WB
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long streamId;
#else
		uint32_t streamId;
#endif
	};
	typedef SStreamingStartWbV1 SStreamingStartWb;

	struct SStreamingStartDdcV1			// STREAMING_START_DDC
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long streamId;
		unsigned char channel;
		unsigned char reserved[3];
#else
		uint32_t streamId;
		uint8_t channel;
		uint8_t reserved[3];
#endif
		Units::Frequency::Raw freq;		// center frequency
		Units::Frequency::Raw bandwidth;	// 80,40,20,10,5,2.5,1.25 MHz for wideband.
	};
	typedef SStreamingStartDdcV1 SStreamingStartDdc;

	struct SStreamingStopDdcV1			// STREAMING_STOP_DDC
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char channel;
#else
		uint8_t channel;
#endif
	};
	typedef SStreamingStopDdcV1 SStreamingStopDdc;

	struct SStreamingInitRespV1		// STREAMING_INIT_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SStreamingInitRespV1 SStreamingInitResp;

	struct SStreamingRfParamsRespV1		// STREAMING_RF_PARAMS_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SStreamingRfParamsRespV1 SStreamingRfParamsResp;

	struct SStreamingRfStopRespV1		// STREAMING_RF_STOP_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SStreamingRfStopRespV1 SStreamingRfStopResp;

	struct SStreamingStartWbRespV1		// STREAMING_START_WB_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SStreamingStartWbRespV1 SStreamingStartWbResp;

	struct SStreamingStopWbRespV1		// STREAMING_STOP_WB_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SStreamingStopWbRespV1 SStreamingStopWbResp;

	struct SStreamingStartDdcRespV1		// STREAMING_START_DDC_RESP
	{
		ErrorCodes::EErrorCode success;
		uint8_t channel;
	};
	typedef SStreamingStartDdcRespV1 SStreamingStartDdcResp;

	struct SStreamingStopDdcRespV1		// STREAMING_STOP_DDC_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SStreamingStopDdcRespV1 SStreamingStopDdcResp;

	// Message header structure
	struct SHdr
	{
		SHdr(void) : sourceAddr(0), destAddr(0), msgType(0), cmdVersion(0), respVersion(0), msgSubType(0), bodySize(0) {}

#ifndef CSMSPLUS_SRV_BUILT
		unsigned long sourceAddr;
		unsigned long destAddr;
		unsigned short msgType;
		unsigned char cmdVersion;
		unsigned char respVersion;
		unsigned long msgSubType;
		unsigned long bodySize;
#else
		uint32_t sourceAddr;
		uint32_t destAddr;
		uint16_t msgType;
		uint8_t cmdVersion;
		uint8_t respVersion;
		uint32_t msgSubType;
		uint32_t bodySize;
#endif
	};

	// Functions
	static bool Is5141Type(_In_ EMetricHw hwType);
};


// Version conversion operators
inline SSmsMsg::SAvdMeasureResultV0::operator SSmsMsg::SAvdMeasureResultV1(void) const // Convert to V1
{
	SAvdMeasureResultV1 v1;
	v1.occHdr = occHdr;
	v1.occHdr.numChannels = (occHdr.numChannels <= std::min(MAX_OCCCHANNELS, occHdr.numTotalChannels) ? occHdr.numChannels : 0);

	for(size_t i = 0; i < v1.occHdr.numChannels; ++i)
	{
		v1.measData[i].result = measData[i].result;
		v1.measData[i].stdDev = float(measData[i].stdDev);
	}

	return v1;
}

inline SSmsMsg::SFrequencyVsChannelRespV0::operator SSmsMsg::SFrequencyVsChannelRespV1(void) const // Convert to V1
{
	SFrequencyVsChannelRespV1 v1;
	v1.occHdr = occHdr;
	v1.occHdr.numChannels = (occHdr.numChannels <= std::min(MAX_OCCCHANNELS, occHdr.numTotalChannels) ? occHdr.numChannels : 0);
	v1.selectedAntenna = selectedAntenna;
	v1.saveIntermediateData = (saveIntermediateData != 0);
	v1.useSecondaryThreshold = (useSecondaryThreshold != 0);
#ifndef CSMSPLUS_SRV_BUILT
	v1.occPrimaryThreshold[0] = short(occPrimaryThreshold[0]);
	v1.occPrimaryThreshold[1] = short(occPrimaryThreshold[1]);
	v1.occSecondaryThreshold[0] = short(occSecondaryThreshold[0]);
	v1.occSecondaryThreshold[1] = short(occSecondaryThreshold[1]);
#else
	v1.occPrimaryThreshold[0] = int16_t(occPrimaryThreshold[0]);
	v1.occPrimaryThreshold[1] = int16_t(occPrimaryThreshold[1]);
	v1.occSecondaryThreshold[0] = int16_t(occSecondaryThreshold[0]);
	v1.occSecondaryThreshold[1] = int16_t(occSecondaryThreshold[1]);
#endif
	strcpy_s(v1.hostName, hostName);
	v1.numBands = numBands;
	memcpy(v1.numChannels, numChannels, sizeof(v1.numChannels));

	for(size_t i = 0; i < v1.occHdr.numChannels; ++i)
	{
		v1.frequencies[i] = Units::Frequency(frequencies[i]).GetRaw();
	}

	return v1;
}

inline SSmsMsg::SFrequencyVsChannelRespV1::operator SSmsMsg::SFrequencyVsChannelRespV2(void) const // Convert to V2
{
	SFrequencyVsChannelRespV2 v2;
	v2.occHdr = occHdr;
	v2.occHdr.numChannels = (occHdr.numChannels <= std::min(MAX_OCCCHANNELS, occHdr.numTotalChannels) ? occHdr.numChannels : 0);
	v2.selectedAntenna = selectedAntenna;
	v2.saveIntermediateData = (saveIntermediateData != 0);
	v2.useSecondaryThreshold = (useSecondaryThreshold != 0);
#ifndef CSMSPLUS_SRV_BUILT
	v2.occPrimaryThreshold[0] = short(occPrimaryThreshold[0]);
	v2.occPrimaryThreshold[1] = short(occPrimaryThreshold[1]);
	v2.occSecondaryThreshold[0] = short(occSecondaryThreshold[0]);
	v2.occSecondaryThreshold[1] = short(occSecondaryThreshold[1]);
#else
	v2.occPrimaryThreshold[0] = int16_t(occPrimaryThreshold[0]);
	v2.occPrimaryThreshold[1] = int16_t(occPrimaryThreshold[1]);
	v2.occSecondaryThreshold[0] = int16_t(occSecondaryThreshold[0]);
	v2.occSecondaryThreshold[1] = int16_t(occSecondaryThreshold[1]);
#endif
	strcpy_s(v2.hostName, hostName);
	v2.numBands = numBands;

	memset(&(v2.numChannels), 0, sizeof(v2.numChannels));	// Upconverting 100 bands to 250 bands. Copy 100 bands' info into the v2 'numChannels', but zero-out the rest
	memcpy(v2.numChannels, numChannels, sizeof(numChannels));	// numChannels[100], v2.numChannels[250], so only copy sizeof numChannels[100] or would copy garbage data too (beyond numChannels[100])

	for (size_t i = 0; i < v2.occHdr.numChannels; ++i)
	{
		v2.frequencies[i] = Units::Frequency(frequencies[i]).GetRaw();
	}

	return v2;
}

inline SSmsMsg::SGetAutoViolateCmdV0::operator SSmsMsg::SGetAutoViolateCmdV1(void) const // Convert to V1
{
	SGetAutoViolateCmdV1 v1;
	v1.numBands = numBands;
	memcpy(v1.lowFrequency, lowFrequency, sizeof(v1.lowFrequency));
	memcpy(v1.highFrequency, highFrequency, sizeof(v1.highFrequency));
	memcpy(v1.channelBandwidth, channelBandwidth, sizeof(v1.channelBandwidth));
	memcpy(v1.includeExclude, includeExclude, sizeof(v1.includeExclude));
	v1.storageTime = storageTime;
	v1.measurementTime = measurementTime;
	v1.avdThreshold = avdThreshold;
	v1.measurementRate = measurementRate;
	v1.ant = ANT1;

	return v1;
}

inline SSmsMsg::SGetAutoViolateCmdV2& SSmsMsg::SGetAutoViolateCmdV2::operator =(const SGetAutoViolateCmdV3 & v3) //Down-Convert from v3.
{
	const SGetAutoViolateCmdV3* src = &v3;

	if(static_cast<void*>(this) == static_cast<const void*>(&v3))
	{
		// Make a copy
		if((src = static_cast<const SGetAutoViolateCmdV3*>(malloc(SizeOf(v3)))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SGetAutoViolateCmdV3*>(src), &v3, SizeOf(v3));
	}

	storageTime = src->storageTime;
	measurementTime = src->measurementTime;
	measurementRate = src->measurementRate;
	ant = src->ant;
	avdThreshold = src->avdThreshold;
	numBands = src->numBands;

	for(size_t i = 0; i < numBands; ++i)
	{
		band[i].highFrequency = src->band[i].highFrequency;
		band[i].lowFrequency = src->band[i].lowFrequency;
		band[i].channelBandwidth = src->band[i].channelBandwidth;
		band[i].exclude = src->band[i].exclude;
	}

	if(src != &v3)
	{
		free(const_cast<SGetAutoViolateCmdV3*>(src));
	}
			
	return *this;
}

inline SSmsMsg::SGetBandRespV0::operator SSmsMsg::SGetBandRespV1(void) const // Convert to V1
{
	SGetBandRespV1 v1;
	v1.status = status;
	v1.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v1.hfDfHardwarePresent = hfDfHardwarePresent;
	v1.vuhfMetricHardwarePresent = vuhfMetricHardwarePresent;
	v1.vuhfDfHardwarePresent = vuhfDfHardwarePresent;
	v1.hfMetricLowFreq = hfMetricLowFreq;
	v1.hfMetricHighFreq = hfMetricHighFreq;
	v1.hfDfLowFreq = hfDfLowFreq;
	v1.hfDfHighFreq = hfDfHighFreq;
	v1.vuhfMetricLowFreq = vuhfMetricLowFreq;
	v1.vuhfMetricHighFreq = vuhfMetricHighFreq;
	v1.vuhfDfLowFreq = vuhfDfLowFreq;
	v1.vuhfDfHighFreq = vuhfDfHighFreq;
	v1.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v1.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
	v1.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v1.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v1.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v1.vuhfMeasurementBw.numBandwidths = vuhfMeasurementBw.numBandwidths;
	v1.vuhfPanDisplayBw.numBandwidths = vuhfPanDisplayBw.numBandwidths;
	v1.vuhfOccupancyBw.numBandwidths = vuhfOccupancyBw.numBandwidths;
	v1.vuhfDfScanBw.numBandwidths = vuhfDfScanBw.numBandwidths;
	v1.vuhfReceiverBw.numBandwidths = vuhfReceiverBw.numBandwidths;
	v1.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v1.vuhfMeasurementDwell.numBandwidths = vuhfMeasurementDwell.numBandwidths;

	for(size_t bw = 0; bw < SGetBandRespV0::MAX_BANDWIDTHS; ++bw)
	{
		v1.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
		v1.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
		v1.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
		v1.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
		v1.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
		v1.vuhfMeasurementBw.bandwidth[bw] = vuhfMeasurementBw.bandwidth[bw];
		v1.vuhfPanDisplayBw.bandwidth[bw] = vuhfPanDisplayBw.bandwidth[bw];
		v1.vuhfOccupancyBw.bandwidth[bw] = vuhfOccupancyBw.bandwidth[bw];
		v1.vuhfDfScanBw.bandwidth[bw] = vuhfDfScanBw.bandwidth[bw];
		v1.vuhfReceiverBw.bandwidth[bw] = vuhfReceiverBw.bandwidth[bw];
		v1.hfMeasurementDwell.frequency[bw] = hfMeasurementDwell.frequency[bw];
		v1.hfMeasurementDwell.bandwidth[bw] = hfMeasurementDwell.bandwidth[bw];
		v1.hfMeasurementDwell.modulation[bw] = hfMeasurementDwell.modulation[bw];
		v1.hfMeasurementDwell.fieldStrength[bw] = hfMeasurementDwell.fieldStrength[bw];
		v1.hfMeasurementDwell.directionFinding[bw] = hfMeasurementDwell.directionFinding[bw];
		v1.vuhfMeasurementDwell.frequency[bw] = vuhfMeasurementDwell.frequency[bw];
		v1.vuhfMeasurementDwell.bandwidth[bw] = vuhfMeasurementDwell.bandwidth[bw];
		v1.vuhfMeasurementDwell.modulation[bw] = vuhfMeasurementDwell.modulation[bw];
		v1.vuhfMeasurementDwell.fieldStrength[bw] = vuhfMeasurementDwell.fieldStrength[bw];
		v1.vuhfMeasurementDwell.directionFinding[bw] = vuhfMeasurementDwell.directionFinding[bw];
	}

	return v1;
}

inline SSmsMsg::SGetBandRespV1::operator SSmsMsg::SGetBandRespV2(void) const // Convert to V2
{
	SGetBandRespV2 v2;
	v2.status = status;
	v2.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v2.hfDfHardwarePresent = hfDfHardwarePresent;
	v2.vushfMetricHardwarePresent = vuhfMetricHardwarePresent;
	v2.vushfDfHardwarePresent = vuhfDfHardwarePresent;
	v2.hfMetricLowFreq = Units::Frequency(hfMetricLowFreq).GetRaw();
	v2.hfMetricHighFreq = Units::Frequency(hfMetricHighFreq).GetRaw();
	v2.hfDfLowFreq = Units::Frequency(hfDfLowFreq).GetRaw();
	v2.hfDfHighFreq = Units::Frequency(hfDfHighFreq).GetRaw();
	v2.vushfMetricLowFreq = Units::Frequency(vuhfMetricLowFreq).GetRaw();
	v2.vushfMetricHighFreq = Units::Frequency(vuhfMetricHighFreq).GetRaw();
	v2.vushfDfLowFreq = Units::Frequency(vuhfDfLowFreq).GetRaw();
	v2.vushfDfHighFreq = Units::Frequency(vuhfDfHighFreq).GetRaw();
	v2.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v2.hfPanDisplayBw.numBandwidths =  hfPanDisplayBw.numBandwidths;
	v2.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v2.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v2.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v2.vushfMeasurementBw.numBandwidths = vuhfMeasurementBw.numBandwidths;
	v2.vushfPanDisplayBw.numBandwidths = vuhfPanDisplayBw.numBandwidths;
	v2.vushfOccupancyBw.numBandwidths = vuhfOccupancyBw.numBandwidths;
	v2.vushfDfScanBw.numBandwidths = vuhfDfScanBw.numBandwidths;
	v2.vushfReceiverBw.numBandwidths = vuhfReceiverBw.numBandwidths;
	v2.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v2.vushfMeasurementDwell.numBandwidths = vuhfMeasurementDwell.numBandwidths;

	for(size_t bw = 0; bw < SGetBandRespV1::MAX_BANDWIDTHS; ++bw)
	{
		v2.hfMeasurementBw.bandwidth[bw] = Units::Frequency(hfMeasurementBw.bandwidth[bw]).GetRaw();
		v2.hfPanDisplayBw.bandwidth[bw] = Units::Frequency(hfPanDisplayBw.bandwidth[bw]).GetRaw();
		v2.hfOccupancyBw.bandwidth[bw] = Units::Frequency(hfOccupancyBw.bandwidth[bw]).GetRaw();
		v2.hfDfScanBw.bandwidth[bw] = Units::Frequency(hfDfScanBw.bandwidth[bw]).GetRaw();
		v2.hfReceiverBw.bandwidth[bw] = Units::Frequency(hfReceiverBw.bandwidth[bw]).GetRaw();
		v2.vushfMeasurementBw.bandwidth[bw] = Units::Frequency(vuhfMeasurementBw.bandwidth[bw]).GetRaw();
		v2.vushfPanDisplayBw.bandwidth[bw] = Units::Frequency(vuhfPanDisplayBw.bandwidth[bw]).GetRaw();
		v2.vushfOccupancyBw.bandwidth[bw] = Units::Frequency(vuhfOccupancyBw.bandwidth[bw]).GetRaw();
		v2.vushfDfScanBw.bandwidth[bw] = Units::Frequency(vuhfDfScanBw.bandwidth[bw]).GetRaw();
		v2.vushfReceiverBw.bandwidth[bw] = Units::Frequency(vuhfReceiverBw.bandwidth[bw]).GetRaw();
		v2.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequency[bw];
		v2.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidth[bw];
		v2.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.modulation[bw];
		v2.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrength[bw];
		v2.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFinding[bw];
		v2.vushfMeasurementDwell.frequencyDwell[bw] = vuhfMeasurementDwell.frequency[bw];
		v2.vushfMeasurementDwell.bandwidthDwell[bw] = vuhfMeasurementDwell.bandwidth[bw];
		v2.vushfMeasurementDwell.modulationDwell[bw] = vuhfMeasurementDwell.modulation[bw];
		v2.vushfMeasurementDwell.fieldStrengthDwell[bw] = vuhfMeasurementDwell.fieldStrength[bw];
		v2.vushfMeasurementDwell.directionFindingDwell[bw] = vuhfMeasurementDwell.directionFinding[bw];
	}

	return v2;
}

#ifdef _MSC_VER
const __declspec(selectany) Units::Frequency	SSmsMsg::NO_HORIZON_LOW_FREQ(0);
const __declspec(selectany) Units::Frequency	SSmsMsg::NO_HORIZON_HIGH_FREQ(0);
const __declspec(selectany) Units::Frequency	SSmsMsg::NO_VERTICAL_LOW_FREQ(0);
const __declspec(selectany) Units::Frequency	SSmsMsg::NO_VERTICAL_HIGH_FREQ(0);
#else
const __attribute__((weak))  Units::Frequency	SSmsMsg::NO_HORIZON_LOW_FREQ(0);
const __attribute__((weak))  Units::Frequency	SSmsMsg::NO_HORIZON_HIGH_FREQ(0);
const __attribute__((weak))  Units::Frequency	SSmsMsg::NO_VERTICAL_LOW_FREQ(0);
const __attribute__((weak))  Units::Frequency	SSmsMsg::NO_VERTICAL_HIGH_FREQ(0);
#endif

inline SSmsMsg::SGetBandRespV2::operator SSmsMsg::SGetBandRespV3(void) const // Convert to V3
{
	SGetBandRespV3 v3;
	v3.status = status;
	v3.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v3.hfDfHardwarePresent = hfDfHardwarePresent;
	v3.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
	v3.vushfDfHardwarePresent = vushfDfHardwarePresent;
	v3.hfMetricLowFreq = hfMetricLowFreq;
	v3.hfMetricHighFreq = hfMetricHighFreq;
	v3.hfDfLowFreq = hfDfLowFreq;
	v3.hfDfHighFreq = hfDfHighFreq;
	v3.vushfMetricVLowFreq = vushfMetricLowFreq;
	v3.vushfMetricVHighFreq = vushfMetricHighFreq;
	v3.vushfMetricHLowFreq = NO_HORIZON_LOW_FREQ.GetRaw();
	v3.vushfMetricHHighFreq = NO_HORIZON_HIGH_FREQ.GetRaw();
	v3.vushfDfVLowFreq = vushfDfLowFreq;
	v3.vushfDfVHighFreq = vushfDfHighFreq;
	if( vushfDfHardwarePresent == DUAL_POL )
	{
		v3.vushfDfHLowFreq = vushfDfLowFreq;
		v3.vushfDfHHighFreq = vushfDfHighFreq;
	}
	else
	{
		v3.vushfDfHLowFreq = NO_HORIZON_LOW_FREQ.GetRaw();
		v3.vushfDfHHighFreq = NO_HORIZON_HIGH_FREQ.GetRaw();
	}
	v3.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v3.hfPanDisplayBw.numBandwidths =  hfPanDisplayBw.numBandwidths;
	v3.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v3.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v3.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v3.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
	v3.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
	v3.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
	v3.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
	v3.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
	v3.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v3.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

	for(size_t bw = 0; bw < SGetBandRespV2::MAX_BANDWIDTHS; ++bw)
	{
		v3.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
		v3.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
		v3.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
		v3.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
		v3.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
		v3.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
		v3.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
		v3.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
		v3.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
		v3.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
		v3.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
		v3.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
		v3.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v3.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v3.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
		v3.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
		v3.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
		v3.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
		v3.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
		v3.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
	}

	return v3;
}

inline SSmsMsg::SGetBandRespV3::operator SSmsMsg::SGetBandRespV4(void) const // Convert to V4
{
	SGetBandRespV4 v4;
	v4.status = status;
	v4.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v4.hfDfHardwarePresent = hfDfHardwarePresent;
	v4.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
	v4.vushfDfHardwarePresent = vushfDfHardwarePresent;
	v4.iqDataCapability = NO_IQDATA;
	v4.hfMetricLowFreq = hfMetricLowFreq;
	v4.hfMetricHighFreq = hfMetricHighFreq;
	v4.hfDfLowFreq = hfDfLowFreq;
	v4.hfDfHighFreq = hfDfHighFreq;
	v4.vushfMetricVLowFreq = vushfMetricVLowFreq;
	v4.vushfMetricVHighFreq = vushfMetricVHighFreq;
	v4.vushfMetricHLowFreq = vushfMetricHLowFreq;
	v4.vushfMetricHHighFreq = vushfMetricHHighFreq;
	v4.vushfDfVLowFreq = vushfDfVLowFreq;
	v4.vushfDfVHighFreq = vushfDfVHighFreq;
	v4.vushfDfHLowFreq = vushfDfHLowFreq;
	v4.vushfDfHHighFreq = vushfDfHHighFreq;
	v4.vushfDfHLowFreq = vushfDfHLowFreq;
	v4.vushfDfHHighFreq = vushfDfHHighFreq;
	v4.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v4.hfPanDisplayBw.numBandwidths =  hfPanDisplayBw.numBandwidths;
	v4.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v4.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v4.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v4.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
	v4.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
	v4.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
	v4.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
	v4.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
	v4.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v4.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

	for(size_t bw = 0; bw < SGetBandRespV3::MAX_BANDWIDTHS; ++bw)
	{
		v4.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
		v4.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
		v4.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
		v4.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
		v4.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
		v4.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
		v4.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
		v4.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
		v4.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
		v4.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
		v4.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
		v4.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
		v4.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v4.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v4.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
		v4.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
		v4.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
		v4.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
		v4.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
		v4.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
	}

	return v4;
}

inline SSmsMsg::SGetBandRespV4::operator SSmsMsg::SGetBandRespV5(void) const // Convert to V5
{
	SGetBandRespV5 v5;
	v5.status = status;
	v5.siteInfo = SITE_UNKNOWN;
	v5.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v5.hfDfHardwarePresent = hfDfHardwarePresent;
	v5.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
	v5.vushfDfHardwarePresent = vushfDfHardwarePresent;
	v5.iqDataCapability = iqDataCapability;
	v5.hfMetricLowFreq = hfMetricLowFreq;
	v5.hfMetricHighFreq = hfMetricHighFreq;
	v5.hfDfLowFreq = hfDfLowFreq;
	v5.hfDfHighFreq = hfDfHighFreq;
	v5.vushfMetricVLowFreq = vushfMetricVLowFreq;
	v5.vushfMetricVHighFreq = vushfMetricVHighFreq;
	v5.vushfMetricHLowFreq = vushfMetricHLowFreq;
	v5.vushfMetricHHighFreq = vushfMetricHHighFreq;
	v5.vushfDfVLowFreq = vushfDfVLowFreq;
	v5.vushfDfVHighFreq = vushfDfVHighFreq;
	v5.vushfDfHLowFreq = vushfDfHLowFreq;
	v5.vushfDfHHighFreq = vushfDfHHighFreq;
	v5.vushfDfHLowFreq = vushfDfHLowFreq;
	v5.vushfDfHHighFreq = vushfDfHHighFreq;
	v5.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v5.hfPanDisplayBw.numBandwidths =  hfPanDisplayBw.numBandwidths;
	v5.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v5.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v5.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v5.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
	v5.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
	v5.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
	v5.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
	v5.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
	v5.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v5.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

	for(size_t bw = 0; bw < SGetBandRespV4::MAX_BANDWIDTHS; ++bw)
	{
		v5.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
		v5.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
		v5.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
		v5.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
		v5.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
		v5.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
		v5.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
		v5.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
		v5.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
		v5.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
		v5.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
		v5.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
		v5.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v5.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v5.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
		v5.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
		v5.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
		v5.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
		v5.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
		v5.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
	}

	return v5;
}

inline SSmsMsg::SGetBandRespV5::operator SSmsMsg::SGetBandRespV6(void) const // Convert to V6
{
	SGetBandRespV6 v6;
	v6.status = status;
	v6.siteInfo = siteInfo;
	v6.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v6.hfDfHardwarePresent = hfDfHardwarePresent;
	v6.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
	v6.vushfDfHardwarePresent = vushfDfHardwarePresent;
	v6.iqDataCapability = iqDataCapability;
	v6.hfMetricLowFreq = hfMetricLowFreq;
	v6.hfMetricHighFreq = hfMetricHighFreq;
	v6.hfDfLowFreq = hfDfLowFreq;
	v6.hfDfHighFreq = hfDfHighFreq;
	v6.hfIonoInfoSourceType = NOT_APPLICABLE;
	v6.vushfMetricVLowFreq = vushfMetricVLowFreq;
	v6.vushfMetricVHighFreq = vushfMetricVHighFreq;
	v6.vushfMetricHLowFreq = vushfMetricHLowFreq;
	v6.vushfMetricHHighFreq = vushfMetricHHighFreq;
	v6.vushfDfVLowFreq = vushfDfVLowFreq;
	v6.vushfDfVHighFreq = vushfDfVHighFreq;
	v6.vushfDfHLowFreq = vushfDfHLowFreq;
	v6.vushfDfHHighFreq = vushfDfHHighFreq;
	v6.vushfDfHLowFreq = vushfDfHLowFreq;
	v6.vushfDfHHighFreq = vushfDfHHighFreq;
	v6.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v6.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
	v6.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v6.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v6.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v6.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
	v6.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
	v6.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
	v6.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
	v6.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
	v6.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v6.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

	for (size_t bw = 0; bw < SGetBandRespV4::MAX_BANDWIDTHS; ++bw)
	{
		v6.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
		v6.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
		v6.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
		v6.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
		v6.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
		v6.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
		v6.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
		v6.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
		v6.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
		v6.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
		v6.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
		v6.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
		v6.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v6.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v6.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
		v6.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
		v6.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
		v6.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
		v6.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
		v6.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
	}

	return v6;
}

inline SSmsMsg::SGetBandRespV6::operator SSmsMsg::SGetBandRespV7(void) const // Convert to V7
{
	SGetBandRespV7 v7;
	v7.status = status;
	v7.siteInfo = siteInfo;
	v7.hfMetricHardwarePresent = hfMetricHardwarePresent;
	v7.hfDfHardwarePresent = hfDfHardwarePresent;
	v7.hfIonoInfoSourceType = hfIonoInfoSourceType;
	v7.vushfMetricHardwarePresent = vushfMetricHardwarePresent;
	v7.vushfDfHardwarePresent = vushfDfHardwarePresent;
	v7.iqDataCapability = iqDataCapability;
	v7.hfMetricLowFreq = hfMetricLowFreq;
	v7.hfMetricHighFreq = hfMetricHighFreq;
	v7.hfDfLowFreq = hfDfLowFreq;
	v7.hfDfHighFreq = hfDfHighFreq;
	v7.vushfMetricHLowFreq = vushfMetricHLowFreq;
	v7.vushfMetricHHighFreq = vushfMetricHHighFreq;
	v7.vushfMetricVLowFreq = vushfMetricVLowFreq;
	v7.vushfMetricVHighFreq = vushfMetricVHighFreq;
	v7.vushfDfHLowFreq = vushfDfHLowFreq;
	v7.vushfDfHHighFreq = vushfDfHHighFreq;
	v7.vushfDfVLowFreq = vushfDfVLowFreq;
	v7.vushfDfVHighFreq = vushfDfVHighFreq;
	v7.hfMeasurementBw.numBandwidths = hfMeasurementBw.numBandwidths;
	v7.hfPanDisplayBw.numBandwidths = hfPanDisplayBw.numBandwidths;
	v7.hfOccupancyBw.numBandwidths = hfOccupancyBw.numBandwidths;
	v7.hfDfScanBw.numBandwidths = hfDfScanBw.numBandwidths;
	v7.hfReceiverBw.numBandwidths = hfReceiverBw.numBandwidths;
	v7.hfDdcBw.numBandwidths = 0;
	v7.vushfMeasurementBw.numBandwidths = vushfMeasurementBw.numBandwidths;
	v7.vushfPanDisplayBw.numBandwidths = vushfPanDisplayBw.numBandwidths;
	v7.vushfOccupancyBw.numBandwidths = vushfOccupancyBw.numBandwidths;
	v7.vushfDfScanBw.numBandwidths = vushfDfScanBw.numBandwidths;
	v7.vushfReceiverBw.numBandwidths = vushfReceiverBw.numBandwidths;
	v7.vushfDdcBw.numBandwidths = 0;
	v7.hfMeasurementDwell.numBandwidths = hfMeasurementDwell.numBandwidths;
	v7.vushfMeasurementDwell.numBandwidths = vushfMeasurementDwell.numBandwidths;

	for (size_t bw = 0; bw < SGetBandRespV6::MAX_BANDWIDTHS; ++bw)
	{
		v7.hfMeasurementBw.bandwidth[bw] = hfMeasurementBw.bandwidth[bw];
		v7.hfPanDisplayBw.bandwidth[bw] = hfPanDisplayBw.bandwidth[bw];
		v7.hfOccupancyBw.bandwidth[bw] = hfOccupancyBw.bandwidth[bw];
		v7.hfDfScanBw.bandwidth[bw] = hfDfScanBw.bandwidth[bw];
		v7.hfReceiverBw.bandwidth[bw] = hfReceiverBw.bandwidth[bw];
		v7.vushfMeasurementBw.bandwidth[bw] = vushfMeasurementBw.bandwidth[bw];
		v7.vushfPanDisplayBw.bandwidth[bw] = vushfPanDisplayBw.bandwidth[bw];
		v7.vushfOccupancyBw.bandwidth[bw] = vushfOccupancyBw.bandwidth[bw];
		v7.vushfDfScanBw.bandwidth[bw] = vushfDfScanBw.bandwidth[bw];
		v7.vushfReceiverBw.bandwidth[bw] = vushfReceiverBw.bandwidth[bw];
		v7.hfMeasurementDwell.frequencyDwell[bw] = hfMeasurementDwell.frequencyDwell[bw];
		v7.hfMeasurementDwell.bandwidthDwell[bw] = hfMeasurementDwell.bandwidthDwell[bw];
		v7.hfMeasurementDwell.modulationDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v7.hfMeasurementDwell.fieldStrengthDwell[bw] = hfMeasurementDwell.fieldStrengthDwell[bw];
		v7.hfMeasurementDwell.directionFindingDwell[bw] = hfMeasurementDwell.directionFindingDwell[bw];
		v7.vushfMeasurementDwell.frequencyDwell[bw] = vushfMeasurementDwell.frequencyDwell[bw];
		v7.vushfMeasurementDwell.bandwidthDwell[bw] = vushfMeasurementDwell.bandwidthDwell[bw];
		v7.vushfMeasurementDwell.modulationDwell[bw] = vushfMeasurementDwell.modulationDwell[bw];
		v7.vushfMeasurementDwell.fieldStrengthDwell[bw] = vushfMeasurementDwell.fieldStrengthDwell[bw];
		v7.vushfMeasurementDwell.directionFindingDwell[bw] = vushfMeasurementDwell.directionFindingDwell[bw];
	}

	return v7;
}

inline SSmsMsg::SGetDwellCmdV0::operator SSmsMsg::SGetDwellCmdV1(void) const // Convert to V1
{
	SGetDwellCmdV1 v1;
	v1.freq = Units::Frequency(freq).GetRaw();
	v1.bandwidth = Units::Frequency(bandwidth).GetRaw();

	return v1;
}

inline SSmsMsg::SGetMeasCmdV0::operator SSmsMsg::SGetMeasCmdV1(void) const // Convert to V1
{
		SGetMeasCmdV1 v1;
		v1.freq = freq;
		v1.bandwidth = bandwidth;
		v1.ant = ANT1;
		v1.bwCmd = bwCmd;
		v1.freqCmd = freqCmd;
		v1.modulationCmd = modulationCmd;
		v1.fieldStrengthCmd = fieldStrengthCmd;
		memcpy(&v1.dfCmd, &dfCmd, sizeof(v1.dfCmd));

		return v1;
}

inline SSmsMsg::SGetMeasCmdV1::operator SSmsMsg::SGetMeasCmdV2(void) const // Convert to V2
{
		SGetMeasCmdV2 v2;
		v2.freq = Units::Frequency(freq).GetRaw();
		v2.bandwidth = Units::Frequency(bandwidth).GetRaw();
		v2.ant = ant;
		v2.bwCmd = bwCmd;
		v2.freqCmd = freqCmd;
		v2.modulationCmd = modulationCmd;
		v2.fieldStrengthCmd = fieldStrengthCmd;
		v2.dfCmd.dwellTime = dfCmd.dwellTime;
		v2.dfCmd.repeatCount = dfCmd.repeatCount;
#ifndef CSMSPLUS_SRV_BUILT
		v2.dfCmd.confThreshold = static_cast<unsigned char>(dfCmd.confThreshold);
#else
		v2.dfCmd.confThreshold = static_cast<uint8_t>(dfCmd.confThreshold);
#endif
		v2.dfCmd.dfBandwidth = Units::Frequency(dfCmd.dfBandwidth).GetRaw();
		v2.dfCmd.outputType = static_cast<SGetMeasCmdV2::SGetDfCmd::EOutputType>(dfCmd.outputType);
		v2.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV2::SGetDfCmd::ESource>(dfCmd.srcOfRequest);

		return v2;
}

inline SSmsMsg::SGetMeasCmdV2::operator SSmsMsg::SGetMeasCmdV3(void) const // Convert to V3
{
		SGetMeasCmdV3 v3;
		v3.freq = freq;
		v3.bandwidth = bandwidth;
		v3.ant = ant;
		v3.bwCmd = bwCmd;
		v3.freqCmd = freqCmd;
		v3.modulationCmd = modulationCmd;
		v3.fieldStrengthCmd = fieldStrengthCmd;
		v3.dfCmd.dwellTime = dfCmd.dwellTime;
		v3.dfCmd.repeatCount = dfCmd.repeatCount;
		v3.dfCmd.confThreshold = dfCmd.confThreshold;
		v3.dfCmd.dfBandwidth = dfCmd.dfBandwidth;
		v3.dfCmd.outputType = static_cast<SGetMeasCmdV3::SGetDfCmd::EOutputType>(dfCmd.outputType);
		v3.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV3::SGetDfCmd::ESource>(dfCmd.srcOfRequest);
		v3.iqCmd.numSamples = 0;
		v3.iqCmd.outputType = NONE;
		return v3;
}

inline SSmsMsg::SGetMeasCmdV3::operator SSmsMsg::SGetMeasCmdV4(void) const // Convert to V4
{
		SGetMeasCmdV4 v4;
		v4.freq = freq;
		v4.bandwidth = bandwidth;
		v4.ant = ant;
		v4.bwCmd = bwCmd;
		v4.freqCmd = freqCmd;
		v4.modulationCmd = modulationCmd;
		v4.fieldStrengthCmd = fieldStrengthCmd;
		v4.dfCmd.dwellTime = dfCmd.dwellTime;
		v4.dfCmd.repeatCount = dfCmd.repeatCount;
		v4.dfCmd.confThreshold = dfCmd.confThreshold;
		v4.dfCmd.dfBandwidth = dfCmd.dfBandwidth;
		v4.dfCmd.outputType = static_cast<SGetMeasCmdV4::SGetDfCmd::EOutputType>(dfCmd.outputType);
		v4.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV4::SGetDfCmd::ESource>(dfCmd.srcOfRequest);
		v4.iqCmd.bwFactor = iqCmd.bwFactor;
		v4.iqCmd.numSamples = iqCmd.numSamples;
		v4.iqCmd.outputType = iqCmd.outputType;
		v4.iqCmd.startTime = 0.0;
		v4.iqCmd.tdoa = iqCmd.tdoa;
		return v4;
}

inline SSmsMsg::SGetMeasCmdV4::operator SSmsMsg::SGetMeasCmdV5(void) const // Convert to V5
{
		SGetMeasCmdV5 v5;
		v5.freq = freq;
		v5.bandwidth = bandwidth;
		v5.ant = ant;
		v5.rcvrAtten = SSmsMsg::SGetMeasCmdV5::AGC;
		v5.bwCmd = bwCmd;
		v5.freqCmd = freqCmd;
		v5.modulationCmd = modulationCmd;
		v5.fieldStrengthCmd = fieldStrengthCmd;
		v5.dfCmd.dwellTime = dfCmd.dwellTime;
		v5.dfCmd.repeatCount = dfCmd.repeatCount;
		v5.dfCmd.confThreshold = dfCmd.confThreshold;
		v5.dfCmd.dfBandwidth = dfCmd.dfBandwidth;
		v5.dfCmd.outputType = static_cast<SGetMeasCmdV5::SGetDfCmd::EOutputType>(dfCmd.outputType);
		v5.dfCmd.srcOfRequest = static_cast<SGetMeasCmdV5::SGetDfCmd::ESource>(dfCmd.srcOfRequest);
		v5.iqCmd.bwFactor = iqCmd.bwFactor;
		v5.iqCmd.numSamples = iqCmd.numSamples;
		v5.iqCmd.outputType = iqCmd.outputType;
		v5.iqCmd.startTime = 0.0;
		v5.iqCmd.tdoa = iqCmd.tdoa;
		return v5;
}

inline SSmsMsg::SGetMeasRespV0::operator SSmsMsg::SGetMeasRespV1(void) const // Convert to V1
{
	SGetMeasRespV1 v1;
	v1.gpsResponse = gpsResponse;
	memcpy(&v1.bwResponse, &bwResponse, sizeof(v1.bwResponse));
	memcpy(&v1.freqResponse, &freqResponse, sizeof(v1.freqResponse));
	memcpy(&v1.modulationResponse, &modulationResponse, sizeof(v1.modulationResponse));
	memcpy(&v1.fieldStrengthResponse, &fieldStrengthResponse, sizeof(v1.fieldStrengthResponse));
	memcpy(v1.bwBinData, bwBinData, sizeof(v1.bwBinData));
	memcpy(v1.amProbDist, amProbDist, sizeof(v1.amProbDist));
	memcpy(v1.fmProbDist, fmProbDist, sizeof(v1.fmProbDist));
	memcpy(v1.pmProbDist, pmProbDist, sizeof(v1.pmProbDist));
	memcpy(&v1.dfResponse[0].dfResponse, &dfResponse, sizeof(v1.dfResponse[0].dfResponse));

	for(size_t cut = 0; cut < DFCUTS_MAX; ++cut)
	{
#ifndef CSMSPLUS_SRV_BUILT
		v1.dfResponse[0].dfCutData[cut].azim = static_cast<unsigned short>(dfCutData[cut].azim);
		v1.dfResponse[0].dfCutData[cut].elev = static_cast<unsigned short>(dfCutData[cut].elev);
		v1.dfResponse[0].dfCutData[cut].conf = static_cast<unsigned short>(dfCutData[cut].conf);
#else
		v1.dfResponse[0].dfCutData[cut].azim = static_cast<uint16_t>(dfCutData[cut].azim);
		v1.dfResponse[0].dfCutData[cut].elev = static_cast<uint16_t>(dfCutData[cut].elev);
		v1.dfResponse[0].dfCutData[cut].conf = static_cast<uint16_t>(dfCutData[cut].conf);
#endif
		v1.dfResponse[0].dfCutData[cut].range = dfCutData[cut].range;
		v1.dfResponse[0].dfCutData[cut].ms = dfCutData[cut].ms;
		v1.dfResponse[0].dfCutData[cut].refPowerDbm = 0;
		v1.dfResponse[0].dfCutData[cut].maxSamplePowerDbm = 0;
		v1.dfResponse[0].dfCutData[cut].maxSamplePowerIdx = 1;
	}

	memcpy(v1.dfResponse[0].dfVoltageData, dfVoltageData, sizeof(v1.dfResponse[0].dfVoltageData));
	v1.dfResponse[1].dfResponse.status = ErrorCodes::NOERROR_OUTPUT_NONE;
	v1.dfResponse[1].dfResponse.numAllCuts = 0;

	return v1;
}

inline SSmsMsg::SGetMeasRespV1::operator SSmsMsg::SGetMeasRespV2(void) const // Convert to V2
{
	SGetMeasRespV2 v2;
	v2.gpsResponse = gpsResponse;
	v2.bwResponse.status = bwResponse.status;
	v2.bwResponse.repeatCount = bwResponse.repeatCount;
	v2.bwResponse.betaBw = bwResponse.betaBw / 10.0f;
	v2.bwResponse.betaFreqLow = bwResponse.betaFreqLow;
	v2.bwResponse.betaFreqHigh = bwResponse.betaFreqHigh;
	v2.bwResponse.betaStdDev = float(bwResponse.betaStdDev);
	v2.bwResponse.x1Bw = bwResponse.x1Bw / 10.0f;
	v2.bwResponse.x1FreqLow = bwResponse.x1FreqLow;
	v2.bwResponse.x1FreqHigh = bwResponse.x1FreqHigh;
	v2.bwResponse.x1StdDev = float(bwResponse.x1StdDev);
	v2.bwResponse.x2Bw = bwResponse.x2Bw / 10.0f;
	v2.bwResponse.x2FreqLow = bwResponse.x2FreqLow;
	v2.bwResponse.x2FreqHigh = bwResponse.x2FreqHigh; 
	v2.bwResponse.x2StdDev = float(bwResponse.x2StdDev);
	v2.bwResponse.freq = Units::Frequency(bwResponse.freq).GetRaw();
	v2.bwResponse.binSize = Units::Frequency(bwResponse.binSize / 10000.0).GetRaw();
	v2.bwResponse.numBins = bwResponse.numBins;
	v2.freqResponse.status = freqResponse.status;
	v2.freqResponse.repeatCount = freqResponse.repeatCount;
	v2.freqResponse.freqOffset = float(freqResponse.freqOffset);
	v2.freqResponse.freq = freqResponse.freq;
	v2.freqResponse.stdDev = float(freqResponse.stdDev);
	v2.modulationResponse.status = modulationResponse.status;
	v2.modulationResponse.repeatCount = modulationResponse.repeatCount;
	v2.modulationResponse.amModPlus = modulationResponse.amModPlus / 1000.0f;
	v2.modulationResponse.amModMinus = modulationResponse.amModMinus / 1000.0f;
	v2.modulationResponse.amModDepth = modulationResponse.amModDepth / 1000.0f;
	v2.modulationResponse.fmPeakFreq = float(modulationResponse.fmPeakFreq);
	v2.modulationResponse.fmRmsFreq = float(modulationResponse.fmRmsFreq);
	v2.modulationResponse.pmPeakPhase = modulationResponse.pmPeakPhase / 100.0f;
	v2.modulationResponse.pmRmsPhase = modulationResponse.pmRmsPhase / 100.0f;
#ifndef CSMSPLUS_SRV_BUILT
	v2.modulationResponse.numAmHistBins = static_cast<unsigned char>(modulationResponse.numAmHistBins);
	v2.modulationResponse.numFmHistBins = static_cast<unsigned char>(modulationResponse.numFmHistBins);
	v2.modulationResponse.numPmHistBins = static_cast<unsigned char>(modulationResponse.numPmHistBins);
#else
	v2.modulationResponse.numAmHistBins = static_cast<uint8_t>(modulationResponse.numAmHistBins);
	v2.modulationResponse.numFmHistBins = static_cast<uint8_t>(modulationResponse.numFmHistBins);
	v2.modulationResponse.numPmHistBins = static_cast<uint8_t>(modulationResponse.numPmHistBins);
#endif
	v2.modulationResponse.amHistBinsize = modulationResponse.amHistBinsize / 1000.0f;
	v2.modulationResponse.fmHistBinsize = float(modulationResponse.fmHistBinsize);
	v2.modulationResponse.pmHistBinsize = modulationResponse.pmHistBinsize / 100.0f;
	v2.fieldStrengthResponse.status = fieldStrengthResponse.status;
	v2.fieldStrengthResponse.repeatCount = fieldStrengthResponse.repeatCount;
	v2.fieldStrengthResponse.fieldMethod = fieldStrengthResponse.fieldMethod;
	v2.fieldStrengthResponse.eField = fieldStrengthResponse.eField / 10.0f;
	v2.fieldStrengthResponse.pFlux = fieldStrengthResponse.pFlux / 10.0f;
	v2.fieldStrengthResponse.stdDev = fieldStrengthResponse.stdDev / 10.0f;
	v2.fieldStrengthResponse.powerDbm = fieldStrengthResponse.powerDbm / 10.0f;
	v2.fieldStrengthResponse.rxPowerDbm = fieldStrengthResponse.rxPowerDbm / 10.0f;

	for(size_t i = 0; i < 2; ++i)
	{
		v2.dfResponse[i].dfResponse.status = dfResponse[i].dfResponse.status;
		v2.dfResponse[i].dfResponse.fluxgateStatus = dfResponse[i].dfResponse.fluxgateStatus;		
		v2.dfResponse[i].dfResponse.headingCorrection = dfResponse[i].dfResponse.headingCorrection / 100.0f;
		v2.dfResponse[i].dfResponse.numSumCuts = dfResponse[i].dfResponse.numSumCuts;
		v2.dfResponse[i].dfResponse.azim = dfResponse[i].dfResponse.azim / 100.0f;
		v2.dfResponse[i].dfResponse.elev = dfResponse[i].dfResponse.elev / 100.0f;
		v2.dfResponse[i].dfResponse.conf = dfResponse[i].dfResponse.conf / 100.0f;
		v2.dfResponse[i].dfResponse.range = dfResponse[i].dfResponse.range / 100.0f; 
		v2.dfResponse[i].dfResponse.stdDev = dfResponse[i].dfResponse.stdDev / 100.0f;
		v2.dfResponse[i].dfResponse.snr = dfResponse[i].dfResponse.snr / 100.0f;
		v2.dfResponse[i].dfResponse.powerDbm = dfResponse[i].dfResponse.powerDbm / 100.0f;
		v2.dfResponse[i].dfResponse.numAllCuts = (dfResponse[i].dfResponse.numAllCuts <= DFCUTS_MAX ? dfResponse[i].dfResponse.numAllCuts : 0);
		v2.dfResponse[i].dfResponse.numAnts =
#ifndef CSMSPLUS_SRV_BUILT
				static_cast<unsigned char>(dfResponse[i].dfResponse.numAnts <= SDfResponse::SDfVoltageData::NUMANTS_MAX ? dfResponse[i].dfResponse.numAnts : 0);
#else
				static_cast<uint8_t>(dfResponse[i].dfResponse.numAnts <= SDfResponse::SDfVoltageData::NUMANTS_MAX ? dfResponse[i].dfResponse.numAnts : 0);
#endif
		for(size_t j = 0; j < v2.dfResponse[i].dfResponse.numAllCuts; ++j)
		{
			v2.dfResponse[i].dfCutData[j].ms = float(dfResponse[i].dfCutData[j].ms);
			v2.dfResponse[i].dfCutData[j].range = dfResponse[i].dfCutData[j].range / 100.0f;
			v2.dfResponse[i].dfCutData[j].azim = dfResponse[i].dfCutData[j].azim / 100.0f;
			v2.dfResponse[i].dfCutData[j].elev = dfResponse[i].dfCutData[j].elev / 100.0f;
			v2.dfResponse[i].dfCutData[j].conf = dfResponse[i].dfCutData[j].conf / 100.0f;
			v2.dfResponse[i].dfCutData[j].refPowerDbm = dfResponse[i].dfCutData[j].refPowerDbm / 100.0f;
			v2.dfResponse[i].dfCutData[j].maxSamplePowerDbm = dfResponse[i].dfCutData[j].maxSamplePowerDbm / 100.0f;
			v2.dfResponse[i].dfCutData[j].maxSamplePowerIdx = dfResponse[i].dfCutData[j].maxSamplePowerIdx;
		}

		for(size_t j = 0; j < DFVOLTS_MAX; ++j)
		{
			for(size_t k = 0; k < v2.dfResponse[i].dfResponse.numAnts; ++k)
			{
				v2.dfResponse[i].dfVoltageData[j].volts[k][0] = dfResponse[i].dfVoltageData[j].volts[2 * k] / 1000.0f;
				v2.dfResponse[i].dfVoltageData[j].volts[k][1] = dfResponse[i].dfVoltageData[j].volts[2 * k + 1] / 100.0f;
			}

			v2.dfResponse[i].dfVoltageData[j].powerDbm[0] = dfResponse[i].dfVoltageData[j].powerDbm[0] / 100.0f;
			v2.dfResponse[i].dfVoltageData[j].powerDbm[1] = dfResponse[i].dfVoltageData[j].powerDbm[1] / 100.0f;
#ifndef CSMSPLUS_SRV_BUILT
			v2.dfResponse[i].dfVoltageData[j].antNum[0] = static_cast<unsigned char>(dfResponse[i].dfVoltageData[j].antNum[0]);
			v2.dfResponse[i].dfVoltageData[j].antNum[1] = static_cast<unsigned char>(dfResponse[i].dfVoltageData[j].antNum[1]);
#else
			v2.dfResponse[i].dfVoltageData[j].antNum[0] = static_cast<uint8_t>(dfResponse[i].dfVoltageData[j].antNum[0]);
			v2.dfResponse[i].dfVoltageData[j].antNum[1] = static_cast<uint8_t>(dfResponse[i].dfVoltageData[j].antNum[1]);
#endif
		}
	}

	memcpy(v2.bwBinData, bwBinData, sizeof(v2.bwBinData));
	memcpy(v2.amProbDist, amProbDist, sizeof(v2.amProbDist));
	memcpy(v2.fmProbDist, fmProbDist, sizeof(v2.fmProbDist));
	memcpy(v2.pmProbDist, pmProbDist, sizeof(v2.pmProbDist));

	return v2;
}

inline SSmsMsg::SGetMeasRespV2::operator SSmsMsg::SGetMeasRespV3(void) const // Convert to V3
{
	SGetMeasRespV3 v3;
	v3.gpsResponse = gpsResponse;
	v3.bwResponse = bwResponse;
	v3.freqResponse = freqResponse;
	v3.modulationResponse = modulationResponse;
	v3.fieldStrengthResponse = fieldStrengthResponse;

	for(size_t i = 0; i < 2; ++i)
	{
		v3.dfResponse[i] = dfResponse[i];
	}

	memcpy(v3.bwBinData, bwBinData, sizeof(v3.bwBinData));
	memcpy(v3.amProbDist, amProbDist, sizeof(v3.amProbDist));
	memcpy(v3.fmProbDist, fmProbDist, sizeof(v3.fmProbDist));
	memcpy(v3.pmProbDist, pmProbDist, sizeof(v3.pmProbDist));

	// iqResponse is new to v3
	memset(&(v3.iqResponse), 0, sizeof(SIqResponse));
	v3.iqResponse.status = ErrorCodes::NOERROR_OUTPUT_NONE;

	return v3;
}

inline SSmsMsg::SGetMeasRespV3::operator SSmsMsg::SGetMeasRespV4(void) const // Convert to V4
{
	SGetMeasRespV4 v4;
	v4.gpsResponse = gpsResponse;
	v4.bwResponse = bwResponse;
	v4.freqResponse = freqResponse;
	v4.modulationResponse = modulationResponse;
	v4.fieldStrengthResponse = fieldStrengthResponse;

	for(size_t i = 0; i < 2; ++i)
	{
		v4.dfResponse[i] = dfResponse[i];
	}

	v4.iqResponse = iqResponse;

	memcpy(v4.bwBinData, bwBinData, sizeof(v4.bwBinData));
	memcpy(v4.amProbDist, amProbDist, sizeof(v4.amProbDist));
	memcpy(v4.fmProbDist, fmProbDist, sizeof(v4.fmProbDist));
	memcpy(v4.pmProbDist, pmProbDist, sizeof(v4.pmProbDist));

	return v4;
}

inline SSmsMsg::SGetMeasRespV4::operator SSmsMsg::SGetMeasRespV5(void) const // Convert to V5
{
	SGetMeasRespV5 v5;
	v5.gpsResponse = gpsResponse;
	v5.bwResponse = bwResponse;
	v5.freqResponse = freqResponse;
	v5.modulationResponse = modulationResponse;
	v5.fieldStrengthResponse = fieldStrengthResponse;
	v5.iqResponse = iqResponse;
	memcpy(v5.bwBinData, bwBinData, sizeof(v5.bwBinData));
	memcpy(v5.amProbDist, amProbDist, sizeof(v5.amProbDist));
	memcpy(v5.fmProbDist, fmProbDist, sizeof(v5.fmProbDist));
	memcpy(v5.pmProbDist, pmProbDist, sizeof(v5.pmProbDist));

	//Change made in V5 is in SDfResponse
	for (size_t i = 0; i < 2; ++i)
	{
		v5.dfResponse[i].dfResponse = dfResponse[i].dfResponse;
		for (size_t j = 0; j < DFCUTS_MAX; ++j)
		{
			v5.dfResponse[i].dfCutData[j] = dfResponse[i].dfCutData[j];
		}
		for (size_t j = 0; j < DFVOLTS_MAX; ++j)
		{
			//v4 has array of 9 voltages vs. v5 that has 11 voltages.
			for (size_t k = 0; k < SGetMeasRespV2::SDfResponse::SDfVoltageData::NUMANTS_MAX; ++k)
			{
				for (size_t m = 0; m < 2; ++m)
				{
					v5.dfResponse[i].dfVoltageData[j].volts[k][m] = dfResponse[i].dfVoltageData[j].volts[k][m];
				}
			}
			
			//Zero out voltages that are not there.
			for (size_t k = SGetMeasRespV2::SDfResponse::SDfVoltageData::NUMANTS_MAX; k < SGetMeasRespV5::SDfResponse::SDfVoltageData::MAX_ANT_EL; ++k)
			{
				for (size_t m = 0; m < 2; ++m)
				{
					v5.dfResponse[i].dfVoltageData[j].volts[k][m] = 0;
				}
			}
			for (size_t k = 0; k < 2; ++k)
			{
				v5.dfResponse[i].dfVoltageData[j].powerDbm[k] = dfResponse[i].dfVoltageData[j].powerDbm[k];
				v5.dfResponse[i].dfVoltageData[j].antNum[k] = dfResponse[i].dfVoltageData[j].antNum[k];
			}
		}
	}
	return v5;
}

inline SSmsMsg::SGetOccupancyCmdV0::operator SSmsMsg::SGetOccupancyCmdV1(void) const // Convert to V1
{
	SGetOccupancyCmdV1 v1;
	v1.numBands = numBands;
	memcpy(v1.lowFrequency, lowFrequency, sizeof(v1.lowFrequency));
	memcpy(v1.highFrequency, highFrequency, sizeof(v1.highFrequency));
	memcpy(v1.channelBandwidth, channelBandwidth, sizeof(v1.channelBandwidth));
	memcpy(v1.includeExclude, includeExclude, sizeof(v1.includeExclude));
	v1.storageTime = storageTime;
	v1.durationMethod = durationMethod;
	v1.measurementTime = measurementTime;
	v1.confidenceLevel = confidenceLevel;
	v1.desiredAccuracy = desiredAccuracy;
	v1.thresholdMethod = thresholdMethod;
	v1.useSecondaryThreshold = useSecondaryThreshold;
	memcpy(v1.occPrimaryThreshold, occPrimaryThreshold, sizeof(v1.occPrimaryThreshold));
	memcpy(v1.occSecondaryThreshold, occSecondaryThreshold, sizeof(v1.occSecondaryThreshold));
	v1.occupancyMinGap = occupancyMinGap;
	v1.output = output;
	v1.saveIntermediateData = saveIntermediateData;
	v1.ant = ANT1;
	v1.forceNarrowBand = false;

	return v1;
}

inline SSmsMsg::SGetOccupancyCmdV2& SSmsMsg::SGetOccupancyCmdV2::operator =(const SGetOccupancyCmdV3& v3) // Convert from V3
{
	const SGetOccupancyCmdV3* src = &v3;

	if(static_cast<void*>(this) == static_cast<const void*>(&v3))
	{
		// Make a copy
		if((src = static_cast<const SGetOccupancyCmdV3*>(malloc(SizeOf(v3)))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SGetOccupancyCmdV3*>(src), &v3, SizeOf(v3));
	}

	storageTime = src->storageTime;
	durationMethod = src->durationMethod;
	measurementTime = src->measurementTime;
	confidenceLevel = src->confidenceLevel;
	desiredAccuracy = src->desiredAccuracy;
	thresholdMethod = src->thresholdMethod;
	useSecondaryThreshold = src->useSecondaryThreshold;
	saveIntermediateData = src->saveIntermediateData;
	occPrimaryThreshold[0] = src->occPrimaryThreshold[0];
	occPrimaryThreshold[1] = src->occPrimaryThreshold[1];
	occSecondaryThreshold[0] = src->occSecondaryThreshold[0];
	occSecondaryThreshold[1] = src->occSecondaryThreshold[1];
	occupancyMinGap = src->occupancyMinGap;
	output = src->output;
	ant = src->ant;
	numBands = src->numBands;

	for(size_t i = 0; i < numBands; ++i)
	{
#ifndef CSMSPLUS_SRV_BUILT
		band[i].highFrequency = Units::Frequency(src->band[i].highFrequency).Hz<unsigned long>(true);
		band[i].lowFrequency = Units::Frequency(src->band[i].lowFrequency).Hz<unsigned long>(true);
		band[i].channelBandwidth = Units::Frequency(src->band[i].channelBandwidth).Hz<unsigned long>(true);
#else
		band[i].highFrequency = Units::Frequency(src->band[i].highFrequency).Hz<uint32_t>(true);
		band[i].lowFrequency = Units::Frequency(src->band[i].lowFrequency).Hz<uint32_t>(true);
		band[i].channelBandwidth = Units::Frequency(src->band[i].channelBandwidth).Hz<uint32_t>(true);
#endif
		band[i].exclude = src->band[i].exclude;
		band[i].forceNarrowBand = src->band[i].forceNarrowBand;
	}

	if(src != &v3)
	{
		free(const_cast<SGetOccupancyCmdV3*>(src));
	}

	return *this;
}

inline SSmsMsg::SGetOccupancyCmdV3& SSmsMsg::SGetOccupancyCmdV3::operator =(const SGetOccupancyCmdV4& v4) // Down Convert from V4
{
	const SGetOccupancyCmdV4* src = &v4;

	if(static_cast<void*>(this) == static_cast<const void*>(&v4))
	{
		// Make a copy
		if((src = static_cast<const SGetOccupancyCmdV4*>(malloc(SizeOf(v4)))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SGetOccupancyCmdV4*>(src), &v4, SizeOf(v4));
	}

	storageTime = src->storageTime;
	durationMethod = src->durationMethod;
	measurementTime = src->measurementTime;
	confidenceLevel = src->confidenceLevel;
	desiredAccuracy = src->desiredAccuracy;
	thresholdMethod = src->thresholdMethod;
	useSecondaryThreshold = src->useSecondaryThreshold;
	saveIntermediateData = src->saveIntermediateData;
	occPrimaryThreshold[0] = src->occPrimaryThreshold[0];
	occPrimaryThreshold[1] = src->occPrimaryThreshold[1];
	occSecondaryThreshold[0] = src->occSecondaryThreshold[0];
	occSecondaryThreshold[1] = src->occSecondaryThreshold[1];
	occupancyMinGap = src->occupancyMinGap;
	output = src->output;
	ant = src->ant;
	numBands = src->numBands;

	for(size_t i = 0; i < numBands; ++i)
	{
		band[i].highFrequency = src->band[i].highFrequency;
		band[i].lowFrequency = src->band[i].lowFrequency;
		band[i].channelBandwidth = src->band[i].channelBandwidth;
		band[i].exclude = src->band[i].exclude;

		if( src->band[i].sType.signalType.narrow != 0 )
		{
			band[i].forceNarrowBand = true;
		}
		else
		{
			band[i].forceNarrowBand = false;
		}
	}

	if(src != &v4)
	{
		free(const_cast<SGetOccupancyCmdV4*>(src));
	}

	return *this;
}

//////////////////////////////////////////////////////////////////////
/// <summary>
/// operator = that converts SGetOccupancyCmdV5 to SGetOccupancyCmdV4.
/// </summary>
/// <param name="v5">
/// SGetOccupancyCmdV5 to be converted.
/// </param>
/// <returns>
/// Reference to converted SGetOccupancyCmdV4.
/// </returns>
/// <remarks>
/// @note: If both SGetOccupancyCmdV5 & SGetOccupancyCmdV4 occupied the
///		   same RAM, SGetOccupancyCmdV5 will be transfer to dynamic allocated
///		   RAM before conversion.
/// </remarks>
inline SSmsMsg::SGetOccupancyCmdV4& SSmsMsg::SGetOccupancyCmdV4::operator =(const SGetOccupancyCmdV5& v5) // Down Convert from V5
{
	const SGetOccupancyCmdV5* src = &v5;

	if (static_cast<void*>(this) == static_cast<const void*>(&v5))
	{
		// Make a copy
		if ((src = static_cast<const SGetOccupancyCmdV5*>(malloc(SizeOf(v5)))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SGetOccupancyCmdV5*>(src), &v5, SizeOf(v5));
	}

	storageTime = src->storageTime;
	durationMethod = src->durationMethod;
	measurementTime = src->measurementTime;
	confidenceLevel = src->confidenceLevel;
	desiredAccuracy = src->desiredAccuracy;
	thresholdMethod = src->thresholdMethod;
	useSecondaryThreshold = src->useSecondaryThreshold;
	saveIntermediateData = src->saveIntermediateData;
	occPrimaryThreshold[0] = src->occPrimaryThreshold[0];
	occPrimaryThreshold[1] = src->occPrimaryThreshold[1];
	occSecondaryThreshold[0] = src->occSecondaryThreshold[0];
	occSecondaryThreshold[1] = src->occSecondaryThreshold[1];
	occupancyMinGap = src->occupancyMinGap;
	output = src->output;
	ant = src->ant;
	numBands = src->numBands;

	for (size_t i = 0; i < numBands; ++i)
	{
		band[i] = src->band[i];
	}

	if (src != &v5)
	{
		free(const_cast<SGetOccupancyCmdV5*>(src));
	}

	return *this;
}

inline SSmsMsg::SGetPanCmdV0::operator SSmsMsg::SGetPanCmdV1(void) const // Convert to V1
{
	SGetPanCmdV1 v1;
	v1.freq = Units::Frequency(freq).GetRaw();
	v1.bandwidth = Units::Frequency(bandwidth).GetRaw();
#ifndef CSMSPLUS_SRV_BUILT
	v1.rcvrAtten = static_cast<unsigned char>(rcvrAtten < 0 || rcvrAtten > 255 ? 0 : rcvrAtten);
#else
	v1.rcvrAtten = static_cast<uint8_t>(rcvrAtten < 0 || rcvrAtten > 255 ? 0 : rcvrAtten);
#endif
	return v1;
};

inline SSmsMsg::SGetPanCmdV1::operator SSmsMsg::SGetPanCmdV2(void) const // Convert to V2
{
	SGetPanCmdV2 v2;
	v2.freq = freq;
	v2.bandwidth = bandwidth;
	v2.rcvrAtten = SGetPanCmdV2::AGC;
	return v2;
};

inline SSmsMsg::SInitializeDfV1::operator SSmsMsg::SInitializeDfV2(void) const // Convert to V2
{
	SInitializeDfV2 v2;
	v2.freq = Units::Frequency(freq).GetRaw();
	v2.rxBw = Units::Frequency(rxBw).GetRaw();
	v2.numIntegrations = numIntegrations;
	v2.hf = hf;

	return v2;
}

inline SSmsMsg::SInitializeDfRespV1& SSmsMsg::SInitializeDfRespV1::operator =(const SSmsMsg::SInitializeDfRespV2& v2) // Convert from V2
{
	const SInitializeDfRespV2* src = &v2;

	if(static_cast<void*>(this) == static_cast<const void*>(&v2))
	{
		// Make a copy
		if((src = static_cast<const SInitializeDfRespV2*>(malloc(offsetof(SInitializeDfRespV2, pbCalStatus[v2.numPbCalStatus])))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SInitializeDfRespV2*>(src), &v2, offsetof(SInitializeDfRespV2, pbCalStatus[v2.numPbCalStatus]));
	}

	vhfAntCalStatus = src->vhfAntCalStatus;
	initIntStatus = src->initIntStatus;
	initWwStatus = src->initWwStatus;
	initWfaStatus = src->initWfaStatus;
	initSymStatus = src->initSymStatus;
	numPbCalStatus = src->numPbCalStatus;

	for(size_t i = 0; i < numPbCalStatus; ++i)
	{
		pbCalStatus[i].freq = src->pbCalStatus[i].freq;
		pbCalStatus[i].rxBw = src->pbCalStatus[i].rxBw;
		pbCalStatus[i].avgRelAmp = src->pbCalStatus[i].avgRelAmp;
		pbCalStatus[i].stdDevRelAmp = src->pbCalStatus[i].stdDevRelAmp;
		pbCalStatus[i].avgRelPhase = src->pbCalStatus[i].avgRelPhase;
		pbCalStatus[i].stdDevRelPhase = src->pbCalStatus[i].stdDevRelPhase;
		pbCalStatus[i].numIntegrations = src->pbCalStatus[i].numIntegrations;
		pbCalStatus[i].hf = src->pbCalStatus[i].hf;
		pbCalStatus[i].status = src->pbCalStatus[i].status;
	}

	if (src != &v2)
	{
		free(const_cast<SInitializeDfRespV2*>(src));
	}

	return *this;
}

inline SSmsMsg::SInitializeDfRespV2& SSmsMsg::SInitializeDfRespV2::operator =(const SSmsMsg::SInitializeDfRespV3& v3) // Convert from V3
{
	const SInitializeDfRespV3* src = &v3;

	if(static_cast<void*>(this) == static_cast<const void*>(&v3))
	{
		// Make a copy
		if((src = static_cast<const SInitializeDfRespV3*>(malloc(offsetof(SInitializeDfRespV3, pbCalStatus[v3.numPbCalStatus])))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SInitializeDfRespV3*>(src), &v3, offsetof(SInitializeDfRespV3, pbCalStatus[v3.numPbCalStatus]));
	}

	vhfAntCalStatus = src->vhfAntCalStatus;
	initIntStatus = src->initIntStatus;
	initWwStatus = src->initWwStatus;
	initWfaStatus = src->initWfaStatus;
	initSymStatus = src->initSymStatus;
	numPbCalStatus = src->numPbCalStatus;

	for(size_t i = 0; i < numPbCalStatus; ++i)
	{
#ifndef CSMSPLUS_SRV_BUILT
		pbCalStatus[i].freq = Units::Frequency(src->pbCalStatus[i].freq).Hz<unsigned long>(true);
		pbCalStatus[i].rxBw = Units::Frequency(src->pbCalStatus[i].rxBw).Hz<unsigned long>(true);
#else
		pbCalStatus[i].freq = Units::Frequency(src->pbCalStatus[i].freq).Hz<uint32_t>(true);
		pbCalStatus[i].rxBw = Units::Frequency(src->pbCalStatus[i].rxBw).Hz<uint32_t>(true);
#endif
		pbCalStatus[i].avgRelAmp = src->pbCalStatus[i].avgRelAmp;
		pbCalStatus[i].stdDevRelAmp = src->pbCalStatus[i].stdDevRelAmp;
		pbCalStatus[i].avgRelPhase = src->pbCalStatus[i].avgRelPhase;
		pbCalStatus[i].stdDevRelPhase = src->pbCalStatus[i].stdDevRelPhase;
		pbCalStatus[i].numIntegrations = src->pbCalStatus[i].numIntegrations;
		pbCalStatus[i].hf = src->pbCalStatus[i].hf;
		pbCalStatus[i].status = src->pbCalStatus[i].status;
	}

	if (src != &v3)
	{
		free(const_cast<SInitializeDfRespV3*>(src));
	}
	return *this;
}

inline SSmsMsg::SInitializeDfRespV3& SSmsMsg::SInitializeDfRespV3::operator =(const SSmsMsg::SInitializeDfRespV4& v4) // Convert from V4
{
	const SInitializeDfRespV4* src = &v4;

	if(static_cast<void*>(this) == static_cast<const void*>(&v4))
	{
		// Make a copy
		if((src = static_cast<const SInitializeDfRespV4*>(malloc(offsetof(SInitializeDfRespV4, pbCalStatus[v4.numPbCalStatus])))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SInitializeDfRespV4*>(src), &v4, offsetof(SInitializeDfRespV4, pbCalStatus[v4.numPbCalStatus]));
	}

	vhfAntCalStatus = src->vhfAntCalStatus;
	initIntStatus = src->initIntStatus;
	initWwStatus = src->initWwStatus;
	initWfaStatus = src->initWfaStatus;
	initSymStatus = src->initSymStatus;
	numPbCalStatus = src->numPbCalStatus;

	for(size_t i = 0; i < numPbCalStatus; ++i)
	{
		pbCalStatus[i].freq = src->pbCalStatus[i].freq;
		pbCalStatus[i].rxBw = src->pbCalStatus[i].rxBw;
		pbCalStatus[i].avgRelAmp = src->pbCalStatus[i].avgRelAmp;
		pbCalStatus[i].stdDevRelAmp = src->pbCalStatus[i].stdDevRelAmp;
		pbCalStatus[i].avgRelPhase = src->pbCalStatus[i].avgRelPhase;
		pbCalStatus[i].stdDevRelPhase = src->pbCalStatus[i].stdDevRelPhase;
		pbCalStatus[i].numIntegrations = src->pbCalStatus[i].numIntegrations;
		pbCalStatus[i].hf = src->pbCalStatus[i].hf;
		pbCalStatus[i].status = src->pbCalStatus[i].status;
	}

	if (src != &v4)
	{
		free(const_cast<SInitializeDfRespV4*>(src));
	}
	return *this;
}

inline SSmsMsg::SRcvrCtrlCmdV0::operator SSmsMsg::SRcvrCtrlCmdV1(void) const // Convert to V1
{
	SRcvrCtrlCmdV1 v1;
	v1.freq = Units::Frequency(freq).GetRaw();
	v1.bandwidth = Units::Frequency(bandwidth).GetRaw();
	v1.detMode = static_cast<SRcvrCtrlCmdV1::EDetMode>(detMode);
	v1.agcTime = agcTime;
	v1.bfo = Units::Frequency(bfo).GetRaw();

	return v1;
}

inline SSmsMsg::SGetPanRespV1& SSmsMsg::SGetPanRespV1::operator =(const SSmsMsg::SGetPanRespV2& v2) // Convert from V2
{

	const SGetPanRespV2* src = &v2;

	if(static_cast<void*>(this) == static_cast<const void*>(&v2))
	{
		// Make a copy
		if((src = static_cast<const SGetPanRespV2*>(malloc(offsetof(SGetPanRespV2, binData[v2.numBins])))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SGetPanRespV2*>(src), &v2, offsetof(SGetPanRespV2, binData[v2.numBins]));
	}

	// SGetPanRespV1 v1;
	status = src->status;
	dateTime = src->dateTime;
	freq = src->freq;
	powerDbm = src->powerDbm;
	binSize = src->binSize;
	numBins = src->numBins;
	rcvrAtten = src->rcvrAtten;
	memcpy(binData, src->binData, numBins);

	if (src != &v2)
	{
		free(const_cast<SGetPanRespV2*>(src));
	}

	return *this;

}

inline SSmsMsg::SGetPanRespV2& SSmsMsg::SGetPanRespV2::operator =(const SSmsMsg::SGetPanRespV3& v3) // Convert from V3
{

	const SGetPanRespV3* src = &v3;

	if(static_cast<void*>(this) == static_cast<const void*>(&v3))
	{
		// Make a copy
		if((src = static_cast<const SGetPanRespV3*>(malloc(offsetof(SGetPanRespV3, binData[v3.numBins])))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SGetPanRespV3*>(src), &v3, offsetof(SGetPanRespV3, binData[v3.numBins]));
	}

	// SGetPanRespV2 v2;
	dateTime = src->dateTime;
	freq = src->freq;
	binSize = src->binSize;
	status = src->status;
	numBins = src->numBins;
	nActiveAudioChannels = src->nActiveAudioChannels;
	powerDbm = src->powerDbm;
	rcvrAtten = src->rcvrAtten;
	for (size_t i = 0; i < MAX_AUDIOCHANNELS; i++)
	{
		audioPower[i].active = src->audioPower[i].active;
		audioPower[i].powerdBm = src->audioPower[i].powerdBm;
	}

	memcpy(binData, src->binData, numBins);

	if (src != &v3)
	{
		free(const_cast<SGetPanRespV3*>(src));
	}

	return *this;

}

//////////////////////////////////////////////////////////////////////
/// <summary>
/// Indicate whether the hardware type is of 5141 variation.
/// </summary>
/// <param name="hwType">
/// Hardware type to check.
/// </param>
/// </returns>
/// true: The specified hardware is of 5141 variation.
/// false: The specified hardware is not of 5141 variation.
/// </returns>
inline bool SSmsMsg::Is5141Type(_In_ SSmsMsg::EMetricHw hwType)
{
	return(hwType == TCI5141 || hwType == TCI5141_1 || hwType == TCI5141_2 ||
		hwType == TCI5141_4 || hwType == TCI5141_640H_HF ||
		hwType == TCI5141_3GHZ_640_HF || hwType == TCI5141_3GHZ_640H);
}

// selectany lets this go in the .h file
#ifdef _MSC_VER
const _declspec(selectany) DATE SSmsMsg::MAX_DATE = 99999.9;
#else
const __attribute__((weak)) DATE SSmsMsg::MAX_DATE = 99999.9;
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
