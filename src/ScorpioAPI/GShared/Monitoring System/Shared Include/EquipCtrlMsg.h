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

#include "ErrorCodes.h"
#include "SmsMsg.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4480)
#else
#include <cassert>
#include <cstddef>
#include "MSCompat.h"
#endif

struct SEquipCtrlMsg
:
	SSmsMsg
{
	// Message subtypes for message type DSP_CTRL
	enum EDspCtrl
	{
		DO_DSP_INIT				= 1,
		DO_DSP_INIT_RESPONSE	= 65537
	};

	// Message subtypes for message type EQUIPMENT_CTRL
	enum EEquipmentCtrl
	{
		TASK_TERMINATE	= 1,
		GET_TASK_STATUS	= 2,
		CHECK_API_LICENSE = 3,
		API_LICENSE_STATUS = 4,
		TASK_STATUS		= 65538
	};

	// Message subtypes for message type AUTOVIOLATE_CTRL, OCCUPANCY_CTRL, OCCUPANCYDF_CTRL and DIRECTIVE_CTRL
	enum EOccupancyDfCtrl
	{
		GET_OCCUPANCY =					1,
		SUSPEND_OCCUPANCY =				2,
		RESUME_OCCUPANCY =				3,
		RESTART_OCCUPANCY =				4,		//Obsolete.
		OCC_DATA_RESTART_RESPONSE =		5,		//Obsolete.
		OCC_NODATA_RESTART_RESPONSE =	6,		//Obsolete.
		VALIDATE_OCCUPANCY =			7,
		GET_TASK_STATE =				8,
		GET_OCCUPANCY_SCANDF =			9,
		VALIDATE_OCCUPANCY_SCANDF =		10,
		GET_AUTOVIOLATE =				11,		// added for AVD
		VALIDATE_AUTOVIOLATE =			12,		// added for AVD
#ifndef SMS_SRV_BUILT
		GET_DM=                         13,     // added for DM
		VALIDATE_DM =                   14,     // added for DM
#endif
		NEXT_OCCUPANCY =				32769,
		SEND_OCCUPANCY_RESPONSE =		32770,
		AVD_MEAS =						32771,	// added for AVD
#ifndef SMS_SRV_BUILT
		DM_MEAS =                       32772,  // added for DM
#endif
		VALIDATE_OCC_RESPONSE =			65537,
// Duplicate		TASK_STATUS =					65538,
		OCC_CHANNEL_RESPONSE =			65539,
		EFLD_CHANNEL_RESPONSE =			65540,
		OCC_TIMEOFDAY_RESPONSE =		65541,
		MSGLEN_CHANNEL_RESPONSE =		65542,
		MSGLEN_DIST_RESPONSE =			65543,
		OCCUPANCY_STATE_RESP =			65544,
		OCCUPANCY_SOLICIT_STATE_RESP =	65545,
		OCC_FREQUENCY_RESULT =			65546,
		OCC_CHANNEL_RESULT =			65547,
		EFLD_CHANNEL_RESULT =			65548,
		OCC_TIMEOFDAY_RESULT =			65549,
		MSGLEN_CHANNEL_RESULT =			65550,
		OCC_SPECGRAM_RESULT =			65551,
		OCC_DATA_RESTART_REQUEST =		65552,
		SCANDF_DATA_RESULT =			65553,
		AUTOVIOLATE_FREQMEAS_RESULT =	65556,	// added for AVD
		AUTOVIOLATE_BWMEAS_RESULT =		65557,	// added for AVD
		AVD_FREQUENCY_RESULT =			65558,
		AVD_CHANNEL_RESULT =			65559,
		OCC_INTER_DATA_RESULT =			65564,	// this is RA3 intermediate data
		OCC_EFLD_TIMEOFDAY_RESULT =     65565,
#ifndef SMS_SRV_BUILT
		DM_OCC_FREQ_RESULT=             65566,  // added for DM
		DM_FREQMEAS_RESULT =	        65567,	// added for DM
		DM_BWMEAS_RESULT =		        65568,	// added for DM
		DM_FREQUENCY_RESULT =			65569,  // added for DM
		DM_CHANNEL_OCC_RESULT =	        65570,  // added for DM
		DM_MODMEAS_AM_PLUS_RESULT =	    65571,	// added for DM
		DM_MODMEAS_AM_MINUS_RESULT =    65572,  // added for DM
		DM_MODMEAS_AM_DEPTH_RESULT =    65573,  // added for DM
		DM_MODMEAS_FM_FREQ_RESULT=      65574,  // added for DM
		DM_MODMEAS_PM_PHASE_RESULT=     65575,  // added for DM
		DM_FSMEAS_RESULT =	            65576,	// added for DM
		DM_DFMEAS_VERT_POL_AZ_RESULT =	65577,	// added for DM
		DM_DFMEAS_VERT_POL_CONF_RESULT =65578,	// added for DM
		DM_DFMEAS_HORIZ_POL_AZ_RESULT =	65579,	// added for DM
		DM_DFMEAS_HORIZ_POL_CONF_RESULT=65580,  // added for DM
		DM_ANT_ROTATOR_AZ_RESULT=       65581,  // Added for DM
		DM_ANT_ROTATOR_EL_RESULT=       65582   // Added for DM
#endif
	};

	// Message subtypes for message type WB_DIGITIZER_SAMPLE
	enum EWbDigitizerSample
	{
		SNGL_SAMPLE =		1,
		SNGL_SAMPLE_RESP =	65537
	};


	// Message version data
	static const SVersionData VERSION_DATA[];

	// Message structures
	struct SAgcOnOffCmd // AGC_ONOFF
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum : unsigned long
#else
		enum : uint32_t
#endif
		{
			OFF = 0,
			ON = 1
		} onOff;
	};

	struct SDspWbSmplCmd // SNGL_SAMPLE
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
		unsigned long rxBandwidth; // s/b 25000, 100000, or 10000000
		unsigned long sampleRate;  // s/b 3200000 or 25600000 or 38400000
		unsigned long decimation;  // s/b 1 for bypass mode or 64 to 65536
		unsigned long ddcFirBw;    // s/b 17, 34, 68, 80, 100, or 150 (normally 80)
		unsigned long sampleSize;  // s/b 64 to 4096 (per burst for DSP_FAST_SAMPLE)
		unsigned long channel;     // s/b 0=chan 1 only; 1=chan 2 only; 2=both channels
		unsigned long ch1Thresh1;  // s/b 1 to 15
		unsigned long ch1Thresh2;  // s/b 1 to 15
		unsigned long ch2Thresh1;  // s/b 1 to 15
		unsigned long ch2Thresh2;  // s/b 1 to 15
		unsigned long inputType;   // s/b from definitions for inputType
		unsigned long outputType;  // s/b from definitions for outputType
		unsigned long rxAtten;     // 0 to 78 by 2 for V/UHF
		unsigned long numBins;     // only applicable for outputType = OUT_FREQ
		unsigned long numDwell;    // only applicable for outputType = OUT_FREQ
		unsigned long numBursts;   // number of bursts for DSP_FAST_SAMPLE
