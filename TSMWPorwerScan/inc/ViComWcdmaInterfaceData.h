// ViComWcdmaInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_WCDMA          16   // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)         // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                        // get a definite structure alignment for CViComWcdmaInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace WCDMA
{
/**
Data containers used for ViCom WCDMA interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_WCDMA,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

//** Frequency settings **/
/**
This defines the frequency in Hz.
\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   /** Center frequency of the channel */
   double               dCenterFrequencyInHz;
};


/**
This defines the different measurement modi. For the TSML device, only the HIGH_SPEED mode can be set.
\@attribute[type="serializable"]
*/
struct MeasurementMode { enum Type{ 
   HIGH_SPEED   = 0,    //!< supported by TSMQ, TSMU, TSML, TSMW, TSME
   HIGH_DYNAMIC = 1,    //!< supported by TSMQ, TSMU, TSMW, TSME
};};

/**
SChannelSettings contains the count of channels or frequencies and the table of frequencies in MHz.

The selection of new frequencies (with SetFrequencyTable) automatically resets the BCH demodulator,
so that it demodulates the default SIBs.
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   SChannelSettings()
      : dwFrontEndSelectionMask( SRFPort::RF_1 )
      , dwCount( 0 )
      , eMeasurementMode( MeasurementMode::HIGH_SPEED )
      , dwToaAvgWindowLengthMs( 0 )
      , dwMeasRatePer1000Sec( 0 )
      , pTableOfFrequencySetting( NULL )
   {}
       
   /** 
   Receiver front end to be used by the scanner for this channel.
   Scanner tasks are not allowed to contain multiple FE's.
   */
   DWORD                   dwFrontEndSelectionMask;

   /**
   Measurement mode settings.
   */
   MeasurementMode::Type   eMeasurementMode;
      
   /**
   The desired measurement rate value. For default value and range see GetMeasRateLimits().
   The limits are dependent from the currently set eMeasurementMode.
   */
   DWORD                   dwMeasRatePer1000Sec;

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
   DWORD                   dwToaAvgWindowLengthMs;

   /**
   Number of frequency configurations that shall be used from the pTableOfFrequencySetting to perform the measurement tasks.
   For value range see GetChannelCountLimits().
   **/
   DWORD                   dwCount;

   /**
   Pointer to an array containing the frequencies of the selected channels.
   The number of members in the array is defined by dwCount (above).
   For value range see SReceiver::SSupportedFrequencies.
   \@attribute[type="array" size="dwCount" sizeismember="true"]
   */
   SFrequencySetting*      pTableOfFrequencySetting;
};

//** BCH demodulation **/

