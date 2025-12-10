// ViComGsmInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_GSM            12   // do not edit

#if !defined(RESOURCE_INCLUDE)               // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                        // get a definite structure alignment for CViComGsmInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace GSM
{

/**
Data containers used for ViCom GSM interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_GSM,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
Frequency settings
\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   SFrequencySetting( )
      : dCenterFrequencyInHz( 0 )
   {}

   /** Center frequency of the channel */
   double               dCenterFrequencyInHz;
};

/**
SChannelSettings contains the count of channels or frequencies and the table of frequencies in Hz.

The selection of new frequencies (with SetFrequencyTable) automatically resets the 
demodulation requests.
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   SChannelSettings()
      : dwFrontEndSelectionMask( SRFPort::RF_1 )
      , dwCount( 0 )
      , dwMeasRatePer1000Sec( 0 )
      , pTableOfFrequencySetting( NULL )
   {}

   /** 
   Receiver front end to be used by the scanner for this channel.
   Scanner tasks are not allowed to contain multiple FE's.
   */
   DWORD                dwFrontEndSelectionMask;

   /**
   Number of frequency configurations that shall be used from the pTableOfFrequencySetting to perform the measurement tasks.
   For value range see GetChannelCountLimits().
   **/
   DWORD                dwCount;

   /**
   The desired measurement rate value. For default value and range see GetMeasRateLimits().
   The limits are dependent from the currently set eMeasurementMode.
   */
   DWORD                dwMeasRatePer1000Sec;

   /**
   Pointer to an array containing the settings of the selected frequencies.
   The number of members in the array is defined by dwCount (above).
     
   \@attribute[type="array" size="dwCount" sizeismember="true"]
   */
   SFrequencySetting    *pTableOfFrequencySetting;
};


/**
SSpectrumSpec contains a definition of channel spectrum measurements which applies for all channels with 
spectrum measurement enabled.
\@attribute[type="serializable"]
*/
struct SSpectrumSpec
{
   SSpectrumSpec()
      : wCountOfPowerValuesPerChannel( wDefaultCountOfPowerValuesPerChannel )
      , wCollectionTimeIn100us( wDefaultCollectionTimeIn100us )
      , eFreqDetector( FreqDetector::PEAK )
      , eTimeDetector( TimeDetector::RMS )
   {}

   // limits for wCountOfPowerValuesPerChannel:
   static const WORD   wMinCountOfPowerValuesPerChannel           = 1; 
   static const WORD   wMaxCountOfPowerValuesPerChannel           = 100;
   static const WORD   wDefaultCountOfPowerValuesPerChannel       = 50;
   /**
   Each GSM channel of 200kHz bandwidth will be divided in wCountOfPowerValues
   per Channel frequency intervals of equal length. Minimum should be 1 and maximum is 100. 
   */
   WORD    wCountOfPowerValuesPerChannel;

   // limits for wCollectionTimeIn100us:
   // Min: 0: No collection of FFTs. Each FFT will be reported as a sub spectrum.
   static const WORD    wMinCollectionTimeIn100us                 = 0;
   // Max: 520: Collection of all FFTs over the complete channel meas time. Report only one sub spectrum.
   static const WORD    wMaxCollectionTimeIn100us                 = 520;   //Meas time about 50ms  
   static const WORD    wDefaultCollectionTimeIn100us             = 50;
   /**
   Each sub spectrum is calculated from a few FFTs. The processing will collect a 
   constant number of FFTs to produce a sub spectrum. This number is chosen in a 
   way that the Collection Time will be approximated as good as possible. The result 
   value for each frequency interval after collecting some FFTs is defined by
   the Time Detector below.
   */
   WORD    wCollectionTimeIn100us;

   /**
   Choice of detector mode in the frequency domain.
   \@attribute[type="serializable"]
   */
   struct FreqDetector { enum Type {
         PEAK = 1,      //!< peak detector
         INVALID = 2    //!< invalid detector
   }; };
   
   /**
   Frequency detector mode.
   */
   FreqDetector::Type   eFreqDetector;

   /**
   Choice of detector mode in the time domain.
   \@attribute[type="serializable"]
   */
   struct TimeDetector { enum Type {
         RMS = 0,       //!< root mean square detector
         PEAK = 1,      //!< peak detector
         INVALID = 2    //!< invalid detector
   };};

   /**
   Time detector mode.
   */
   TimeDetector::Type   eTimeDetector;
};


/**
SChannelPowerSpec  contains a definition of the channel power graph which applies for all channels with 
channel power measurement enabled.
\@attribute[type="serializable"]
*/
struct SChannelPowerSpec
{
   SChannelPowerSpec()
      : wRMSLengthIn40ns( wDefaultRMSLengthIn40ns )
      , wCountOfResultsPerGSMTimeSlot( wDefaultCountOfResultsPerGSMTimeSlot )
   {}

   // limits for wRMSLengthIn40ns:
   static const WORD    wMinRMSLengthIn40ns                       = 160;   //!< 6.4us
   static const WORD    wMaxRMSLengthIn40ns                       = 14425; //!< 577us, one GSM time slot
   static const WORD    wDefaultRMSLengthIn40ns                   = 900;   //!< 36us
   /**
   the power is averaged over a time interval given in this value
   */
   WORD    wRMSLengthIn40ns;      
     
