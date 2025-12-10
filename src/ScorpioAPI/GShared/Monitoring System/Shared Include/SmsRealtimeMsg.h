/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2006-2014 TCI International, Inc. All rights reserved         *
**************************************************************************/

#pragma once

#include "Units.h"

// Message definitions for messages from port 3307 (tci-equiprealtime)
//
// Send a header only to request a version other then 1. Version 0 turns off a specific message
struct SSmsRealtimeMsg
{
	// Realtime port msgType definitions
	enum EMsgType
	{
		RT_SPECTRUM_START	= 1,
		RT_SPECTRUM_STOP	= 2,
		RT_SPECTRUM			= 3,
		RT_DF_START			= 4,
		RT_DF_STOP			= 5,
		RT_DF_DATA			= 6,
		RT_IQ_DATA			= 7,
		// rds/modrec/sigrec all processed data instead iq that windows will need post processing
		RT_RDS_DATA			= 8,
		RT_MODREC_DATA		= 9,
		RT_SIGREC_DATA		= 10
	};

	// Message structure for RT_SPECTRUM_START / RT_DF_START msgType
	struct SStart  // msgTypeVersion = 1
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned int MAX_OCCBANDS = 100;

		unsigned long	taskId;							// Task ID (unique number, although can repeat -- not a good collection key)
		unsigned long	numBands;						// Number of tasked bands for this task ID
		unsigned long	firstChanFreq[MAX_OCCBANDS];	// Center RF frequency of the first (lowest) channel (Hz)
		unsigned long	chanSize[MAX_OCCBANDS];		// Channel spacing or bandwidth (Hz)
		unsigned long	numChan[MAX_OCCBANDS];		// Number of channels
#else
		static const uint32_t MAX_OCCBANDS = 100;

		uint32_t	taskId;							// Task ID (unique number, although can repeat -- not a good collection key)
		uint32_t	numBands;						// Number of tasked bands for this task ID
		uint32_t	firstChanFreq[MAX_OCCBANDS];	// Center RF frequency of the first (lowest) channel (Hz)
		uint32_t	chanSize[MAX_OCCBANDS];		// Channel spacing or bandwidth (Hz)
		uint32_t	numChan[MAX_OCCBANDS];		// Number of channels
#endif
	};

	//  this is a variable length message -- only numBands worth of data is sent
	struct SStartV2  // msgTypeVersion = 2
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	taskId;		// Task ID (unique number, although can repeat -- not a good collection key)
		unsigned long	numBands;	// Number of tasked bands for this task ID
#else
		uint32_t	taskId;		// Task ID (unique number, although can repeat -- not a good collection key)
		uint32_t	numBands;	// Number of tasked bands for this task ID
#endif
		struct SBand
		{
			Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel
			Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth
#ifndef CSMSPLUS_SRV_BUILT
			unsigned long			numChan;		// Number of channels
#else
			uint32_t			numChan;		// Number of channels
#endif
		} band[1];
	};

	// Message structure for RT_SPECTRUM_STOP / RT_DF_STOP msgType
	struct SStop  // msgTypeVersion = 1
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	taskId;					// taskID (same as RT_SPECTRUM_START / RT_DF_START msgType taskID)
#else
		uint32_t	taskId;					// taskID (same as RT_SPECTRUM_START / RT_DF_START msgType taskID)
#endif
	};

	// Message structure for RT_SPECTRUM_DATA msgType
	//  this is a variable length message -- only numChan worth of data is sent
	struct SSpectrum  // msgTypeVersion = 1
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	taskId;			// Task ID (same as RT_SPECTRUM_START msgType taskID)
		unsigned long	bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		unsigned long	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		unsigned long	chanSize;		// Channel spacing or bandwidth (Hz)
		unsigned long	numChan;		// Number of channels
		unsigned char	noiseFloor;		// Noise floor (dBm + 200)

		//Variable length array, the size is specifed by numChan above.
		unsigned char	chanData[1];	// Channel spectrum data (dBm + 200)
#else
		uint32_t	taskId;			// Task ID (same as RT_SPECTRUM_START msgType taskID)
		uint32_t	bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		uint32_t	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		uint32_t	chanSize;		// Channel spacing or bandwidth (Hz)
		uint32_t	numChan;		// Number of channels
		uint8_t	noiseFloor;		// Noise floor (dBm + 200)

		//Variable length array, the size is specifed by numChan above.
		uint8_t	chanData[1];	// Channel spectrum data (dBm + 200)
#endif
	};

	struct SSpectrumV2  // msgTypeVersion = 2
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			taskId;			// Task ID (same as RT_SPECTRUM_START msgType taskID)
		unsigned long			bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel
		Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth (Hz)
		unsigned long			numChan;		// Number of channels
		unsigned char			noiseFloor;		// Noise floor (dBm + 200)

		//Variable length array, the size is specifed by numChan above.
		unsigned char			chanData[1];	// Channel spectrum data (dBm + 200)
#else
		uint32_t			taskId;			// Task ID (same as RT_SPECTRUM_START msgType taskID)
		uint32_t			bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel
		Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth (Hz)
		uint32_t			numChan;		// Number of channels
		uint8_t			noiseFloor;		// Noise floor (dBm + 200)

		//Variable length array, the size is specifed by numChan above.
		uint8_t			chanData[1];	// Channel spectrum data (dBm + 200)
#endif
	};

	//Version 3 has extra field-strength value compared to version 2.
	struct SSpectrumV3  // msgTypeVersion = 3
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			taskId;			// Task ID (same as RT_SPECTRUM_START msgType taskID)
		unsigned long			bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel
		Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth (Hz)

		//Define union to optimize the clearing of unused field.
		union
		{
			unsigned long		zeroVal;
			unsigned char		noiseFloor;	// Noise floor (dBm + 200)
											//unsigned char unused[3]; Always 0.
		};

		unsigned long		numChan;		//Number of channels.

		//Both chanData & efield are variable size array, whose size are
		// determined by numChan value above.
		//@note: efield can only be accessed through pointer due to chanData
		//		 being variable size.
		unsigned char		chanData[1];  	// Channel spectrum data (dBm + 200)
		signed char         efield[1];      //Field strength dbuV/m.
#else
		uint32_t			taskId;			// Task ID (same as RT_SPECTRUM_START msgType taskID)
		uint32_t			bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel
		Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth (Hz)

		//Define union to optimize the clearing of unused field.
		union
		{
			uint32_t		zeroVal;
			uint8_t		noiseFloor;	// Noise floor (dBm + 200)
											//uint8_t unused[3]; Always 0.
		};

		uint32_t		numChan;		//Number of channels.

		//Both chanData & efield are variable size array, whose size are
		// determined by numChan value above.
		//@note: efield can only be accessed through pointer due to chanData
		//		 being variable size.
		uint8_t		chanData[1];  	// Channel spectrum data (dBm + 200)
		int8_t         efield[1];      //Field strength dbuV/m.
#endif
	};

	//This number must match with the latest SSpectrum version.
#ifndef CSMSPLUS_SRV_BUILT
	static const unsigned long C_LATEST_RT_SPECTRUM_VERSION = 3;
#else
	static const uint32_t C_LATEST_RT_SPECTRUM_VERSION = 3;
#endif

	// Message structure for RT_DF_DATA msgType
	//  this is a variable length message -- only numChan worth of data is sent
	struct SDfData  // msgTypeVersion = 1
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	taskId;			// Task ID (same as RT_DF_START msgType taskID)
		unsigned long	bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		unsigned long	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		unsigned long	chanSize;		// Channel spacing or bandwidth (Hz)
		unsigned long	numChan;		// Number of channels
		unsigned char	noiseFloor;		// Noise floor (dBm + 200)

		struct SChanData
		{
			unsigned short	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
										// 65535 = no DF result(signal not present, low confidence, etc)
			unsigned char	specData;	// Channel spectrum data (dBm + 200)
		} chanData[1];
#else
		uint32_t	taskId;			// Task ID (same as RT_DF_START msgType taskID)
		uint32_t	bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		uint32_t	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		uint32_t	chanSize;		// Channel spacing or bandwidth (Hz)
		uint32_t	numChan;		// Number of channels
		uint8_t	noiseFloor;		// Noise floor (dBm + 200)

		struct SChanData
		{
			uint16_t	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
										// 65535 = no DF result(signal not present, low confidence, etc)
			uint8_t	specData;	// Channel spectrum data (dBm + 200)
		} chanData[1];
#endif
	};

	//  this is a variable length message -- only numChan worth of data is sent
	struct SDfDataV2  // msgTypeVersion = 2
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long	taskId;			// Task ID (same as RT_DF_START msgType taskID)
		unsigned long	bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		unsigned long	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		unsigned long	chanSize;		// Channel spacing or bandwidth (Hz)
		unsigned long	numChan;		// Number of channels DF'd
		unsigned char	noiseFloor;		// Noise floor (dBm + 200)
		bool			horizPol;		// Horizontal polarization

		struct SChanData
		{
			unsigned short	chan;		// Channel number (zero-based)
			unsigned short	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
			unsigned short	conf;		// DF confidence (1/10th %, 0 - 1000)
			unsigned char	specData;	// Channel spectrum data (dBm + 200)
			unsigned char	dfSpecData;	// DF antenna spectrum data (dBm + 200)
#else
		uint32_t	taskId;			// Task ID (same as RT_DF_START msgType taskID)
		uint32_t	bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		uint32_t	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		uint32_t	chanSize;		// Channel spacing or bandwidth (Hz)
		uint32_t	numChan;		// Number of channels DF'd
		uint8_t	noiseFloor;		// Noise floor (dBm + 200)
		bool			horizPol;		// Horizontal polarization

		struct SChanData
		{
			uint16_t	chan;		// Channel number (zero-based)
			uint16_t	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
			uint16_t	conf;		// DF confidence (1/10th %, 0 - 1000)
			uint8_t	specData;	// Channel spectrum data (dBm + 200)
			uint8_t	dfSpecData;	// DF antenna spectrum data (dBm + 200)
#endif
		} chanData[1];
	};

	//  this is a variable length message -- only numChan worth of data is sent
	struct SDfDataV3  // msgTypeVersion = 3
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long			taskId;			// Task ID (same as RT_DF_START msgType taskID)
		unsigned long			bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth (Hz)
		unsigned long			numChan;		// Number of channels DF'd
		unsigned char			noiseFloor;		// Noise floor (dBm + 200)
		bool					horizPol;		// Horizontal polarization

		struct SChanData
		{
			unsigned short	chan;		// Channel number (zero-based)
			unsigned short	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
			unsigned short	conf;		// DF confidence (1/10th %, 0 - 1000)
			unsigned char	specData;	// Channel spectrum data (dBm + 200)
			unsigned char	dfSpecData;	// DF antenna spectrum data (dBm + 200)
#else
		uint32_t			taskId;			// Task ID (same as RT_DF_START msgType taskID)
		uint32_t			bandIndex;		// Band index (for each task ID, goes from 0 to the number of tasked bands - 1)
		Units::Frequency::Raw	firstChanFreq;	// Center RF frequency of the first (lowest) channel (Hz)
		Units::Frequency::Raw	chanSize;		// Channel spacing or bandwidth (Hz)
		uint32_t			numChan;		// Number of channels DF'd
		uint8_t			noiseFloor;		// Noise floor (dBm + 200)
		bool					horizPol;		// Horizontal polarization

		struct SChanData
		{
			uint16_t	chan;		// Channel number (zero-based)
			uint16_t	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
			uint16_t	conf;		// DF confidence (1/10th %, 0 - 1000)
			uint8_t	specData;	// Channel spectrum data (dBm + 200)
			uint8_t	dfSpecData;	// DF antenna spectrum data (dBm + 200)
#endif
		} chanData[1];
	};

	// Message structure for RT_IQ_DATA msgType
	//  this is a variable length message -- only numIq (up to MAX_IQ) worth of data is sent
	struct SIqData  // msgTypeVersion = 1
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short MAX_IQ = 1024;

		DATE			dateTime;	// Time of first sample (may be 0 if not tagged)
		unsigned long	freq;		// Center frequency (may be 0 if not tagged)
		unsigned long	bw;			// Bandwidth (may be 0 if not tagged)
		unsigned long	sampleRate;	// Sampling rate
		unsigned short	numIq;		// Number of samples
		unsigned char	rxAtten;	// Receiver attenuation
		signed char		scale;		// Scale factor (dB - FSD in IQ data represents 0 dBm before scaling)
		char			rxChan;		// Channel (0, 1)

		struct SIq
		{
			short q;
			short i;
#else
		static const uint16_t MAX_IQ = 1024;

		DATE			dateTime;	// Time of first sample (may be 0 if not tagged)
		uint32_t	freq;		// Center frequency (may be 0 if not tagged)
		uint32_t	bw;			// Bandwidth (may be 0 if not tagged)
		uint32_t	sampleRate;	// Sampling rate
		uint16_t	numIq;		// Number of samples
		uint8_t	rxAtten;	// Receiver attenuation
		int8_t		scale;		// Scale factor (dB - FSD in IQ data represents 0 dBm before scaling)
		char			rxChan;		// Channel (0, 1)

		struct SIq
		{
			int16_t q;
			int16_t i;
#endif
		} iq[MAX_IQ];
	};

	//  this is a variable length message -- only numIq (up to MAX_IQ) worth of data is sent
	struct SIqDataV2  // msgTypeVersion = 2
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short MAX_IQ = 1024;

		DATE					dateTime;	// Time of first sample (may be 0 if not tagged)
		Units::Frequency::Raw	freq;		// Center frequency (may be 0 if not tagged)
		Units::Frequency::Raw	bw;			// Bandwidth (may be 0 if not tagged)
		unsigned long			sampleRate;	// Sampling rate
		unsigned short			numIq;		// Number of samples
		unsigned char			rxAtten;	// Receiver attenuation
		signed char				scale;		// Scale factor (dB - FSD in IQ data represents 0 dBm before scaling)
		char					rxChan;		// Channel (0, 1)

		struct SIq
		{
			short q;
			short i;
		} iq[MAX_IQ];
#else
		static const uint16_t MAX_IQ = 1024;

		DATE					dateTime;	// Time of first sample (may be 0 if not tagged)
		Units::Frequency::Raw	freq;		// Center frequency (may be 0 if not tagged)
		Units::Frequency::Raw	bw;			// Bandwidth (may be 0 if not tagged)
		uint32_t			sampleRate;	// Sampling rate
		uint16_t			numIq;		// Number of samples
		uint8_t			rxAtten;	// Receiver attenuation
		int8_t				scale;		// Scale factor (dB - FSD in IQ data represents 0 dBm before scaling)
		char					rxChan;		// Channel (0, 1)

		struct SIq
		{
			int16_t q;
			int16_t i;
		} iq[MAX_IQ];
#endif
	};

	//  this is a variable length message -- only numIq (up to MAX_IQ) worth of data is sent
	// Since body will start on 4-byte alignment (pack(4) in message format), this struct also uses pack(4) so that the
	// double and int64_t variables will be properly aligned.
#pragma pack(push, 4)
	struct SIqDataV3  // msgTypeVersion = 3
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short MAX_IQ = 1024;
// NOTE: Because header is 12 bytes long and pack is 4, the body starts on a 4-byte alignment.
//		 So, the order of the variables in this version has been changed to give correct alignment
		unsigned long			sampleRate;	// Sampling rate
		DATE					dateTime;	// Time of first sample (may be 0 if not tagged)
		Units::Frequency::Raw	freq;		// Center frequency (may be 0 if not tagged)
		Units::Frequency::Raw	bw;			// Bandwidth (may be 0 if not tagged)
		unsigned short			numIq;		// Number of samples
		unsigned char			rxAtten;	// Receiver attenuation
		signed char				scale;		// Scale factor (dB - FSD in IQ data represents 0 dBm before scaling)
		unsigned char			rxChan;		// Channel (0, 4)
		unsigned char 			pad[3];		// Make sure alignment of iq[] is on 4-byte boundary

		struct SIq
		{
			short q;
			short i;
		} iq[MAX_IQ];
#else
		static const uint16_t MAX_IQ = 1024;
// NOTE: Because header is 12 bytes long and pack is 4, the body starts on a 4-byte alignment.
//		 So, the order of the variables in this version has been changed to give correct alignment
		uint32_t			sampleRate;	// Sampling rate
		DATE					dateTime;	// Time of first sample (may be 0 if not tagged)
		Units::Frequency::Raw	freq;		// Center frequency (may be 0 if not tagged)
		Units::Frequency::Raw	bw;			// Bandwidth (may be 0 if not tagged)
		uint16_t			numIq;		// Number of samples
		uint8_t			rxAtten;	// Receiver attenuation
		int8_t				scale;		// Scale factor (dB - FSD in IQ data represents 0 dBm before scaling)
		uint8_t			rxChan;		// Channel (0, 4)
		uint8_t 			pad[3];		// Make sure alignment of iq[] is on 4-byte boundary

		struct SIq
		{
			int16_t q;
			int16_t i;
		} iq[MAX_IQ];
#endif
	};
#pragma pack(pop)

#ifndef CSMSPLUS_SRV_BUILT
	typedef struct { short re; short im; }   TciInt16c;    // same as Ipp16sc
	typedef struct { int re;   int im; }     TciInt32c;    // same as Ipp32sc
#else
	typedef struct { int16_t re; int16_t im; }   TciInt16c;    // same as Ipp16sc
	typedef struct { int32_t re;   int32_t im; }     TciInt32c;    // same as Ipp32sc
#endif
	typedef struct { float re; float im; }   TciFloat32c;  // same as Ipp16fc

	enum DataType {
		DataType_Int16c = 0,
		DataType_Int32c = 1,
		DataType_Float32c = 2
	};

#pragma pack(push, 4)
	// IQ_DATA
	struct SIqDataV4  // msgTypeVersion = 4
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short MAX_SAMPLES = 1024;

		unsigned long	       streamID;          // Stream ID 
		Units::Timestamp::Raw  streamStartTime;   // Time of the very first sample for the stream 
		unsigned long long     sampleOffset;       // Offset of this data set from the beginning of the stream 
		Units::Frequency::Raw  freq;	          // Center frequency (may be 0 if not tagged)
		double		 		   actualSampleRate;        // sampling rate
		Units::Frequency::Raw  actualBW;		          // Bandwidth (may be 0 if not tagged)
		unsigned char          ddcChannel;        // Channel (0, 4)
		unsigned short         seqNumber ;       // Frame sequence number (starts with 0, wraps) (to compute lost msg/blocks)
		double                 scaleFactor;       // TBD
		unsigned char          inputPort;         // Input/Antenna port (enums TBD)	
		unsigned char	       rxAtten;	          // Receiver attenuation (dB)
		DataType               dataType;          // type of data delivered below 
		unsigned char 		   EOS;               // non-zero means EOS  
		unsigned short         numSamples;        // Can be less than MAX_SAMPLES; if so message length is truncated; 0 => QUERY response
//		unsigned char 			pad[n];		      // Make sure alignment of iq[] is on 4-byte boundary
#else
		static const uint16_t MAX_SAMPLES = 1024;

		uint32_t	       streamID;          // Stream ID 
		Units::Timestamp::Raw  streamStartTime;   // Time of the very first sample for the stream 
		uint64_t     sampleOffset;       // Offset of this data set from the beginning of the stream 
		Units::Frequency::Raw  freq;	          // Center frequency (may be 0 if not tagged)
		double		 		   actualSampleRate;        // sampling rate
		Units::Frequency::Raw  actualBW;		          // Bandwidth (may be 0 if not tagged)
		uint8_t          ddcChannel;        // Channel (0, 4)
		uint16_t         seqNumber ;       // Frame sequence number (starts with 0, wraps) (to compute lost msg/blocks)
		double                 scaleFactor;       // TBD
		uint8_t          inputPort;         // Input/Antenna port (enums TBD)	
		uint8_t	       rxAtten;	          // Receiver attenuation (dB)
		DataType               dataType;          // type of data delivered below 
		uint8_t 		   EOS;               // non-zero means EOS  
		uint16_t         numSamples;        // Can be less than MAX_SAMPLES; if so message length is truncated; 0 => QUERY response
//		uint8_t 			pad[n];		      // Make sure alignment of iq[] is on 4-byte boundary
#endif
		union SIqType {
			TciInt16c samplesInt16[MAX_SAMPLES];     // Samples
			TciInt32c samplesInt32[MAX_SAMPLES];    // Samples
			TciFloat32c samplesFloat32[MAX_SAMPLES]; // Samples
		} SIq;
	};
#pragma pack(pop)
#pragma pack(push, 4)
	// IQ_DATA
	struct SIqDataV5  // msgTypeVersion = 5
	{
#ifndef CSMSPLUS_SRV_BUILT
		static const unsigned short MAX_SAMPLES = 1024;

		unsigned long	       streamID;          // Stream ID
		Units::Timestamp::Raw  streamStartTime;   // Time of the very first sample for the stream
		unsigned long long     sampleOffset;       // Offset of this data set from the beginning of the stream
		Units::Frequency::Raw  freq;	          // Center frequency (may be 0 if not tagged)
		double		 		   actualSampleRate;        // sampling rate
		Units::Frequency::Raw  actualBW;		          // Bandwidth (may be 0 if not tagged)
		unsigned char          ddcChannel;        // Channel (0, 4)
		unsigned short         seqNumber ;       // Frame sequence number (starts with 0, wraps) (to compute lost msg/blocks)
		double                 scaleFactor;       // TBD
		unsigned char          inputPort;         // Input/Antenna port (enums TBD)
		unsigned char	       rxAtten;	          // Receiver attenuation (dB)
		DataType               dataType;          // type of data delivered below
		unsigned char 		   EOS;               // non-zero means EOS
		unsigned short         numSamples;        // Can be less than MAX_SAMPLES; if so message length is truncated; 0 => QUERY response
		bool				   doRds;
		bool				   doModRec;
		unsigned char 			pad[2];		      // Make sure alignment of iq[] is on 4-byte boundary
#else
		static const uint16_t MAX_SAMPLES = 1024;

		uint32_t	       		streamID;          // Stream ID 
		Units::Timestamp::Raw  	streamStartTime;   // Time of the very first sample for the stream 
		uint64_t     			sampleOffset;       // Offset of this data set from the beginning of the stream 
		Units::Frequency::Raw  	freq;	          // Center frequency (may be 0 if not tagged)
		double		 		   	actualSampleRate;        // sampling rate
		Units::Frequency::Raw  	actualBW;		          // Bandwidth (may be 0 if not tagged)
		uint8_t          		ddcChannel;        // Channel (0, 4)
		uint16_t         		seqNumber ;       // Frame sequence number (starts with 0, wraps) (to compute lost msg/blocks)
		double                 	scaleFactor;       // TBD
		uint8_t          		inputPort;         // Input/Antenna port (enums TBD)	
		uint8_t	       			rxAtten;	          // Receiver attenuation (dB)
		DataType               	dataType;          // type of data delivered below 
		uint8_t 		   		EOS;               // non-zero means EOS  
		uint16_t         		numSamples;        // Can be less than MAX_SAMPLES; if so message length is truncated; 0 => QUERY response
		bool				   	doRds;
		bool				   	doModRec;
		uint8_t 				pad[2];		      // Make sure alignment of iq[] is on 4-byte boundary
#endif
		union SIqType {
			TciInt16c samplesInt16[MAX_SAMPLES];     // Samples
			TciInt32c samplesInt32[MAX_SAMPLES];    // Samples
			TciFloat32c samplesFloat32[MAX_SAMPLES]; // Samples
		} SIq;
	};
#pragma pack(pop)


#pragma pack(push, 4)
	struct SRDSData  // msgTypeVersion = 1
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned short m_piCode;
		unsigned short m_decoderInfo;
		unsigned short m_trafficprog;
		unsigned short m_trafficanncement;
		unsigned int m_extent;
		unsigned int m_event;
		unsigned int m_location;
		unsigned int m_EONPI;
		unsigned int m_EONpty;
		unsigned int m_AFhz;
		short m_stereopilottone;
		short m_validEONInfo;
		short m_validEONpty;
		unsigned char m_direction;
		unsigned char	m_TA;
		unsigned char m_TP;
#ifdef _MSC_VER
		wchar_t m_pty[16];
		wchar_t m_programType[16]; // ps
		wchar_t m_radioText[68];
		wchar_t m_musicSpeech[16];
		wchar_t m_altFreq[68];
#else
		char16_t m_pty[16];
		char16_t m_programType[16]; // ps
		char16_t m_radioText[68];
		char16_t m_musicSpeech[16];
		char16_t m_altFreq[68];
#endif
		char m_country[48];
		char m_clockTime[24];
		char m_region[12];
		char m_EONps[20];
		unsigned char m_channel;

		unsigned char 			spare[19];
#else
		uint16_t m_piCode;
		uint16_t m_decoderInfo;
		uint16_t m_trafficprog;
		uint16_t m_trafficanncement;
		uint32_t m_extent;
		uint32_t m_event;
		uint32_t m_location;
		uint32_t m_EONPI;
		uint32_t m_EONpty;
		uint32_t m_AFhz;
		int16_t m_stereopilottone;
		int16_t m_validEONInfo;
		int16_t m_validEONpty;
		unsigned char m_direction;
		unsigned char	m_TA;
		unsigned char m_TP;
		char16_t m_pty[16];
		char16_t m_programType[16]; // ps
		char16_t m_radioText[68];
		char16_t m_musicSpeech[16];
		char16_t m_altFreq[68];
		char m_country[48];
		char m_clockTime[24];
		char m_region[12];
		char m_EONps[20];
		uint8_t m_channel;

		unsigned char 			spare[19];
#endif
	};
#pragma pack(pop)

#pragma pack(push, 4)
	struct SSigRecData  // msgTypeVersion = 1
	{
		// m_sigType is variable size array, whose size is
		// determined by m_numOfBytes value above.
		// m_sigType can only be accessed through pointer due to
		// variable size.
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char m_channel;
		unsigned short m_numOfBytes;
		char m_sigType[1];
#else
		uint8_t m_channel;
		uint16_t m_numOfBytes;
		char m_sigType[1];
#endif
	};
#pragma pack(pop)

#pragma pack(push, 4)
	struct SModRecData  // msgTypeVersion = 1
	{
		// m_modType is variable size array, whose size is
		// determined by m_numOfBytes value above.
		// m_modType can only be accessed through pointer due to
		// variable size.
#ifndef CSMSPLUS_SRV_BUILT
		unsigned char m_channel;
		unsigned short m_numOfBytes;
		char m_modType[1];
#else
		uint8_t m_channel;
		uint16_t m_numOfBytes;
		char m_modType[1];
#endif
	};
#pragma pack(pop)

	// Message format
#pragma pack(push, 4)
	struct SHdr
	{
#ifndef CSMSPLUS_SRV_BUILT
		unsigned long msgType;
		unsigned long msgTypeVersion;
		unsigned long bodySize;
#else
		uint32_t msgType;
		uint32_t msgTypeVersion;
		uint32_t bodySize;
#endif
	} hdr;

	union UBody
	{
		SStart		start;
		SStartV2	startV2;
		SStop		stop;
		SSpectrum	spectrum;
		SSpectrumV2	spectrumV2;
		SSpectrumV3	spectrumV3;
		SDfData		dfData;
		SDfDataV2	dfDataV2;
		SDfDataV3	dfDataV3;
		SIqData		iqData;
		SIqDataV2	iqDataV2;
		SIqDataV3	iqDataV3;
		SIqDataV4	iqDataV4;
		SIqDataV5	iqDataV5;
		SRDSData    rdsData;
		SSigRecData sigrecData;
		SModRecData modrecData;
	} body;
#pragma pack(pop)

	// new and delete operators for variable-length messages longer than the union body
	static void* operator new(size_t size, size_t sizeOverride = 0)
		{ return malloc(sizeOverride <= size ? size : sizeOverride); }
	static void operator delete(void* ptr, size_t) { free(ptr); }

	static void Realloc(SSmsRealtimeMsg*& msg, size_t size)
	{
		void* newMsg = realloc(msg, size);
	
		if(newMsg == nullptr)
		{
			ASSERT(FALSE);
			throw std::bad_alloc();
		}

		msg = static_cast<SSmsRealtimeMsg*>(newMsg);
	}

#ifdef _DEBUG
#ifndef CSMSPLUS_SRV_BUILT
	static void* operator new(size_t size, LPCSTR fileName, int line, size_t sizeOverride = 0)
		{ return _malloc_dbg(sizeOverride <= size ? size : sizeOverride, _NORMAL_BLOCK, fileName, line); }
	static void operator delete(void* ptr, LPCSTR, int, size_t) { _free_dbg(ptr, _NORMAL_BLOCK); }

	static void Realloc(SSmsRealtimeMsg*& msg, size_t size, LPCSTR fileName, int line)
#else
	static void* operator new(size_t size, LPCSTR fileName, int32_t line, size_t sizeOverride = 0)
		{ return _malloc_dbg(sizeOverride <= size ? size : sizeOverride, _NORMAL_BLOCK, fileName, line); }
	static void operator delete(void* ptr, LPCSTR, int32_t, size_t) { _free_dbg(ptr, _NORMAL_BLOCK); }

	static void Realloc(SSmsRealtimeMsg*& msg, size_t size, LPCSTR fileName, int32_t line)
#endif
	{
		void* newMsg = _realloc_dbg(msg, size, _NORMAL_BLOCK, fileName, line);
	
		if(newMsg == nullptr)
		{
			ASSERT(FALSE);
			throw std::bad_alloc();
		}

		msg = static_cast<SSmsRealtimeMsg*>(newMsg);
	}
#endif
};


struct SRTCapabilities  // if no rt capabliity or both fields are off, then only rds iq streaming can be done
{
#ifndef CSMSPLUS_SRV_BUILT
	unsigned short m_rdsData; 
	unsigned short m_modREc; // not currently possible
	unsigned char spare[16];
#else
	uint16_t m_rdsData;
	uint16_t m_modREc; // not currently possible
	unsigned char spare[16];
#endif
};