/**
This enumeration defines a translation table from SIB to PDU. Use this in the
SDemodRequest structure to define what SIB shall be decoded.
\@attribute[type="serializable"]
*/
struct Pdu { enum Type {
   MIB        = 14, //!< MIB is PDU 14         Do not request, if necessary, this PDU will be acquired automatically.
   SIB1       = 15, //!< SIB 1 is PDU 15
   SIB2       = 16, //!< SIB 2 is PDU 16
   SIB3       = 17, //!< SIB 3 is PDU 17
   SIB5       = 19, //!< SIB 5 is PDU 19
   SIB5bis	  = 53, //!< SIB 5bis is PDU 53
   SIB7       = 21, //!< SIB 7 is PDU 21
   SIB11      = 25, //!< SIB 11 is PDU 25
   SIB11bis	  = 61, //!< SIB 11bis is PDU 61
   SIB12      = 26, //!< SIB 12 is PDU 26
   SIB13      = 27, //!< SIB 13 is PDU 27
   SIB13_1    = 28, //!< SIB 13.1 is PDU 28
   SIB13_2    = 29, //!< SIB 13.2 is PDU 29
   SIB13_3    = 30, //!< SIB 13.3 is PDU 30
   SIB13_4    = 31, //!< SIB 13.4 is PDU 31
   SIB15      = 33, //!< SIB 15 is PDU 33
   SIB15_bis  = 67, //!< SIB 15bis is PDU 67
   SIB15_1    = 34, //!< SIB 15.1 is PDU 34
   SIB15_1bis = 68, //!< SIB 15.1bis is PDU 68
   SIB15_2    = 35, //!< SIB 15.2 is PDU 35
   SIB15_2bis = 69, //!< SIB 15.2bis is PDU 69
   SIB15_2ter = 70, //!< SIB 15.2ter is PDU 70
   SIB15_3    = 36, //!< SIB 15.3 is PDU 36
   SIB15_3bis = 71, //!< SIB 15.3bis is PDU 71
   SIB15_4    = 37, //!< SIB 15.4 is PDU 37
   SIB15_5    = 38, //!< SIB 15.5 is PDU 38
   SIB15_6    = 72, //!< SIB 15.6 is PDU 72
   SIB15_7    = 73, //!< SIB 15.7 is PDU 73
   SIB15_8    = 74, //!< SIB 15.8 is PDU 74
   SIB16      = 39, //!< SIB 16 is PDU 39
   SIB18      = 41, //!< SIB 18 is PDU 41
   SIB19      = 75, //!< SIB 19 is PDU 75
   SIB20      = 76, //!< SIB 20 is PDU 76
   SB1        = 42, //!< SB 1 is PDU 42        Do not request, if necessary, this PDU will be acquired automatically.
   SB2        = 43, //!< SB 2 is PDU 43        Do not request, if necessary, this PDU will be acquired automatically.
   UNKNOWN    = SDefs::INVALID_DWORD_VALUE
};};

/**
This enumeration defines modes for the demodulation.
Use this in the SDemodRequest structure to specify when the demodulation 
of this PDU shall be done.
\@attribute[type="serializable"]
*/
struct DemodMode{ enum Type {
   /**
   The PDU is demodulated only once for each BTS.
   Only to be used in SDemodulationSettings::sStartMeasurementRequests.
   */
   ONCE             = 0,

   /**
   The PDU is only demodulated if an extra command requests the demodulation.
   Only to be used in SDemodulationSettings::sStartMeasurementRequests.

   If this is used, then no PDU is decoded until it is explicitly requested. The method
   IssueDemodRequests can be used for that purpose.

   PDUs configured with this setting can be decoded using one of BTS, BTS_FORCE,
   BTS_OLD, DEMOD_BTS_OLD_HIGH modes.
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
This structure is used to configure PDU requests.
It is used in start measurement requests and it is used to issue demodulation requests during measurement,
e.g. SIB requests on demand or to reset SIB demodulation.
\@attribute[type="serializable"]
*/
struct SDemodRequests
{
   SDemodRequests()
      : dwCountOfRequests( 0 )
      , pDemodRequests( NULL )
   {}

   DWORD                dwCountOfRequests;  //!< Number of demodulation requests.
   /**
   There must be one instance of this structure for each demodulation request,
   therefore different PDUs can be measured on different channels.

   Example for start measurement:
            On the first channel SIBs 3 and 4 shall be decoded, on the the second channel SIBs 3, 5 and 7 shall be decoded.
            SIB 5 shall be decoded repeatedly every 10 seconds (100*100 ms) and SIB 7 shall be decoded on demand.
            This requires five structures containing the following parameter sets:
   @code
            (0, SIB3, 0, 0, 0, 0)
            (0, SIB4, 0, 0, 0, 0)
            (1, SIB3, 0, 0, 0, 0)
            (1, SIB5, REPETITION, 100, 0, 0)
            (1, SIB7, ON_CMD, 0, 0, 0)
   @endcode
   Example during measurement:
            On channel 0 PDU demodulation shall be completely reset for the node b identified by SC 375 SC ID 0
            and on the the second channel SIB 7 shall be decoded for the node b identified by SC 429 SC ID 0 with
            a timeout for the request of 1 sec (10*100ms):
   @code
            (0, 0, BTS_RESET, 0, 375, 0)
            (1, SIB7, BTS, 10, 429, 0)
   @endcode
   Note that you are responsible to manage the memory when you use SetDemodulationSettings().
   Note: All parameters which are not necessary for a certain request shall be set to 0.

   If any PDU besides MIB, SB1 and SB2 shall be decoded, SIB3 needs to be requested as well.
   \@attribute[type="serializable"]
   */
   struct SDemodRequest
   {
      SDemodRequest()
         : dwChannelIndex( 0 )
         , ePDU( Pdu::SIB3 )
         , eDemodulationMode( DemodMode::ONCE )
         , wRepetitionDelayIn100ms( 0 )
         , wPrimarySCFrom0To511( 0 )
         , wScId( 0 )
      {}

