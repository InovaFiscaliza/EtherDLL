// ViComTdScdmaInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_TDSCDMA    8     // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)      // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                     // get a definite structure alignment for CViComTdScdmaInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace TDSCDMA
{
/**
Data containers used for ViCom TD-SCDMA interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_TDSCDMA,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit


/**
Configuration of the measurement that is performed on one single frequency.
\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   SFrequencySetting()
      : dwFrontEndSelectionMask( SRFPort::RF_1 )
      , dCenterFrequencyInHz( 0 )
      , enMeasurementMode( MeasurementMode::HIGH_SENSITIVITY )
      , dwActiveMeasurements( dwMEAS_ALL )
      , dwMeasRatePer1000Sec( 0 )
   {}

   /** 
   Receiver front end to be used by the scanner for this channel.
   Scanner tasks are not allowed to contain multiple FE's.
   */
   DWORD                      dwFrontEndSelectionMask;
      
   /**
   The center frequency on which the TD-SCDMA signals shall be measured.
   For value range see SReceiver::SSupportedFrequencies.
   */
   double                     dCenterFrequencyInHz;

   /** 
   Measurement mode. Used to switch between the standard mode which is fast but has less sensitivity or the
   high sensitivity mode which lacks speed.
   \@attribute[type="serializable"]
   */
   struct  MeasurementMode { enum Type {
                              HIGH_SENSITIVITY     = 1,        //!< High sensitivity mode. In this mode the measurements are performed
                                                               //!< over several sub-frames thus gaining sensitivity and accuracy on
                                                               //!< the cost of measurement speed.
                              HIGH_SPEED           = 2         //!< High speed mode. In this mode the measurements are performend
                                                               //!< over one sub-frame only to maximize the measurement speed.
   }; };

   MeasurementMode::Type      enMeasurementMode;


   static const DWORD         dwMEAS_SPECTRUM      =  2;       //!< Measurement of the frequency spectrum.
   static const DWORD         dwMEAS_POWER_PROFILE =  4;       //!< Measurement of the power profile.
   static const DWORD         dwMEAS_PCCPCH_CONST  =  8;       //!< Constellation diagram measurements on PCCPCH (slot 0).
   static const DWORD         dwMEAS_EXT_SLOT      = 16;       //!< Extended Analysis on the timeslots for HSDPA.
                                                               //!< Activate all supported measurements.
   static const DWORD         dwMEAS_ALL           =  dwMEAS_SPECTRUM | dwMEAS_POWER_PROFILE | dwMEAS_PCCPCH_CONST | dwMEAS_EXT_SLOT;

   /**  
   Active measurements. Used to configure if frequency spectrum of power profile measurements should be done or not.
   The measurements can be combined, i.e. to get power profile and frequency spectrum measurements 
   dwMEAS_SPECTRUM | dwMEAS_POWER_PROFILE shall be set.
   */
   DWORD                      dwActiveMeasurements;

   /**
   The desired measurement rate value. For default value and range see GetMeasRateLimits().
   The limits are dependent from the currently set enMeasurementMode.
   */
   DWORD                      dwMeasRatePer1000Sec;
};

   
/**
This structure holds the measurement setup for the TD-SCDMA measurement. It
basically consists of the set of frequencies that shall be measured.
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   SChannelSettings()
      : dwCount( 0 )
      , dwToaAvgWindowLengthMs( 0 )
      , pTableOfFrequencySetting( NULL )
   {}

   /** 
   Number of frequency configurations that shall be used from the aTableOfFrequencySetting to perform the measurement tasks.
   For value range see GetChannelCountLimits().
   */
   DWORD                      dwCount;

   /**
   Length of the sliding window for Time of Arrival averaging in ms. 
   Values in the near past of the current value contribute more to the average 
   than older values. Values older than dwToaAvgWindowLengthMs will contribute 
   significantly less to the average value.
   If > 1, this value is treated as double Sigma threshold of a Gaussian distribution
   in time, on which the averaging is based.
   If 1, results are provided without averaging.
   If 0, feature deactivated, no ToA processing and no results. This is the default!
   */
   DWORD                      dwToaAvgWindowLengthMs;

   /** 
   This array of structures can be filled with single frequency configurations.
   Only dwCount entries from this table will be used.
   \@attribute[type="array" size="dwCount" sizeismember="true"]
   */
   SFrequencySetting*         pTableOfFrequencySetting;
};

//** BCH demodulation **/

/**
This enumeration defines IDs for the TD-SCDMA Layer 3 PDUs. 
Use this in the SDemodRequest structure to specify which PDUs shall be decoded.
\@attribute[type="serializable"]
*/
struct Pdu { enum Type { 
   MIB       = 14, //!< MIB is PDU 14         Do not request, if necessary, this PDU will be acquired automatically.
   SIB1      = 15, //!< SIB 1 is PDU 15
   SIB2      = 16, //!< SIB 2 is PDU 16
   SIB3      = 17, //!< SIB 3 is PDU 17
   SIB5      = 19, //!< SIB 5 is PDU 19
   SIB7      = 21, //!< SIB 7 is PDU 21
   SIB11     = 25, //!< SIB 11 is PDU 25
   SIB12     = 26, //!< SIB 12 is PDU 26
   SIB14     = 32, //!< SIB 14 is PDU 32
   SIB16     = 39, //!< SIB 16 is PDU 39
   SIB17     = 40, //!< SIB 17 is PDU 40
   SIB18     = 41, //!< SIB 18 is PDU 41
   SB1       = 42, //!< SB 1 is PDU 42        Do not request, if necessary, this PDU will be acquired automatically.
   SB2       = 43, //!< SB 2 is PDU 43        Do not request, if necessary, this PDU will be acquired automatically.
   UNKNOWN   = SDefs::INVALID_DWORD_VALUE
};};

/**
This enumeration defines modes for the BCH demodulation.
Use this in the S_TDSCDMA_PDU_Request structure to specify when the demodulation 
of this PDU shall be done.
\@attribute[type="serializable"]
*/
struct DemodMode { enum Type {
   /**
   Demodulate the PDU for each BTS at the specified channel only once.
   Only to be used in start measurement.
   */
   ONCE             = 0,

   /**
   The PDU is only demodulated if an extra command for demodulation requests the demodulation.

   If this is used, then no PDU is decoded until it is explicitly requested. 
   The method IssueDemodRequests can be used for that purpose.
   */
   ON_CMD           = 1,

   /**
   After each successful demodulation a new one will be started automatically after the repetition time specified in wRepetitionDelayIn100ms.
   Only to be used in start measurement.
   */
   REPETITION       = 2,

   /**
   The demodulation is only for a special BTS.
   This can be requested only during a running measurement and when the PDU was also requested in the start measurement frame.
   If dwTimeOutOrRepetitionDelayIn100ms is different from 0 then the demodulation is only tried during the given time.
   */
   BTS              = 3,

   /**
   Delete all demodulation results for all BTS at the specified channel. 
   Restart demodulation for this channel as specified in the command.
   This can be requested only during a running measurement.
   */
   CHANNEL_RESET    = 6,
};};

/**
This structure is used to configure PDU requests.
It is used in start measurement requests and it is used to issue BCH demodulation requests during measurement,
e.g. SIB requests on demand or to reset SIB demodulation.
\@attribute[type="serializable"]
*/
struct SDemodRequests
{
   SDemodRequests()
      : dwCountOfRequests( 0 )
      , pDemodRequests( NULL )
   {}
         
   /** 
   Number of demodulation requests. Must be 0, if no demodulation shall be done.
   */
   DWORD                      dwCountOfRequests;

