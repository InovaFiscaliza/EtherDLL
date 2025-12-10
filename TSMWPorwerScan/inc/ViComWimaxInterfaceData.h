// ViComWiMaxInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_WIMAX        8    // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)       // here stops the inclusion for resource files.
#pragma once
#pragma pack(push, 8)                      // get a definite structure alignment for CViComWimaxInterface.


namespace RohdeSchwarz
{
namespace ViCom
{
namespace WIMAX
{
/**
Data container for ViCom WiMax interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_WIMAX,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
Configuration of the measurement that is performed on one single frequency.
\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   SFrequencySetting()
      : dwNomBandWidthInHz     ( BandWidth::MHZ_10 )
      , dwFftSizeMask          ( FftSize::ALL )
      , dwCpRatioMask          ( CpRatio::RATIO_1_BY_ALL )
      , dwFrameRateRangeMinInHz( FrameRate::HZ_50 )
      , dwFrameRateRangeMaxInHz( FrameRate::HZ_500 ) 
      , bGpsPpsSync            ( FALSE )
      , bUseForRecSync         ( TRUE )
      , dFrameDelayToPpsInSec  ( 0.0 )
      , dwSyncTrialsPer1000Sec ( dwOptSyncTrialsPer1000Sec )
      , dwTraceMeasPer1000Sec  ( 0 )
   {}

   /**
   The center frequency of the channel in Hz.
   */
   double                  dFreqInHz;

   struct BandWidth { enum Type {
      MHZ_3_5  = 3500000,
      MHZ_5    = 5000000,
      MHZ_7    = 7000000,
      MHZ_8_75 = 8750000,
      MHZ_10   = 10000000,
   };};

   static const DWORD      dwMinPossibleNomBandWidthInHz  = 1250000;          //!< Minimum possible nominal bandwidth (inclusive).
   static const DWORD      dwMaxPossibleNomBandWidthInHz  = 20000000;         //!< Maximum possible nominal bandwidth (inclusive).

   /**
   The nominal bandwidth in Hz.
   */
   DWORD                   dwNomBandWidthInHz;
      
      
   /**
   Possible FFT Size Masks.
   */
   struct FftSize { enum Type {
      BIN_128  =  128,
      BIN_512  =  512,
      BIN_1024 = 1024,
      BIN_2048 = 2048,
      ALL      = (128 | 512 | 1024 | 2048)
   };};

   /**
   FFT Size mask matching different FFT Size. At least one bit shall be set.
   */
   DWORD                   dwFftSizeMask;

   /**
   Possible CP Ration Masks.
   */
   struct CpRatio { enum Type {
      RATIO_1_BY_4   =    4,
      RATIO_1_BY_8   =    8,
      RATIO_1_BY_16  =   16,
      RATIO_1_BY_32  =   32,
      RATIO_1_BY_ALL =   (4 | 8 | 16 | 32)
   };};

   /**
   The corresponding Ratios of Cyclic Prefix to useful symbol time. At least one bit shall be set.
   */
   DWORD                   dwCpRatioMask;

   /**
   Possible Frame Rates.
   */
   struct FrameRate { enum Type {
      HZ_50  =  50,
      HZ_80  =  80,
      HZ_100 = 100,
      HZ_125 = 125,
      HZ_200 = 200,
      HZ_250 = 250,
      HZ_400 = 400,
      HZ_500 = 500
   };};

   /**
   Minimum frame repetition rate in Hertz.
   */
   DWORD                   dwFrameRateRangeMinInHz;

   /**
   Maximum frame repetition rate in Hertz.
   */
   DWORD                   dwFrameRateRangeMaxInHz;

   /**
   TRUE if all signals on the channel are assumed synchronized to the PPS of the GPS.
   */
   BYTE                    bGpsPpsSync;

   /**
   TRUE if the measurement results of this channel shall be used for
   synchronization of the receiver. If FALSE then the measurement results
   on this channel are ignored for the receiver sync.
   */
   BYTE                    bUseForRecSync;

   /**
   Delay of the first WiMAX frame after an PPS. It is only used if bGpsPpsSync is TRUE.
   */
   double                  dFrameDelayToPpsInSec;

      
   static const DWORD      dwOptSyncTrialsPer1000Sec = 1000;      //!< Normal rate for 'dwSyncTrialsPer1000Sec', i.e. 1/sec.

   /**
   Rate to perform a sync regarding the given constrains. The unit is per 1000 seconds.
   For one measurement, we need the receiver for about
   (5 + 2000 / dwFrameRateRangeMinInHz) milliseconds.
   This is the measurement rate used for sync scanning. The resources used 
   for sync scanning cannot be used for measurement scanning. 
   If we measure a network that doesn't change, this rate could be very low. 
   If we expect changes in the network, we have to set this value to an 
   appropriate higher value.
   dwSyncTrialsPer1000Sec = 1000 means, one sync trial per second, which is 
   high enough for a drive test.
   */
   DWORD                   dwSyncTrialsPer1000Sec;
      
   /**
   As soon as a signal was found for a certain BS, then it is traced with the 
   given rate. This will appear as a measurement rate for the scanner. 
   For one measurement we need the receiver for about
   (5 + 32 * 1000 / SMeasResult::dwFrameRateInHz) milliseconds. 
   E.g. for 50Hz frame rate we need 645ms. So we can
   measure this channel with not more than 1500mHz (1.5 measurements per 
   second) if we have full receiver capacity for this channel. 
   So a maximum MeasRate for the trace measurement can be achieved when the 
   WiMAX Frame Rate is 500Hz. Then we can get 14.5Hz
   (dwTraceMeasPer1000Sec = 14500) in case that the receiver time used
   for synchronization is neglected.
   For default value and range see GetMeasRateLimits().
   */
   DWORD                   dwTraceMeasPer1000Sec;
};

/**
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   SChannelSettings()
      : dwCount( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
      , pTableOfFrequencySetting( NULL )
   {}

   /** 
   Number of frequency configurations that shall be used from the pTableOfFrequencySetting
   to perform the measurement tasks.
   For value range see GetChannelCountLimits().
   */
   DWORD                   dwCount;

   /** 
   Receiver front end to be used by the scanner for this channel.
   Scanner tasks are not allowed to contain multiple FE's.
   */
   DWORD                   dwFrontEndSelectionMask;

   /**
   This array of structures can be filled with single frequency configurations.
   Only dwCount entries from this table will actually be used.
   \@attribute[type="array" size="dwCount" sizeismember="true"]
   */
   SFrequencySetting*      pTableOfFrequencySetting;
};

/** BCH demodulation **/

/**
This enumeration defines the MAC Management PDU types.
Use this in the S_PDU_Request structure to specify which PDUs
inside the MAC Management message shall be decoded.
\@attribute[type="serializable"]
*/
struct Pdu { enum Type {
   UCD          =  0,  //!< Decode UCD
   DCD          =  1,  //!< Decode DCD
   DL_MAP       =  2,  //!< Decode DL-MAP
   UL_MAP       =  3,  //!< Decode UL-MAP
   RNG_RSP      =  5,  //!< Decode RNG-RSP
   REG_RSP      =  7,  //!< Decode REG-RSP
   MOB_NBR_ADV  = 53,  //!< Decode MOB_NBR-ADV
   MOB_BSHO_REQ = 56,  //!< Decode MOB_BSHO-REQ
   MOB_BSHO_RSP = 58,  //!< Decode MOB_BSHO-RSP
   MOB_PAG_ADV  = 61,  //!< Decode MOB_PAG-ADV
   UNKNOWN      = SDefs::INVALID_DWORD_VALUE
};};

/**
This enumeration defines modes for the BCH demodulation.
Use this in the SDemodRequest structure to specify when the demodulation 
of this PDU shall be done.
\@attribute[type="serializable"]
*/
struct DemodMode { enum Type {
   /**
   Demodulate the PDU for each BTS at the specified channel only once.
   This is only to be used in start measurement.
   */
   ONCE             = 0,

   /**
   The PDU is only demodulated if an extra command for demodulation requests the demodulation.
   Only to be used in start measurement.

   If this is used, then no PDU is decoded until it is explicitly requested. 
   The method IssueDemodRequests can be used for that purpose.
   */
   ON_CMD           = 1,

   /**
   After each successful demodulation a new one will be started automatically after the repetition time specified in wRepetitionDelayIn100ms.
   This is only to be used in start measurement.
   */
   REPETITION       = 2,

   /**
   Demodulate the PDU only for a specific BTS. The BTS is specified in the S_PDU_Request::dwBtsId field.
   This demodulation mode is only valid during a running measurement (i.e. on demand).
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
         On the first channel DL-MAP shall be decoded, on the the second channel UL-MAP and DCD shall be decoded.
         UL-MAP shall be decoded repeatedly every 10 seconds (10000 ms) and DCD shall be decoded on demand.
         This requires three structures containing the following parameter sets:
   @code
         (0, DL_MAP,          0,     0, 0)
         (1, UL_MAP, REPETITION, 10000, 0)
         (1, DCD,    ON_CMD,     0,     0)
   @endcode
   Example during measurement:
         On channel 0 PDU demodulation shall be completely reset for the BTS identified by ID 3
         and on the the second channel DL-MAP shall be decoded for the BTS identified by ID 4 
         with a timeout for the request of 1 sec (1000ms):
   @code
         (0, 0,      CHANNEL_RESET, 0,    3)
         (1, DL_MAP, ON_CMD,        1000, 4)
   @endcode
   Note that you are responsible to manage the memory when you use SetDemodulationSettings().
   Note: All parameters which are not necessary for a certain request shall be set to 0.
   * \@attribute[type="serializable"]
   */
   struct SDemodRequest
   {
      SDemodRequest()
         : dwChannelIndex( 0 )
         , ePDU( Pdu::DL_MAP )
         , eDemodulationMode( DemodMode::ONCE )
         , dwRepetitionTimeOutInMs( 0 )
         , dwBtsId( 0 )
      {}

      /** 
      Index of the frequency in SChannelSettings::aTableOfFrequencySetting.
      */
      DWORD                                       dwChannelIndex;

      /** 
      The PDU to be demodulated.
      */
      Pdu::Type                                   ePDU;

      /** 
      The mode used for BCH demodulation.
      */
      DemodMode::Type                             eDemodulationMode;

      /** 
      Repetition time out in ms (only used if bDemodulationMode == REPETITION).
      */
      DWORD                                       dwRepetitionTimeOutInMs;

      /** 
      Unique BTS ID provided by the demodulator. This ID is used by the BCHDemodulator to demodulate a specific BTS,
      when the bDemodulationMode is set to ON_CMD.
      */
      DWORD                                       dwBtsId;
   };

   /** 
   Pointer to an array of demodulation requests. Must be NULL, if no demodulation shall be done.
   */
   SDemodRequest*       pDemodRequests;
};

/**
This structure is used to configure the BCH demodulation process.
\@attribute[type="serializable"]
*/
struct SDemodulationSettings
{
   SDemodulationSettings()
      : sCINRThresholdInDB100( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /**
   CINR threshold for demodulation, given in 1/100 dB.
   For default value and range see GetDemodThresholdLimits().
   */
   short                sCINRThresholdInDB100;

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
This structure holds all the measurement setup that is configured in the ViCom WiMax scanner module. 
It is a result of the GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   DWORD                      dwReceiverIndex;           //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth         ResultBufferDepth;         //!< Number of results allowed in the result buffer
   SChannelSettings           ChannelSettings;           //!< The set of frequencies that shall be measured
   SDemodulationSettings      DemodulationSettings;      //!< Configuration of the BCH demodulation
};

/**
The WiMax measurement result for a single channel returned by
GetResult().
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /**
   The channel to which all the data relates is specified using this index.
   The index refers to the position in the SChannelSettings::aTableOfFrequencySetting
   array.
   */
   DWORD       dwChannelIndex;

   //! PC local time when the signal was measured (or was started to being measured) on the device.
   DWORD       dwPcTimeStampInMs;

   double      dCenterFreqInHz;     //<! Measured center frequency regarding the receiver time base.
   DWORD       dwNomBandwidthInHz;  //<! Measured nominal bandwidth.
   DWORD       dwFftSize;           //<! Used FFT size
   DWORD       dwCpRatio;           //<! CP Ratio or 0x7fff if the CP Ratio was not found.
   DWORD       dwFrameRateInHz;     //<! Measured frame repetition rate in Hz or 0x7fff if repetition rate was not found.

   /**
   Segment measurement results.
   \@attribute[type="serializable"]
   */
   struct SSegmentMeas
   {
      BYTE    bPreambleIndex;      //<! WiMAX preamble index [0..113]
      BYTE    bSegment;            //<! Segment [0..2]
      BYTE    bIdCell;             //<! ID Cell [0..31]
      DWORD   dwCirIndex;          //<! Index of the CIR frame in the CIR frame list.
   };
   SViComList<SSegmentMeas>    ListOfSegmentMeas;

   /**
   Optional data container that holds information about the channel impulse
   responses measured in this cycle. The CIR consists of an optional power
   delay profile, and a list of peaks.

   Channel impulse responses are calculated from a 100ms block.
   \@attribute[type="serializable"]
   */
   struct SCir
   {
      //! Calculated time of the first CIR sample based on start of the
      DWORD       dwPcTimeStampInMs;
      //! Time when the first CIR sample has been measured, related to the device internal clock.
      ULONGLONG   u64DeviceTimeInNs;

      /**
      Structure containing the actual CIR measurement results combined with delay
      information.
      \@attribute[type="serializable"]
      */
      struct SPowerDelayProfile
      {
         //! Inband power measured during the CIR measurement (which is a subset of the overall signal)
         float fInbandPowerInDBm;
         //! Total received power over the complete signal
         float fAggregatePowerInDBm;
         //! The CINR value.
         double dCINR;               
         /**
         Estimated noise level. Power values which are in the vicinity
         of this value can be considered to be caused or influenced by noise.
         */
         float fNoiseFloorInDBm;
         //! Sampling time for a CIR measurement. This is also the time between two CIR measurements.
         float fSamplingTimeInSec;