   // limits for wCountOfResultsPerGSMTimeSlot:
   static const WORD    wMinCountOfResultsPerGSMTimeSlot          = 1;
   static const WORD    wMaxCountOfResultsPerGSMTimeSlot          = 50;
   static const WORD    wDefaultCountOfResultsPerGSMTimeSlot      = 16;
   /**
   The time distance between adjacent results is always a fraction of a GSM time slot. 
   This value gives the time distance dT/us = 12 * 625 / (13 * wCountOfResultsPerGSMTimeSlot)
   */
   WORD    wCountOfResultsPerGSMTimeSlot; 
};


/**
SChannelMeasSpec defines a new set of measurements for a channel (as given by frequency index).
\@attribute[type="serializable"]
*/
struct SChannelMeasSpec
{
   SChannelMeasSpec()
      : dwFrequencyIndex( 0 )
      , bMEAS_SCH( bDefaultMEAS_SCH )
      , bMEAS_CARRIER_TO_INTERFERENCE( bDefaultMEAS_CARRIER_TO_INTERFERENCE )
      , bMEAS_DB_REMOVAL( bDefaultMEAS_DB_REMOVAL )
      , bMEAS_DB_POWER( bDefaultMEAS_DB_POWER )
      , bMEAS_TSC( bDefaultMEAS_TSC )
      , bMEAS_CHANNEL_POWER( bDefaultMEAS_CHANNEL_POWER )
      , bMEAS_SPECTRUM( bDefaultMEAS_SPECTRUM )
      , bMEAS_REPORT_FAILED_TRIALS( bDefaultMEAS_REPORT_FAILED_TRIALS )
   {}

   /**
   Frequency index for the new measurement set. 
   */
   DWORD    dwFrequencyIndex;

   /**
   If a following switch is set then the measurement will be done, else it will be stopped for this channel.
   */

   /** 
   SCH related measurements. (SCH, ETS power, BSIC, FN demodulation) Should be used for downlink only.
   */
   bool     bMEAS_SCH;
   static const bool    bDefaultMEAS_SCH                          = true;
   /**
   Carrier to Interference measurement.
   This is only valid for SCH related measurements and downlink. 
   The C/I is measured using an equalizer on the Extended Training Sequence within the SCH.
   */
   bool     bMEAS_CARRIER_TO_INTERFERENCE;
   static const bool    bDefaultMEAS_CARRIER_TO_INTERFERENCE      = true;
   /**
   Detection of dummy bursts and removal of dummy bursts from the GSM signal before further analysis. 
   This can make SCH and TCH measurements significantly more sensitive. It should be used for downlink channels only. 
   bMEAS_DB_POWER = true is a precondition to remove  dummy bursts.
   */
   bool     bMEAS_DB_REMOVAL;
   static const bool    bDefaultMEAS_DB_REMOVAL                   = false;
   /** 
   Power measurement for dummy bursts. This can produce much more power results for C0 carriers than 
   the SCH measurement. It shall be used for downlink only. 
   */
   bool     bMEAS_DB_POWER;
   static const bool    bDefaultMEAS_DB_POWER                     = false;
   /** 
   Trainings Sequence Code Measurement. This is a time slot measurement for normal GSM bursts. 
   It is applicable for up- and downlink. 
   */
   bool     bMEAS_TSC;
   static const bool    bDefaultMEAS_TSC                          = false;
   /** 
   This measurement will do the channel power measurement as defined in SChannelPowerSpec.
   */
   bool     bMEAS_CHANNEL_POWER;
   static const bool    bDefaultMEAS_CHANNEL_POWER                = false;
   /** 
   Spectrum measurement for this channel as defined in SSpectrumSpec.
   */
   bool     bMEAS_SPECTRUM;
   static const bool    bDefaultMEAS_SPECTRUM                     = false;
   /** 
   Failed SCH meas trials will be reported.
   */
   bool     bMEAS_REPORT_FAILED_TRIALS;
   static const bool    bDefaultMEAS_REPORT_FAILED_TRIALS         = false;
};


/**
SMeasurementDetails contains parameters for certain measurement functions and a selection for 
measurements for a list of individual channels. 
All channels which are not given in the list will remain with their default setting.
If this structure is not used at the start command, the default 
measurements (SCH Power measurement and SCH demodulation) is done for all channels.

By using this structure in the "RedefineMeasurementDetails" function, it is possible to 
redefine measurements between the start and stop command during measurement. 
Note that the redefinition will not allow to set a new frequency list but it will refer to the initial 
frequency list given with the first start command.
\@attribute[type="serializable"]
*/
struct SMeasurementDetails
{
   SMeasurementDetails()
      : dwCount( 0 )
      , pTableOfChannelMeasSpec( NULL )
   {}

   SSpectrumSpec        SpectrumSpec;                 //!< Channel spectrum measurement configuration
   SChannelPowerSpec    ChannelPowerSpec;             //!< Channel power measurement configuration

   /**
   Number of selected channels. Same as SChannelSettings::dwCount.
   */
   DWORD                dwCount;

   /**
   Each frequency index shall occur only once in this array.
   \@attribute[type="array" size="dwCount" sizeismember="true"]   
   */
   SChannelMeasSpec     *pTableOfChannelMeasSpec;
};