      DWORD                dwChannelIndex;            //!< Index of the frequency in SChannelSettings::pdTableOfFrequencyInMHz.
      Pdu::Type            ePDU;                      //!< The PDU to be demodulated.
      DemodMode::Type      eDemodulationMode;         //!< The mode used for demodulation.
      WORD                 wRepetitionDelayIn100ms;   //!< Repeat measurement after this delay time, if eDemodulationMode is REPETITION. Timeout for request, if eDemodulationMode is ON_CMD.
      WORD                 wPrimarySCFrom0To511;      //!< Used to specify node b for DemodulationModes BTS_FORCE or BTS.
      WORD                 wScId;                     //!< Used to specify node b for DemodulationModes BTS_FORCE or BTS.
   };
     
   /**
   @brief Array of SDemodRequest, counter is dwCountOfRequests above
   \@attribute[type="array" size="dwCountOfRequests" sizeismember="true"]
   */
   SDemodRequest *pDemodRequests;
};

/**
This structure is used to configure the BCH demodulation process.
\@attribute[type="serializable"]
*/
struct SDemodulationSettings
{
   SDemodulationSettings()
      : lEcToIoThresholdInDB100( 0 )
      , dwMaxNodeBHoldTimeInSec( dwOptMaxNodeBHoldTimeInSec )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /**
   Threshold for demodulation trials in 0.01dB.
   For default value and range see GetDemodThresholdLimits().
   */
   long                 lEcToIoThresholdInDB100;
      
   static const DWORD   dwMinMaxNodeBHoldTimeInSec = 15;    //!< lower limit for dwMaxNodeBHoldTimeInSec
   static const DWORD   dwOptMaxNodeBHoldTimeInSec = 300;   //!< suggested value for dwMaxNodeBHoldTimeInSec

   /** 
   Time limit the scanner remembers an already measured NodeB (channel, SC, SCId).
   If a NodeB defined by channel, SC and SCId was not received for the here specified time,
   the combination channel, SC and SCId will not be used anymore.
   This causes a restart of all demodulation requests for this NodeB.
   */
   DWORD                dwMaxNodeBHoldTimeInSec;

   /**
   Receiver front end to be used by the demodulator for this channel. 
   If no demodulation shall be provided, this shall be set to 0.
   Demodulation tasks are not allowed to contain multiple FE's.
   */
   DWORD                dwFrontEndSelectionMask;