         /**
         A (possibly empty) set of power delay values. This list contains the CIR power
         values, where each power value has been measured fSamplingTimeInSec seconds after the
         previous one. The first sample in here has been measured at u64DeviceTimeInNs resp.
         dwPcTimeStampInMs.

         The list may contain invalid values, which denote that at that specific time there
         was no CIR measurement possible.
         \@attribute[type="serializable"]
         */
         struct SPowerDelayProfileValues
         {
            //! number of elements available in the psValuesInDBm100 array
            DWORD                dwCountOfValues;
            /**
            Array that holds dwCountOfValues elements,
            each specifying a power delay value in 100th dBm
            (e.g. a value of -6719 is to be read as -67.19 dBm).
            Can be SDefs::INVALID_SHORT_VALUE when a value could not be measured
            */
            short                *psValuesInDBm100;
            SPowerDelayProfileValues()
            {
               dwCountOfValues  = 0;
               psValuesInDBm100 = NULL;
            }

         }                        PowerDelayProfileValues;

      };
      const SPowerDelayProfile*    pPowerDelayProfile;
      //!< Optional power profile measurement further detailing the CIR measurement

      /**
      Contains information about the single peaks in the Channel Impulse Response.
      \@attribute[type="serializable"]
      */
      struct SPeak
      {
         float                fInbandPowerInDBm;
         //!< Inband power of the signal from which the CIR was extracted
         float                fPeakPowerInDBm;
         //!< The actual power of the impulse peak.
         short                sDopplerInHz;
         //!< Main part of a Doppler frequency. Is not necessarily
         //!< available and may be set to SDefs::INVALID_SHORT_VALUE.
         float                fDelayInSec;
         //!< Delay in seconds related to the first sample.
         //!< This normally provides a higher resolution than the sampling
         //!< time from the power delay measurement results (see above).
      };

      //! List of CIR peaks found in the signal.
      SViComList<SPeak> ListOfPeaks;

      SCir()
      {
         pPowerDelayProfile = NULL;
      }
   };
   SViComList<SCir>            ListOfCirs;
   //!<\@attribute[filterable="true"]

   /**
   Downlink Frame Prefix.
   \@attribute[type="serializable"]
   */
   struct SDlfp
   {
      /**
      Sub-channel group. Bitmask identifying the subchannel groups present.
      Bit0 for group 0 ....  Bit5 for group 5. Default 0
      */
      BYTE      bSubchannelGroups;
      //! Indicates the repetition code used for the DL-MAP.
      BYTE      bRepetitionCoding;
      /**
      Indicates the FEC encoding code used for the DL-MAP
      in case of OFDMA DL Frame.
      Prefix format for all FFT sizes except 128
      */
      BYTE      bDlmapCoding;
      //! Length of the DL-MAP in bytes.
      BYTE      bDlmapLength;
   };

   const SDlfp*  pDlfp;    //!< This is NULL if no DLFP information is available

   /** 
   BCH demodulator results
   \@attribute[type="serializable"]
   */
   struct SDemodResult
   {
      //! The channel index from which channel the PDU result is retrieved.
      DWORD                 dwChannelIndex;

      //! The time stamp of the PDU result retrieval.
      DWORD                 dwTimeStampInMs;

      /**
      Unique BTS ID provided by the demodulator. Each time the demodulator is not
      sure whether the signal it could demodulate belongs to an already known BTS, it sets a new identifier
      here. I.e. the same BTS could have multiple dwBtsIds, but each dwBtsId relates always to a single BTS.
      */
      DWORD                 dwBtsId;

      //! Preamble index, value 0 - 131. 0xFFFF if invalid.
      BYTE                  bPreambleIndex;

      //! Segment number (0, 1 2). 0xFF if invalid.
      BYTE                  bSegment;

      /**
      Base Station ID. 0xFFFFFFFFFFFF if unknown.
      The Base Station ID (BS ID) is a 48-bit (i.e 6 byte) long field identifying the BS.
      The barBaseStationID array is filled beginning with the most significant byte
      at index 0 down to the least significant byte at index 5.
      Example:
               BS ID                =  0x010203040506
               barBaseStationID[0]  =  0x01
               barBaseStationID[1]  =  0x02
               barBaseStationID[2]  =  0x03
               barBaseStationID[3]  =  0x04
               barBaseStationID[4]  =  0x05
               barBaseStationID[5]  =  0x06
               This leads to 
                     bSectorID      = 0x06
               in case of an uncompressed DL-MAP
                     dwOperatorID   = 0x00010203
               and in case of a compressed MAP
                     dwOperatorID   = 0x00000003
      */
      BYTE                  barBaseStationID[6];

      //! Sector ID. Least significant byte of Base Station ID.
      BYTE                  bSectorID;

      /**
      Operator ID. Most significant 3 bytes of the Base Station ID.
      In case of a compressed DL-MAP, only the least significant byte
      of this 3 byte Operator ID is filled in.
      */
      DWORD                 dwOperatorID;

      //! PDU that has been demodulated, @see etMacManagementPDU.
      Pdu::Type                 ePDU;