//** BCH System Information Types demodulation **/

/**
This enumeration defines the System Information Types which can be demodulated from the BCH.
\@attribute[type="serializable"]
*/
struct Pdu { enum Type
{
   SITYPE_1          = 0x19,        //!< System Information Type 1
   SITYPE_2          = 0x1A,    
   SITYPE_2_BIS      = 0x02,    
   SITYPE_2_N        = 0x45,    
   SITYPE_2_QUATER   = 0x07,    
   SITYPE_2_TER      = 0x03,    
   SITYPE_3          = 0x1B,    
   SITYPE_4          = 0x1C,    
   SITYPE_7          = 0x1F,    
   SITYPE_8          = 0x18,    
   SITYPE_9          = 0x04,   
   SITYPE_13         = 0x00,    
   SITYPE_13_ALT     = 0x44,    
   SITYPE_15         = 0x43,    
   SITYPE_16         = 0x3D,    
   SITYPE_17         = 0x3E,    
   SITYPE_18         = 0x40,    
   SITYPE_19         = 0x41,    
   SITYPE_20         = 0x42,
   UNKNOWN           = SDefs::INVALID_DWORD_VALUE
}; };

/**
This enumeration defines modes for the demodulation.
Use this in the SDemodRequest structure to specify when the demodulation 
of this PDU shall be done.
\@attribute[type="serializable"]
*/
struct DemodMode{ enum Type
{
   /**
   The PDU is demodulated only once for each BTS.
   Only to be used in SDemodulationSettings::sStartMeasurementRequests.
   */
   ONCE             = 0,

   /**
   The PDU is only demodulated if an extra command requests the demodulation.
   Only to be used in SDemodulationSettings::sStartMeasurementRequests.

   If this is used, then no PDU is decoded until it is explicitly requested. The method
   CViComWcdmaInterface::IssueDemodRequests can be used for that purpose.

   PDUs configured with this setting can be decoded using one of BTS, BTS_FORCE,
   BTS_OLD, BTS_OLD_FORCE modes.
   */
   ON_CMD           = 1,

   /**
   After each successful demodulation a new one will be started automatically after the delay time specified in wRepetitionDelayIn100ms
   Only to be used in start measurement.
   */
   REPETITION       = 2,

   /**
   The demodulation is only for a BTS.
   This can be requested only during a running measurement and when the PDU was also requested in SDemodulationSettings::sStartMeasurementRequests.
   If dwTimeOutOrRepetitionDelayIn100ms is different from 0 then the demodulation is only tried during the given time.
   */
   BTS              = 3,

   /**
   Equal to BTS but the limitation in receiver resource usage will be ignored and the demodulation will be
   be done until the PDU has been demodulated or timeout occurred.
   */
   BTS_FORCE        = 4,

   /**
   All demodulation results for a special BTS will be deleted and
   the demodulation for this BTS starts as specified in SDemodulationSettings::sStartMeasurementRequests.
   This can be requested only during a running measurement.
   */
   BTS_RESET        = 5,

   /**
   All demodulation results for each BTS on the specified channel will be deleted and
   the demodulation for this channel starts as specified in SDemodulationSettings::sStartMeasurementRequests.
   This can be requested only during a running measurement.
   */
   CHANNEL_RESET    = 6,

   /**
   Equal to BTS but previously measured messages may be used for the result.
   This can be requested only during a running measurement.
   */
   BTS_OLD          = 7,

   /**
   Equal to BTS_FORCE but previously measured messages may be used for the result.
   This can be requested only during a running measurement.
   */
   BTS_OLD_FORCE    = 8
};};


/**
This structure is used to configure System Information Type demodulation requests.
It is used in start measurement requests and it is used to issue BCH System Information Type demodulation requests during measurement,
e.g. SI Type demodulation requests on demand or to reset SI Type demodulation.
\@attribute[type="serializable"]
*/
struct SDemodRequests
{
   SDemodRequests()
      : dwCountOfRequests( 0 )
      , pDemodRequests( NULL )
   {}

   DWORD                dwCountOfRequests;   //!< Number of demodulation requests.
   /**      
   There must be one instance of this structure for each BCH System Information Type demodulation request,
   therefore different SITypes can be measured on different channels.

   Example for start measurement:
   On the first channel SI Types 3 and 4 shall be decoded, on the the second channel SI Types 3, 5 and 7 shall be decoded.
   SI Type 5 shall be decoded repeatedly every 10 seconds (100*100 ms) and SI Type 7 shall be decoded on demand.
   This requires five structures containing the following parameter sets:
   @code
   (0, SITYPE_3, ONCE, 0, 0)
   (0, SITYPE_4, ONCE, 0, 0)
   (1, SITYPE_3, ONCE, 0, 0)
   (1, SITYPE_5, REPETITION, 100, 0)
   (1, SITYPE_7, ON_CMD, 0, 0)
   @endcode

   Example during measurement:
   On channel 0 SI Types demodulation shall be completely reset for the BTS identified by dwSCHIndicator 2
   and on the the second channel SI Type 7 shall be decoded for the BTS identified by dwSCHIndicator 3 with 
   a timeout for the request of 1 sec (10*100ms):
   @code
   (0, PDU_SITYPE_NULL, BTS_RESET, 0, 2)
   (1, SITYPE_7, BTS, 10, 3)
   @endcode

   Note that you are responsible to manage the memory when you use SetDemodulationSettings().
   Note: All parameters which are not necessary for a certain request shall be set to 0.
   * \@attribute[type="serializable"]
   */
   struct SDemodRequest
   {
      SDemodRequest()
         : dwChannelIndex( 0 )
         , ePDU( Pdu::SITYPE_1 )
         , eDemodulationMode( DemodMode::ONCE )
         , wRepetitionDelayIn100ms( 0 )
         , dwSCHIndicator( 0 )
      {}