   SDemodRequests       sStartMeasurementRequests;    //!< The requests for the general start measurement command.
};

//** Request Settings **/
/**
By means of the functions: SetFrequencyTable(), SetMeasurementMode() and SetDemodulationSettings() internal
settings of the PN-Scanner may be changed and with the functions SelectReceiver(), SetResultBufferDepth(),
SetTimebaseSynchronisationMode() of CViComBasicInterfaceData change basic settings.
Except of frequency settings all other settings have default values.

This structure bundles these settings and will be returned as result from the GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   // basic settings
   DWORD                                  dwReceiverIndex;              //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   TimeBaseSynchronisationMode::Type      eTimeBaseSynchronisationMode; //!< Specifies with what device the internal clock shall be synchronized
   SResultBufferDepth                     ResultBufferDepth;            //!< Number of results allowed in the result buffer

   // measurement specific settings
   SChannelSettings                       ChannelSettings;              //!< Defines the measured frequencies.
   SDemodulationSettings                  DemodulationSettings;         //!< Configuration of the BCH demodulation
};

//** Measurement results **/
/**
This structure holds a measurement result from the result buffer. Please refer to the manual for a detailed discussion of
how to interpret the results.
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   //* PN-Scanner result specific structures:

   /**
   Scrambling codes (SC) are measured and used to identify the Node B which is using the SC. If C-PICH signals
   generated with the same SC have been received then the PN-Scanner normally relates this signal to each other
   with the assumption that this signals have been transmitted from the same Node B. But in the case of different
   frame timing (outside a range for reflections) or if the measurements have been acquired at different times
   (with more than a few minutes delay), the assumption that both signals have been transmitted from the same
   Node B is wrong or may be uncertain.
   To indicate whether received signals with the same SC at the same RF channel are reliably related
   (transmitted from the same Node B) the PN-Scanner attaches an indicator to each measured SC.
   If this indicator is different for the same SC, the signals may be transmitted from the same Node B,
   but this is not certain. This uncertainty can be resolved by Cell Identity measurements (SIB3 demodulation).
   \@attribute[type="serializable"]
   */
   struct SExtendedSC
   {
      WORD           wSC;           //!< Scrambling code, one of: 0, 16, ..., 16 * 511
      WORD           wIndicator;    //!< 0,  1, ... see explanation above
   };

   //* Added TSMx specifics:

   /**
   Attenuation introduced in the measurement.
   */
   STSMxAttenuation
                     *pTSMxAttenuation;

   /**
   Time synchronisation used to derive results.
   */
   STimeBaseSynchronisation
                     *pTimeBaseSynchronisation;

   //* PN-Scanner results common to all measurements:

   /**
   Channel index to which the measurement belongs.

   Each result is for only one of the selected RF channels. The channel index counts from 0 and corresponds to the
   array index in SChannelSettings::pdTableOfFrequencyInMHz.
   */
   DWORD             dwChannelIndex;

   /**
   dwPcTimeStampInMs provides a conversion of the R&S TSM device time to PC time obtained with the 
   GetTickCount() function. The adjustment of the time drift between R&S TSM device time and PC time 
   as well as the resolution of GetTickCount() can cause a conversion error of a few milliseconds.

   dwPcTimeStampInMs may be used to synchronise PN-Scanner results with other measurement data in the system 
   or to determine the age of the PN-Scanner result = GetTickCount() - dwPcTimeStampInMs;

   In general, this is the start time of the CIR measurement.
   In case of a PDU result, this is the time, when the start of the first radio frame of the 
   most recently received segment arrived at the receiver (R&S TSM device).

   @Note: PDU results and CIR results are always part of separate measurement results.
   */
   DWORD             dwPcTimeStampInMs;

   /**
   This value provides a R&S TSM device time reported in ns.

   In general, this is the start time of the CIR measurement.
   In case of a PDU result, this is the time, when the start of the first radio frame of the 
   most recently received segment arrived at the receiver (R&S TSM device).

   Please see SFN_Info:dwSystemFrameNumber, in case of a PDU result.      
   */
   ULONGLONG         u64DeviceTimeInNs;

