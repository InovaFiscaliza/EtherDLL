/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2016-2017 TCI International, Inc. All rights reserved         *
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

struct SVCPMsg
:
	SSmsMsg
{
	// Message subtypes for message type VCP_CTRL
	enum EVCPCtrl : unsigned long
	{
		SET_VCP_MONITOR 		= 1,
		SET_VCP_MONITOR_RESP 	= 2,
		// The next set of messages are for the radio (2630)
		GET_RX_DATA 			= 5,
		GET_RX_DATA_RESP 		= 6,
		GET_RX_CAPABILITIES 	= 7,
		GET_RX_CAPABILITIES_RESP = 8,
		RX_TUNE					= 9,
		RX_TUNE_RESP			= 10,
		GET_RX_MOC				= 11,
		GET_RX_MOC_RESP			= 12,
		GET_RX_ATTEN			= 13,
		SET_RX_ATTEN			= 14,
		RX_ATTEN_RESP			= 15,
		GET_RX_CALGEN			= 16,
		SET_RX_CALGEN			= 17,
		RX_CALGEN_RESP			= 18,
		GET_RX_OCXO				= 19,
		GET_RX_OCXO_RESP		= 20,
		GET_RX_TEMPS			= 21,
		GET_RX_TEMPS_RESP		= 22,
		// The next set of messages are for the digitizer (3230)
		GET_REG_VALS 			= 51,
		GET_REG_VALS_RESP 		= 52,
		GET_SPECTRUM_DATA	 	= 53,
		GET_SPECTRUM_DATA_RESP 	= 54,
		GET_DIGI_TEMPS			= 55,
		GET_DIGI_TEMPS_RESP		= 56,
		GET_DIGI_VOLTS			= 57,
		GET_DIGI_VOLTS_RESP		= 58,
		// The next set are for the antenna switch via 3230
		GET_ANTENNA_SWITCH		= 81,
		GET_ANTENNA_SWITCH_RESP = 82,
		SET_ANTENNA_SWITCH		= 83,
		SET_ANTENNA_SWITCH_RESP = 84
	};

	// Message version data
	static const SVersionData VERSION_DATA[];

	// Message structures

	struct SSetMonitorMode
	{
		bool bMonitorMode;
	};

	struct SSetMonitorModeResp
	{
		ErrorCodes::EErrorCode status;
		bool bMonitorMode;
	};

	struct SGetReceiverDataResp	//	GET_RX_DATA_RESP
	{
		float rffreq;
		float lo1freq;
		float lo2freq;
		float if1freq;
		float if2freq;
		int AttMode;
		float psFreqLow;
		float psFreqHigh;
		unsigned long antSetting;
		unsigned long attenuation;
		unsigned long bandwidth;
		unsigned long lolockstatus;
		unsigned long FPIndex;
		unsigned long lnastatus;
		unsigned long spectralsense;
		//char [32];
	};

	struct SGetRxCapabilitiesResp		//GET_RX_CAPABILITIES_RESP Variable length
	{
		struct SPsFreq
		{
			unsigned long low;
			unsigned long high;
		};
		unsigned long firmwareVersion;
		unsigned long fpgaVersion;
		unsigned long flashVersion;
		unsigned long firmwareDate;
		unsigned long flashSize;
		unsigned int freqtableCount; // number of entries in table
		SPsFreq psfreq[1];
	};

	struct SGetRegValsData // GET_REG_VALS
	{
		unsigned int start;
		unsigned int end;
	};

	struct SGetRegValsDataResp // GET_REG_VALS_RESP (variable length)
	{
		unsigned int start;
		unsigned int end;
		unsigned long  regValsData[1]; // Variable length
	};

	// No body // GET_RX_MOC

	enum ERfInput : unsigned char { TERMINATE, VUSHFANT1, VUSHFANT2, HFANT, VUSHFCAL, HFCAL };
	struct SRxMocResp // RX_MOC_RESP
	{
		ErrorCodes::EErrorCode status;
		unsigned long freqMHz;
		unsigned char fpi;	// 0 => automatic
		unsigned char bw;
		ERfInput rfInput;
	};

	struct SRxTune // RX_TUNE
	{
		Units::Frequency::Raw freq;
		unsigned char fpi;	// 0 => automatic
		unsigned char bw;
		ERfInput rfInput;
	};

	struct SRxTuneResp // RX_TUNE_RESP
	{
		SRxMocResp moc;
		bool inverted;
		bool direct;
		Units::Frequency::Raw radioFreq;
		Units::Frequency::Raw ifFreq;
	};

	enum EGainMode { NORMAL, RURAL, URBAN, CONGESTED };
	// No body for GET_RX_ATTEN
	struct SSetRxAtten	// SET_RX_ATTEN
	{
		EGainMode gainMode;
		unsigned char atten;
	};
	
	struct SRxAttenResp	// RX_ATTEN_RESP
	{
		ErrorCodes::EErrorCode status;
		EGainMode gainMode;
		unsigned char atten;
		bool lna;
	};

	// No body for GET_RX_CALGEN
	struct SSetRxCalgen  // SET_RX_CALGEN
	{
		bool on;	// set on or off
		unsigned long freqMHz;
	};

	struct SRxCalgenResp  // RX_CALGEN_RESP
	{
		ErrorCodes::EErrorCode status;
		bool on;	// set on or off
		unsigned long freqMHz;
	};

	// No body for GET_RX_OCXO
	struct SGetRxOcxoResp	// GET_RX_OCXO_RESP
	{
		ErrorCodes::EErrorCode status;
		unsigned long dacCode;
		unsigned long interval;
		unsigned char loopStatus;
	};

	// No body for GET_RX_TEMPS
	struct SGetRxTempsResp	// GET_RX_TEMPS_RESP
	{
		ErrorCodes::EErrorCode status;
		float nios;
		float sf2450;
		float ocxo;
	};

	struct SGetSpectrumData	// GET_SPECTRUM_DATA
	{
		float average;			// number must be >= 0 and < 1
		bool gainEqualization;	// use gain equalization
	};
	struct SGetSpectrumDataResp	// GET_SPECTRUM_DATA_RESP
	{
		ErrorCodes::EErrorCode status;
		Units::Frequency::Raw binSize;
		unsigned long numBins;
		float binData[1];
	};


	// No body for GET_DIGI_TEMPS
	struct SGetDigiTempsResp	// GET_DIGI_TEMPS_RESP
	{
		ErrorCodes::EErrorCode status;
		float ZYNQ_INTERNAL_TEMP;
		float BOARD_TEMP;
	};
	// No body for GET_DIGI_VOLTS
	struct SGetDigiVoltsResp // GET_DIGI_VOLTS_RESP
	{
		ErrorCodes::EErrorCode status;
		float ZYNQ_INTERNAL_VCCINT;
		float ZYNQ_INTERNAL_VCCAUX;
		float ZYNQ_INTERNAL_VCCBRAM;
		float ZYNQ_INTERNAL_VCCPINT;
		float ZYNQ_INTERNAL_VCCPAUX;
		float ZYNQ_INTERNAL_VCCODDR;
		float BOARD_5p3V_INPUT;
		float DIGITAL_3p3V;
		float DIGITAL_2p5V;
		float MGT_1p0V;
		float MGT_1p2V;
		float MGT_1p8V;
		float PS_DDR3_VTT;
		float PL_DDR3_VTT;
		float BOARD_TOTAL_CURR_5p3V;
		float ADC_3p0V_ANALOG;
		float ADC_1p8V_ANALOG;
		float ADC_1p2V_ANALOG;
		float CLOCK_3p3_ANALOG;
		float CLOCK_3p3_DIGITAL;
		float TEMP_REF_2p5V;
	};

	// No body for GET_ANTENNA_SWITCH
	union USwitch
	{
		struct SHf
		{
			bool calToAnt;
			bool calToRef;
			bool calEnable;
			bool noiseGen;
			bool noiseSwitch;
			bool refSource;
			bool lbBypass;
			bool refTerm;
			unsigned char chanSelect;
		} hf;
		struct SVushf
		{
			bool calToAnt;
			bool calToRef;
			bool cal0;
			bool cal1;
			bool calToneNoiseSource;
			bool cBandVuhfElement;
			bool cBandVuhfAmp;
			bool vertHoriz;
			unsigned char defaultAnt;
		} vushf;
	};

	struct SGetAntennaSwitchResp	// GET_ANTENNA_SWITCH_RESP
	{
		ErrorCodes::EErrorCode status;
		bool isHf;
		USwitch uSw;
	};

	struct SSetAntennaSwitch	// SET_ANTENNA_SWITCH
	{
		bool isHf;
		USwitch uSw;
	};

	struct SSetAntennaSwitchResp	// GET_ANTENNA_SWITCH_RESP
	{
		ErrorCodes::EErrorCode status;
		bool isHf;
		USwitch uSw;
	};

	// Message format
#pragma pack(push, 4)
	SHdr hdr;

	union UBody
	{
		SGetMsgVersionsData getMsgVersionsData; // Variable length
		SGetRegValsData getRegValsData;
		SGetRegValsDataResp getRegValsDataResp;
		SGetReceiverDataResp getReceiverDataResp;
		SGetRxCapabilitiesResp getRxCapabilitiesResp;
		SSetMonitorMode setMonitorMode;
		SSetMonitorModeResp setMonitorModeResp;
		SRxTune rxTune;
		SRxTuneResp rxTuneResp;
		SRxMocResp rxMocResp;
		SSetRxAtten setRxAtten;
		SRxAttenResp rxAttenResp;
		SSetRxCalgen setRxCalgen;
		SRxCalgenResp rxCalgenResp;
		SGetRxOcxoResp getRxOcxoResp;
		SGetRxTempsResp getRxTempsResp;
		SGetSpectrumData getSpectrumData;
		SGetSpectrumDataResp getSpectrumDataResp;
		SGetDigiTempsResp getDigiTempsResp;
		SGetDigiVoltsResp getDigiVoltsResp;
		SGetAntennaSwitchResp getAntennaSwitchResp;
		SSetAntennaSwitch setAntennaSwitch;
		SSetAntennaSwitchResp setAntennaSwitchResp;
	} body;
#pragma pack(pop)
};

// Version conversion operators

// Message versions (selectany lets this go in the .h file)
#ifdef _MSC_VER
__declspec(selectany)
#else
__attribute__((__weak__))
#endif
const SVCPMsg::SVersionData SVCPMsg::VERSION_DATA[] =
{
#ifndef SMS_SRV_BUILT
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::SET_VCP_MONITOR }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::SET_VCP_MONITOR_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_DATA }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_DATA_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_CAPABILITIES }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_CAPABILITIES_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::RX_TUNE }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::RX_TUNE_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_MOC }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_MOC_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_ATTEN }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::SET_RX_ATTEN }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::RX_ATTEN_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_CALGEN }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::SET_RX_CALGEN }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::RX_CALGEN_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_OCXO }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_OCXO_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_TEMPS }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_RX_TEMPS_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_REG_VALS }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_REG_VALS_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_SPECTRUM_DATA }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_SPECTRUM_DATA_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_DIGI_TEMPS }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_DIGI_TEMPS_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_DIGI_VOLTS }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_DIGI_VOLTS_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_ANTENNA_SWITCH }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::GET_ANTENNA_SWITCH_RESP }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::SET_ANTENNA_SWITCH }, { 1, 1 } },
	{ { SVCPMsg::VCP_CTRL, SVCPMsg::SET_ANTENNA_SWITCH_RESP }, { 1, 1 } }
#endif
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif
