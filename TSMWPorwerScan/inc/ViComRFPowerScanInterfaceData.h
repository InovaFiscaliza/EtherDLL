// ViComRFPowerScanInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_RFPOWERSCAN      7  // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)         // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                        // get a definite structure alignment for CViComRFPowerScanInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace RFPOWERSCAN
{
/**
Data containers used for ViCom RFPowerScan interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_RFPOWERSCAN,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
Spectrum settings contains the basic calculation parameters of a a sweep
\@attribute[type="serializable"]
*/
struct SSpectrumSettings
{	
   SSpectrumSettings()
      : fMaxReportingRateInHz( 1 )
      , fMaxDeviceMeasRateInHz( 1 )
      , eWindowType( RFPOWERSCAN_WINDOWTYPE_FLATTOP )
      , eFFTSize( RFPOWERSCAN_FFTSIZE_1024 )
      , bAutoBandwidth( TRUE )
      , dwBandwidthInHz( (DWORD)20e6 )
      , bLevelThreshold( TRUE )
      , fThresholdInDbm( -150.0f )
      , bPreamplifier( TRUE )
      , bAutoAttenuation( TRUE )
      , bAttenuationInDb( 0 )
   {}

   /**
   FFT window function type:
   Used to define the window function applied to the samples.
   */
   enum etWindowType
   {
      RFPOWERSCAN_WINDOWTYPE_RECT			= 0,	//!<Rectangular window (no window function)
      RFPOWERSCAN_WINDOWTYPE_HANNING		= 1,	//!<Hanning window
      RFPOWERSCAN_WINDOWTYPE_HAMMING		= 2,	//!<Hamming window
      RFPOWERSCAN_WINDOWTYPE_BLACKMAN		= 3,	//!<Blackman window
      RFPOWERSCAN_WINDOWTYPE_FLATTOP		= 4,	//!<Flat top window
   };

   /**
   FFT Size:
   Used to define the count of samples for the FFT.
   */
   enum etFFTSize
   {
      RFPOWERSCAN_FFTSIZE_16					= 16,		//!<FFT with 16 samples
      RFPOWERSCAN_FFTSIZE_32					= 32,		//!<FFT with 32 samples
      RFPOWERSCAN_FFTSIZE_64					= 64,		//!<FFT with 64 samples 
      RFPOWERSCAN_FFTSIZE_128				   = 128,	//!<FFT with 128 samples
      RFPOWERSCAN_FFTSIZE_256				   = 256,	//!<FFT with 256 samples
      RFPOWERSCAN_FFTSIZE_512				   = 512,	//!<FFT with 512 samples
      RFPOWERSCAN_FFTSIZE_1024				= 1024,	//!<FFT with 1024 samples
      RFPOWERSCAN_FFTSIZE_2048				= 2048,	//!<FFT with 2048 samples
      RFPOWERSCAN_FFTSIZE_4096				= 4096,	//!<FFT with 4096 samples
      RFPOWERSCAN_FFTSIZE_8192				= 8192,	//!<FFT with 8192 samples
   };

   /**
   Maximum result reporting rate
   */
   float	         fMaxReportingRateInHz;

   /**
   Maximum measurement rate (reading rate on the TSMW)
   */
   float	         fMaxDeviceMeasRateInHz;

   /**
   Window type according enumerator description
   */
   etWindowType	eWindowType;

   /**
   FFT size according enumerator description
   */
   etFFTSize		eFFTSize;
      
   /**
   Auto bandwidth flag: 1 = auto bandwidth enabled, 0 = auto bandwidth disabled.
   If auto bandwidth is used, blocks of 20 MHz in a 20 MHz raster with the corresponding center frequencies 30, 50, 70 ... MHz are used.
   @note SampleRateInHz = 1.15 * dwBandwidthInHz = 23 MHz
   */
   BYTE		      bAutoBandwidth;

   /**
   FFT-Bandwidth:	Blocksize, which will be scanned at once, if auto bandwidth is disabled. The valid range is between 1 MHz and 20 MHz.
   @note SampleRateInHz = 1.15 * dwBandwidthInHz
   */
   DWORD	         dwBandwidthInHz;

   /**
   Level threshold flag: 1 = level threshold check enabled, 0 = level threshold check disabled
   */
   BYTE		      bLevelThreshold;

   /**
   Level threshold value if the threshold check is enabled:	only records with minimum one value above the threshold will be sent to the host.
   */
   float	         fThresholdInDbm;

   /**
   Preamplifier flag: 1 = enable preamplifier, 0 = disable preamplifier. Only for TSMW receivers.
   */
   BYTE		      bPreamplifier;

   /**
   Auto attenuation flag: 1 = enable auto attenuation, 0 = use fixed attenuation. Only for TSMW receivers.
   */
   BYTE		      bAutoAttenuation;

   /**
   Fixed attenuation in dB if auto attenuation is disabled. Only for TSMW receivers.
   */
   BYTE		      bAttenuationInDb;

};	// struct SSpectrumSettings

/** 
The structure SMeasurementTime contains the parameters to define the measurement time
\@attribute[type="serializable"]
*/
struct SMeasurementTime
{
   SMeasurementTime()
      : dwMeasTimeInNs( 1 )
      , eDetectorType( RFPOWERSCAN_DETECTOR_TYPE_RMS )
   {}

   /**
   Definition of measurement time
   Used to define the measurement time independent of the FFT size.
   */
   enum etDetectorType
   {
      RFPOWERSCAN_DETECTOR_TYPE_MAXPEAK		= 0,		//!<Peak detector (detects the maximum value)
      RFPOWERSCAN_DETECTOR_TYPE_RMS			   = 1,		//!<RMS detector (calculates the root mean square)
      RFPOWERSCAN_DETECTOR_TYPE_MINPEAK		= 2,		//!<Min detector (detects the minmal level 
   };

   /**
   Measurement time in nsec
   */
   DWORD	         dwMeasTimeInNs;

   /**
   Detector type according enumerator description
   */
   etDetectorType	eDetectorType;
};

/** 
The structure SFrequencyDetector contains the parameters for the level measurement in frequency domain
\@attribute[type="serializable"]
*/
struct SFrequencyDetector
{
   SFrequencyDetector()
      : dwCountOfLines( 1024 )
      , eDetectorType( RFPOWERSCAN_FREQDET_TYPE_RMS )
   {}

   /**
   Detector type in frequency domain.
   Used to define the type of level detector used in frequency domain.
   */
   enum etFrequencyDetectorType
   {
      RFPOWERSCAN_FREQDET_TYPE_PEAK		   = 0,		//!<Peak detector (detects the maximum value)
      RFPOWERSCAN_FREQDET_TYPE_RMS			= 1,		//!<RMS detector (calculates the root mean square)
      RFPOWERSCAN_FREQDET_TYPE_AUTOPEAK	= 2,		//!<Auto peak detector 
   };

   /**
   Count of spectral lines for the detector
   */
   DWORD	                  dwCountOfLines;

   /**
   Detector type according enumerator description
   */
   etFrequencyDetectorType	eDetectorType;
};

   
/**
The structure STimeDetector contains the parameters for the level measurement in time domain
\@attribute[type="serializable"]
*/
struct STimeDetector
{
   STimeDetector()
      : eDetectorType( RFPOWERSCAN_TIMEDET_TYPE_RMS )
      , eDetectorIntervalType( RFPOWERSCAN_TIMEDET_INTERVAL_TIMERANGE )
      , dwTimeParameterInMs( 1000 )
   {}

   /**
   Detector type in time domain.
   Used to define the type of time detector used in time domain.
   */
   enum etTimeDetectorType
   {
      RFPOWERSCAN_TIMEDET_TYPE_PEAK		   = 0,		//!<Peak detector (detects the maximum value)
      RFPOWERSCAN_TIMEDET_TYPE_RMS			= 1,		//!<RMS detector (calculates the root mean square)
      RFPOWERSCAN_TIMEDET_TYPE_AUTOPEAK	= 2,		//!<Auto peak detector
      RFPOWERSCAN_TIMEDET_TYPE_MINPEAK	   = 3,		//!<Min peak (detects the minimum value)
      RFPOWERSCAN_TIMEDET_TYPE_ALL			= 5,		//!<all values remain in the result
   };

   /**
   Description of the usage of the time interval value.
   Used to define how the value of the time interval should be used in the level measurement
   */
   enum etTimeDetectorIntervalType
   {
      RFPOWERSCAN_TIMEDET_INTERVAL_MINTIME		= 0,	//!<All sweeps with a timestamp above the time parameter shall be used
      RFPOWERSCAN_TIMEDET_INTERVAL_TIMERANGE		= 1,	//!<All sweeps not older than the time parameter shall be used.
   };
      
   /**
   Detector type according enumerator description
   */
   etTimeDetectorType			eDetectorType;

   /**
   Time interval type according enumerator description
   */
   etTimeDetectorIntervalType	eDetectorIntervalType;

   /**
   Time parameter: eDetectorIntervalType defines how this value should be used
   */
   DWORD	                     dwTimeParameterInMs;
};


/**
The channel filter sequence contains several subsequences of equally spaced frequency subbands, all subbands use the same channel filter
\@attribute[type="serializable"]
*/
struct SChannelFilterSequence
{
   SChannelFilterSequence()
      : dwCountOfSubsequences( 0 )
      , pSubsequences( NULL )
   {}

   /**
   The structure SChannelFilterDefinition defines the frequency response of the channel filter (one-sided, filter assumed to be symmetrical)
   * \@attribute[type="serializable"]
   */
   struct SChannelFilterDefinition
   {
      SChannelFilterDefinition()
         : dwCountOfFrequencies( 0 )
         , dFrequencySpacingInHz( 0 )
         , pfMagnitudeOfTransferFunction( NULL )
      {}

      /**
      Count of frequencies: count of entries for the filter definition
      */
      DWORD		dwCountOfFrequencies;

      /**
      Frequency resolution in the table (first frequency is 0 --> center frequency of the channel)
      */
      double	dFrequencySpacingInHz;

      /**
      Magnitude array of the transfer function, the final values of the transfer function will be a linear interpolation of the table
      */
      float*	pfMagnitudeOfTransferFunction;
      //!< \@attribute[type="array" size="dwCountOfFrequencies" sizeismember="true"]

   } sChannelFilterDefinition;

   /**
   Count of individual subsequences within the sequence
   */
   DWORD		dwCountOfSubsequences;

   /**
   The channel filter subsequence defines an equally segmented frequency band where the channel filter will be applied
   * \@attribute[type="serializable"]
   */
   struct SChannelFilterSubsequence
   {
      SChannelFilterSubsequence()
         : dFirstFrequencyInHz( 0 )
         , dSpacingInHz( 0 )
         , dwCountOfElements( 0 )
      {}

      /**
      First frequency of the subsequence
      */
      double	dFirstFrequencyInHz;

      /**
      Frequency raster
      */
      double	dSpacingInHz;

      /**
      Width (number of elements) of the subsequence
      */
      DWORD	   dwCountOfElements;
   } *pSubsequences;
   //!< \@attribute[type="array" size="dwCountOfSubsequences" sizeismember="true"]
};

/**
Marker settings
One single marker is available. The marker will be set to the frequency with the highest level.
\@attribute[type="serializable"]
*/
struct SMarker
{
   SMarker()
      : bUseMarker( 0 )
      , bReturnsPowerValues( 0 )
   {}

   /**
   Flag to activate the marker mode
   */
   BYTE		bUseMarker;

   /**
   Flag to request all power values
   */
   BYTE		bReturnsPowerValues;
};

/**
The structure Settings contains all parameters of a sweep.
Each instance of the interface controls a single sweep.
\@attribute[type="serializable"]
*/
struct SSweepSettings
{
   SSweepSettings()
      : dwFrontEndSelectionMask( SRFPort::RF_1 )
      , dStartFrequencyInHz( 0 )
      , dStopFrequencyInHz( 0 )
      , bRequestRawData( FALSE )
   {}

   /** 
   Receiver front end to be used by the scanner for this measurement.
   RFPowerScan tasks are not allowed to contain multiple FE's.
   */
   DWORD                   dwFrontEndSelectionMask;

   /**
   Lower frequency limit of the sweep range
   */
   double                  dStartFrequencyInHz;

   /**
   Upper frequency limit of the sweep range
   */
   double		            dStopFrequencyInHz;

   /**
   Raw data reporting: If enabled the FFT results are delivered to the host application without processing (frequency-/time-detector and channel filter)
   */
   BYTE			            bRequestRawData;

   /**
   Spectrum settings for the sweep (see explanation of the SSpectrumSettings structure)
   */
   SSpectrumSettings		   sSpectrumSettings;

   /**
   Measurement time (see explanation of the SMeasurementTime structure)
   */
   SMeasurementTime		   sMeasurementTime;

   /**
   Frequency detector setting for the sweep (see explanation of the SFrequencyDetector structure)
   */
   SFrequencyDetector	   sFrequencyDetector;

   /**
   Time detector setting for the sweep (see explanation of the STimeDetector structure)
   */
   STimeDetector		      sTimeDetector;

   /**
   Channel filter sequence setting for the sweep (see explanation of SChannelFilterSequence, SChannelFilterSubsequence and SChannelFilterDefinition)
   */
   SChannelFilterSequence	sChannelFilterSequence;

   /**
   Marker activation and configuration (see explanation of SMarker structure)
   */
   SMarker				      sMarker;

};

//** Request Settings **/
/**
This structure holds all the measurement setup that is configured in the ViCom RFPowerScan scanner module. 
It is a result of the CViComRFPowerScanInterface::GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   // basic settings
   DWORD                               dwReceiverIndex;              //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth                  ResultBufferDepth;            //!< Number of results allowed in the result buffer

   // measurement specific settings
   SSweepSettings                      SweepSettings;                //!< Defines the measured frequencies.
};

/**
Result structure for all measurements.
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /**
   Measurement timestamp (PC tic) in ms.
   */
   DWORD			dwPcTimeStampInMs;

   /**
   Overflow indicator
   */
   BYTE			bOverflow;

   /**
   Result structure for spectrum measurements.
   \@attribute[type="serializable"]
   */
   struct SSpectrumResult
   {
      /**
      Count of level results in the result array
      */
      DWORD			dwCount;

      /**
      Array of levels in dBm (only valid if the marker is not used)
      \@attribute[type="array" size="dwCount" sizeismember="true"]
      */
      float*		pfSpectrumValuesInDBm;

   } *pSpectrumResult;
      
   /**
   Result structure for measurements using the marker.
   \@attribute[type="serializable"]
   */
   struct SMarkerResult
   {
      /**
      count of individual results (0 if the marker is not used)
      */
      DWORD			dwCount;

      /**
      Marker frequency in Hz (the marker is set to the spectral line with the highest level)
      */
      double		dMarkerFrequencyInHz;

      /**
      Array of level values in dBm.
      \@attribute[type="array" size="dwCount" sizeismember="true"]
      */
      float*		pfMarkerValuesInDBm;

   } *pMarkerResult;

   /**
   List of measurement rates rates and other values in relation to the measurement rate for
   one or more time intervals of measurement rate calculation. The measurement rate is
   the ratio of all measurements of the same type of measurement at the time
   the measurement signals have been picked from the air. So the time of arrival
   at the host application is not used. And equal measurements at different RF channels are put together.
   */
   SViComList<SMeasurementRate> ListMeasurementRates;

};

} // namespace RFPOWERSCAN
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