#else
		uint32_t taskId;
		uint32_t rxBandwidth; // s/b 25000, 100000, or 10000000
		uint32_t sampleRate;  // s/b 3200000 or 25600000 or 38400000
		uint32_t decimation;  // s/b 1 for bypass mode or 64 to 65536
		uint32_t ddcFirBw;    // s/b 17, 34, 68, 80, 100, or 150 (normally 80)
		uint32_t sampleSize;  // s/b 64 to 4096 (per burst for DSP_FAST_SAMPLE)
		uint32_t channel;     // s/b 0=chan 1 only; 1=chan 2 only; 2=both channels
		uint32_t ch1Thresh1;  // s/b 1 to 15
		uint32_t ch1Thresh2;  // s/b 1 to 15
		uint32_t ch2Thresh1;  // s/b 1 to 15
		uint32_t ch2Thresh2;  // s/b 1 to 15
		uint32_t inputType;   // s/b from definitions for inputType
		uint32_t outputType;  // s/b from definitions for outputType
		uint32_t rxAtten;     // 0 to 78 by 2 for V/UHF
		uint32_t numBins;     // only applicable for outputType = OUT_FREQ
		uint32_t numDwell;    // only applicable for outputType = OUT_FREQ
		uint32_t numBursts;   // number of bursts for DSP_FAST_SAMPLE
#endif
		};

	struct SDspWbSmplResp // SNGL_SAMPLE_RESP
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
		ErrorCodes::EErrorCode status;
		unsigned long ch1Thrsh1Cntr;  // 0 to 16777215 (24 bits)
		unsigned long ch1Thrsh2Cntr;  // 0 to 16777215 (24 bits)
		unsigned long ch2Thrsh1Cntr;  // 0 to 16777215 (24 bits)
		unsigned long ch2Thrsh2Cntr;  // 0 to 16777215 (24 bits)
		long data[2048];     // 2 16-bit "packed" values/word
#else
		uint32_t taskId;
		ErrorCodes::EErrorCode status;
		uint32_t ch1Thrsh1Cntr;  // 0 to 16777215 (24 bits)
		uint32_t ch1Thrsh2Cntr;  // 0 to 16777215 (24 bits)
		uint32_t ch2Thrsh1Cntr;  // 0 to 16777215 (24 bits)
		uint32_t ch2Thrsh2Cntr;  // 0 to 16777215 (24 bits)
		int32_t data[2048];     // 2 16-bit "packed" values/word
#endif
	};

	struct SEquipTaskStatusResp // TASK_STATUS
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId; // unique ID assigned by equipcontrol
		unsigned long key; // unique key needed for other commands

		enum EStatus : unsigned long
#else
		uint32_t taskId; // unique ID assigned by equipcontrol
		uint32_t key; // unique key needed for other commands

		enum EStatus : uint32_t
#endif
		{
			STARTED		= 1,
			COMPLETED	= 2,
			TERMINATED	= 3,
			RESTARTED	= 4,
			ACTIVE		= 5
		}; 

		EStatus status;
		double dateTime;
	};

	struct SGetIonogram // GET_IONOGRAM
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
#else
		uint32_t taskId;
#endif
	};

	struct SGetIonogramResp // GET_IONOGRAM_RESPONSE
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;
#else
		uint32_t taskId;