      //! Flags constants.
      static const DWORD    dwPDU_FLAG_NOT_SET      = 0;  //!< "Normal" PDU.
      static const DWORD    dwPDU_FLAG_COMPRESSED   = 1;  //!< The PDU contains compressed DL/UL-MAPs.
      static const DWORD    dwPDU_FLAG_SHORT_HEADER = 2;  //!< The message uses a special short headers, e.g. GSM RR Short Header Messages.
      static const DWORD    dwPDU_FLAG_MSG_FRAGMENT = 4;  //!< The PDU contains a fragment of a message, e.g. fragment of a WIMWAX DCD or UCD message.
      static const DWORD    dwPDU_FLAG_NO_HEADER    = 8;  //!< The PDU has no header, i.e. it is only payload. E.g. WIMAX MAC Management Message Payload.

      /**
      Flags for additional information about the PDU contained.
      The flags depend on the technology.
      */
      DWORD                 dwFlags;

      //! Number of bits the PDU consists of.
      DWORD                 dwBitCount;

      /** 
      Pointer to (dwBitCount + 7) / 8 bytes which contain the PDU bits. The first bit is stored in the bit 7 of pbBitStream[0]. 
      If dwBitCount is not a multiple of 8, the trailing bits in the last byte pbBitStream[(dwBitCount + 7) / 8 - 1] shall be ignored.
      \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
      */
      const BYTE*           pbBitStream;

      /**
      Start time of the radio frame which contained the MAC Management PDU.
      If the PDU was assembled from multiple fragments, it's the arrival time of the first radio frame.
      Concatenated PDUs are provided with the same arrival time.
      */
      ULONGLONG             u64ArrivalTimeInNs;
   };

   /**
   Pointer to demodulation result. If NULL, no demodulation result is present.
   */
   const SDemodResult*      pDemodResult;

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
Request data container for the Layer 3 Decoder. It is used to decode a demodulation result into
a human-readable structure. The values from SDemodResult are normally used for input data.
\@attribute[type="serializable"]
*/
struct SL3DecoderRequest
{
   SL3DecoderRequest()
      : ePdu( (Pdu::Type) 0 )
      , dwFlags( 0 )
      , dwBitCount( 0 )
      , pbBitStream( NULL )
   {}

   Pdu::Type                ePdu;             //!< PDU that has been demodulated, @see etMacManagementPDU.
   DWORD                dwFlags;          //!< Flags for additional information about the PDU contained.
   DWORD                dwBitCount;       //!< Number of bits the PDU consists of.
   BYTE*                pbBitStream;      //!< Pointer to a buffer containing the PDU bits.
                                          //!< The first bit is stored in bit 7 of pbBitStream[0].
                                          //!< If m_dwBitCount is not a multiple of 8, there are remaining bits,
                                          //!< which shall be ignored.
                                          //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
};

/**
Result from the Layer 3 Decoder.
The string returned via this structure is indented text describing the contents of one completely decoded PDU.
\@attribute[type="serializable"]
*/
struct SL3DecoderResult
{
   SL3DecoderResult()
      : ePdu         ( Pdu::DCD )
      , pcPduText    ( NULL )
      , pDlmap       ( NULL )
      , pUlmap       ( NULL )
      , pDcd         ( NULL )
      , pUcd         ( NULL )
      , pRngRsp      ( NULL )
      , pRegRsp      ( NULL )
      , pMobNbrAdv   ( NULL )
      , pMobBshoReq  ( NULL )
      , pMobBshoRsp  ( NULL )
      , pMobPagAdv   ( NULL )
   {}

   //! PDU contained.
   Pdu::Type            ePdu;
   //! Number of decoding result text bytes in the buffer (incl. terminating '\0').
   DWORD                dwStringLength;
   /**
   Pointer to a buffer containing the decoded PDU text, as zero terminated ASCII string.
   \@attribute[type="array" size="dwStringLength" sizeismember="true"]
   */
   const char*          pcPduText;

   //! This struct holds a TLV entry.
   //! The maximum length can be represented is 4294967295 (0xFFFFFFFF),
   //! which is large enough for practical cases.
   //! \@attribute[type="serializable"]
   struct STlv
   {
      BYTE        bType;
      const char* szType;  //!< NULL-terminated string describing the type.
                           //!<\@attribute[type="array" size="dwLength" sizeismember="true"]
      DWORD       dwLength;

     /** Downlink Burst Profile is special because it contains an embedded TLV.
     \@attribute[type="serializable"]
     */
      struct SDownlinkBurstProfile
      {
         BYTE    bDIUC;
         STlv*   pTlv;
         SDownlinkBurstProfile()
         {
            pTlv = NULL;
         }
      };

      /**
     *Uplink Burst Profile is special because it contains an embedded TLV.
     * \@attribute[type="serializable"]
     */
      struct SUplinkBurstProfile
      {
         BYTE    bUIUC;
         STlv*   pTlv;
         SUplinkBurstProfile()
         {
            pTlv = NULL;
         }
      };