      DWORD                dwChannelIndex;            //!< Index of the frequency in SChannelSettings::pdTableOfFrequencyInMHz.
      Pdu::Type            ePDU;                      //!< The SI Type to be demodulated.
      DemodMode::Type      eDemodulationMode;         //!< The mode used for BCH demodulation.

      static const WORD    wMaxRepetitionDelayIn100ms = 36000;   //!< 1 hour

      WORD                 wRepetitionDelayIn100ms;   //!< Repeat measurement after this delay time, if eDemodulationMode is REPETITION. Timeout for request, if eDemodulationMode is BTS...
         
      DWORD                dwSCHIndicator;            //!< Used to specify BTS for DemodulationModes BTS_FORCE or BTS.
   };
      
   SDemodRequest* pDemodRequests;
   //!< \@attribute[type="array" size="dwCountOfRequests" sizeismember="true"]
};

/**
This structure is used to configure the BCH System Information Type demodulation process.
\@attribute[type="serializable"]
*/
struct SDemodulationSettings
{
   SDemodulationSettings()
      : lTotalPowerOffsetInDB10( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /**
   Threshold for demodulation trials in 0.1dB.
   For default value and range see GetDemodThresholdLimits().
   */
   long                 lTotalPowerOffsetInDB10;

   /**
   Receiver front end to be used by the demodulator for this channel. 
   If no demodulation shall be provided, this shall be set to 0.
   Demodulation tasks are not allowed to contain multiple FE's.
   */
   DWORD                dwFrontEndSelectionMask;

   SDemodRequests       sStartMeasurementRequests;          //!< The requests for the general start measurement command.
};

//** Request Settings **/
/**
By means of the functions: SetFrequencyTable(), SetMeasurementRate() and SetMeasurementDetails() internal settings of the 
GSM-Network-Scanner may be changed and with the functions SelectReceiver(), SetResultBufferDepth(), 
SetTimebaseSynchronisationMode() of CViComBasicInterfaceData change basic settings. 
Except of frequency settings all other settings have default values.

This structure bundles these settings and will be returned as result from the GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   // basic settings
   DWORD                               dwReceiverIndex;              //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   TimeBaseSynchronisationMode::Type   eTimeBaseSynchronisationMode; //!< Specifies with what device the internal clock shall be synchronized
   SResultBufferDepth                  ResultBufferDepth;            //!< Number of results allowed in the result buffer

   // measurement specific settings
   SChannelSettings                    ChannelSettings;              //!< Defines the measured frequencies.
   SMeasurementDetails                 MeasurementDetails;           //!< Defines channel specific measurement details.
   SDemodulationSettings               DemodulationSettings;         //!< Configuration of the BCH System Information Type demodulation
};


//** Measurement results **/
/**
This structure holds a measurement result from the result buffer. Please refer to the manual for a detailed discussion of
how to interpret the results.
@note
   that the result data are for reading only and available until the next interface function is called.

\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /**
   The device time of start of the RF measurement which was used to produce the result.

   The dwPcTimeStampInMs is a conversion of this device time to the PC system time obtained with the GetTickCount() function.
   The adjustment of possible time drift between device time and GetTickCount() as well as the resolution of  GetTickCount() causes
   a conversion error of a few milliseconds.
   dwPcTimeStampInMs may be used to synchronize results with other measurement data in the system or to determine the age
   of the result which is GetTickCount() - dwPcTimeStampInMs;
   */
   DWORD             dwPcTimeStampInMs;

   /**
   If available the device time in ns is reported.
   */
   ULONGLONG         u64DeviceTimeInNs;

   /**
   Attenuation introduced in the measurement.
   */
   STSMxAttenuation
      *pTSMxAttenuation;

   /**
   Time synchronization used to derive results.
   */
   STimeBaseSynchronisation
      *pTimeBaseSynchronisation;

   /**
   SCH(Synchronization channel) power or Total inband power measurement result
   \@attribute[type="serializable"]
   */
   struct SPowerResult
   {
      /**
      */
      double   dTimeOfSlotInSec;

      /**
      Index of the measured frequency
      */
      DWORD    dwFrequencyIndex;

      /**
      Type of power measurement
      */
      enum etMeasType{
         /**
         Invalid power
         */
         MEASTYPE_POWINVALID = 0,
         /**
         Total inband power. Maximum average power for one GSM time slot length.
         Valid independent from eMeasMode.
         */
         MEASTYPE_POWCH = 1,
         /**
         SCH power. Please consider eMeasMode for further information.
         */
         MEASTYPE_POWSCH = 4,
         /**
         Carrier to Interference measurement
         The C/I is measured using an equalizer on the Extended Training Sequence within the SCH.
         */
         MEASTYPE_CARRIER_TO_INTERFERENCE = 6
      }        eMeasType;

      /**
      Mode of power measurement
      */
      enum etMeasMode{
         /**
         Mode is not valid
         */
         MEASMODE_INVALID = 0,
         /**
         SCH power has been measured with the ETS only
         */
         MEASMODE_ETS = 1,
         /**
         SCH power has been demodulated
         */
         MEASMODE_DEMOD = 2,
         /**
         SCH power measurement is from active set 1
         */
         MEASMODE_ACTIVESET1 = 3,
         /**
         SCH power measurement is from active set 2
         */
         MEASMODE_ACTIVESET2 = 4,
         /**
         SCH power measurement for a BTS which was measured before, but
         the measurement failed.
         */
         MEASMODE_FAILED_TRIALS = 6,
         /**
         Carrier to Interference measurement
         The C/I is measured using an equalizer on the Extended Training Sequence within the SCH.
         */
         MEASMODE_CARRIER_TO_INTERFERENCE = 7
      }        eMeasMode;

      /**
      Measured power in dBm 100th
      */
      short    sPowerInDBm100; 

      /**
      Indicator: If other than SDefs::INVALID_DWORD_VALUE it references to a demodulated SCH Measurement (SCHInfo) on 
      the same frequency which is stored in this or an earlier MeasResult.
      */
      DWORD    dwIndicatorOfSCHInfo;

      /**
      Difference between total inband power during the power measurement to the measured power. 
      If sPowerInDBm100 refers to a code power measurement and reflections as well as measurement 
      impacts due to frequency offsets, phase noise and correlation errors are neglected
      then the following two relations hold:
      bTotalPowerOffsetInDB10 = 10 * (-Ec/I0 in dB) and 
      bTotalPowerOffsetInDB10 = 100 * log10(1 + 1/(C/I)).
      The value 255 is defined as an saturation value i.e. when Saturation is reached the measured 
      value of bTotalPowerOffsetInDB10 is >= 255. The accuracy of bTotalPowerOffsetInDB10 is 
      typically better than 0.5dB for the case that reflections and frequency offsets can be neglected.
      */
      byte     bTotalPowerOffsetInDB10;

      /**
      Carrier to Interference in 0.01dB
      This pointer is NULL if the result contains no C/I value.
      */
      short*   psCarrierToInterferenceInDB100;
   };

   /**
   List of Power measurement results
   */
   SViComList<SPowerResult> ListPowerResults;
   //!< \@attribute[filterable="true"]

   /**
   SCH(Synchronization channel) demodulated BTS info results
   * \@attribute[type="serializable"]
   */
   struct SSCHInfoResult
   {
      /**
      */
      double   dTimeOfSlotInSec;

      /**
      Index of the measured frequency
      */
      DWORD    dwFrequencyIndex;

      /**
      Indicator: Referenced by SCH power results and CellIdent.
      */
      DWORD    dwIndicatorOfSCHInfo;

      /**
      This dwIndicatorOfFirstSCHInfo is the indicator of a previous  SCHInfoResult which is related 
      to the current SCHInfoResult.
      For the GSM network scanner two SCH measurements belong together, 
      if the arrival time does not differ for more than 20 us reflections plus 0.30 ppm time drift 
      and in total not more than 2 GSM Slots (1.15ms).
      */
      DWORD    dwIndicatorOfFirstSCHInfo;

      /**
      Decoded BSIC code
      */
      WORD     wBSIC; 

      /**
      Decoded frame number
      */
      DWORD    dwGSMFrameNumber;

      /**
      Indicator: If other than SDefs::INVALID_DWORD_VALUE it references to a CellIdent measurement on 
      the same frequency which is stored in this or a later MeasResult.
      */
      DWORD    dwIndicatorOfCellIdent;
   };

   /**
   List of SCH Info measurement results
   */
   SViComList<SSCHInfoResult> ListSCHInfoResults;

   /**
   Global Cell Identifier from demodulated and decoded ST3 (SYTEM INFORMATION TYPE 3)
   * \@attribute[type="serializable"]
   */
   struct SCellIdentResult
   {
      /**
      */
      double   dTimeOfSlotInSec;

      /**
      Index of the measured frequency
      */
      DWORD    dwFrequencyIndex;

      /**
      Indicator: Referenced by SCHInfo measurements.
      */
      DWORD    dwIndicator;

      /**
      cell identity
      */
      WORD    wCI;

      /**
      location area code
      */
      WORD    wLAC;

      /**
      Mobile country code: 0..0xfff the hexadecimal output of this number gives the decimal code or any digits > 9 if 
      a wrong number was transmitted
      */
      WORD    wMCC;

      /**
      Mobile network code: 0..0xfff the hexadecimal output of this number gives the decimal code or any digits > 9 if 
      a wrong number was transmitted
      */
      WORD    wMNC;
   };

   /**
   List of CellIdent measurement results
   */
   SViComList<SCellIdentResult> ListCellIdentResults;

   /**
   ScanCount: A scan is a measurement for all channels. The Network Scanner tries to fill a scan 
   as soon as possible before starting to measure channels of another scan. But in the case the PC cannot 
   handle all result, the TSMU is forced to throw away some result. In this case the measurement of this 
   channel is repeated as soon as possible but measurements of different scans can interleave with each other.
   */
   DWORD       *pdwScanCount;