   /** BCH demodulator results
   \@attribute[type="serializable"]
    
   If the Time of Arrival feature is enabled and available (i.e. 0 < SSettings::dwToaAvgWindowLengthMs),
   the virtual Radio Frame Number will be added to the PDU result.

   The relation between the SFN and the RFN is given by the following equations:

   The offset between virtual RFN and real SFN can be calculated by

      Delta = ( 4096 + SFN - (RFN mod 4096) )

   You can add or subtract integral multiples of 4096, because the complete set of System Information Messages is
   repeated after 4096 radio frames. Due to this, any of the possible Delta values provides a correct relation between 
   SFN and RFN, but the time of the repetition is arbitrary. If you want to find the correct relation in time, you must
   calculate the absolute Time of arrival (ToA) and add or subtract integral multiples of 40.96 s, 
   till you get the same ToA calculated from RFN as was reported for the PDU.

   This value is constant for all PDUs provided by the same base station sector (cell).

   To verify the relation for any PDU result you can calculate

      SFN   = (RFN + Delta) mod 4096

   @Note: If a SIB is assembled from more than one segment, the PC time interval of segment collection is given. 
   */
   struct SDemodResult
   {
      SExtendedSC    ExtendedSC;             //!< Scrambling code and indicator of the channel
      Pdu::Type      ePDU;                   //!< PDU that has been measured
      DWORD          dwBitCount;             //!< count of bits the PDU consists of
      BYTE           *pbBitStream;           //!< pointer to (dwBitCount + 7) / 8 bytes which contain the PDU bits. The first bit is stored in the
                                             //!< bit 7 of pbBitStream[0]. If dwBitCount is not a multiple of 8 than some tail bits in the last byte
                                             //!< pbBitStream[(dwBitCount + 7) / 8 - 1] should be ignored.
                                             //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
      /**
      Optional information about radio frame numbers and times.
      Each element is only valid, if its value is not SDefs::INVALID_DWORD_VALUE.

      @Note: 
      If you want to use scheduling information from MIB or Scheduling Blocks (SB1, SB2) together with dwSystemFrameNumber,
      you must take into account, that the SFN start value of the scheduling information is related to the first segment, 
      whereas SFN and RFN reported here are related to the most recently received segment (i.e. the last segment) of the PDU.
      */
      struct SSFN_Info
      {
         DWORD          dwSystemFrameNumber;    //!< According to the 3GPP standard, the WCDMA System frame number (SFN) is provided modulo 4096.
                                                //!< This is the SFN of the most recently measured BCCH BCH segment, which contributed to the PDU.
                                                //!< The device time, when the start of the first radio frame of the most recently received segment
                                                //!< arrived at the receiver, is stored in u64DeviceTimeInNs.
         DWORD          dwRadioFrameNumber;     //!< (Virtual) Radio Frame Number (RFN) assigned to the first radio frame of the most recently received segment, 
                                                //!< i.e. this RFN indicates the same radio frame as the SFN (dwSystemFrameNumber) from a source identified by 
                                                //!< dwChannelIndex and ExtendedSC. See explanation for SMaxPeak::dwRadioFrameNumber.
         DWORD          dwTimeOfFirstSegInMs;   //!< This is the PC time, when the first radio frame of the first segment has been received.
         DWORD          dwTimeOfLastSegInMs;    //!< This is the PC time of the first radio frame of the most recently received segment.
                                                //!< It corresponds to the device time (u64DeviceTimeInNs) and contains the same value as dwPcTimeStampInMs.
      }              SFN_Info;

   }                 *pDemodResult;          //!< If a PDU was measured then the pointer to the PDU result for this PDU measurement is valid (!= NULL)


   //* Channel impulse response (CIR) related measurement results.

   /**
   The CIR is given as a discrete power delay spectrum. The resolution (time distance of adjacent power values) is 10ms/2^16.
   This value is given as an integer constant in femto seconds (10^-15 sec). It is 38400/2^16 = 0.5859375 times the
   chip distance and about 0.15 micro seconds.
   */
   static const DWORD  dwCirResolutionInFemtoSeconds = 152587891; //(DWORD)(0.01e15 / 0x10000 + .5);

   /**
   Sometimes it is more convenient to operate with the reciprocal of the CIR resolution. This is the CIR sampling rate = 2^16/10ms.
   */
   static const DWORD  dwCirSamplingRateInHz = (100 * 0x10000);

   /**
   The power values of the CIR are given in 0.01dB resolution as 16 bit signed integer values in the range of (-327.67..327.67dBm).
   Some power values of a CIR may be below the noise limit or have not been measured. In this case the integer gets the invalid power value
   0x8000 = -32768 = -2^15 = -327.68dBm.
   */
   static const short  sInvalidPowerInDBm100 = - 32768;

   /**
   CIR for the P-Sync sequence. This CIR is not specific for a special C-PICH but for the P-SCH channel which is common to all node b.

   If a CIR measurement had no success (e.g. there is no WCDMA signal on air) then we get at least the received power of the
   specified channel.
   The pointer to this power value is valid (!= NULL) if a power measurement was performed.
   */
   short             *psAverageInbandPowerInDBm100;

   /**
   These are the actual CIR measurements.
   \@attribute[type="serializable"]
   */
   struct SPSyncCir
   {
      /**
      Maximum number of CIR  values in measurement
      */
      static const DWORD dwFixCountOfCirValues = 0x10000 / 15;

      /**
      This array contains the measured power values
      */
      short          sCodePowerInDBm100[dwFixCountOfCirValues];
   }                 *pPSyncCir;
   //!<\@attribute[filterable="true"]

   /**
   For most code power measurements and for P-CPICH CIR measurements not only the absolute code power value (in 0.01dBm) is
   reported but in addition the absolute total inband power value (InbandPower) belonging to the code power measurement is reported.
   This makes it possible to calculate the Ec/Io for the measured code power by subtracting the InbandPower which belongs to the
   code power.
   \@attribute[type="serializable"]
   */
   struct SCPichCir
   {
      SExtendedSC    ExtendedSC;               //!< The identifier for this measurement.

      /**
      If the BCH demodulator has already demodulated signal of the node b we assume for the ExtendedSC then we get a valid
      pointer to this structure. If no demodulation result is available then the pointer to this structure is NULL.
      \@attribute[type="serializable"]
      */
      struct SBchCellIdentification
      {
         DWORD       dwCI;                     //!< cell identity
                                               //!< Only available if SIB3 demodulation is active.
         WORD        wMCC;                     //!< mobile country code, coded as 3 BCD (4 bit for each digit 0..9)
                                               //!< Only available if SIB3 demodulation is active.
         WORD        wMNC;                     //!< mobile network code, coded as 3 BCD (4 bit for each digit 0..9)
                                               //!< Only available if SIB3 demodulation is active.
         WORD        wLAC;                     //!< location area code if not SDefs::INVALID_WORD_VALUE.
                                               //!< Only available if SIB1 demodulation is active.
      }              *pBchCellIdentification;


      DWORD          dwTimeDelayInCirSamples;  //!< the first CIR signal psCodePowerInDBm100[0] has arrived the receiver
                                               //!< dwTimeDelayInCirSamples/dwCirSamplingRateInHz seconds delayed from u64DeviceTimeInNs
                                               //!< this value can be used to display the time delay between different CIRs.
      DWORD          dwCountOfCodePowerValues; //!< The length of the CIR depends on distance between minimum and maximum delayed signal
                                               //!< components.
      short          *psCodePowerInDBm100;     //!< CIR power values in 0.01dBm in time distance dwCirResolutionInFemtoSeconds.
                                               //!< \@attribute[filterable="true"]
                                               //!< \@attribute[type="array" size="dwCountOfCodePowerValues" sizeismember="true"]
      short          sInbandPowerInDBm100;     //!< Power in the RF band during the CIR measurement.
      short          sRSCPInDBm100;            //!< Integral measured power for the C-PICH signal. (Composition of all peaks)
      short          sISCPInDBm100;            //!< Interference signal code power for spreading factor 256. (C-PICH spreading)