      //! pValue is a pointer to the value contents. It normally points to an array of bytes.
      //! There are several exceptions:
      //!  - For DCD/UCD PDU, if the type of the TLV is Downlink/Uplink Burst Profile (bType == 1),
      //!    pValue actually points to the above SDownlinkBurstProfile/SUplinkBurstProfile structure.
      //!    The user needs a cast to the appropriate structure type.
      //!  - If the value part of the TLV is not a "simple" value in the L3 decode tree,
      //!    i.e. It contains sub-fields (This can be examined through the pcStringPDU string),
      //!    Then this pValue will be NULL and szValue will contain the text description of the value.
      const BYTE* pValue;   //!< \@attribute[type="array" size="dwLength" sizeismember="true"]
      const char* szValue;  //!< NULL-terminated string describing the value contents.

      STlv()
      {
         bType    = 255;
         szType   = "";
         dwLength = 0;
         pValue   = NULL;
         szValue  = "";
      }
   };

   /**
   \@attribute[type="serializable"]
   */
   struct SDlmap
   {
      //! Base Station ID. 0xFFFFFFFFFFFF if unknown.
      BYTE    barBaseStationID[6];
      //! Sector ID. Least significant byte of Base Station ID.
      BYTE    bSectorID;
      /**
      Operator ID. Most significant 3 bytes of the Base Station ID.
      In case of a compressed DL-MAP, only the least significant byte
      of this 3 byte Operator ID is filled in.
      */
      DWORD   dwOperatorID;
      /**
      @note Please refer to spec 802.16-2009: 8.4.5.2 Frame duration codes.
      Frame duration code:
      0b0000 = Reserved
      0b0001 = 2.0 ms
      0b0010 = 2.5 ms
      0b0011 = 4 ms
      0b0100 = 5 ms
      0b0101 = 8 ms
      0b0110 = 10 ms
      0b0111 = 12.5 ms
      0b1000 = 20 ms
      0b1001 - 0b1111= Reserved
      */
      BYTE    bFrameDuration;
      //! Frame number
      DWORD   dwFrameNumber;
      //! Downlink Channel Descriptor count
      BYTE    bDCDCount;
      //! Number of OFDMA symbols
      BYTE    bNoOfDLSymbols;
         
      //! Number of IEs inside this DL-MAP message.
      BYTE    bNoOfIEs;

      /**
      \@attribute[type="serializable"]
      */
      struct SDlmapIE
      {
         /**
         DIUC. [8.4.5.3.1 DIUC allocation] in WiMAX spec.
         0–12 Different burst profiles
         13 Gap/PAPR reduction
         14 Extended-2 DIUC IE
         15 Extended DIUC
         @note, Only burst profiles IEs (for DIUC between 0 and 12) are currently supported.
         */
         BYTE bDIUC;
         /**
         \@attribute[type="serializable"]
         */
         struct SIEBurstProfile
         {
            //! OFDMA Symbol offset
            BYTE bSymbolOffset;
            //! Subchannel offset
            BYTE bSubchannelOffset;
            /**
            000: Normal (not boosted)
            001: +6dB
            010: –6dB
            011: +9dB
            100: +3dB
            101: -3dB
            110: –9dB
            111: –12dB
            */
            BYTE bBoosting;
            //! No. OFDMA Symbols
            BYTE bNoOfDLSymbols;
            //! No. Subchannels
            BYTE bNoOfSubchannels;
            /**
            Repetition Coding Indication.
            0b00: No repetition coding
            0b01: Repetition coding of 2 used
            0b10: Repetition coding of 4 used
            0b11: Repetition coding of 6 used
            */
            BYTE bRepetitionCoding;
         };

         //! Declare a union to keep code-level compatibility when more IE types are supported in the future.
         union UIE
         {
            SIEBurstProfile  burst;
         };

         UIE uIE;

         SDlmapIE()
         {
            // Mark the DIUC as invalid.
            bDIUC = 255;
         }
      };
      SViComList<SDlmapIE>    ListOfDlmapIEs;

      SDlmap()
      {
         for(int i = 0; i < sizeof(barBaseStationID)/sizeof(barBaseStationID[0]); i++)
         {
            barBaseStationID[i] = 0xFF;
         }
         bSectorID = 0;
         dwOperatorID = 0;
      }
   };
   const SDlmap* pDlmap;    //!< This is NULL if no DLMAP information is available

   /**
   \@attribute[type="serializable"]
   */
   struct SUlmap
   {
      //! Uplink Channel Descriptor count
      BYTE    bUCDCount;
      //! Allocation Start Time
      DWORD   dwAllocationStartTime;
      //! No. OFDMA symbols
      BYTE    bNoOfULSymbols;

     /**
     \@attribute[type="serializable"]
     */
      struct SUlmapIE
      {
         /**
         UIUC. [8.4.5.4.1 UIUC allocation] in WiMAX spec.
         0    Fast-feedback channel
         1–10 Different burst profiles (Data Grant Burst Type)
         11   Extended UIUC 2 IE
         12   CDMA BR, CDMA ranging
         13   PAPR reduction allocation, safety zone, Sounding Zone
         14   CDMA Allocation IE
         15   Extended UIUC
         @note Only CDMA BR, CDMA ranging IEs (for UIUC equals to 12) are currently supported.
         */
         BYTE bUIUC;

       /**
       \@attribute[type="serializable"]
       */
         struct SIERanging
         {
            BYTE bSymbolOffset;      //!< OFDMA Symbol offset
            BYTE bSubchannelOffset;  //!< Subchannel offset
            BYTE bNoOfULSymbols;     //!< No. OFDMA Symbols
            BYTE bNoOfSubchannels;   //!< No. Subchannels
            BYTE bRangingMethod;     //!< Ranging Method
            BYTE bDedicatedRangingIndicator;  //!< Dedicated ranging indicator
         };

         //! Declare a union to keep code-level compatibility when more IE types are supported in the future.
         union UIE
         {
            SIERanging  ranging;
         };
         UIE uIE;

         SUlmapIE()
         {
            // Mark the DIUC as invalid.
            bUIUC = 255;
         }
      };
      SViComList<SUlmapIE>    ListOfUlmapIEs;
   };
   const SUlmap* pUlmap;    //!< This is NULL if no DLMAP information is available

   /**
   \@attribute[type="serializable"]
   */
   struct SDcd  //!< DCD
   {
      BYTE bConfigurationChangeCount;  //!< Configuration Change Count
      SViComList<STlv> ListOfTlvs;     //!< TLV entries.
   };
   const SDcd* pDcd;                    //!< This is NULL if no DCD information is available

   /**
   \@attribute[type="serializable"]
   */
   struct SUcd  //!< UCD
   {
      BYTE bConfigurationChangeCount;  //!< Configuration Change Count
      BYTE bRangingBackoffStart;       //!< Ranging Backoff Start
      BYTE bRangingBackoffEnd;         //!< Ranging Backoff End
      BYTE bRequestBackoffStart;       //!< Request Backoff Start
      BYTE bRequestBackoffEnd;         //!< Request Backoff End

      SViComList<STlv> ListOfTlvs;     //!< TLV entries.
   };
   const SUcd* pUcd;                    //!< This is NULL if no DCD information is available

   /**
   \@attribute[type="serializable"]
   */
   struct SRngRsp  //!< RNG-RSP
   {
      SViComList<STlv> ListOfTlvs;      //!< TLV entries.
   };
   const SRngRsp* pRngRsp;               //!< This is NULL if no RNG-RSP information is available

   /**
   \@attribute[type="serializable"]
   */
   struct SRegRsp  //!< REG-RSP
   {
      BYTE  bResponse;                  //!< Response. 0 = OK; 1 = Message authentication failure
      SViComList<STlv> ListOfTlvs;      //!< TLV entries.
   };
   const SRegRsp* pRegRsp;               //!< This is NULL if no REG-RSP information is available

   /**
   \@attribute[type="serializable"]
   */
   struct SMobNbrAdv  //!< MOB_NBR-ADV
   {
      BYTE  bSkipBitmap;                //!< Skip-optional-fields bitmap
      DWORD dwOperatorID;               //!< Operator ID (Optional, present only if Skip-optional-fields-[0] == 0)
      BYTE  bConfigurationChangeCount;  //!< Configuration Change Count
      BYTE  bFragmentationIndex;        //!< Fragmentation Index
      BYTE  bTotalFragmentation;        //!< Total Fragmentation
      BYTE  bNNeighbors;                //!< N_NEIGHBORS

     /**
     \@attribute[type="serializable"]
     */
      struct SNeighbor
      {
         BYTE  bLength;                //!< Length of message information including all fields
         BYTE  bPhyProfileID;          //!< PHY Profile ID
         BYTE  bFAIndex;               //!< FA Index (Optional, present only if the FA index indicator in PHY profile ID is set)
         BYTE  bBsEirp;                //!< BS EIRP (Optional, present only if the BS EIRP indicator in PHY profile ID is set)
         DWORD dwNeighborBSID;         //!< Neighbor BSID (Optional, present only if Skip-optional-fields[1] == 0)
         BYTE  bPreambleIndex;         //!< Preamble Index/Subchannel Index
         BYTE  bHOProcessOptimization; //!< HO Process Optimization (Optional, present only if Skip-optional-fields[2] == 0)
         BYTE  bSchedulingServiceSupported;  //!< Scheduling Service Supported (Optional, present only if Skip-optional-fields[3] == 0)
         BYTE  bDCDConfigurationChangeCount; //!< DCD Configuration Change Count
         BYTE  bUCDConfigurationChangeCount; //!< UCD Configuration Change Count

         SViComList<STlv> ListOfTlvs;  //!< TLV entries.

         SNeighbor()
         {
            bLength = 0;
         }
      };
      SViComList<SNeighbor> ListOfNeighbors;
   };
   const SMobNbrAdv* pMobNbrAdv;         //!< This is NULL if no MOB_NBR-ADV information is available

   /** struct SRecommended ... SCurrentBS are elements of members of union UBSHO.
   \@attribute[type="serializable"]
   */
   struct SRecommended {                 //!< Recommended
      BYTE  barNeighborBSID[6];          //!< Neighbor BSID
      BYTE  bPreambleOrSubchannelIndex;  //!< Preamble index/Subchannel Index
   };

   struct SCID {
      WORD  wID;                         //!< CID
   };

   struct SSAID {
      WORD  wID;                         //!< SAID
   };

   struct SNewBS {                       //!< New BS
      BYTE  barNeighborBSID[6];          //!< Neighbor BSID
      BYTE  bTempBSID;                   //!< Diversity set member ID assigned to this BS.
   };

   struct SCurrentBS {
      BYTE  bTempBSID;                   //!< Diversity set member ID assigned to this BS.
   };

   //! union UBSHO holds the contents for different modes.
   union UBSHO
   {
      struct SMode0 {                   //!< If (Mode == 0b000)
         SViComList<SRecommended> ListOfRecommended;
      } mode0;

      struct SMode1 {                   //!< If (Mode == 0b001)
         BYTE                     bTempBSID;  //!< TEMP BSID of the recommended anchor BS.
         SViComList<SCID>         ListOfCIDs;
         SViComList<SSAID>        ListOfSAIDs;
      } mode1;

      struct SMode2 {                   //!< If (Mode == 0b010)
         BYTE                     bTempBSID;  //!< TEMP BSID of the recommended anchor BS.
      } mode2;

      struct SMode3 {                   //!< If (Mode == 0b011)
         SViComList<SNewBS>       ListOfNewBSs;
         SViComList<SCurrentBS>   ListOfCurrentBSs;
         SViComList<SCID>         ListOfCIDs;
         SViComList<SSAID>        ListOfSAIDs;
      } mode3;

      struct SMode4 {                   //!< If (Mode == 0b100)
         SViComList<SNewBS>       ListOfNewBSs;
         SViComList<SCurrentBS>   ListOfCurrentBSs;
      } mode4;

      struct SMode5 {                   //!< If (Mode == 0b101)
         SViComList<SCID>         ListOfCIDs;
         SViComList<SSAID>        ListOfSAIDs;
         SViComList<SNewBS>       ListOfNewBSs;
         SViComList<SCurrentBS>   ListOfCurrentBSs;
      } mode5;

      struct SMode6 {                   //!< If (Mode == 0b110)
         SViComList<SNewBS>       ListOfNewBSs;
         SViComList<SCID>         ListOfCIDs;
         SViComList<SSAID>        ListOfSAIDs;
         SViComList<SCurrentBS>   ListOfCurrentBSs;
      } mode6;
   };

   struct SMobBsho {
      /**
      Mode
      0b000: HO request
      0b001: MDHO/FBSS request: Anchor BS update with CID update
      0b010: MDHO/FBSS request: Anchor BS update without CID update
      0b011: MDHO/FBSS request: Diversity set update with CID update
      0b100: MDHO/FBSS request: Diversity set update without CID update
      0b101: MDHO/FBSS request: Diversity set update with CID update for newly added BS
      0b110: MDHO/FBSS request: Diversity set update with CID update and CQICH allocation for newly added BS
      0b111: Reserved
      */
      BYTE   bMode;
      UBSHO  uBsho;
      SViComList<STlv> ListOfTlvs;      //!< TLV entries.
   };

   typedef SMobBsho SMobBshoReq;  //!< MOB_BSHO-REQ. Please refer to "6.3.2.3.47 MOB_BSHO-REQ (BS HO request) message" of the WiMAX spec.
   const SMobBshoReq* pMobBshoReq;      //!< This is NULL if no MOB_BSHO-REQ information is available

   typedef SMobBsho SMobBshoRsp;  //!< MOB_BSHO-RSP. Please refer to "6.3.2.3.49 MOB_BSHO-RSP (BS HO response) message" of the WiMAX spec.
   const SMobBshoRsp* pMobBshoRsp;      //!< This is NULL if no MOB_BSHO-RSP information is available

   struct SMobPagAdv  //!< MOB_PAG-ADV. Please refer to "6.3.2.3.51 MOB_PAG-ADV (BS broadcast paging) message" of the WiMAX spec.
   {
      struct SPagingGroupID {
         WORD wID;
      };

      struct SMsMac                     //!< MS MAC addresses
      {
         DWORD dwMacHash;               //!< MS MAC Address hash. (3 bytes)
         BYTE  bActionCode;             //!< Action code (last two LSB bits only)
      };

      SViComList<SPagingGroupID> ListOfPagingGroupIDs;
      SViComList<SMsMac>         ListOfMsMacs;
      SViComList<STlv>           ListOfTlvs;  //!< TLV entries.
   };
   const SMobPagAdv* pMobPagAdv;        //!< This is NULL if no MOB_PAG-ADV information is available
};

} // namespace WIMAX
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