   /**
   If true this measurement is the last of the scan number given in dwScanCount.
   */
   bool	      bLastOfScan;

   /**
   List of measurement rates rates and other values in relation to the measurement rate for 
   one or more time intervals of measurement rate calculation. The measurement rate is 
   the ratio of all measurements of the same type of measurement at the time
   the measurement signals have been picked from the air. So the time of arrival 
   at the host application is not used. And equal measurements at different RF channels are put together.      
   */
   SViComList<SMeasurementRate> ListMeasurementRates;

   /**
   Results of BCCH demodulations
   * \@attribute[type="serializable"]
   */
   struct SDemodResult
   {
      /**
      Index of the measured frequency
      */
      DWORD    dwFrequencyIndex;

      /**
      Indicator: References to an demodulated SCH Measurement (SCHInfo) on 
      the same frequency which is stored in this or an earlier MeasResult.
      */
      DWORD    dwIndicatorOfSCHInfo;

      /**
      Demodulation status
      */
      enum etDemodStatus
      {
         DEMOD_STATUS_SUCCESS = 0,
         DEMOD_STATUS_TIME_OUT = 1,
         DEMOD_STATUS_CANCELLED = 2,
         DEMOD_STATUS_INVALID = 3
      }     eDemodStatus; 

      /**
      BCCH type
      */
      enum etBcchType
      {
         BCCHTYPE_NORMAL = 0,
         BCCHTYPE_EXTENDED = 1,
         BCCHTYPE_INVALID = 2
      }     eBcchType; 

      /**
      Number of accumulated signals
      */
      WORD    wBCCHAccumulationCount;

      /**
      Frame number of the FCCH in the last used M51 Frame
      */
      DWORD   dwFrameNumber;

      /**
      PDU that has been measured.
      */
      Pdu::Type   ePDU;

      /**
      Bytes of demodulated sequence
      */
      DWORD   dwBitCount;             //!< count of bits the PDU consists of
      BYTE    *pbBitStream;           //!< pointer to (dwBitCount + 7) / 8 bytes which contain the PDU bits. The first bit is stored in the
                                      //!< bit 7 of pbBitStream[0]. If dwBitCount is not a multiple of 8 than some tail bits in the last byte
                                      //!< pbBitStream[(dwBitCount + 7) / 8 - 1] should be ignored.
                                      //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
   }          *pDemodResult;          //!< If a PDU was measured then the pointer to the PDU result for this PDU measurement is valid (!= NULL)


   /**
   This list contains measured power values for trials of measurements which would occur in the ListPowerResults if 
   we had enough sensitivity. The power results are not valid power values but the besides some correlation noise it
   is an upper limit for the received power of the measured signal.
   */
   SViComList<SPowerResult> ListPowerResultsOfFailedTrials;
   //!< \@attribute[filterable="true"]

   /**
   This list gives a report of executed measurement detail orders for specific channels.
   */
   SViComList<SChannelMeasSpec> ListExecutedMeasSpec;

   /**
   Spectrum for all or a subset of the measured GSM channels.
   The channels for which spectrum measurements have been done are listed in structure SExecutedMesurements 
   in the same order as in they occur in the sub spectrum. 
   \@attribute[type="serializable"]
   */
   struct SSpectrumResult
   {
      /**
      This is the number of subspectrums stored in this structure.
      */
      WORD    wSubSpectrumCount;

      /**
      Count of power values per channel per spectrum. Count of frequency intervals per channel.
      */
      WORD    wCountOfPowerValuesPerChannel;

      /**
      Is the count of used channel times the count of power values per channel.
      */
      WORD    wCountOfPowerValuesPerSubSpec;

      /**
      For each sub spectrum we may use one or more FFTs. This is the number of FFTS in the sub spectrum 
      except the last one which may use less FFTs
      */
      WORD    wCountOfFFTsPerSubSpectrum;
         
      /**
      this is the count of FFTs we have used in the last sub spectrum
      */
      WORD    wCountOfFFTsInLastSubSpectrum;
         
      /**
      It is an FFT spectrum and the time distance of 2 adjacent FFTs is given. Each FFT may use a longer 
      signal section for filtering. 
      */
      WORD    wTimeBetweenFFTsIn100ns;
         
      /**
      resolution bandwidth of the internal filter
      */
      WORD    wResolutionBandwidthInHz;
         
      /*
      Choice of detector mode in the frequency domain
      */
      SSpectrumSpec::FreqDetector::Type eFreqDetector;

      /*
      Choice of detector mode in the time domain
      */
      SSpectrumSpec::TimeDetector::Type eTimeDetector;
         
      /**
      Minimum power value which can be reported in 0.01dBm. It is chosen, that the maximum power (byte Value 255) 
      can be expressed in the possible dynamic range of 128 dB.
      */
      short   sMinPowerValueInDBm100;
         
      /*
      Time Of First FFT In Sec
      */
      double   dTimeOfFirstFFTInSec;

      /**
      Power values for all sub spectrum entries. For each sub spectrum the buffer 
      contains wCountOfPowerValuesPerSubSpec power value offsets in 0.5dB from sMinPowerValueInDBm100 lower power limit. 
      The frequency spacing is 200kHz/wCountOfPowerValuesPerChannel within one channel. 
      The channels for which spectrum measurements have been done are listed in structure SExecutedMesurements 
      in the same order as in they occur in the sub spectrum. 
      This order is from low to high frequencies but the channel list may have gaps e.g. channel 1,2,3,7,8. 
      In this case (GSM900) we have a subspectrum from 935.1 .. 935.7MHz and 936.3 .. 936.7MHz. 
      Note that center frequency for each channel index has to be taken from the start command.
      */
      DWORD   dwCountOfBytes;    //!< Number of bytes in the buffer
       
           
      byte    *pbBuffer;         //!< Pointer to the buffer
                                 //!< \@attribute[type="array" size="dwCountOfBytes" sizeismember="true"]   
   }                    *pSpectrumResult;//!< \@attribute[filterable="true"]


   /**
   Inband power value description for all or a subset of measured channels. The channels itself can be 
   retrieved from the list of ListPowerExecutedMeasSpec. 
   \@attribute[type="serializable"]
   */
   struct SChannelPowerResult
   {
      /**
      Number of channels we have channel power measurements for. The channels itself can be 
      retrieved from the list of ListPowerExecutedMeasSpec. 
      */
      DWORD   dwCountOfChannels;

      /**
      Number of result bytes in the power result buffer per channel. It is equal for each channel.
      */
      DWORD   dwCountOfResultsPerChannel;
         
      /**
      The time distance between adjacent results is always a fraction of a GSM time slot. 
      This value gives the time distance dT/us = 12 * 625 / (13 * wCountOfResultsPerGSMTimeSlot)
      */
      WORD    wCountOfResultsPerGSMTimeSlot;
         
      /**
      Each power value is averaged over a short time interval given in this value.
      */
      WORD    wRMSLengthIn40ns;

      /**
      Power value in 0.01dBm to calculate the power over time. See PowerValueBuffer.
      */
      short   sMinPowerInDBm100;

      /**
      Time of the first power values per channel. The power sequences 
      for all channels are taken in the same time interval.
      */
      double   dStartTimeInSec;

      /**
      PowerValueBuffer: For each channel dwCountOfResultsPerChannel are stored. The first power value 
      is taken at  dStartTimeInSec. Each value bValue represents a power 0.5 * bValue dB above sMinPowerInDBm100.  
      */
      DWORD   dwCountOfBytes;    //!< Number of bytes in the buffer

      byte    *pbBuffer;         //!< Pointer to a buffer
                                 //!< \@attribute[type="array" size="dwCountOfBytes" sizeismember="true"]   
   }                    *pChannelPowerResult;//!< \@attribute[filterable="true"]

   /**
   Values for GSM time slot power measurements.
   Time slot power measurements are done for clusters. In a cluster results from different time slots are used 
   to get results (e.g. the training sequence code) and to collect the results.
   The time slot measurements of the cluster are in the ListTimeSlotPowerValuesResult.
   \@attribute[type="serializable"]
   */
   struct STimeSlotPowerClusterResult
   {
      /**
      Index of the measured frequency
      */
      DWORD    dwFrequencyIndex;

      /**
      Time Slot Type
      */
      enum etTimeSlotType
      {
         TS_POWER_SLOT_TYPE_DB = 0,
         TS_POWER_SLOT_TYPE_TSC = 1,
         TS_POWER_SLOT_TYPE_INVALID = 3
      }        eTimeSlotType;

      /**
      0..7 or invalid byte (0xff)
      */
      byte    bTsc; 

      /**
      Time Of First Slot:
      Average time calculated for the first slot in this cluster. Each slot measurement may have a time shift to this time.
      */
      double   dTimeOfFirstSlotInSec;

      /**
      minimum measured time slot power
      */
      short   sMinPowerInDBm100; 

      /**
      The time slot power in 0.01dBm for the byte value bPower is: 
      sMinPowerInDBm100 + (short)(((dword)wPowerRangeInDB100 * bPower) >> 8)
      */
      WORD    wPowerRangeInDB100;

      /**
      Estimated Slot Alignment:
      For a downlink channel the slot alignment is either equidistant with a slot length 
      of 156.25 bits or a fill bit is used before each forth slot and the slot length is 156
      bits. So there are 5 possibilities for the time slot cluster: equidistant, or fill bit before 
      the first, second, third or forth time slot. The 4 mentioned slots are adjacent and start 
      at dTimeOfFirstSlotInSec and are probably not all measured. The estimation may give more then one 
      of the 5 possibilities if they have the same likelihood. E.g. if only one time slot ist measured 
      or only time slots with equal time slot number, then we have the same probability for all
      5 possibilities. For uplink channels the scanner may do an estimation but this should be ignored 
      as the mobiles have other types of slot alignment. 
      The alignments possibilities are coded as bits, see the following constants, 
      and can be used also in combination as noted above.
      */
      static const byte     bSlotAlignmentEquidistant       = 128;   //!< Used for equidistant slot alignment
      static const byte     bSlotAlignmentFillBit_0         = 1;     //!< used for slot alignment with fill bit before slot 0
      static const byte     bSlotAlignmentFillBit_1         = 2;     //!< used for slot alignment with fill bit before slot 1
      static const byte     bSlotAlignmentFillBit_2         = 4;     //!< used for slot alignment with fill bit before slot 2
      static const byte     bSlotAlignmentFillBit_3         = 8;     //!< used for slot alignment with fill bit before slot 3

      byte    bEstimatedSlotAlignment;
         
      /**
      Estimated Slot Alignment Weak: 
      Same a above but also possible slot alignments with smaller likelihood are indicated.
      Using the same bit coding as above.
      */
      byte    bEstimatedSlotAlignmentWeak;

      /**
      Specific values for time slots belonging to the cluster
      \@attribute[type="serializable"]
      */
      struct  STimeSlotPowerValuesResult
      {
         /**
         The time slot power in 0.01dBm for this byte value bPower uses values from the STimeSlotPowerClusterResult: 
         sMinPowerInDBm100 + (short)(((dword)wPowerRangeInDB100 * bPower) >> 8)
         */
         byte    bPower;

         /**
         The total power for this time slot in 0.01dBm is:  
         sMinPowerInDBm100 + (short)(((dword)wPowerRangeInDB100 * bPower) >> 8) + 10 * bOffsetToTotalPowerInDB10 
         */
         byte    bOffsetToTotalPowerInDB10;

         /**
         The max peak is shifted for this time from the average time. 
         */
         char    scTimeShiftIn100ns;

         /**
         Number of time slots this time slot is behind the previous time slot. It should be 0 for the first time slot 
         measurement. 
         */
         byte    bSlotDistanceFromPrevTimeSlot;
      };

      /**
      This list gives the times slot power values of the cluster
      */
      SViComList<STimeSlotPowerValuesResult> ListTimeSlotPowerValuesResult;
   };

   /**
   This list gives Info about time slot clusters.
   */
   SViComList<STimeSlotPowerClusterResult> ListTimeSlotPowerClusterResult;
   //!< \@attribute[filterable="true"]

};


/**
Request for the Layer 3 Decoder to decode the System Information.
\@attribute[type="serializable"]
*/
struct SL3DecoderRequest
{
   SL3DecoderRequest()
      : dwBitCount( 0 )
      , pbBitStream( NULL )
   {}

   DWORD                dwBitCount;       //!< Number of bits for this PDU.
   BYTE*                pbBitStream;      //!< Pointer to a buffer containing the PDU bits.
                                          //!< The first bit is stored in bit 7 of pbBitStream[0].
                                          //!< If dwBitCount is not a multiple of 8, there are remaining bits,
                                          //!< which shall be ignored.
                                          //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
};

/**
Result from the Layer 3 Decoder.

The string returned via this structure contains one completely decoded L3 System Information Message terminated by CR+LF. 
Data subsets (i.e. Information Elements) 
within the string are headed by "->CRLF" and terminated by "<-CRLF".

For instance, a SI Type 3 will be decoded as follows:

@code
System Information Type 3:
   Cell Identity:
      CIValue: 24023, 0x5DD7
   Location Area Identification:
      Mobile Country Code: 2 6 2
      Mobile Network Code: 0 2
      Location Area Code: 890, 0x37A
   Control Channel Description:
      MSs in the cell should apply IMSI attach and detach
      Blocks reserved for access grant: 2
      1 basic physical channel used for CCCH, not combined with SDCCHs
      6 multiframes period for transmission of PAGING REQUEST messages to the same paging subgroup
      Time-out value: 10
   Cell Options:
      PWRC is set
      The MSs shall use discontinuous transmission
      Radio-link-timeout: 24
   Cell Selection Parameters:
      Max. TX power level: 5
      6 dB RXLEV hysteresis for cell re-selection
      Min. received signal level: 4
      NECI: New establishment causes are supported
      ACS: System information type 16 and 17 are not broadcast on the BCCH
   RACH Control Parameters:
      Call Reestablishment not allowed in the cell
      The cell is not barred
      32 slots used to spread transmission
      Max. 4 retransmission(s)
      Emergency call allowed to all MSs
      Access Control Class N:
         Class 0: not barred
         Class 1: not barred
         Class 2: not barred
         Class 3: not barred
         Class 4: not barred
         Class 5: not barred
         Class 6: not barred
         Class 7: not barred
         Class 8: not barred
         Class 9: not barred
         Class 11: not barred
         Class 12: not barred
         Class 13: not barred
         Class 14: not barred
         Class 15: not barred
   SI3 Rest Octets:
      Selection Parameters: 
         Cell Bar Qualify: 0
         Cell Reselect Offset: 0 dB
         Temporary Offset: 0 dB
         Penalty Time: 20 s
      System Info 2ter Indicator: 1
      Early Classmark Sending Control: 1
      GPRS indicator: 
         RA COLOUR: 0
         SI13 POSITION: 0
      Early Classmark Sending Restriction: 0
      SI2quater Indicator: 
         SI2quater Position: BCCH Ext.
@endcode
\@attribute[type="serializable"]
*/
struct SL3DecoderResult
{
   Pdu::Type                ePDU;             //!< System Information Type contained.
   DWORD                dwStringLength;   //!< Number of decoding result text bytes in the buffer (incl. terminating '\0').
   char*                pcPduText;        //!< Pointer to a buffer containing the decoded System Information Type text, 
                                          //!< as zero terminated ASCII string.
                                          //!< \@attribute[type="array" size="dwStringLength" sizeismember="true"]
};

} // namespace GSM
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