   /**
   There must be one instance of this structure for each BCH demodulation request,
   therefore different PDUs can be measured on different channels.

   Example for start measurement:
            On the first channel SIBs 3 and 4 shall be decoded, on the the second channel SIBs 3, 5 and 7 shall be decoded.
            SIB 5 shall be decoded repeatedly every 10 seconds (10000 ms) and SIB 7 shall be decoded on demand.
            This requires five structures containing the following parameter sets:
   @code
            (0, SIB3, 0, 0, 0, 0)
            (0, SIB4, 0, 0, 0, 0)
            (1, SIB3, 0, 0, 0, 0)
            (1, SIB5, REPETITION, 10000, 0, 0)
            (1, SIB7, ON_CMD, 0, 0, 0)
   @endcode
   Example during measurement:
            On channel 0 PDU demodulation shall be completely reset for the BTS identified by ID 3
            and on the the second channel SIB 7 shall be decoded for the BTS identified by ID 4 
            SC 5 with a timeout for the request of 1 sec (1000ms):
   @code
            (0, 0, CHANNEL_RESET, 0, 3, 5)
            (1, PDU_FOR_SIB7, ON_CMD, 1000, 4, 5)
   @endcode
   @note
   You are responsible to manage memory, when you use SetDemodulationSettings().
   All parameters which are not necessary for a certain request shall be set to 0.
   \@attribute[type="serializable"]
   */
   struct SDemodRequest
   {
      SDemodRequest()
         : dwChannelIndex( 0 )
         , ePDU( Pdu::MIB )
         , eDemodulationMode( DemodMode::ONCE )
         , wRepetitionTimeOutInMs( 0 )
         , dwBtsId( 0 )
         , wSC( 0 )
      {}

      /** 
      Index of the frequency in SChannelSettings::aTableOfFrequencySetting.
      */
      DWORD                   dwChannelIndex;

      /** 
      The PDU to be demodulated.
      */
      Pdu::Type               ePDU;

      /** 
      The mode used for BCH demodulation.
      */
      DemodMode::Type         eDemodulationMode;

      /** 
      Repetition time out in ms (only used if bDemodulationMode == REPETITION).
      */
      WORD                    wRepetitionTimeOutInMs;

      /** 
      Unique BTS ID provided by the scanner as SSignals::dwScannerBtsIdent. This ID is used by the 
      BCHDemodulator to demodulate a specific BTS, when the bDemodulationMode is set to ON_CMD.
      */
      DWORD                   dwBtsId;

      /** 
      Scrambling code provided by the Scanner as SSignals::wScramblingCode. This scrambling code is used
      by the BCHDemodulator to demodulate a specific BTS, when the bDemodulationMode is set to ON_CMD.
      */
      WORD                    wSC;
   };

   /** 
   Pointer to an array of demodulation requests. Must be NULL, if no demodulation shall be done.
   \@attribute[type="array" size="dwCountOfRequests" sizeismember="true"]
   */
   SDemodRequest*             pDemodRequests;
};

/**
This structure is used to configure the BCH demodulation process.
\@attribute[type="serializable"]
*/
struct SDemodulationSettings
{
   SDemodulationSettings()
      : sEcToIoThresholdDB100( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /**
   Ec/Io threshold for demodulation, given in 1/100 dBm.
   For default value and range see GetDemodThresholdLimits().
   */
   short                sEcToIoThresholdDB100;

   /**
   Receiver front end to be used by the demodulator for this channel. 
   If no demodulation shall be provided, this shall be set to 0.
   Demodulation tasks are not allowed to contain multiple FE's.
   */
   DWORD                dwFrontEndSelectionMask;

   /**
   The requests for the general start measurement command.
   */
   SDemodRequests       sStartMeasurementRequests;
};


//** Request Settings **/
/**
This structure holds all the measurement setup that is configured in the ViCom TD-SCDMA scanner module. 
It is a result of the CViComTdScdmaInterface::GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   DWORD                      dwReceiverIndex;           //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth         ResultBufferDepth;         //!< Number of results allowed in the result buffer
   SChannelSettings           ChannelSettings;           //!< The set of frequencies that shall be measured
   SDemodulationSettings      DemodulationSettings;      //!< Configuration of the BCH demodulation. Not supported yet!
};


/**
The TD-SCDMA measurement result for a single channel returned by CViComTdScdmaInterface::GetResult().
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /** 
   The channel to which all the data relates is specified using this index.
   The index refers to the position in the SChannelSettings::aTableOfFrequencySetting array.
   */
   DWORD       dwChannelIndex;

   /** 
   PC local time when the signal was measured (or was started to being measured) on the device.
   */
   DWORD       dwPcTimeStampInMs;

   /** 
   The time when the signal was started to being measured on the device, with a higher resolution than the default PC clock. 
   This time is relative to the last restart of the device.
   */
   ULONGLONG   u64DeviceTimeInNs;

   /**
   List of measurement rates rates and other values in relation to the measurement rate for 
   one or more time intervals of measurement rate calculation. The measurement rate is 
   the ratio of all measurements of the same type of measurement at the time
   the measurement signals have been picked from the air. So the time of arrival 
   at the host application is not used. And equal measurements at different RF channels are put together.      
   */
   SViComList<SMeasurementRate> ListMeasurementRates;

   /**
   Actual measurement values for a single server.
   \@attribute[type="serializable"]
   */
   struct SSignals
   {
      /** 
      Internal ID assigned from the scanner to that specific station. Different
      stations will become different IDs assigned to them, but if the scanner
      cannot identify a cell exactly, it also might assign different IDs to
      the same station.

      @note
      This is unique only within one measurement. Do not use it when
      combining the results of different measurements
      */
      DWORD                      dwScannerBtsIdent;

      /** 
      The scrambling code.
      */
      WORD                       wScramblingCode;

      /** 
      Time offset of time slot 0 in ns with respect to the device time (SMeasResult::u64DeviceTimeInNs).
      */
      DWORD                      dwTS0TimeOffsetInNs;

      /** 
      Time offset of DwPTS in ns with respect to the device time (SMeasResult::u64DeviceTimeInNs),
      if pdwDwPtsTimeOffsetInNs is not NULL.
      This time is the Time of Arrival of the DwPTS radio signal, i.e. the start of the sync chips, 
      which follow Time Slot 0 (TS0) after a 32 chip guard period. 
      This means, the difference between this value and ToA of TS0 (dwTS0TimeOffsetInNs) is 700 microseconds.
      */
      DWORD*                     pdwDwPtsTimeOffsetInNs;

      /**
      Description of extented information for one time slot.
      \@attribute[type="serializable"]
      */
      struct SSlotDescription
      {
         /**
         Definition of the slot format.
         \@attribute[type="serializable"]
         */
         struct Slot{ enum Format{
            SLOT_QPSK_SF1     = 1,  //!< Modulation QPSK SF1
            SLOT_QPSK_SF16    = 2,  //!< Modulation QPSK SF16
            SLOT_16QAM_SF1    = 4,  //!< Modulation 16-QAM SF1
            SLOT_16QAM_SF16   = 8,  //!< Modulation 16-QAM SF16
            SLOT_64QAM_SF1    = 16, //!< Modulation 64-QAM SF1
            SLOT_64QAM_SF16   = 32, //!< Modulation 64-QAM SF16
            SLOT_PCCPCH       = 64  //!< PCCPCH information on slot 0
         };};

         /**
         Format of the slot.
         */
         Slot::Format         enSlotFormat;

         /** 
         Modulation error ratio in dB
         */
         float               	fMER_dB;

         /**
         Measurement data for the constellation diagram.
         \@attribute[type="serializable"]
         */
         struct SConstellationData
         {
            /**
            Number of values available in pSComplexDataPoints array, if pSComplexDataPoints is not NULL.
            */
            DWORD             dwCountOfComplexDataPoints;

            /**
            Constellation data points.
            \@attribute[type="array" size="dwCountOfComplexDataPoints" sizeismember="true"]
            */
            const SComplex*   pSComplexDataPoints;
         }* pConstellationData;
         //!<\@attribute[filterable="true"]
      };


      /**
      Time slot measurement results.
      \@attribute[type="serializable"]
      */
      struct STimeSlot
      {
         /**
         Definition of the time slot type.
         \@attribute[type="serializable"]
         */
         struct TimeSlot{ enum Type{ 
            TS0    = 0,
            TS1    = 1,
            TS2    = 2,
            TS3    = 3,
            TS4    = 4,
            TS5    = 5,
            TS6    = 6,
            DwPTS  = 7,
            UpPTS  = 8,
            PCCPCH = 9
         };};

         /**
         Type of the time slot.
         */
         TimeSlot::Type             enTimeSlot;

         /** 
         RSCP in 0.01 dBm if not SDefs::INVALID_SHORT_VALUE.
         */
         short                      sRSCPinDBm100;

         /** 
         ISCP in 0.01 dBm if not SDefs::INVALID_SHORT_VALUE.
         */
         short                      sISCPinDBm100;

         /** 
         C/I in 0.01 dB if not SDefs::INVALID_SHORT_VALUE.
         */
         short                      sCIinDB100;

         /** 
         RSSI in 0.01 dBm if not SDefs::INVALID_SHORT_VALUE.
         The RSSI measurement is performed over the entire slot for TSx, DwPTS, UpPTS and over the PCCPCH segments for PCCPCH.
         */
         short                      sRSSIinDBm100;

         /** 
         Ec/I0 in 0.01 dB if not SDefs::INVALID_SHORT_VALUE.
         */
         short                      sEcI0inDB100;

         /**
         Description of extented information for the time slot, if not NULL.
         */
         SSlotDescription*          pSlotDescription;
      };
      
      /**
      The actual set of time slot measurements.
      */
      SViComList<STimeSlot>    ListOfTimeSlots;


      
      /**
      Optional data container that holds information about the channel impulse
      responses measured in this cycle. The CIR consists of a list of peaks. 
      Additionally there is a power profile.
      \@attribute[type="serializable"]
      */
      struct SCir
      {

         /** 
         Internal ID assigned from the scanner to that specific station. Different
         stations will become different IDs assigned to them, but if the scanner
         cannot identify a cell exactly, it also might assign different IDs to
         the same station.

         @note
         This is unique only within one measurement. Do not use it when
         combining the results of different measurements
         */
         DWORD                      dwScannerBtsIdent;

         /** 
         The scrambling code.
         */
         WORD                       wScramblingCode;

         /**
         Contains information about the single peaks in the Channel Impulse Response.
         \@attribute[type="serializable"]
         */
         struct SPeak
         {
            /**
            Peak type
            */
            enum etPeakType
            {
               /**
               Peak from Midamble.
               */
               PEAK_MIDAMBLE  = 1,

               /**
               Peak from PCCPCH.
               */
               PEAK_PCCPCH    = 2,

               /**
               Peak from DwPTS.
               */
               PEAK_DWPTS     = 4

            }                    ePeakType;

            /**
            The actual power of the impulse peak in 0.01 dBm.
            */
            short                sPowerInDBm100;

            /**
            Delay of the peak related to the device time in ns.
            */
            DWORD                dwDelayInNs;
         };

         /**
         List of CIR peaks found in the signal.
         */
         SViComList<SPeak> ListOfPeaks;

         /**
         Parameters of the maximum peak.

         This result will only be provided, if the peak list is not empty, PPS has been acquired and 
         the Time of Arrival feature has been enabled (i.e. 0 < SSettings::dwToaAvgWindowLengthMs).
         In addition, it will take about 100 s from start of measurement, before the first results are provided.

         @note
         This feature requires a GPS antenna to be connected to the receiver.

         \@attribute[type="serializable"]
         */
         struct SMaxPeak
         {
            /**
            Pointer to the maximum element of the CIR Peak list. 
            This value is not optional, it is a pointer to avoid copying data.
            */
            SPeak*      pMaxPeakInfo;

            /**
            Time difference between Radio Frame Time of Arrival and the latest PPS pulse received.
            The time, when the maximum CIR peak has been received, is used as Radio Frame Time of Arrival.

            According to 3GPP TS25.221 Chapters 5, 5A and 5B Frame structure,
            the TD-SCDMA radio frame (10 ms) consists of two 5 ms subframes for each complete radio frame.
            Each Subframe can be seen as a radio frame, therefore we must use the half radio frame 
            length (5 ms) for the ToA (Time of Arrival) offset to PPS calculation.

            The time difference between DwPTS and the latest PPS pulse received can be calculated by
            double dDwPTS_OffsetToPpsInSec = dFrameToaOffsetToPpsInSec + 0.000675 without the DwPTS Guard Period.
            If you include the duration of the DwPTS Guard period, you must calculate
            double dDwPTS_OffsetToPpsInSec = dFrameToaOffsetToPpsInSec + 0.000700,
            see explanation of pdwDwPtsTimeOffsetInNs.

            To determine the frame delay as value within +/- half of the length considered (+/- 5 ms / 2  =>  2.5 ms),
            the provided dFrameToaOffsetToPpsInSec must be processed according to he following principle:
            double dFrameDelay_ms = ( (1000*dFrameToaOffsetToPpsInSec  + 5 ms) modulo 10 ms ) - 5 ms

            @note
            This feature requires a GPS antenna to be connected to the receiver.
            */
            double      dFrameToaOffsetToPpsInSec;

            /**
            Virtual Radio Frame Number (RFN) assigned to each radio frame received.
            The RFN counting is started for each new combination of Channel and ExtendedSC,
            i.e. dwChannelIndex, SExtendedSC::wSC, SExtendedSC::wIndicator.
            See explanation for SExtendedSC.
            */
            DWORD       dwRadioFrameNumber;

         }* pMaxPeak;

      }* pCir;
      //!<\@attribute[filterable="true"]
   };

   /**
   The actual set of signals associated with the current channel specified
   by the dwChannelIndex member.
   */
   SViComList<SSignals>    ListOfSignals;

   /**
   Optional spectrum measurement results. Only available if spectrum measurements
   have been configured using SChannelSettings::dwMEAS_SPECTRUM.
   \@attribute[type="serializable"]
   */
   struct SSpectrumResult
   {
      /**
      Frequency spacing of spectrum values available in psRmsSpectralPowerValueInDBm100 array.
      */
      DWORD                   dwSpectrumFreqDistanceInHz;

      /**
      First frequency of spectrum values available in psRmsSpectralPowerValueInDBm100 array.
      */
      DWORD                   dwFirstSpectrumFreqInHz;

      /**
      Number of values available in psRmsSpectralPowerValueInDBm100 array if psRmsSpectralPowerValueInDBm100 is not NULL.
      */
      DWORD                   dwCountOfSpectrumValues;

      /**
      RMS spectrum value in 0.01 dBm.
      \@attribute[type="array" size="dwCountOfSpectrumValues" sizeismember="true"]
      */
      const short*            psRmsSpectralPowerValueInDBm100;

   } *pSpectrum;      
   //!<\@attribute[filterable="true"]

   /**
   Optional power profile information. Only available if power profile
   measurements have been configured using SChannelSettings::dwMEAS_POWER_PROFILE.
   The power profile contains the received signal power (RSSI) over time curve.
   \@attribute[type="serializable"]
   */
   struct SPowerProfile
   {
      /**
      Time resolution in ns.
      */
      DWORD                   dwTimeResolutionInNs;

      /**
      Number of values available in psPowerValueInDBm100 array if psPowerValueInDBm100 is not NULL.
      */
      DWORD                   dwCountPowerValues;

      /**
      RMS power value in 0.01 dBm.
      \@attribute[type="array" size="dwCountPowerValues" sizeismember="true"]
      */
      const short*            psPowerValueInDBm100;

   } *pPowerProfile;
   //!<\@attribute[filterable="true"]

   /** 
   BCH demodulator results
   \@attribute[type="serializable"]
   */
   struct SDemodResult
   {
      /** 
      Unique BTS ID provided by the scanner as SSignals::dwScannerBtsIdent. Each time the scanner is not
      sure whether the signal it could demodulate belongs to an already known eNodeB, it sets a new identifier
      here. I.e. the same eNodeB could have multiple dwBtsIds, but each dwBtsId relates always to a single cell.
      */
      DWORD          dwBtsId;

      /** 
      Scrambling code provided by the scanner as SSignals::wScramblingCode.
      */
      WORD           wSC;

      /** 
      The demodulator fills this parameter with the BTS ID the scanner assigned to this BTS first.
      If later on, the scanner uses a different BTS ID, but after demodulation it is detected that it is the same eNodeB,
      the demodulator fills this value with the first BTS ID the scanner used.  
      Example: 
      The scanner has measured a eNodeB with ID = 10 and the demodulator has demodulated it successfully. 
      20 minutes later the scanner found signals of a eNodeB with ID = 55. After demodulating, the demodulator finds 
      out (by comparing unique BTS information from decoding), that it is the same eNodeB as the one with ID = 10. 
      In this case, dwFirstBtsId will be set to 10.
      */
      DWORD          dwFirstBtsId;

      /** 
      This is the first time in ms of signals used for this message.
      */
      DWORD          dwStartTimeInMs; 

      /** 
      This is the last time in ms of signals used for this message. 
      All IQ signals used to produce this demodulation result are in between dwStartTimeInMs and dwStopTimeInMs.
      */
      DWORD          dwStopTimeInMs;

      /** 
      PDU that has been demodulated
      */
      Pdu::Type      ePDU;

      /** 
      Number of bits the PDU consists of.
      */
      DWORD          dwBitCount;

      /** 
      Pointer to (dwBitCount + 7) / 8 bytes which contain the PDU bits. The first bit is stored in the bit 7 of pbBitStream[0]. 
      If dwBitCount is not a multiple of 8, the trailing bits in the last byte pbBitStream[(dwBitCount + 7) / 8 - 1] shall be ignored.
      \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
      */
      const BYTE     *pbBitStream;
   };

   /** 
   Pointer to demodulation result. If NULL, no demodulation result is present.
   */
   const SDemodResult  *pDemodResult;
};

/**
Request data container for the Layer 3 Decoder. Is used to decode a demodulation result into
a human-readable structure.
\@attribute[type="serializable"]
*/
struct SL3DecoderRequest
{
   SL3DecoderRequest()
      : ePdu( (Pdu::Type) 0 )
      , dwBitCount( 0 )
      , pbBitStream( NULL )
   {}

   Pdu::Type            ePdu;             //!< PDU contained.
   DWORD                dwBitCount;       //!< Number of bits for this PDU.
   BYTE*                pbBitStream;      //!< Pointer to a buffer containing the PDU bits.
                                          //!< The first bit is stored in bit 7 of pbBitStream[0].
                                          //!< If dwBitCount is not a multiple of 8, there are remaining bits,
                                          //!< which shall be ignored.
                                          //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
};

/**
Result from the Layer 3 Decoder.

The string returned via this structure is formatted using the ASN.1 notation of the standard (3GPP TS 36.331).
It contains one completely decoded PDU terminated by CR+LF. Data subsets (i.e. Information Elements)
within the string are headed by "CR+LF+{", indented via spaces and terminated by "}+CR+LF".

In addition to the standard, some Information Elements are added twice (see example cellIdentity below):
   1. Formatted exactly according to the standard.
   2. Formatted with unit or format description between square brackets ("[" "]") and representing a reasonable value.

\@attribute[type="serializable"]
*/
struct SL3DecoderResult
{
   Pdu::Type            ePdu;             //!< PDU contained.
   DWORD                dwStringLength;   //!< Number of decoding result text bytes in the buffer (incl. terminating '\0').
   const char*          pcPduText;        //!< Pointer to a buffer containing the decoded PDU text, as zero terminated ASCII string.
                                          //!< \@attribute[type="array" size="dwStringLength" sizeismember="true"]
};

} // namespace TDSCDMA
} // namespace ViCom
} // namespace RohdeSchwarz


#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