#endif
		ErrorCodes::EErrorCode status;
		SIonogramDataMsg ionoData;
	};

	struct SStateResp // OCCUPANCY_SOLICIT_STATE_RESP, OCCUPANCY_STATE_RESP
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum EState : int
#else
		enum EState : int32_t
#endif
		{
			IDLE		= 0,
			RUNNING		= 1,
			SUSPENDED	= 2
		};

		EState state;
		double completionTime;
	};

	struct STaskIdKey // RESUME_OCCUPANCY, SUSPEND_OCCUPANCY, TASK_TERMINATE
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long taskId;			// equipcontrol ID for task (from start msg)
		unsigned long key;				// equipcontrol key for task (from start msg)
#else
		uint32_t taskId;			// equipcontrol ID for task (from start msg)
		uint32_t key;				// equipcontrol key for task (from start msg)
#endif
	};

	struct SValidateMeasurementResp // VALIDATE_MEAS_RESPONSE
	{
		ErrorCodes::EErrorCode status;
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	bwDwellTime;
		unsigned long	freqDwellTime;
		unsigned long	modulationDwellTime;
		unsigned long	fieldStrengthDwellTime;
		unsigned long	dfDwellTime;
		unsigned long	totalTime;
#else
		uint32_t	bwDwellTime;
		uint32_t	freqDwellTime;
		uint32_t	modulationDwellTime;
		uint32_t	fieldStrengthDwellTime;
		uint32_t	dfDwellTime;
		uint32_t	totalTime;
#endif
	};

	struct SValidateOccupancyRespV1; // Forward declaration

	struct SValidateOccupancyRespV0 // VALIDATE_OCC_RESP V0
	{
		inline operator SValidateOccupancyRespV1(void) const; // Convert to V1

		ErrorCodes::EErrorCode status;
		SGetOccupancyCmdV0 occCmd;
	};

	struct SValidateOccupancyRespV1 // VALIDATE_OCC_RESP V1
	{
		operator SValidateOccupancyRespV0(void) const // Convert to V0
		{
			SValidateOccupancyRespV0 v0;
			v0.status = status;
			v0.occCmd = occCmd;

			return v0;
		}

		ErrorCodes::EErrorCode status;
		SGetOccupancyCmdV1 occCmd;
	};

	struct SValidateOccupancyRespV3; // Forward declaration

	//@note: SValidateOccupancyRespV2 is fixed size eventhough it contains
	//       SGetOccupancyCmdV2 that is of variable length. To make it fixed,
	//		 the code only populate the 1st band array and discards the
	//		 remaining band info.
	struct SValidateOccupancyRespV2 // VALIDATE_OCC_RESP V2.
	{
		operator SValidateOccupancyRespV1(void) const // Convert to V1
		{
//			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV2.occCmd.band[occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV2.occCmd.band) +
				occCmd.numBands * sizeof(SEquipCtrlMsg::SGetOccupancyCmdV2::SBand) <= sizeof(SEquipCtrlMsg));
			SValidateOccupancyRespV1 v1;
			v1.status = status;
			v1.occCmd = occCmd;

			return v1;
		}

		SValidateOccupancyRespV2& operator =(SValidateOccupancyRespV1 v1) // Convert from V1
		{
//			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV2.occCmd.band[occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV2.occCmd.band) +
				occCmd.numBands * sizeof(SEquipCtrlMsg::SGetOccupancyCmdV2::SBand) <= sizeof(SEquipCtrlMsg));

			status = v1.status;
			occCmd = v1.occCmd;

			return *this;
		}

		inline SValidateOccupancyRespV2& operator =(const SValidateOccupancyRespV3& v3); // Convert from V3

		ErrorCodes::EErrorCode status;
		SGetOccupancyCmdV2 occCmd;
	};

	struct SValidateOccupancyRespV4; // Forward declaration

	//@note: SValidateOccupancyRespV3 is fixed size eventhough it contains
	//       SGetOccupancyCmdV3 that is of variable length. To make it fixed,
	//		 the code only populate the 1st band array and discards the
	//		 remaining band info.
	struct SValidateOccupancyRespV3 // VALIDATE_OCC_RESP V3 (Fixed due to single band array element)
	{
		operator SValidateOccupancyRespV2(void) const // Convert to V2
		{
//			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV3.occCmd.band[occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV3.occCmd.band) +
				occCmd.numBands * sizeof(SEquipCtrlMsg::SGetOccupancyCmdV3::SBand) <= sizeof(SEquipCtrlMsg));
			SValidateOccupancyRespV2 v2;
			v2.status = status;
			v2.occCmd = occCmd;

			return v2;
		}

		SValidateOccupancyRespV3& operator =(const SValidateOccupancyRespV2& v2) // Convert from V2
		{
			ASSERT(!IsOnStack(&v2) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV2.occCmd.band[v2.occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			status = v2.status;
			occCmd = v2.occCmd;

			return *this;
		}

		inline SValidateOccupancyRespV3& operator =(const SValidateOccupancyRespV4& v4); //Down-Convert from V4

		ErrorCodes::EErrorCode status;
		SGetOccupancyCmdV3 occCmd;
	};

	struct SValidateOccupancyRespV5; // Forward declaration

	//@note: SValidateOccupancyRespV4 is fixed size eventhough it contains
	//       SGetOccupancyCmdV4 that is of variable length. To make it fixed,
	//		 the code only populate the 1st band array and discards the
	//		 remaining band info.
	struct SValidateOccupancyRespV4 // VALIDATE_OCC_RESP V4 (Fixed due to single band array element)
	{
		operator SValidateOccupancyRespV3(void) const // Convert to V3
		{
//			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV4.occCmd.band[occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV4.occCmd.band) +
				occCmd.numBands * sizeof(SEquipCtrlMsg::SBandV4) <= sizeof(SEquipCtrlMsg));

			SValidateOccupancyRespV3 v3;
			v3.status = status;
			v3.occCmd = occCmd;

			return v3;
		}

		SValidateOccupancyRespV4& operator =(const SValidateOccupancyRespV3& v3) // Convert from V3
		{
			ASSERT(!IsOnStack(&v3) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV3.occCmd.band[v3.occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			status = v3.status;
			occCmd = v3.occCmd;

			return *this;
		}

		ErrorCodes::EErrorCode status;
		SGetOccupancyCmdV4 occCmd;
	};

	//@note: SValidateOccupancyRespV5 is fixed size eventhough it contains
	//       SGetOccupancyCmdV5 that is of variable length. To make it fixed,
	//		 the code only populate the 1st band array and discards the
	//		 remaining band info.
	struct SValidateOccupancyRespV5 // VALIDATE_OCC_RESP V5 (Fixed due to single band array element)
	{
		operator SValidateOccupancyRespV4(void) const // Convert to V4
		{
//			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV5.occCmd.band[occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			ASSERT(!IsOnStack(this) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV5.occCmd.band) +
				occCmd.numBands * sizeof(SEquipCtrlMsg::SBandV4) <= sizeof(SEquipCtrlMsg));

			SValidateOccupancyRespV4 v4;
			v4.status = status;
			v4.occCmd = occCmd;

			return v4;
		}

		SValidateOccupancyRespV5& operator =(const SValidateOccupancyRespV4& v4) // Convert from V4
		{
			ASSERT(!IsOnStack(&v4) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV4.occCmd.band[v4.occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
			status = v4.status;
			occCmd = v4.occCmd;

			return *this;
		}

		ErrorCodes::EErrorCode status;
		SGetOccupancyCmdV5 occCmd;
	};

	typedef SValidateOccupancyRespV5 SValidateOccupancyResp;

	struct SAPILicenseCheckResp // OCCUPANCY_SOLICIT_STATE_RESP, OCCUPANCY_STATE_RESP
	{
#ifndef CSMSPLUS_SRV_BUILT
		enum ELicenseState : int
#else
		enum ELicenseState : int32_t
#endif
		{
			API_LICENSE_PASSED						= 0,
			FAILED_NOLICENSE						= 1,
			FAILED_CONNECTIONS_EXCEEDED				= 2
		};

		ELicenseState state;
	};

#if defined(SMS_SRV_BUILT) || defined(CSMS_SRV_BUILT)
#else
	struct SWbRecorderSignInRespV1 // WBRECORDER_SIGN_IN_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SWbRecorderSignInRespV1 SWbRecorderSignInResp;

	struct SWbRecorderSignOutRespV1 // WBRECORDER_SIGN_OUT_RESP
	{
		ErrorCodes::EErrorCode success;
	};
	typedef SWbRecorderSignOutRespV1 SWbRecorderSignOutResp;
#endif

	// Message format
#pragma pack(push, 4)
	SHdr hdr;

	union UBody
	{
		SAgcOnOffCmd agcOnOffCmd;
		SAntGetSetCtrlResp antGetSetCtrlResp;
#ifndef SMS_SRV_BUILT
		SAntInfoListResp	antInfoListResp;
		SAntInfoListRespV1	antInfoListRespV1;
#endif
		SAntSetCtrlCmd antSetCtrlCmd;
		SAudioParamsCmd audioParamsCmd;
		SAudioParamsCmdV1 audioParamsCmdV1;
		SAudioParamsCmdV2 audioParamsCmdV2;
#ifndef SMS_SRV_BUILT
		SAudioParamsCmdV3 audioParamsCmdV3;
		SAudioParamsCmdV4 audioParamsCmdV4;
		SAudioParamsCmdV5 audioParamsCmdV5;
#endif
		SAudioParamsResp audioParamsResp;
		SAudioParamsRespV1 audioParamsRespV1;
		SAudioParamsRespV2 audioParamsRespV2;
		SAudioSwitchStatusResp audioSwitchStatusResp;
		SAvdMeasureResult avdMeasureResult; // Variable length
		SAvdMeasureResultV0 avdMeasureResultV0;
		SAvdMeasureResultV1 avdMeasureResultV1; // Variable length
#ifndef SMS_SRV_BUILT
		SDmMeasureResult dmMeasureResult; // Variable length
#endif
		SDspWbSmplCmd dspWbSmplCmd;
		SDspWbSmplResp dspWbSmplResp;
		SEquipTaskStatusResp equipTaskStatusResp;
		SFreeAudioChannelCmd freeAudioChannelCmd;
		SFreeAudioChannelCmdV1 freeAudioChannelCmdV1;
		SFrequencyVsChannelResp frequencyVsChannelResp; // Variable length
		SFrequencyVsChannelRespV0 frequencyVsChannelRespV0;
		SFrequencyVsChannelRespV1 frequencyVsChannelRespV1; // Variable length
		SFrequencyVsChannelRespV2 frequencyVsChannelRespV2;
#ifndef SMS_SRV_BUILT
		SFrequencyVsChannelRespForDmV1 frequencyVsChannelRespForDm;
#endif
		SGenericResp genericResp;
		SGetAutoViolateCmd getAutoViolateCmd; // Variable length
		SGetAutoViolateCmdV0 getAutoViolateCmdV0;
		SGetAutoViolateCmdV1 getAutoViolateCmdV1;
		SGetAutoViolateCmdV2 getAutoViolateCmdV2; // Variable length
		SGetAutoViolateCmdV3 getAutoViolateCmdV3; // Variable length
#ifndef SMS_SRV_BUILT
		SGetDmCtrlCmd getDmCtrlCmd; // Variable length
		SGetDmCtrlCmdV1 getDmCtrlCmdV1; // Variable length
		SGetDmCtrlCmdV2 getDmCtrlCmdV2;
#endif
		SGetBandResp getBandResp;
		SGetBandRespV0 getBandRespV0;
		SGetBandRespV1 getBandRespV1;
		SGetBandRespV2 getBandRespV2;
		SGetBandRespV3 getBandRespV3;
		SGetBandRespV4 getBandRespV4;
		SGetBandRespV5 getBandRespV5;
		SGetBandRespV6 getBandRespV6;
		SGetBandRespV7 getBandRespV7;
		SGetBistResp getBistResp; // Variable length
		SGetBistRespOld getBistRespOld;
		SGetDwellCmd getDwellCmd;
		SGetDwellCmdV0 getDwellCmdV0;
		SGetDwellCmdV1 getDwellCmdV1;
		SGetDwellResp getDwellResp;
		SGetFaultRespV1 getFaultRespV1;
		SGetFaultResp getFaultResp;
		SGetCsmsFaultRespV1 getCsmsFaultRespV1;
		SGetCsmsFaultRespV2 getCsmsFaultRespV2;
		SGetCsmsFaultResp getCsmsFaultResp;
		SGetCsmsplusFaultRespV1 getCsmsplusFaultRespV1;
		SGetCsmsplusFaultResp getCsmsplusFaultResp;
		SGetGpsResp getGpsResp;
		SGetHeadingResp getHeadingResp;
		SGetIonogram getIonogram;
		SGetIonogramResp getIonogramResp;
		SGetMeasCmd getMeasCmd;
		SGetMeasCmdV0 getMeasCmdV0;
		SGetMeasCmdV1 getMeasCmdV1;
		SGetMeasCmdV2 getMeasCmdV2;
		SGetMeasCmdV3 getMeasCmdV3;
		SGetMeasCmdV4 getMeasCmdV4;
		SGetMeasCmdV5 getMeasCmdV5;
		SGetMeasResp getMeasResp;
		SGetMeasRespV0 getMeasRespV0;
		SGetMeasRespV1 getMeasRespV1;
		SGetMeasRespV2 getMeasRespV2;
		SGetMeasRespV3 getMeasRespV3;
		SGetMeasRespV4 getMeasRespV4;
		SGetMeasRespV5 getMeasRespV5;
		SIqDataResp iqDataResp;
		SIqDataRespV0 iqDataRespV0;
		SIqDataRespV1 iqDataRespV1;
		SGetMsgVersionsData getMsgVersionsData; // Variable length
		SGetOccupancyCmd getOccupancyCmd; // Variable length
		SGetOccupancyCmdV0 getOccupancyCmdV0;
		SGetOccupancyCmdV1 getOccupancyCmdV1;
		SGetOccupancyCmdV2 getOccupancyCmdV2; // Variable length
		SGetOccupancyCmdV3 getOccupancyCmdV3; // Variable length
		SGetOccupancyCmdV4 getOccupancyCmdV4; // Variable length
		SGetOccupancyCmdV5 getOccupancyCmdV5; // Variable length
		SGetPanCmd getPanCmd;
		SGetPanCmdV0 getPanCmdV0;
		SGetPanCmdV1 getPanCmdV1;
		SGetPanCmdV2 getPanCmdV2;
		SGetPanResp getPanResp; // Variable length
		SGetPanRespV0 getPanRespV0;
		SGetPanRespV1 getPanRespV1; // Variable length
		SGetPanRespV2 getPanRespV2; // Variable length
		SGetPanRespV3 getPanRespV3; // Variable length
		SGetScanDfCmd getScanDfCmd; // Variable length
		SGetScanDfCmdV0 getScanDfCmdV0;
		SGetScanDfCmdV1 getScanDfCmdV1; // Variable length
		SInitializeDf initializeDf;
		SInitializeDfV1 initializeDfV1; // V0 has no body
		SInitializeDfV2 initializeDfV2;
		SInitializeDfResp initializeDfResp; // Variable length
		SInitializeDfRespV0 initializeDfRespV0;
		SInitializeDfRespV1 initializeDfRespV1; // Variable length
		SInitializeDfRespV2 initializeDfRespV2; // Variable length
		SInitializeDfRespV3 initializeDfRespV3; // Variable length
		SInitializeDfRespV4 initializeDfRespV4; // Variable length
		SLoadIonogram loadIonogram;
		SLoadIonogramResp loadIonogramResp;
		SMsgLengthDistributionResp msgLengthDistributionResp;
		SOccResult occResult;
#ifndef SMS_SRV_BUILT
		SOccResultForDM occResultDM;
#endif
		SStateResp stateResp;
		SRcvrCtrlCmd rcvrCtrlCmd;
		SRcvrCtrlCmdV0 rcvrCtrlCmdV0;
		SRcvrCtrlCmdV1 rcvrCtrlCmdV1;
		SRcvrManCmd rcvrManCmd;
//		SPanCapabilitiesResponse panCapabilitiesResponse;
		SPanParaCmd panParaCmd;
		SPanParaCmdV1 panParaCmdV1;
		SScanDfVsChannelResp scanDfVsChannelResp;
		SSetAudioSwitchCmd setAudioSwitchCmd;
		SSetAutoAnswerCmd setAutoAnswerCmd;
		SRcvrLanCmd rcvrLanCmd;
		SSetPhoneHookCmd setPhoneHookCmd;
		STaskIdKey taskIdKey;
		SValidateMeasurementResp validateMeasurementResp;
		SValidateOccupancyResp validateOccupancyResp; // Fixed size with 1 band.
		SValidateOccupancyRespV0 validateOccupancyRespV0;
		SValidateOccupancyRespV1 validateOccupancyRespV1;
		SValidateOccupancyRespV2 validateOccupancyRespV2; // Fixed size with 1 band.
		SValidateOccupancyRespV3 validateOccupancyRespV3; // Fixed size with 1 band.
		SValidateOccupancyRespV4 validateOccupancyRespV4; // Fixed size with 1 band.
		SValidateOccupancyRespV5 validateOccupancyRespV5; // Fixed size with 1 band.
		SSounderSetupCmd sounderSweepParamsSetCmd;
		SSounderSetupCmdV1 sounderSweepParamsSetCmdV1;
		SBlankerListCmd sounderBlankerFreqParamsSetCmd;
		SBlankerListCmdV1 sounderBlankerFreqParamsSetCmdV1;
		SSounderSweepCtrlCmd sounderSweepOnOffCtrlCmd;
		SSounderSweepCtrlCmdV1 sounderSweepOnOffCtrlCmdV1;
		SSounderSelfTestCmd sounderSelfTestCmd;
		SSounderSelfTestCmdV1 sounderSelfTestCmdV1;
		SGetSounderStatusResp sounderStatusResp;
		SGetSounderStatusRespV1 sounderStatusRespV1;
		SSounderGenericResp   sounderGenericResp;
		SSounderGenericRespV1   sounderGenericRespV1;
		SAPILicenseCheckResp validateAPILicenseResp;
#if defined(SMS_SRV_BUILT) || defined(CSMS_SRV_BUILT)
#else
		SWbRecorderStart wbRecorderStart;
		SWbRecorderQueryReq wbRecorderQueryReq;
		SWbRecorderIqdataReq wbRecorderIqdataReq;
		SWbRecorderStatusResp wbRecorderStatusResp;
		SWbRecorderCatalogResp wbRecorderCatalogResp;
		SWbRecorderQueryResp wbRecorderQueryResp;
		SWbRecorderIqdataResp wbRecorderIqdataResp;
		SWbRecorderIqdataStopResp wbRecorderIqdataStopResp;
		SWbRecorderSpectrumResp wbRecorderSpectrumResp;
		SWbRecorderSignInResp wbRecorderSignInResp;
		SWbRecorderSignOutResp wbRecorderSignOutResp;
		SStreamingInit streamingInit;
		SStreamingRfParams streamingRfParams;
		SStreamingStartWb streamingStartWb;
		SStreamingStartDdc streamingStartDdc;
		SStreamingStopDdc streamingStopDdc;
		SStreamingInitResp streamingInitResp;
		SStreamingRfParamsResp streamingRfParamsResp;
		SStreamingRfStopResp streamingRfStopResp;
		SStreamingStartWbResp streamingStartWbResp;
		SStreamingStopWbResp streamingStopWbResp;
		SStreamingStartDdcResp streamingStartDdcResp;
		SStreamingStopDdcResp streamingStopDdcResp;
#endif
	} body;
#pragma pack(pop)
};

// Version conversion operators
inline SEquipCtrlMsg::SValidateOccupancyRespV0::operator SEquipCtrlMsg::SValidateOccupancyRespV1(void) const
{
	SValidateOccupancyRespV1 v1;
	v1.status = status;
	v1.occCmd = occCmd;

	return v1;
}

inline SEquipCtrlMsg::SValidateOccupancyRespV2& SEquipCtrlMsg::SValidateOccupancyRespV2::operator =(const SValidateOccupancyRespV3& v3) // Convert from V3
{
	ASSERT(!IsOnStack(&v3) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV3.occCmd.band[v3.occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
	status = v3.status;
	occCmd = v3.occCmd;

	return *this;
}

inline SEquipCtrlMsg::SValidateOccupancyRespV3& SEquipCtrlMsg::SValidateOccupancyRespV3::operator =(const SValidateOccupancyRespV4& v4) //Down-Convert from V4
{
	ASSERT(!IsOnStack(&v4) || offsetof(SEquipCtrlMsg, body.validateOccupancyRespV4.occCmd.band[v4.occCmd.numBands]) <= sizeof(SEquipCtrlMsg));
	status = v4.status;
	occCmd = v4.occCmd;

	return *this;
}

// Message versions (selectany lets this go in the .h file)
#ifdef _MSC_VER
__declspec(selectany)
#else
__attribute__((__weak__))
#endif
const SEquipCtrlMsg::SVersionData SEquipCtrlMsg::VERSION_DATA[] =
{
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::AUTOVIOLATE_BWMEAS_RESULT }, { 2, 1 } },
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::AUTOVIOLATE_FREQMEAS_RESULT }, { 2, 1 } },
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::GET_AUTOVIOLATE }, { 3, 2 } },
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::OCC_FREQUENCY_RESULT }, { 3, 2 } },
#else
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::OCC_FREQUENCY_RESULT }, { 3, 1 } },
#endif
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::VALIDATE_AUTOVIOLATE }, { 3, 5 } },
	{ { SEquipCtrlMsg::AUTOVIOLATE_CTRL, SEquipCtrlMsg::VALIDATE_OCC_RESPONSE }, { 3, 5 } },
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_FREQUENCY_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_BWMEAS_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_FREQMEAS_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_MODMEAS_AM_PLUS_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_MODMEAS_AM_MINUS_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_MODMEAS_AM_DEPTH_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_MODMEAS_FM_FREQ_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_MODMEAS_PM_PHASE_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_FSMEAS_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_DFMEAS_VERT_POL_AZ_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_DFMEAS_VERT_POL_CONF_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_DFMEAS_HORIZ_POL_AZ_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_DFMEAS_HORIZ_POL_CONF_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_ANT_ROTATOR_AZ_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::DM_ANT_ROTATOR_EL_RESULT }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::GET_DM }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::VALIDATE_DM }, { 1, 1 } },
	{ { SEquipCtrlMsg::DM_CTRL, SEquipCtrlMsg::VALIDATE_OCC_RESPONSE }, { 1, 1 } },
#endif
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_RCVR }, { 1, 0 } },
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_RCVR_RESP }, { 1, 0 } },
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_PAN_PARA }, { 1, 0 } },
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_PAN_PARA_RESP }, { 1, 0 } },
#ifndef SMS_SRV_BUILT
#ifndef CSMS_SRV_BUILT // for client and CSMSPlus
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_AUDIO_PARAMS }, { 5, 2 } },
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_AUDIO_PARAMS_RESP }, { 5, 2 } },
#else
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_AUDIO_PARAMS }, { 4, 2 } },
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::SET_AUDIO_PARAMS_RESP }, { 4, 2 } },

#endif
#endif
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::FREE_AUDIO_CHANNEL }, { 1, 0 } },
	{ { SEquipCtrlMsg::DEMOD_CTRL, SEquipCtrlMsg::FREE_AUDIO_CHANNEL_RESP }, { 1, 0 } },
#endif
	{ { SEquipCtrlMsg::DF_CTRL, SEquipCtrlMsg::INITIALIZE_DF }, { 2, 4 } },
	{ { SEquipCtrlMsg::DF_CTRL, SEquipCtrlMsg::INITIALIZE_DF_RESPONSE }, { 2, 4 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_BAND_REQUEST }, { 0, 7 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_BAND_RESPONSE }, { 0, 7 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_DWELL }, { 1, 0 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_DWELL_RESPONSE }, { 1, 0 } },
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::MEASURE_CTRL, SEquipCtrlMsg::GET_ANT_LIST_INFO }, { 1, 1 } },
	{ { SEquipCtrlMsg::MEASURE_CTRL, SEquipCtrlMsg::GET_ANT_LIST_RESP }, { 1, 1 } },
#endif
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_MEAS }, { 5, 5 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_MEAS_IQDATA_RESP }, { 0, 1 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::GET_MEAS_RESPONSE }, { 5, 5 } },
	{ { SEquipCtrlMsg::METRICS_CTRL, SEquipCtrlMsg::VALIDATE_MEAS }, { 4, 0 } },
	{ { SEquipCtrlMsg::OCCUPANCY_CTRL, SEquipCtrlMsg::GET_OCCUPANCY }, { 5, 2 } },
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::OCCUPANCY_CTRL, SEquipCtrlMsg::OCC_FREQUENCY_RESULT }, { 5, 2 } },
#else
	{ { SEquipCtrlMsg::OCCUPANCY_CTRL, SEquipCtrlMsg::OCC_FREQUENCY_RESULT }, { 5, 1 } },
#endif
	{ { SEquipCtrlMsg::OCCUPANCY_CTRL, SEquipCtrlMsg::VALIDATE_OCCUPANCY }, { 4, 5 } },
	{ { SEquipCtrlMsg::OCCUPANCY_CTRL, SEquipCtrlMsg::VALIDATE_OCC_RESPONSE }, { 4, 5 } },
	{ { SEquipCtrlMsg::OCCUPANCYDF_CTRL, SEquipCtrlMsg::GET_OCCUPANCY_SCANDF }, { 1, 2 } },
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::OCCUPANCYDF_CTRL, SEquipCtrlMsg::OCC_FREQUENCY_RESULT }, { 1, 2 } },
#else
	{ { SEquipCtrlMsg::OCCUPANCYDF_CTRL, SEquipCtrlMsg::OCC_FREQUENCY_RESULT }, { 1, 1 } },
#endif
	{ { SEquipCtrlMsg::OCCUPANCYDF_CTRL, SEquipCtrlMsg::VALIDATE_OCCUPANCY_SCANDF }, { 1, 5 } },
	{ { SEquipCtrlMsg::OCCUPANCYDF_CTRL, SEquipCtrlMsg::VALIDATE_OCC_RESPONSE }, { 1, 5 } },
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::PAN_DISP_CTRL, SEquipCtrlMsg::GET_PAN }, { 2, 3 } },
	{ { SEquipCtrlMsg::PAN_DISP_CTRL, SEquipCtrlMsg::GET_PAN_AGC }, { 2, 3 } },
	{ { SEquipCtrlMsg::PAN_DISP_CTRL, SEquipCtrlMsg::GET_PAN_RESPONSE }, { 2, 3 } },
#else
	{ { SEquipCtrlMsg::PAN_DISP_CTRL, SEquipCtrlMsg::GET_PAN }, { 2, 1 } },
	{ { SEquipCtrlMsg::PAN_DISP_CTRL, SEquipCtrlMsg::GET_PAN_AGC }, { 2, 1 } },
	{ { SEquipCtrlMsg::PAN_DISP_CTRL, SEquipCtrlMsg::GET_PAN_RESPONSE }, { 2, 1 } },
#endif
#ifndef SMS_SRV_BUILT
	{ { SEquipCtrlMsg::PRIORITY_CTRL, SEquipCtrlMsg::TAKE_PRIORITY }, { 1, 1 } },
	{ { SEquipCtrlMsg::PRIORITY_CTRL, SEquipCtrlMsg::TAKE_PRIORITY_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::SUPER_USER_CTRL, SEquipCtrlMsg::SIGN_IN }, { 1, 1 } },
	{ { SEquipCtrlMsg::SUPER_USER_CTRL, SEquipCtrlMsg::SIGN_IN_RESP }, { 1, 1 } },
#endif
	{ { SEquipCtrlMsg::STATUS_CTRL, SEquipCtrlMsg::GET_FAULT_REQUEST }, { 1, 1 } },
#if !defined(CSMS_SRV_BUILT) && !defined(CSMSPLUS_SRV_BUILT)
	{ { SEquipCtrlMsg::STATUS_CTRL, SEquipCtrlMsg::GET_FAULT_RESP }, { 1, 1 } },	// for client and SMS
#endif
#if !defined(SMS_SRV_BUILT) && !defined(CSMSPLUS_SRV_BUILT)
	{ { SEquipCtrlMsg::STATUS_CTRL, SEquipCtrlMsg::GET_CSMS_FAULT_RESP }, { 1, 2 } },	// for client and CSMS
#endif
#if !defined(SMS_SRV_BUILT) && !defined(CSMS_SRV_BUILT)
	{ { SEquipCtrlMsg::STATUS_CTRL, SEquipCtrlMsg::GET_CSMSPLUS_FAULT_RESP }, { 1, 1 } },	// for client and CSMSPlus
#endif
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_SWEEP_SETUP }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_SETUP_RESPONSE }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_BLANKER_FREQ }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_BLANKER_FREQ_RESPONSE }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_START_STOP }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_START_STOP_RESPONSE }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_TEST }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_TEST_RESPONSE }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_IONOREQUEST }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_IONO_RESPONSE }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_STATUS_REQ }, { 1, 1 } },
	{ { SEquipCtrlMsg::SOUNDER_CTRL, SEquipCtrlMsg::SOUNDER_STATUS_RESPONSE }, { 1, 1 } }
// These are needed in csmsplus and clients but not in sms or csms
#if defined(SMS_SRV_BUILT) || defined(CSMS_SRV_BUILT)
#else
	,
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_START }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_STOP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_CATALOG_REQ }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_QUERY_REQ }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_IQDATA_REQ }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_IQDATA_STOP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_SPECTRUM_REQ }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_SIGN_IN}, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_SIGN_OUT}, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_STATUS_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_CATALOG_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_QUERY_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_IQDATA_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_IQDATA_STOP_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_SPECTRUM_RESP }, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_SIGN_IN_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::WBRECORDER_CTRL, SEquipCtrlMsg::WBRECORDER_SIGN_OUT_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_INIT}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_RF_PARAMS}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_RF_STOP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_START_WB}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_STOP_WB}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_START_DDC}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_STOP_DDC}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_INIT_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_RF_PARAMS_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_RF_STOP_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_START_WB_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_STOP_WB_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_START_DDC_RESP}, { 1, 1 } },
	{ { SEquipCtrlMsg::STREAMING_CTRL, SEquipCtrlMsg::STREAMING_STOP_DDC_RESP}, { 1, 1 } },

#endif
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif
