/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is pursuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2006-2020 TCI. ALL RIGHTS RESERVED                            *
**************************************************************************/

#pragma once

#include "ErrorCodes.h"
#include "SmsMsg.h"
#include "Units.h"

struct SFastModeMsg
:
	SSmsMsg
{
	// Message subtypes for message type FAST_MODE_CTRL
	enum EFastModeCtrl
	{
		GOTO_STANDARD_MODE			= 1,
		GOTO_FAST_MODE				= 2,
		START_RESUME				= 3,
		STOP_PAUSE					= 4,
		SET_FAST_MODE_PARAMS		= 5,
		CLEAR_HISTORY				= 6,
		GOTO_STANDARD_MODE_RESPONSE	= 65537,
		GOTO_FAST_MODE_RESPONSE		= 65538,
		START_RESUME_RESPONSE		= 65539,
		STOP_PAUSE_RESPONSE			= 65540,
		SET_FAST_MODE_PARAMS_RESPONSE = 65541,
		CLEAR_HISTORY_RESPONSE		= 65542,
		SIGNAL_UP_REPORT			= 65543,
		SIGNAL_DOWN_REPORT			= 65544,
		DF_REPORT					= 65545,
		SPECTRUM_OUTPUT				= 65546,
		ACTIVITY_OUTPUT				= 65547,
		FAST_MODE_STATUS			= 524289
	};


	// Message version data
	static const SVersionData VERSION_DATA[];

	// Message structures
#ifndef CSMSPLUS_SRV_BUILT
	static const unsigned int MAX_BANDS = 25;
	static const unsigned int MAX_NUM_AZIM_ENTRIES = 10;
#else
	static const uint32_t MAX_BANDS = 25;
	static const uint32_t MAX_NUM_AZIM_ENTRIES = 10;
#endif
	struct SSetFastModeParamsV0 // SET_FAST_MODE_PARAMS V0
	{
		struct SFastModeParams
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long detectionThreshold;	// dB above noise floor
			unsigned long dwell;				// number of "looks" for each report (cannot be 0)
			unsigned long attackTime;			// up signal (ms)
			unsigned long decayTime;			// down signal (ms)
			unsigned long upCount;				// up signal
			unsigned long downCount;			// down signal
			unsigned long combineAdjacentChans;	// discrete channels if 0
			unsigned long spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			unsigned long dfOnlyOnFirst;		// DF on every sweep if 0
			unsigned long rxBw;					// the receiver bandwidth
#else
			uint32_t detectionThreshold;	// dB above noise floor
			uint32_t dwell;				// number of "looks" for each report (cannot be 0)
			uint32_t attackTime;			// up signal (ms)
			uint32_t decayTime;			// down signal (ms)
			uint32_t upCount;				// up signal
			uint32_t downCount;			// down signal
			uint32_t combineAdjacentChans;	// discrete channels if 0
			uint32_t spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			uint32_t dfOnlyOnFirst;		// DF on every sweep if 0
			uint32_t rxBw;					// the receiver bandwidth
#endif
		};
		struct SScanList
		{
#ifndef CSMSPLUS_SRV_BUILT
			struct SBandList
			{
				unsigned long	startFreq;			// band start frequency in Hz
				unsigned long	stopFreq;			// band stop frequency in Hz
				unsigned long	chanBw;				// channel bandwidth in Hz
				bool			include;			// exclude if FALSE
			};
	
			unsigned long	numBands;			// number of frequency bands
			SBandList		bandList[MAX_BANDS];
#else
			struct SBandList
			{
				uint32_t	startFreq;			// band start frequency in Hz
				uint32_t	stopFreq;			// band stop frequency in Hz
				uint32_t	chanBw;				// channel bandwidth in Hz
				bool			include;			// exclude if FALSE
			};
	
			uint32_t	numBands;			// number of frequency bands
			SBandList		bandList[MAX_BANDS];
#endif
		}; 
		SFastModeParams	param;
		SScanList		scanList;
	};

	struct SSetFastModeParamsV2; // Forward declaration

	struct SSetFastModeParamsV1 // SET_FAST_MODE_PARAMS V1 (variable length)
	{
		operator SSetFastModeParamsV0(void) const // Convert to V0
		{
			SSetFastModeParamsV0 cmd;
			cmd.param.detectionThreshold = param.detectionThreshold;
			cmd.param.dwell = param.dwell;
			cmd.param.attackTime = param.attackTime;
			cmd.param.decayTime = param.decayTime;
			cmd.param.upCount = param.upCount;
			cmd.param.downCount = param.downCount;
			cmd.param.combineAdjacentChans = param.combineAdjacentChans;
			cmd.param.spectrumOutput = param.spectrumOutput;
			cmd.param.dfOnlyOnFirst = param.dfOnlyOnFirst;
			cmd.param.rxBw = param.rxBw;
			cmd.scanList.numBands = (scanList.numBands <= MAX_BANDS ? scanList.numBands : MAX_BANDS);

			for(size_t band = 0; band < cmd.scanList.numBands; ++band)
			{
				cmd.scanList.bandList[band].startFreq = scanList.bandList[band].startFreq;
				cmd.scanList.bandList[band].stopFreq = scanList.bandList[band].stopFreq;
				cmd.scanList.bandList[band].chanBw = scanList.bandList[band].chanBw;
				cmd.scanList.bandList[band].include = scanList.bandList[band].include;
			}

			return cmd;
		}

		SSetFastModeParamsV1& operator =(SSetFastModeParamsV0 v0) // Convert from V0
		{
			param.detectionThreshold = v0.param.detectionThreshold;
			param.dwell = v0.param.dwell;
			param.attackTime = v0.param.attackTime;
			param.decayTime = v0.param.decayTime;
			param.upCount = v0.param.upCount;
			param.downCount = v0.param.downCount;
			param.combineAdjacentChans = v0.param.combineAdjacentChans;
			param.spectrumOutput = v0.param.spectrumOutput;
			param.dfOnlyOnFirst = v0.param.dfOnlyOnFirst;
			param.rxBw = v0.param.rxBw;
			scanList.numBands = (v0.scanList.numBands <= MAX_BANDS ? v0.scanList.numBands : 0);

			for(size_t i = 0; i < scanList.numBands; ++i)
			{
				scanList.bandList[i].startFreq = v0.scanList.bandList[i].startFreq;
				scanList.bandList[i].stopFreq = v0.scanList.bandList[i].stopFreq;
				scanList.bandList[i].chanBw = v0.scanList.bandList[i].chanBw;
				scanList.bandList[i].include = v0.scanList.bandList[i].include;
				scanList.bandList[i].horizPol = false;
			}

			return *this;
		}

		SSetFastModeParamsV1& operator =(const SSetFastModeParamsV2& v2); // Convert from V2

		struct SFastModeParams
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long detectionThreshold;	// dB above noise floor
			unsigned long dwell;				// number of "looks" for each report (cannot be 0)
			unsigned long attackTime;			// up signal (ms)
			unsigned long decayTime;			// down signal (ms)
			unsigned long upCount;				// up signal
			unsigned long downCount;			// down signal
			unsigned long combineAdjacentChans;	// discrete channels if 0
			unsigned long spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			unsigned long dfOnlyOnFirst;		// DF on every sweep if 0
			unsigned long rxBw;					// the receiver bandwidth
#else
			uint32_t detectionThreshold;	// dB above noise floor
			uint32_t dwell;				// number of "looks" for each report (cannot be 0)
			uint32_t attackTime;			// up signal (ms)
			uint32_t decayTime;			// down signal (ms)
			uint32_t upCount;				// up signal
			uint32_t downCount;			// down signal
			uint32_t combineAdjacentChans;	// discrete channels if 0
			uint32_t spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			uint32_t dfOnlyOnFirst;		// DF on every sweep if 0
			uint32_t rxBw;					// the receiver bandwidth
#endif
		};
		struct SScanList
		{
#ifndef CSMSPLUS_SRV_BUILT
			struct SBandList
			{
				unsigned long	startFreq;	// band start frequency in Hz
				unsigned long	stopFreq;	// band stop frequency in Hz
				unsigned long	chanBw;		// channel bandwidth in Hz
				bool			include;	// exclude if FALSE
				bool			horizPol;	// use horizontally polarized antenna if available
			};
	
			unsigned long	numBands;		// number of frequency bands
			SBandList		bandList[1];	// variable length (numBands)
#else
			struct SBandList
			{
				uint32_t	startFreq;	// band start frequency in Hz
				uint32_t	stopFreq;	// band stop frequency in Hz
				uint32_t	chanBw;		// channel bandwidth in Hz
				bool			include;	// exclude if FALSE
				bool			horizPol;	// use horizontally polarized antenna if available
			};
	
			uint32_t	numBands;		// number of frequency bands
			SBandList		bandList[1];	// variable length (numBands)
#endif
		};
		SFastModeParams	param;
		SScanList		scanList;
	};

	struct SSetFastModeParamsV3; // Forward declaration

	struct SSetFastModeParamsV2 // SET_FAST_MODE_PARAMS V2 (variable length)
	{
		SSetFastModeParamsV2& operator =(const SSetFastModeParamsV1& v1) // Convert from V1
		{
			param.detectionThreshold = v1.param.detectionThreshold;
			param.dwell = v1.param.dwell;
			param.attackTime = v1.param.attackTime;
			param.decayTime = v1.param.decayTime;
			param.upCount = v1.param.upCount;
			param.downCount = v1.param.downCount;
			param.combineAdjacentChans = v1.param.combineAdjacentChans;
			param.spectrumOutput = v1.param.spectrumOutput;
			param.numDfs = v1.param.dfOnlyOnFirst;
			param.rxBw = v1.param.rxBw;
			memmove(scanList.bandList, v1.scanList.bandList, v1.scanList.numBands * sizeof(scanList.bandList[0]));
			scanList.numBands = v1.scanList.numBands;
			param.numAzimEntries = 0;
			param.loRange = 0;
			param.hiRange = 0xffffffff;
			param.loBandwidth = 0;
			param.hiBandwidth = 0xffffffff;
			param.loSigStrength = 0;
			param.hiSigStrength = 255;
			param.loDfConf = 0;

			return *this;
		}

		inline SSetFastModeParamsV2& operator =(const SSetFastModeParamsV3& v3); // Convert from V3

		struct SFastModeParams
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	detectionThreshold;	// dB above noise floor
			unsigned long	dwell;				// number of "looks" for each report (cannot be 0)
			unsigned long	attackTime;			// up signal (ms)
			unsigned long	decayTime;			// down signal (ms)
			unsigned long	upCount;			// up signal
			unsigned long	downCount;			// down signal
			unsigned long	combineAdjacentChans;	// discrete channels if 0
			unsigned long	spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			unsigned long	numDfs;				// DF on every sweep if 0, otherwise do this number of DFs on each new signal
			unsigned long	rxBw;				// the receiver bandwidth
			unsigned char	numAzimEntries;
			unsigned short	startAzim[MAX_NUM_AZIM_ENTRIES];	// deg (0 to 359)
			unsigned short	stopAzim[MAX_NUM_AZIM_ENTRIES];		// deg (0 to 359)
			unsigned long	loRange;		// km
			unsigned long	hiRange;		// km
			unsigned long	loBandwidth;	// Hz
			unsigned long	hiBandwidth;	// Hz
			unsigned char	loSigStrength;	// dBm + 200
			unsigned char	hiSigStrength;	// dBm + 200
			unsigned char	loDfConf;		// % (0 to 100)
#else
			uint32_t	detectionThreshold;	// dB above noise floor
			uint32_t	dwell;				// number of "looks" for each report (cannot be 0)
			uint32_t	attackTime;			// up signal (ms)
			uint32_t	decayTime;			// down signal (ms)
			uint32_t	upCount;			// up signal
			uint32_t	downCount;			// down signal
			uint32_t	combineAdjacentChans;	// discrete channels if 0
			uint32_t	spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			uint32_t	numDfs;				// DF on every sweep if 0, otherwise do this number of DFs on each new signal
			uint32_t	rxBw;				// the receiver bandwidth
			uint8_t	numAzimEntries;
			uint16_t	startAzim[MAX_NUM_AZIM_ENTRIES];	// deg (0 to 359)
			uint16_t	stopAzim[MAX_NUM_AZIM_ENTRIES];		// deg (0 to 359)
			uint32_t	loRange;		// km
			uint32_t	hiRange;		// km
			uint32_t	loBandwidth;	// Hz
			uint32_t	hiBandwidth;	// Hz
			uint8_t	loSigStrength;	// dBm + 200
			uint8_t	hiSigStrength;	// dBm + 200
			uint8_t	loDfConf;		// % (0 to 100)
#endif
		};
		struct SScanList
		{
#ifndef CSMSPLUS_SRV_BUILT
			struct SBandList
			{
				unsigned long	startFreq;	// band start frequency in Hz
				unsigned long	stopFreq;	// band stop frequency in Hz
				unsigned long	chanBw;		// channel bandwidth in Hz
				bool			include;	// exclude if FALSE
				bool			horizPol;	// use horizontally polarized antenna if available
			};
	
			unsigned long	numBands;		// number of frequency bands
			SBandList		bandList[1];	// variable length (numBands)
#else
			struct SBandList
			{
				uint32_t	startFreq;	// band start frequency in Hz
				uint32_t	stopFreq;	// band stop frequency in Hz
				uint32_t	chanBw;		// channel bandwidth in Hz
				bool			include;	// exclude if FALSE
				bool			horizPol;	// use horizontally polarized antenna if available
			};
	
			uint32_t	numBands;		// number of frequency bands
			SBandList		bandList[1];	// variable length (numBands)
#endif
		}; 
		SFastModeParams	param;
		SScanList		scanList;
	};

	struct SSetFastModeParamsV4; // Forward declaration

	struct SSetFastModeParamsV3 // SET_FAST_MODE_PARAMS V3 (variable length)
	{
#ifdef CSMSPLUS_SRV_BUILT
		inline SSetFastModeParamsV3& operator =(const SSetFastModeParamsV4& v4); // Convert from V4
#endif
		SSetFastModeParamsV3& operator =(const SSetFastModeParamsV2& v2) // Convert from V2
		{
			const SSetFastModeParamsV2* src = &v2;
			size_t sizeofV2 = offsetof(SFastModeMsg, body.setFastModeParamsV2.scanList.bandList[v2.scanList.numBands]);

			if(static_cast<void*>(this) == static_cast<const void*>(&v2))
			{
				// Make a copy
				if((src = static_cast<const SSetFastModeParamsV2*>(malloc(sizeofV2))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SSetFastModeParamsV2*>(src), &v2, sizeofV2);
			}

#ifndef CSMSPLUS_SRV_BUILT
			params.rxBw = Units::Frequency(src->param.rxBw).GetRaw();
			params.loBandwidth = Units::Frequency(src->param.loBandwidth).GetRaw();
			params.hiBandwidth = Units::Frequency(src->param.hiBandwidth).GetRaw();
			params.loRange = static_cast<unsigned short>(src->param.loRange);
			params.hiRange = static_cast<unsigned short>(src->param.hiRange);
			params.attackTime = static_cast<unsigned short>(src->param.attackTime);
			params.decayTime = static_cast<unsigned short>(src->param.decayTime);
			params.upCount = static_cast<unsigned short>(src->param.upCount);
			params.downCount = static_cast<unsigned short>(src->param.downCount);
			params.detectionThreshold = static_cast<unsigned char>(src->param.detectionThreshold);
			params.dwell = static_cast<unsigned char>(src->param.dwell);
			params.combineAdjacentChans = (src->param.combineAdjacentChans != 0);
			params.spectrumOutput = static_cast<unsigned char>(src->param.spectrumOutput);
			params.numDfs = static_cast<unsigned char>(src->param.numDfs);
			params.loSigStrength = src->param.loSigStrength;
			params.hiSigStrength = src->param.hiSigStrength;
			params.loDfConf = src->param.loDfConf;
			params.numAzimEntries = src->param.numAzimEntries;

			for(unsigned char entry = 0; entry < params.numAzimEntries; ++entry)
			{
				params.startAzim[entry] = src->param.startAzim[entry];
				params.stopAzim[entry] = src->param.stopAzim[entry];
			}

			scanList.numBands = src->scanList.numBands;

			for(unsigned int band = 0; band < scanList.numBands; ++band)
			{
				scanList.bandList[band].startFreq = Units::Frequency(src->scanList.bandList[band].startFreq).GetRaw();
				scanList.bandList[band].stopFreq = Units::Frequency(src->scanList.bandList[band].stopFreq).GetRaw();
				scanList.bandList[band].chanBw = Units::Frequency(src->scanList.bandList[band].chanBw).GetRaw();
				scanList.bandList[band].include = src->scanList.bandList[band].include;
				scanList.bandList[band].horizPol = src->scanList.bandList[band].horizPol;
			}
#else
			params.rxBw = Units::Frequency(src->param.rxBw).GetRaw();
			params.loBandwidth = Units::Frequency(src->param.loBandwidth).GetRaw();
			params.hiBandwidth = Units::Frequency(src->param.hiBandwidth).GetRaw();
			params.loRange = uint16_t(src->param.loRange);
			params.hiRange = uint16_t(src->param.hiRange);
			params.attackTime = uint16_t(src->param.attackTime);
			params.decayTime = uint16_t(src->param.decayTime);
			params.upCount = uint16_t(src->param.upCount);
			params.downCount = uint16_t(src->param.downCount);
			params.detectionThreshold = uint8_t(src->param.detectionThreshold);
			params.dwell = uint8_t(src->param.dwell);
			params.combineAdjacentChans = (src->param.combineAdjacentChans != 0);
			params.spectrumOutput = uint8_t(src->param.spectrumOutput);
			params.numDfs = uint8_t(src->param.numDfs);
			params.loSigStrength = src->param.loSigStrength;
			params.hiSigStrength = src->param.hiSigStrength;
			params.loDfConf = src->param.loDfConf;
			params.numAzimEntries = src->param.numAzimEntries;

			for(uint8_t entry = 0; entry < params.numAzimEntries; ++entry)
			{
				params.startAzim[entry] = src->param.startAzim[entry];
				params.stopAzim[entry] = src->param.stopAzim[entry];
			}

			scanList.numBands = src->scanList.numBands;

			for(uint32_t band = 0; band < scanList.numBands; ++band)
			{
				scanList.bandList[band].startFreq = Units::Frequency(src->scanList.bandList[band].startFreq).GetRaw();
				scanList.bandList[band].stopFreq = Units::Frequency(src->scanList.bandList[band].stopFreq).GetRaw();
				scanList.bandList[band].chanBw = Units::Frequency(src->scanList.bandList[band].chanBw).GetRaw();
				scanList.bandList[band].include = src->scanList.bandList[band].include;
				scanList.bandList[band].horizPol = src->scanList.bandList[band].horizPol;
			}
#endif
			if(src != &v2)
			{
				free(const_cast<SSetFastModeParamsV2*>(src));
			}

			return *this;
		}
		struct SFastModeParams
		{
#ifndef CSMSPLUS_SRV_BUILT
			Units::Frequency::Raw	rxBw;				// The receiver bandwidth
			Units::Frequency::Raw	loBandwidth;
			Units::Frequency::Raw	hiBandwidth;
			unsigned short			loRange;			// km
			unsigned short			hiRange;			// km
			unsigned short			attackTime;			// Up signal (ms)
			unsigned short			decayTime;			// Down signal (ms)
			unsigned short			upCount;			// Up signal
			unsigned short			downCount;			// Down signal
			unsigned char			detectionThreshold;	// dB above noise floor
			unsigned char			dwell;				// Number of "looks" for each report (cannot be 0)
			bool					combineAdjacentChans;	// Discrete channels if false
			unsigned char			spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			unsigned char			numDfs;				// DF on every sweep if 0, otherwise do this number of DFs on each new signal
			unsigned char			loSigStrength;		// dBm + 200
			unsigned char			hiSigStrength;		// dBm + 200
			unsigned char			loDfConf;			// % (0 to 100)
			unsigned char			numAzimEntries;
			unsigned short			startAzim[MAX_NUM_AZIM_ENTRIES];	// deg (0 to 359)
			unsigned short			stopAzim[MAX_NUM_AZIM_ENTRIES];		// deg (0 to 359)
#else
			Units::Frequency::Raw	rxBw;				// The receiver bandwidth
			Units::Frequency::Raw	loBandwidth;
			Units::Frequency::Raw	hiBandwidth;
			uint16_t			loRange;			// km
			uint16_t			hiRange;			// km
			uint16_t			attackTime;			// Up signal (ms)
			uint16_t			decayTime;			// Down signal (ms)
			uint16_t			upCount;			// Up signal
			uint16_t			downCount;			// Down signal
			uint8_t			detectionThreshold;	// dB above noise floor
			uint8_t			dwell;				// Number of "looks" for each report (cannot be 0)
			bool					combineAdjacentChans;	// Discrete channels if false
			uint8_t			spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			uint8_t			numDfs;				// DF on every sweep if 0, otherwise do this number of DFs on each new signal
			uint8_t			loSigStrength;		// dBm + 200
			uint8_t			hiSigStrength;		// dBm + 200
			uint8_t			loDfConf;			// % (0 to 100)
			uint8_t			numAzimEntries;
			uint16_t			startAzim[MAX_NUM_AZIM_ENTRIES];	// deg (0 to 359)
			uint16_t			stopAzim[MAX_NUM_AZIM_ENTRIES];		// deg (0 to 359)
#endif
		};
		struct SScanList
		{
			struct SBandList
			{
				Units::Frequency::Raw	startFreq;	// Band start frequency
				Units::Frequency::Raw	stopFreq;	// Band stop frequency
				Units::Frequency::Raw	chanBw;		// Channel bandwidth
				bool					include;	// Exclude if false
				bool					horizPol;	// Use horizontally polarized antenna if available
			};
	
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long	numBands;		// number of frequency bands
#else
			uint32_t	numBands;		// number of frequency bands
#endif
			SBandList	bandList[1];	// variable length (numBands)
		}; 
	
		SFastModeParams	params;
		SScanList		scanList;
	};

#ifdef CSMSPLUS_SRV_BUILT
	struct SSetFastModeParamsV4 // SET_FAST_MODE_PARAMS V4 (variable length)
	{
		SSetFastModeParamsV4& operator =(const SSetFastModeParamsV3& v3) // Convert from V3
		{
			const SSetFastModeParamsV3* src = &v3;
			size_t sizeofV3 = offsetof(SFastModeMsg, body.setFastModeParamsV3.scanList.bandList[v3.scanList.numBands]);

			if(static_cast<void*>(this) == static_cast<const void*>(&v3))
			{
				// Make a copy
				if((src = static_cast<const SSetFastModeParamsV3*>(malloc(sizeofV3))) == nullptr)
				{
					AfxThrowMemoryException();
				}

				memcpy(const_cast<SSetFastModeParamsV3*>(src), &v3, sizeofV3);
			}

			params.loBandwidth = Units::Frequency(src->params.loBandwidth).GetRaw();
			params.hiBandwidth = Units::Frequency(src->params.hiBandwidth).GetRaw();
			params.loRange = uint16_t(src->params.loRange);
			params.hiRange = uint16_t(src->params.hiRange);
			params.combineAdjacentChans = (src->params.combineAdjacentChans != 0);
			params.spectrumOutput = uint8_t(src->params.spectrumOutput);
			params.numDfs = uint8_t(src->params.numDfs);
			params.loSigStrength = src->params.loSigStrength;
			params.hiSigStrength = src->params.hiSigStrength;
			params.loDfConf = src->params.loDfConf;
			params.numAzimEntries = src->params.numAzimEntries;

			for(uint8_t entry = 0; entry < params.numAzimEntries; ++entry)
			{
				params.startAzim[entry] = src->params.startAzim[entry];
				params.stopAzim[entry] = src->params.stopAzim[entry];
			}

			scanList.numBands = src->scanList.numBands;

			for(uint32_t band = 0; band < scanList.numBands; ++band)
			{
				scanList.bandList[band].startFreq = Units::Frequency(src->scanList.bandList[band].startFreq).GetRaw();
				scanList.bandList[band].stopFreq = Units::Frequency(src->scanList.bandList[band].stopFreq).GetRaw();
				scanList.bandList[band].rxBw = Units::Frequency(src->params.rxBw).GetRaw();
				scanList.bandList[band].chanBw = Units::Frequency(src->scanList.bandList[band].chanBw).GetRaw();
				scanList.bandList[band].attackTime = uint16_t(src->params.attackTime);
				scanList.bandList[band].decayTime = uint16_t(src->params.decayTime);
				scanList.bandList[band].upCount = uint16_t(src->params.upCount);
				scanList.bandList[band].downCount = uint16_t(src->params.downCount);
				scanList.bandList[band].detectionThreshold = uint8_t(src->params.detectionThreshold);
				scanList.bandList[band].dwell = uint8_t(src->params.dwell);
				scanList.bandList[band].include = src->scanList.bandList[band].include;
				scanList.bandList[band].horizPol = src->scanList.bandList[band].horizPol;
			}
			if(src != &v3)
			{
				free(const_cast<SSetFastModeParamsV3*>(src));
			}

			return *this;
		}
		struct SFastModeParams
		{
			Units::Frequency::Raw	loBandwidth;
			Units::Frequency::Raw	hiBandwidth;
			uint16_t		loRange;			// km
			uint16_t		hiRange;			// km
			bool			combineAdjacentChans;	// Discrete channels if false
			uint8_t			spectrumOutput;		// 0 = no output; 1 = spectrum; 2 = activity; 3 = both
			uint8_t			numDfs;				// DF on every sweep if 0, otherwise do this number of DFs on each new signal
			uint8_t			loSigStrength;		// dBm + 200
			uint8_t			hiSigStrength;		// dBm + 200
			uint8_t			loDfConf;			// % (0 to 100)
			uint8_t			numAzimEntries;
			uint16_t		startAzim[MAX_NUM_AZIM_ENTRIES];	// deg (0 to 359)
			uint16_t		stopAzim[MAX_NUM_AZIM_ENTRIES];		// deg (0 to 359)
		};
		struct SScanList
		{
			struct SBandList
			{
				Units::Frequency::Raw	startFreq;	// Band start frequency
				Units::Frequency::Raw	stopFreq;	// Band stop frequency
				Units::Frequency::Raw	rxBw;		// Receiver bandwidth
				Units::Frequency::Raw	chanBw;		// Channel bandwidth
				uint16_t				attackTime;			// Up signal (ms)
				uint16_t				decayTime;			// Down signal (ms)
				uint16_t				upCount;			// Up signal
				uint16_t				downCount;			// Down signal
				uint8_t					detectionThreshold;	// dB above noise floor
				uint8_t					dwell;				// Number of "looks" for each report (cannot be 0)
				bool					include;	// Exclude if false
				bool					horizPol;	// Use horizontally polarized antenna if available
			};

			uint32_t	numBands;		// number of frequency bands
			SBandList	bandList[1];	// variable length (numBands)
		};

		SFastModeParams	params;
		SScanList		scanList;
	};
#endif

#ifndef CSMSPLUS_SRV_BUILT
	typedef SSetFastModeParamsV3 SSetFastModeParams;
#else
	typedef SSetFastModeParamsV4 SSetFastModeParams;
#endif
	
	struct SSetFastModeParamsRespV0 // SET_FAST_MODE_PARAMS_RESPONSE V0
	{
#ifndef CSMSPLUS_SRV_BUILT
#ifdef _MSC_VER
#if _MSC_VER >= 1400
		static const unsigned int MAX_TUNE = 250;
#else
		enum { MAX_TUNE = 250 };
#endif
#else
		static const unsigned int MAX_TUNE = 250;
#endif
		struct SBandLinks
		{
			unsigned long	numRcvrBlocks;		// number of receiver blocks in client band
			unsigned long	rcvrBlockNum[MAX_TUNE];	// receiver block number
		};
		
		struct SFreqLinks
		{
			unsigned long	numRcvrBlocks;				// number of receiver blocks in all client bands
			unsigned long	startFrequency[MAX_TUNE];	// block start freq in Hertz 
		};
	
		ErrorCodes::EErrorCode	status;					// status
		unsigned long			missionId;				// mission ID
		SSetFastModeParamsV0	clientParams;			// original client tasking parameters
		SBandLinks				bandLinks[MAX_BANDS];	// links between client bands & receiver blocks
		SFreqLinks				freqLinks;				// links between rx blocks & rx tune freqs
#else
		static const uint32_t MAX_TUNE = 250;

		struct SBandLinks
		{
			uint32_t	numRcvrBlocks;		// number of receiver blocks in client band
			uint32_t	rcvrBlockNum[MAX_TUNE];	// receiver block number
		};
		
		struct SFreqLinks
		{
			uint32_t	numRcvrBlocks;				// number of receiver blocks in all client bands
			uint32_t	startFrequency[MAX_TUNE];	// block start freq in Hertz 
		};
	
		ErrorCodes::EErrorCode	status;					// status
		uint32_t			missionId;				// mission ID
		SSetFastModeParamsV0	clientParams;			// original client tasking parameters
		SBandLinks				bandLinks[MAX_BANDS];	// links between client bands & receiver blocks
		SFreqLinks				freqLinks;				// links between rx blocks & rx tune freqs
#endif
	};

	struct SSetFastModeParamsRespV2; // Forward declaration

	struct SSetFastModeParamsRespV1 // SET_FAST_MODE_PARAMS_RESPONSE V1 (variable length)
	{
		operator SSetFastModeParamsRespV0(void) const // Convert to V0
		{
			SSetFastModeParamsRespV0 resp;
			resp.status = status;
			resp.missionId = missionId;
			resp.clientParams = clientParams;
			resp.freqLinks.numRcvrBlocks = (numRcvrBlocks <= SSetFastModeParamsRespV0::MAX_TUNE ? numRcvrBlocks : SSetFastModeParamsRespV0::MAX_TUNE);
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band = 0;
			unsigned long bandStart = 0;
			unsigned long block;
#else
			uint32_t band = 0;
			uint32_t bandStart = 0;
			uint32_t block;
#endif
			for(block = 0; block < resp.freqLinks.numRcvrBlocks; ++block)
			{
				if(links[block].band != band)
				{
					if(links[block].band >= MAX_BANDS)
					{
						break;
					}

					resp.bandLinks[band].numRcvrBlocks = block - bandStart;
					band = links[block].band;
					bandStart = block;
				}

				resp.bandLinks[band].rcvrBlockNum[block - bandStart] = block;
				resp.freqLinks.startFrequency[block] = links[block].startFrequency;
			}

			resp.bandLinks[band].numRcvrBlocks = block - bandStart;

			return resp;
		}

		SSetFastModeParamsRespV1& operator =(SSetFastModeParamsRespV0 v0) // Convert from V0
		{
			status = v0.status;
			missionId = v0.missionId;
			clientParams.param.detectionThreshold = v0.clientParams.param.detectionThreshold;
			clientParams.param.dwell = v0.clientParams.param.dwell;
			clientParams.param.attackTime = v0.clientParams.param.attackTime;
			clientParams.param.decayTime = v0.clientParams.param.decayTime;
			clientParams.param.upCount = v0.clientParams.param.upCount;
			clientParams.param.downCount = v0.clientParams.param.downCount;
			clientParams.param.combineAdjacentChans = v0.clientParams.param.combineAdjacentChans;
			clientParams.param.spectrumOutput = v0.clientParams.param.spectrumOutput;
			clientParams.param.dfOnlyOnFirst = v0.clientParams.param.dfOnlyOnFirst;
			clientParams.param.rxBw = v0.clientParams.param.rxBw;
			clientParams.scanList.numBands = 1; // Only copy first band
			clientParams.scanList.bandList[0].startFreq = v0.clientParams.scanList.bandList[0].startFreq;
			clientParams.scanList.bandList[0].stopFreq = v0.clientParams.scanList.bandList[0].stopFreq;
			clientParams.scanList.bandList[0].chanBw = v0.clientParams.scanList.bandList[0].chanBw;
			clientParams.scanList.bandList[0].include = v0.clientParams.scanList.bandList[0].include;
			clientParams.scanList.bandList[0].horizPol = false;
			numRcvrBlocks = (v0.freqLinks.numRcvrBlocks <= SSetFastModeParamsRespV0::MAX_TUNE ? v0.freqLinks.numRcvrBlocks : 0);
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band = 0;
			unsigned long bandBlock = 0;
#else
			uint32_t band = 0;
			uint32_t bandBlock = 0;
#endif
			for(size_t block = 0; block < numRcvrBlocks; ++block)
			{
				if(bandBlock++ == v0.bandLinks[band].numRcvrBlocks)
				{
					++band;
					bandBlock = 0;
				}

				links[block].band = band;
				links[block].startFrequency = v0.freqLinks.startFrequency[block];
			}

			return *this;
		}

		SSetFastModeParamsRespV1& operator =(const SSetFastModeParamsRespV2& v2); // Convert from V2

		struct SLinks
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band;				// Band for block
			unsigned long startFrequency;	// Block start freq in Hertz
#else
			uint32_t band;				// Band for block
			uint32_t startFrequency;	// Block start freq in Hertz
#endif
		};

#ifndef CSMSPLUS_SRV_BUILT
		ErrorCodes::EErrorCode	status;			// Status
		unsigned long			missionId;		// Mission ID
		SSetFastModeParamsV1	clientParams;	// Original client tasking parameters (variable length, but here we only return the 
												//  first band so it's actually fixed length)
		unsigned long			numRcvrBlocks;	// Number of receiver blocks in all client bands
		SLinks					links[1];		// Variable length (numRcvrBlocks)
#else
		ErrorCodes::EErrorCode	status;			// Status
		uint32_t			missionId;		// Mission ID
		SSetFastModeParamsV1	clientParams;	// Original client tasking parameters (variable length, but here we only return the 
												//  first band so it's actually fixed length)
		uint32_t			numRcvrBlocks;	// Number of receiver blocks in all client bands
		SLinks					links[1];		// Variable length (numRcvrBlocks)
#endif
	};

	struct SSetFastModeParamsRespV3; // Forward declaration

	struct SSetFastModeParamsRespV2 // SET_FAST_MODE_PARAMS_RESPONSE V2 (variable length)
	{
		SSetFastModeParamsRespV2& operator =(const SSetFastModeParamsRespV1& v1) // Convert from V1
		{
			status = v1.status;
			missionId = v1.missionId;
			numRcvrBlocks = v1.numRcvrBlocks;
			memmove(links, v1.links, v1.numRcvrBlocks * sizeof(links[0]));

			return *this;
		}

		inline SSetFastModeParamsRespV2& operator =(const SSetFastModeParamsRespV3& v3); // Convert from V3

		struct SLinks
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band;				// Band for block
			unsigned long startFrequency;	// Block start freq in Hertz
#else
			uint32_t band;				// Band for block
			uint32_t startFrequency;	// Block start freq in Hertz
#endif
		};

#ifndef CSMSPLUS_SRV_BUILT
		ErrorCodes::EErrorCode	status;			// Status
		unsigned long			missionId;		// Mission ID
		unsigned long			numRcvrBlocks;	// Number of receiver blocks in all client bands
		SLinks					links[1];		// Variable length (numRcvrBlocks)
#else
		ErrorCodes::EErrorCode	status;			// Status
		uint32_t			missionId;		// Mission ID
		uint32_t			numRcvrBlocks;	// Number of receiver blocks in all client bands
		SLinks					links[1];		// Variable length (numRcvrBlocks)
#endif
	};

	struct SSetFastModeParamsRespV3 // SET_FAST_MODE_PARAMS_RESPONSE V3 (variable length)
	{
		SSetFastModeParamsRespV3& operator =(const SSetFastModeParamsRespV2& v2) // Convert from V2
		{
			// Copy backwards to allow in-place
#ifdef _MSC_VER
#pragma warning(suppress : 6293) // Uses overflow to terminate loop
#endif
#ifndef CSMSPLUS_SRV_BUILT
			for(unsigned long block = v2.numRcvrBlocks - 1; block < v2.numRcvrBlocks; --block)
#else
			for(uint32_t block = v2.numRcvrBlocks - 1; block < v2.numRcvrBlocks; --block)
#endif
			{
				links[block].startFrequency = Units::Frequency(v2.links[block].startFrequency).GetRaw();
				links[block].band = v2.links[block].band;
			}

			numRcvrBlocks = v2.numRcvrBlocks;
			missionId = v2.missionId;
			status = v2.status;

			return *this;
		}

		struct SLinks
		{
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long band;						// Band for block
#else
			uint32_t band;						// Band for block
#endif
			Units::Frequency::Raw startFrequency;	// Block start freq
		};

#ifndef CSMSPLUS_SRV_BUILT
		ErrorCodes::EErrorCode	status;			// Status
		unsigned long			missionId;		// Mission ID
		unsigned long			numRcvrBlocks;	// Number of receiver blocks in all client bands
		SLinks					links[1];		// Variable length (numRcvrBlocks)
#else
		ErrorCodes::EErrorCode	status;			// Status
		uint32_t			missionId;		// Mission ID
		uint32_t			numRcvrBlocks;	// Number of receiver blocks in all client bands
		SLinks					links[1];		// Variable length (numRcvrBlocks)
#endif
	};

	typedef SSetFastModeParamsRespV3 SSetFastModeParamsResp;

	struct SWbActivityOutput // ACTIVITY_OUTPUT
	{
#ifndef CSMSPLUS_SRV_BUILT
		double dateTime;			// using DATE format
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
		unsigned long numBins;		// number of bins in spectrum data
		unsigned char activityData[1]; // Variable length
#else
		double dateTime;			// using DATE format
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
		uint32_t numBins;		// number of bins in spectrum data
		uint8_t activityData[1]; // Variable length
#endif
	}; 

	struct SWbSignalDownReportV1; // Forward declaration

	struct SWbSignalDownReportV0 // SIGNAL_DOWN_REPORT V0
	{
		inline operator SWbSignalDownReportV1(void) const; // Convert to V1

#ifndef CSMSPLUS_SRV_BUILT
		double	dateTime;			// using COleTime format
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
		unsigned long frequency;	// signal center frequency in Hz
		unsigned long bandwidth;	// signal bandwidth in Hz
#else
		double	dateTime;			// using COleTime format
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
		uint32_t frequency;	// signal center frequency in Hz
		uint32_t bandwidth;	// signal bandwidth in Hz
#endif
	}; 

	struct SWbSignalDownReportV1 // SIGNAL_DOWN_REPORT V1
	{
		operator SWbSignalDownReportV0(void) const // Convert to V0
		{
			SWbSignalDownReportV0 v0;
			v0.dateTime = abs(dateTime);
			v0.missionId = missionId;
			v0.rcvrBlockNum = rcvrBlockNum;
#ifndef CSMSPLUS_SRV_BUILT
			v0.frequency = static_cast<unsigned long>(frequency + 0.5);
			v0.bandwidth = static_cast<unsigned long>(bandwidth + 0.5);
#else
			v0.frequency = uint32_t(frequency + 0.5);
			v0.bandwidth = uint32_t(bandwidth + 0.5);
#endif
			return v0;
		}

#ifndef CSMSPLUS_SRV_BUILT
		double	dateTime;			// using COleTime format
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
		double frequency;			// signal center frequency in Hz
		double bandwidth;			// signal bandwidth in Hz
#else
		double	dateTime;			// using COleTime format
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
		double frequency;			// signal center frequency in Hz
		double bandwidth;			// signal bandwidth in Hz
#endif
	};

	typedef SWbSignalDownReportV1 SWbSignalDownReport;

	struct SWbSignalUpDfReportV1; // Forward declaration

	struct SWbSignalUpDfReportV0 // SIGNAL_UP_REPORT / DF_REPORT V0
	{
		inline operator SWbSignalUpDfReportV1(void) const; // Convert to V1

#ifndef CSMSPLUS_SRV_BUILT
		double dateTime;			// using COleTime format
		double latitude;			// degrees (+ North, - South)
		double longitude;			// degrees (+ East, - West)
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
		unsigned long frequency;	// signal center frequency in Hz
		unsigned long bandwidth;	// signal bandwidth in Hz
		unsigned long azimuth;		// DF azimuth in 1/100s degrees (0 to 35999)
		unsigned long confidence;	// confidence (0 to 100)
		unsigned long heading;		// degrees * 100 (0 to 35999)
		long power;					// power in dBm * 10
#else
		double dateTime;			// using COleTime format
		double latitude;			// degrees (+ North, - South)
		double longitude;			// degrees (+ East, - West)
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
		uint32_t frequency;	// signal center frequency in Hz
		uint32_t bandwidth;	// signal bandwidth in Hz
		uint32_t azimuth;		// DF azimuth in 1/100s degrees (0 to 35999)
		uint32_t confidence;	// confidence (0 to 100)
		uint32_t heading;		// degrees * 100 (0 to 35999)
		int32_t power;					// power in dBm * 10
#endif
	};

	struct SWbSignalUpDfReportV2; // Forward declaration

	struct SWbSignalUpDfReportV1 // SIGNAL_UP_REPORT / DF_REPORT V1
	{
		inline operator SWbSignalUpDfReportV2(void) const; // Convert to V2

		operator SWbSignalUpDfReportV0(void) const // Convert to V0
		{
			SWbSignalUpDfReportV0 v0;
			v0.dateTime = abs(dateTime);
			v0.latitude = latitude;
			v0.longitude = longitude;
			v0.missionId = missionId;
			v0.rcvrBlockNum = rcvrBlockNum;
#ifndef CSMSPLUS_SRV_BUILT
			v0.frequency = static_cast<unsigned long>(frequency + 0.5);
			v0.bandwidth = static_cast<unsigned long>(bandwidth + 0.5);
			v0.azimuth = (100 * static_cast<unsigned long>(azimuth + 0.005f)) % 36000;
			v0.confidence = static_cast<unsigned long>(100 * confidence);
			v0.heading = (100 * static_cast<unsigned long>(heading + 0.005f)) % 36000;
			v0.power = long(10 * power);
#else
			v0.frequency = uint32_t(frequency + 0.5);
			v0.bandwidth = uint32_t(bandwidth + 0.5);
			v0.azimuth = (100 * uint32_t(azimuth + 0.005f)) % 36000;
			v0.confidence = uint32_t(100 * confidence);
			v0.heading = (100 * uint32_t(heading + 0.005f)) % 36000;
			v0.power = int32_t(10 * power);
#endif
			return v0;
		}

		double dateTime;			// using COleTime format
		double latitude;			// degrees (+ North, - South)
		double longitude;			// degrees (+ East, - West)
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
#else
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
#endif
		double frequency;			// signal center frequency in Hz
		double bandwidth;			// signal bandwidth in Hz
		float azimuth;				// DF azimuth in degrees
		float el;					// DF elevation in degrees
		float range;				// DF range in km
		float confidence;			// confidence (0 to 1.0)
		float heading;				// degrees
		float power;				// power in dBm
	};

	struct SWbSignalUpDfReportV3; // Forward declaration

	struct SWbSignalUpDfReportV2 // SIGNAL_UP_REPORT / DF_REPORT V2
	{
		inline operator SWbSignalUpDfReportV3(void) const; // Convert to V3

		operator SWbSignalUpDfReportV1(void) const // Convert to V1
		{
			SWbSignalUpDfReportV1 v1;
			v1.dateTime = abs(dateTime);
			v1.latitude = latitude;
			v1.longitude = longitude;
			v1.missionId = missionId;
			v1.rcvrBlockNum = rcvrBlockNum;
			v1.frequency = frequency;
			v1.bandwidth = bandwidth;
			v1.azimuth = azimuth;
			v1.el = el;
			v1.range = range;
			v1.confidence = (badHeading ? 0 : confidence);
			v1.heading = heading;
			v1.power = power;

			return v1;
		}

		double dateTime;			// using COleTime format
		double latitude;			// degrees (+ North, - South)
		double longitude;			// degrees (+ East, - West)
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
#else
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
#endif
		double frequency;			// signal center frequency in Hz
		float bandwidth;			// signal bandwidth in Hz
		float azimuth;				// DF azimuth in degrees (relative to heading)
		float el;					// DF elevation in degrees
		float range;				// DF range in km
		float confidence;			// confidence (0 to 1.0)
		float heading;				// degrees
		float power;				// power in dBm
		bool badHeading;			// heading is bad
	};

	struct SWbSignalUpDfReportV3 // SIGNAL_UP_REPORT / DF_REPORT V3
	{
		operator SWbSignalUpDfReportV2(void) const // Convert to V2
		{
			SWbSignalUpDfReportV2 v2;
			v2.dateTime = abs(dateTime);
			v2.latitude = latitude;
			v2.longitude = longitude;
			v2.missionId = missionId;
			v2.rcvrBlockNum = rcvrBlockNum;
			v2.frequency = frequency;
			v2.bandwidth = bandwidth;
			v2.azimuth = azimuth;
			v2.el = el;
			v2.range = range;
			v2.confidence = (badHeading ? 0 : confidence);
			v2.heading = heading;
			v2.power = power;
			v2.badHeading = badHeading;

			return v2;
		}

		double dateTime;			// using COleTime format
		double latitude;			// degrees (+ North, - South)
		double longitude;			// degrees (+ East, - West)
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
#else
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
#endif
		double frequency;			// signal center frequency in Hz
		float bandwidth;			// signal bandwidth in Hz
		float azimuth;				// DF azimuth in degrees (relative to heading)
		float el;					// DF elevation in degrees
		float range;				// DF range in km
		float confidence;			// confidence (0 to 1.0)
		float heading;				// degrees
		float power;				// power in dBm
		bool badHeading;			// heading is bad
		bool badSiteLocation;		// site location is bad
	};

	typedef SWbSignalUpDfReportV3 SWbSignalUpDfReport;

	struct SWbSpectrumOutput // SPECTRUM_OUTPUT
	{
#ifndef CSMSPLUS_SRV_BUILT
		double dateTime;			// using DATE format
		unsigned long missionId;	// mission ID
		unsigned long rcvrBlockNum;	// starts at 0
		unsigned long numBins;		// number of bins in spectrum data
		unsigned char spectrumData[1]; // Variable length
#else
		double dateTime;			// using DATE format
		uint32_t missionId;	// mission ID
		uint32_t rcvrBlockNum;	// starts at 0
		uint32_t numBins;		// number of bins in spectrum data
		uint8_t spectrumData[1]; // Variable length
#endif
	};

	// Message format
#pragma pack(push, 4)
	SHdr hdr;

	union UBody
	{
		SGenericResp genericResp;
		SGetBistResp getBistResp;
		SGetBistRespOld getBistRespOld;
		SGetGpsResp getGpsResp;
		SGetHeadingResp getHeadingResp;
		SGetMsgVersionsData getMsgVersionsData; // Variable length
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
		SSetFastModeParams setFastModeParams; // Variable length
		SSetFastModeParamsV0 setFastModeParamsV0;
		SSetFastModeParamsV1 setFastModeParamsV1; // Variable length
		SSetFastModeParamsV2 setFastModeParamsV2; // Variable length
		SSetFastModeParamsV3 setFastModeParamsV3; // Variable length
#ifdef CSMSPLUS_SRV_BUILT
		SSetFastModeParamsV4 setFastModeParamsV4; // Variable length
#endif
		SSetFastModeParamsResp setFastModeParamsResp; // Variable length
		SSetFastModeParamsRespV0 setFastModeParamsRespV0;
		SSetFastModeParamsRespV1 setFastModeParamsRespV1; // Variable length
		SSetFastModeParamsRespV2 setFastModeParamsRespV2; // Variable length
		SSetFastModeParamsRespV3 setFastModeParamsRespV3; // Variable length
		SWbActivityOutput wbActivityOutput; // Variable length
		SWbSignalDownReport wbSignalDownReport;
		SWbSignalDownReportV0 wbSignalDownReportV0;
		SWbSignalDownReportV1 wbSignalDownReportV1;
		SWbSignalUpDfReport wbSignalUpDfReport;
		SWbSignalUpDfReportV0 wbSignalUpDfReportV0;
		SWbSignalUpDfReportV1 wbSignalUpDfReportV1;
		SWbSignalUpDfReportV2 wbSignalUpDfReportV2;
		SWbSignalUpDfReportV3 wbSignalUpDfReportV3;
		SWbSpectrumOutput wbSpectrumOutput; // Variable length
	} body;
#pragma pack(pop)
};

// Conversion operators
inline SFastModeMsg::SSetFastModeParamsV1& SFastModeMsg::SSetFastModeParamsV1::operator =
	(const SFastModeMsg::SSetFastModeParamsV2& v2) // Convert from V2
{
	param.detectionThreshold = v2.param.detectionThreshold;
	param.dwell = v2.param.dwell;
	param.attackTime = v2.param.attackTime;
	param.decayTime = v2.param.decayTime;
	param.upCount = v2.param.upCount;
	param.downCount = v2.param.downCount;
	param.combineAdjacentChans = v2.param.combineAdjacentChans;
	param.spectrumOutput = v2.param.spectrumOutput;
	param.dfOnlyOnFirst = v2.param.numDfs;
	param.rxBw = v2.param.rxBw;
	scanList.numBands = v2.scanList.numBands;

	for(size_t i = 0; i < scanList.numBands; ++i)
	{
		scanList.bandList[i].startFreq = v2.scanList.bandList[i].startFreq;
		scanList.bandList[i].stopFreq = v2.scanList.bandList[i].stopFreq;
		scanList.bandList[i].chanBw = v2.scanList.bandList[i].chanBw;
		scanList.bandList[i].include = v2.scanList.bandList[i].include;
		scanList.bandList[i].horizPol = v2.scanList.bandList[i].horizPol;
	}

	return *this;
}

#ifdef CSMSPLUS_SRV_BUILT
inline SFastModeMsg::SSetFastModeParamsV3& SFastModeMsg::SSetFastModeParamsV3::operator =
	(const SFastModeMsg::SSetFastModeParamsV4& v4) // Convert from V4
{
	const SSetFastModeParamsV4* src = &v4;
	size_t sizeofV4 = offsetof(SFastModeMsg, body.setFastModeParamsV4.scanList.bandList[v4.scanList.numBands]);

	if(static_cast<void*>(this) == static_cast<const void*>(&v4))
	{
		// Make a copy
		if((src = static_cast<const SSetFastModeParamsV4*>(malloc(sizeofV4))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SSetFastModeParamsV4*>(src), &v4, sizeofV4);
	}

	if (src->scanList.numBands != 0)
	{
		params.rxBw = src->scanList.bandList[0].rxBw;
		params.detectionThreshold = src->scanList.bandList[0].detectionThreshold;
		params.dwell = src->scanList.bandList[0].dwell;
		params.attackTime = src->scanList.bandList[0].attackTime;
		params.decayTime = src->scanList.bandList[0].decayTime;
		params.upCount = src->scanList.bandList[0].upCount;
		params.downCount = src->scanList.bandList[0].downCount;
	}
	else
	{
		params.rxBw = Units::Frequency::Raw::ZERO;	// TODO: This will be a problem.
		params.detectionThreshold = 0;
		params.dwell = 0;
		params.attackTime = 0;
		params.decayTime = 0;
		params.upCount = 0;
		params.downCount = 0;
	}
	params.combineAdjacentChans = (src->params.combineAdjacentChans ? 1 : 0);
	params.spectrumOutput = src->params.spectrumOutput;
	params.numDfs = src->params.numDfs;
	params.numAzimEntries = src->params.numAzimEntries;

	for(uint8_t entry = 0; entry < params.numAzimEntries; ++entry)
	{
		params.startAzim[entry] = src->params.startAzim[entry];
		params.stopAzim[entry] = src->params.stopAzim[entry];
	}

	params.loRange = src->params.loRange;
	params.hiRange = src->params.hiRange;
	params.loBandwidth = src->params.loBandwidth;
	params.hiBandwidth = src->params.hiBandwidth;
	params.loSigStrength = src->params.loSigStrength;
	params.hiSigStrength = src->params.hiSigStrength;
	params.loDfConf = src->params.loDfConf;
	scanList.numBands = src->scanList.numBands;

	for(size_t i = 0; i < scanList.numBands; ++i)
	{
		scanList.bandList[i].startFreq = src->scanList.bandList[i].startFreq;
		scanList.bandList[i].stopFreq = src->scanList.bandList[i].stopFreq;
		scanList.bandList[i].chanBw = src->scanList.bandList[i].chanBw;
		scanList.bandList[i].include = src->scanList.bandList[i].include;
		scanList.bandList[i].horizPol = src->scanList.bandList[i].horizPol;
	}

	if(src != &v4)
	{
		free(const_cast<SSetFastModeParamsV4*>(src));
	}

	return *this;
}
#endif

inline SFastModeMsg::SSetFastModeParamsV2& SFastModeMsg::SSetFastModeParamsV2::operator =
	(const SFastModeMsg::SSetFastModeParamsV3& v3) // Convert from V3
{
	const SSetFastModeParamsV3* src = &v3;
	size_t sizeofV3 = offsetof(SFastModeMsg, body.setFastModeParamsV3.scanList.bandList[v3.scanList.numBands]);

	if(static_cast<void*>(this) == static_cast<const void*>(&v3))
	{
		// Make a copy
		if((src = static_cast<const SSetFastModeParamsV3*>(malloc(sizeofV3))) == nullptr)
		{
			AfxThrowMemoryException();
		}

		memcpy(const_cast<SSetFastModeParamsV3*>(src), &v3, sizeofV3);
	}

	param.detectionThreshold = src->params.detectionThreshold;
	param.dwell = src->params.dwell;
	param.attackTime = src->params.attackTime;
	param.decayTime = src->params.decayTime;
	param.upCount = src->params.upCount;
	param.downCount = src->params.downCount;
	param.combineAdjacentChans = (src->params.combineAdjacentChans ? 1 : 0);
	param.spectrumOutput = src->params.spectrumOutput;
	param.numDfs = src->params.numDfs;
#ifndef CSMSPLUS_SRV_BUILT
	param.rxBw = Units::Frequency(src->params.rxBw).Hz<unsigned long>();
#else
	param.rxBw = Units::Frequency(src->params.rxBw).Hz<uint32_t>();
#endif
	param.numAzimEntries = src->params.numAzimEntries;

#ifndef CSMSPLUS_SRV_BUILT
	for(unsigned char entry = 0; entry < param.numAzimEntries; ++entry)
#else
	for(uint8_t entry = 0; entry < param.numAzimEntries; ++entry)
#endif
	{
		param.startAzim[entry] = src->params.startAzim[entry];
		param.stopAzim[entry] = src->params.stopAzim[entry];
	}

	param.loRange = src->params.loRange;
	param.hiRange = src->params.hiRange;
#ifndef CSMSPLUS_SRV_BUILT
	param.loBandwidth = Units::Frequency(src->params.loBandwidth).Hz<unsigned long>();
	param.hiBandwidth = Units::Frequency(src->params.hiBandwidth).Hz<unsigned long>();
#else
	param.loBandwidth = Units::Frequency(src->params.loBandwidth).Hz<uint32_t>();
	param.hiBandwidth = Units::Frequency(src->params.hiBandwidth).Hz<uint32_t>();
#endif
	param.loSigStrength = src->params.loSigStrength;
	param.hiSigStrength = src->params.hiSigStrength;
	param.loDfConf = src->params.loDfConf;
	scanList.numBands = src->scanList.numBands;

	for(size_t i = 0; i < scanList.numBands; ++i)
	{
#ifndef CSMSPLUS_SRV_BUILT
		scanList.bandList[i].startFreq = Units::Frequency(src->scanList.bandList[i].startFreq).Hz<unsigned long>(true);
		scanList.bandList[i].stopFreq = Units::Frequency(src->scanList.bandList[i].stopFreq).Hz<unsigned long>(true);
		scanList.bandList[i].chanBw = Units::Frequency(src->scanList.bandList[i].chanBw).Hz<unsigned long>();
#else
		scanList.bandList[i].startFreq = Units::Frequency(src->scanList.bandList[i].startFreq).Hz<uint32_t>(true);
		scanList.bandList[i].stopFreq = Units::Frequency(src->scanList.bandList[i].stopFreq).Hz<uint32_t>(true);
		scanList.bandList[i].chanBw = Units::Frequency(src->scanList.bandList[i].chanBw).Hz<uint32_t>();
#endif
		scanList.bandList[i].include = src->scanList.bandList[i].include;
		scanList.bandList[i].horizPol = src->scanList.bandList[i].horizPol;
	}

	if(src != &v3)
	{
		free(const_cast<SSetFastModeParamsV3*>(src));
	}

	return *this;
}

inline SFastModeMsg::SSetFastModeParamsRespV1& SFastModeMsg::SSetFastModeParamsRespV1::operator =
	(const SFastModeMsg::SSetFastModeParamsRespV2& v2) // Convert from V2
{
	status = v2.status;
	missionId = v2.missionId;
	memmove(links, v2.links, v2.numRcvrBlocks * sizeof(links[0]));
	numRcvrBlocks = v2.numRcvrBlocks;
	clientParams.param.detectionThreshold = 12;
	clientParams.param.dwell = 1;
	clientParams.param.attackTime = 5;
	clientParams.param.decayTime = 5;
	clientParams.param.upCount = 1;
	clientParams.param.downCount = 1;
	clientParams.param.combineAdjacentChans = false;
	clientParams.param.spectrumOutput = false;
	clientParams.param.dfOnlyOnFirst = 0;
	clientParams.param.rxBw = 0;
	clientParams.scanList.numBands = 0;

	return *this;
}

inline SFastModeMsg::SSetFastModeParamsRespV2& SFastModeMsg::SSetFastModeParamsRespV2::operator =
	(const SFastModeMsg::SSetFastModeParamsRespV3& v3) // Convert from V3
{
	status = v3.status;
	missionId = v3.missionId;
	numRcvrBlocks = v3.numRcvrBlocks;

#ifndef CSMSPLUS_SRV_BUILT
	for(unsigned long block = 0; block < numRcvrBlocks; ++block)
	{
		links[block].band = v3.links[block].band;
		links[block].startFrequency = Units::Frequency(v3.links[block].startFrequency).Hz<unsigned long>(true);
	}
#else
	for(uint32_t block = 0; block < numRcvrBlocks; ++block)
	{
		links[block].band = v3.links[block].band;
		links[block].startFrequency = Units::Frequency(v3.links[block].startFrequency).Hz<uint32_t>(true);
	}
#endif
	return *this;
}

inline SFastModeMsg::SWbSignalDownReportV0::operator SFastModeMsg::SWbSignalDownReportV1(void) const // Convert to V1
{
	SWbSignalDownReportV1 v1;
	v1.dateTime = dateTime;
	v1.missionId = missionId;
	v1.rcvrBlockNum = rcvrBlockNum;
	v1.frequency = frequency;
	v1.bandwidth = bandwidth;

	return v1;
}

inline SFastModeMsg::SWbSignalUpDfReportV0::operator SFastModeMsg::SWbSignalUpDfReportV1(void) const // Convert to V1
{
	SWbSignalUpDfReportV1 v1;
	v1.dateTime = dateTime;
	v1.latitude = latitude;
	v1.longitude = longitude;
	v1.missionId = missionId;
	v1.rcvrBlockNum = rcvrBlockNum;
	v1.frequency = frequency;
	v1.bandwidth = bandwidth;
	v1.azimuth = azimuth / 100.0f;
	v1.el = 90;
	v1.range = 0;
	v1.confidence = confidence / 100.0f;
	v1.heading = heading / 100.0f;
	v1.power = power / 10.0f;

	return v1;
}

inline SFastModeMsg::SWbSignalUpDfReportV1::operator SFastModeMsg::SWbSignalUpDfReportV2(void) const // Convert to V2
{
	SWbSignalUpDfReportV2 v2;
	v2.dateTime = dateTime;
	v2.latitude = latitude;
	v2.longitude = longitude;
	v2.missionId = missionId;
	v2.rcvrBlockNum = rcvrBlockNum;
	v2.frequency = frequency;
	v2.bandwidth = float(bandwidth);
	v2.azimuth = azimuth;
	v2.el = el;
	v2.range = range;
	v2.confidence = confidence;
	v2.heading = heading;
	v2.power = power;
	v2.badHeading = false;

	return v2;
}

inline SFastModeMsg::SWbSignalUpDfReportV2::operator SFastModeMsg::SWbSignalUpDfReportV3(void) const // Convert to V3
{
	SWbSignalUpDfReportV3 v3;
	v3.dateTime = dateTime;
	v3.latitude = latitude;
	v3.longitude = longitude;
	v3.missionId = missionId;
	v3.rcvrBlockNum = rcvrBlockNum;
	v3.frequency = frequency;
	v3.bandwidth = float(bandwidth);
	v3.azimuth = azimuth;
	v3.el = el;
	v3.range = range;
	v3.confidence = confidence;
	v3.heading = heading;
	v3.power = power;
	v3.badHeading = badHeading;
	v3.badSiteLocation = false;

	return v3;
}

// Message versions (selectany lets this go in the .h file)
#ifdef _MSC_VER
__declspec(selectany)
#else
__attribute__((__weak__))
#endif
const SFastModeMsg::SVersionData SFastModeMsg::VERSION_DATA[] =
{
	{ { SFastModeMsg::DF_CTRL, SFastModeMsg::INITIALIZE_DF }, { 2, 4 } },
	{ { SFastModeMsg::DF_CTRL, SFastModeMsg::INITIALIZE_DF_RESPONSE }, { 2, 4 } },
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::DF_REPORT }, { 0, 3 } },
#ifndef CSMSPLUS_SRV_BUILT
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::SET_FAST_MODE_PARAMS }, { 3, 3 } },
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::SET_FAST_MODE_PARAMS_RESPONSE }, { 3, 3 } },
#else
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::SET_FAST_MODE_PARAMS }, { 4, 3 } },
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::SET_FAST_MODE_PARAMS_RESPONSE }, { 4, 3 } },
#endif
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::SIGNAL_DOWN_REPORT }, { 0, 1 } },
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::SIGNAL_UP_REPORT }, { 0, 3 } },
	{ { SFastModeMsg::FAST_MODE_CTRL, SFastModeMsg::START_RESUME }, { 0, 3 } }
};