      WORD           wRmsDelaySpreadInChip100; //!< RMS delay spread value in an accuracy of 0.01 chip time = 2.6041ns.
                                               //!< Only available if not 0.
      long           lDriftInNsPerSec;         //!< This is time drift of the received signal. It is calculated and estimated from
                                               //!< different measurements. Existing reflections or changeable time shifts or less
                                               //!< measurements yield to less confidence of this value. Long term stationary measurements
                                               //!< result in high confidence of the measured time drift.
                                               //!< The time drift is measured against the device time base. So it is best to use GPS
                                               //!< synchronisation to get the time drift against the most accurate GPS signal.
                                               //!< Only available if not 0.
      long           lStdDevOfDriftInNsPerSec; //!< Standard deviation for an approximated Gaussian distribution of the value lDriftInNsPerSec.
                                               //!< If this value is small then we have high confidence for the measured time drift above.
                                               //!< Only available if lDriftInNsPerSec is not 0.

      /**
      structure which contains results for separated peak values in the CIR
      \@attribute[type="serializable"]
      */
      struct SPeakInfo
      {
         DWORD       dwTimeDelayInCirSamples;  //!< Signal of the peak has arrived the receiver
                                               //!< dwTimeDelayInCirSamples/dwCirSamplingRateInHz seconds delayed from u64DeviceTimeInNs
         short       sPeakPowerInDBm100;       //!< This power includes short term reflections and may be different to the power
                                               //!< in the CIR graph at the same position.

         /**
         Structure for code power values of synchronisation channels. This values are only valid, if a new synchronisation to this
         C-PICH signal was possible. If this values are invalid, then the measurement had be taken only because of a previous
         measurement of the same Node B. In this case the pointer to this structure is NULL.
         \@attribute[type="serializable"]
         */
         struct SSCHPowerValues
         {
            short    sInbandPowerInDBm100;     //!< As the arrival time of the P-SCH and the S-SCH is different from the arrival time of the
                                               //!< used C-PICH signal for CIR measurements we have an inband power different from
                                               //!< SCPichCir::sInbandPowerInDBm100
            short    sPSCHPowerInDBm100;       //!< code power for one P-SCH code
            short    sSSCHPowerInDBm100;       //!< code power for one S-SCH code

         }           *pSCHPowerValues;

         short       *psDopplerShiftInHz;      //!< The Doppler shift is measured only in MEAS_MODE_HIGH_DYNAMIC. For MEAS_MODE_HIGH_SPEED
                                               //!< the pointer is NULL;
      };

      /**
      Peak result list.
      */
      SViComList<SPeakInfo> ListOfPeaks;
      //!< \@attribute[filterable="true"]

      /**
      Parameters of the maximum peak.

      This result will only be provided, if the peak list is not empty, PPS has been acquired and 
      the Time of Arrival feature has been enabled (i.e. 0 < SSettings::dwToaAvgWindowLengthMs).
      In addition, it will take about 100 s from start of measurement, before the first results are provided.

      @Note: This feature requires a GPS antenna to be connected to the receiver.
      \@attribute[type="serializable"]
      */
      struct SMaxPeak
      {
         /**
         Pointer to the maximum element of the CIR Peak list. 
         This value is not optional, it is a pointer to avoid copying data.
         */
         SPeakInfo*     pMaxPeakInfo;

         /**
         Time difference between Radio Frame Time of Arrival and the latest PPS pulse received.
         The time, when the maximum CIR peak has been received, is used as Radio Frame Time of Arrival.

         According to 3GPP TS25.211 Chapter 5, the WCDMA FDD radio frame (P-CPICH) has 10 ms length.

         To determine the frame delay as value within +/- half of the radio frame length (+/- 10 ms / 2  =>  5 ms),
         the provided dFrameToaOffsetToPpsInSec must be processed according to he following principle:
         double dFrameDelay_ms = ( (1000*dFrameToaOffsetToPpsInSec  + 5 ms) modulo 10 ms ) - 5 ms

         @note
         This feature requires a GPS antenna to be connected to the receiver.
         */
         double         dFrameToaOffsetToPpsInSec;

         /**
         Virtual Radio Frame Number (RFN) assigned to each radio frame received.
         The RFN counting is started for each new combination of Channel and ExtendedSC,
         i.e. dwChannelIndex, SExtendedSC::wSC, SExtendedSC::wIndicator.
         See explanation for SExtendedSC.
         */
         DWORD          dwRadioFrameNumber;
      }* pMaxPeak;
      //!< \@attribute[filterable="true"]
   };

   /**
   Result list of C-PICH CIRs.
   */
   SViComList<SCPichCir> ListOfCPichCirs;

   /**
   List of measurement rates rates and other values in relation to the measurement rate for
   one or more time intervals of measurement rate calculation. The measurement rate is
   the ratio of all measurements of the same type of measurement at the time
   the measurement signals have been picked from the air. So the time of arrival
   at the host application is not used. And equal measurements at different RF channels are put together.
   */
   SViComList<SMeasurementRate> ListMeasurementRates;

};

/**
Request for the Layer 3 Decoder.
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
                                          //!< If m_dwBitCount is not a multiple of 8, there are remaining bits,
                                          //!< which shall be ignored.
                                          //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
};

/**
Result from the Layer 3 Decoder.

The string returned via this structure is formatted using the ASN.1 notation of the UMTS standard
( 3GPP TS 25.331 V6.9.0 (2006-03) ), which can be found under the following location
"http://www.3gpp.org/ftp/Specs/2006-03/Rel-6/25_series/".
It contains one completely decoded PDU terminated by CR+LF. Data subsets (i.e. Information Elements)
within the string are headed by "CR+LF+{", indented via spaces and terminated by "}+CR+LF".

In addition to the standard, some Information Elements are added twice (see example cellIdentity below):
   1. Formatted exactly according to the standard.
   2. Formatted with unit or format description between square brackets ("[" "]") and representing a reasonable value.

For instance, a SIB3 PDU will be decoded as follows:

@code
value SysInfoType3 ::=
{
   sib4indicator FALSE,
   cellIdentity '11011011 01110110 01011001 1101'B,
   cellIdentity[16 Bit] 26013,
   cellSelectReselectInfo
   {
      mappingInfo
      {
         {
            rat gsm,
            mappingFunctionParameterList
            {
               {
                  functionType linear,
                  mapParameter1 1,
                  mapParameter2 1,
                  upperLimit 1
               }
            }
         }
      },
      cellSelectQualityMeasure cpich-Ec-N0 :
      {
         q-HYST-2-S 1,
         q-HYST-2-S[dB] 2
      },
      modeSpecificInfo fdd :
      {
         s-Intrasearch 7,
         s-Intrasearch[dB] 14,
         s-Intersearch 3,
         s-Intersearch[dB] 6,
         s-SearchHCS 2,
         s-SearchHCS[dB] 5,
         rat-List
         {
            {
               rat-Identifier gsm,
               s-SearchRAT 2,
               s-HCS-RAT 1,
               s-HCS-RAT[dB] 3,
               s-Limit-SearchRAT 2
            }
         },
         q-QualMin -18,
         q-RxlevMin -58,
         q-RxlevMin[dBm] -115
      },
      q-Hyst-l-S 1,
      q-Hyst-l-S[dB] 2,
      t-Reselection-S 2,
      hcs-ServingCellInformation
      {
         hcs-PRIO 2,
         q-HCS 0,
         t-CR-Max notUsed : NULL
      },
      maxAllowedUL-TX-Power 24
   },
   cellAccessRestriction
   {
      cellBarred notBarred : NULL,
      cellReservedForOperatorUse notReserved,
      cellReservationExtension notReserved,
      accessClassBarredList
      {
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred,
         notBarred
      }
   }
}
@endcode
\@attribute[type="serializable"]
*/
struct SL3DecoderResult
{
   Pdu::Type            ePdu;             //!< PDU contained.
   DWORD                dwStringLength;   //!< Number of decoding result text bytes in the buffer (incl. terminating '\0').
   char*                pcPduText;        //!< Pointer to a buffer containing the decoded PDU text, as zero terminated ASCII string.
                                          //!< \@attribute[type="array" size="dwStringLength" sizeismember="true"]
};

} // namespace WCDMA
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE

