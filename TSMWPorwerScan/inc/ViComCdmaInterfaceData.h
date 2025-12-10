// ViComCdmaInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_CDMA           16   // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)         // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                        // get a definite structure alignment for CViComCdmaInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace CDMA
{
/**
Data containers used for ViCom LTE interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_CDMA,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit


//** Frequency settings **/
/**
This defines the frequency in Hz and a table, where each single PN offset can be specified
to be measure or not. If this is an EVDO channel is also set here.
\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   SFrequencySetting()
      : dCenterFrequencyInHz( 0 )
      , bIsEvdoFrequency( 0 )
   {
      memset( bTableOfPNOffsetArbitraryLimitation, 1, dwFixCountOfPNOffsetArbitraryLimits );
   }

   /** Maximum number of PN Offset Arbitrary Limits per frequency**/
   static const DWORD   dwFixCountOfPNOffsetArbitraryLimits = 512;

   /** Center frequency of the channel */
   double               dCenterFrequencyInHz;

   /**
   512 TRUE/FALSE values. TRUE is if the Offset is used in the network, FALSE if not.
   This limitation is used for the offset estimation by arrival time. It is not used for demodulation.
   */
   bool                 bTableOfPNOffsetArbitraryLimitation[dwFixCountOfPNOffsetArbitraryLimits];

   /**
   If this shall be demodulated as EVDO channel, then this has to be set to true. 
   */
   bool                 bIsEvdoFrequency;
};

/**
SChannelSettings contains the count of channels or frequencies and the table of frequencies in MHz.
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   SChannelSettings()
      : dwCount( 0 )
      , pTableOfFrequencySetting( NULL )
      , dwMeasRatePer1000Sec( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /** 
   Receiver front end to be used by the scanner for this channel.
   Scanner tasks are not allowed to contain multiple FE's.
   */
   DWORD                dwFrontEndSelectionMask;

   /**
   The desired measurement rate value. For default value and range see GetMeasRateLimits().
   */
   DWORD                dwMeasRatePer1000Sec;

   /**
   Number of frequency configurations that shall be used from the pTableOfFrequencySetting to perform the measurement tasks.
   For value range see GetChannelCountLimits().
   **/
   DWORD                dwCount;

   /**
   Pointer to an array containing the frequencies of the selected channels.
   The number of members in the array is defined by dwCount (above).
   */
   SFrequencySetting    *pTableOfFrequencySetting;
   //!< \@attribute[type="array" size="dwCount" sizeismember="true"]
};


/**
PPS Pulse
The PPS (pulse per second) signal from an external GPS receiver fed to the PULSE IN connector at the rear panel of
the device is used for synchronization. This signal provides maximum accuracy.

If the iDelayOfPPSFallingEdgeIn100ns is valid and PPS synchronisation is used for the receiver than we use the PPS for 
time synchronization. See also comments to STimeEstimationResult::lMeasuredPPSDelayInNs to find the correct value.
If the iDelayOfPPSFallingEdgeIn100ns is invalid, than the PPS is not used for time synchronization even if a PPS is available.
\@attribute[type="serializable"]
*/
struct SPPSSettings
{
   SPPSSettings()
      : iDelayOfPPSFallingEdgeIn100ns( iInvalidPPSDelayIn100ns )
   {}

   static const int  iDelayOfPPSFallingEdgeIn100nsLowerLimit   =   -10000;       //!< Lower limit for Delay Of PPS Falling Edge In 100ns
   static const int  iDelayOfPPSFallingEdgeIn100nsUpperLimit   =  9999999;       //!< Upper limit for Delay Of PPS Falling Edge In 100ns
   static const int  iInvalidPPSDelayIn100ns = 10 * 1000 * 1000;                 //!< invalid value: 1 sec

   int               iDelayOfPPSFallingEdgeIn100ns;   //!< Delay of the PPS Pulse in 100ns or iInvalidPPSDelayIn100ns
};


/**
Sync channel demodulation mode settings
\@attribute[type="serializable"]
*/
struct SyncChannelDemodulationMode { enum Type {
   /**
   Sync channel has to be demodulated only once for time synchronization. This has to be done for each
   RF band, but processing power is saved for other tasks in that mode.
   */
   ONCE    = 1,

   /**
   Sync channel has to be demodulated for each new pilot (if the PN-Scanner has found a new pilot). This
   is a very time consuming tasks, since the software constantly has to perform a demodulation for new pilots.
   */
   ALL     = 2,

   /**
   Usage of fast demodulation if new pilots have been detected. This is the preferred mode, since it
   combines a high accuracy and a processing saving detection method.
   */
   FAST    = 4
};};


/**
Maximum speed of the test vehicle to adjust fading channel limitation in the measurement algorithms.

To calculate a value from miles per hour to kilometer per hour, use the formula shown below:
v[km/h] = 0.621371192237334 * v[m/h];
\@attribute[type="serializable"]
*/
struct SMaxVelocity
{
   SMaxVelocity()
      :dMaxVelocityInKmPerHour( dwMaxVelocityInKmPerHourDefault )
   {}

   static const DWORD  dwMaxVelocityInKmPerHourLowerLimit   =  0;       //!< Lower limit for Max Velocity In Km Per Hour
   static const DWORD  dwMaxVelocityInKmPerHourUpperLimit   =  300;     //!< Upper limit for Max Velocity In Km Per Hour
   static const DWORD  dwMaxVelocityInKmPerHourDefault      =  100;     //!< Default value for Max Velocity In Km Per Hour

   double               dMaxVelocityInKmPerHour;
};


//** Demodulation **/

/**
Here you can find a list of messages that can be demodulated by the CDMA 2000 scanner. The values in this enumeration
can be used as in- and output parameters to specify which kind of message shall or has been demodulated. In many cases,
this is combined with a channel filter specifying on which channel the message occurred.
   
@see etChannelType and PduSpec for the combined usage
\@attribute[type="serializable"]
*/
struct Pdu { enum Type{
   NONE                   = 0,      //!< Empty message ID that refers to no message. Required for BTS_RESET
   SYS_PARAMS             = 1,      //!< System Parameters Message containing general purpose information of the network
   EXT_SYS_PARAMS         = 2,      //!< Extended System Parameters Message
   CHAN_LIST              = 3,      //!< CDMA Channel List Message
   EXT_CHAN_LIST          = 4,      //!< Extended CDMA Channel List Message*/
   NEIGHBOR_LIST          = 5,      //!< Neighbor List Message, containing a list of BTSs that can be used for Handoff
   EXT_NEIGHBOR_LIST      = 6,      //!< Extended Neighbour List Message
   GEN_NEIGHBOR_LIST      = 7,      //!< General Neighbour List Message
   GLOBAL_SERV_DIR        = 8,      //!< Global Service Redirection Message
   EXT_GLOBAL_SERV_RE     = 9,      //!< Extended Global Service Redirection Message
   ACCESS_PARAMETERS      = 10,     //!< CDMA Access Parameters Message
   SYNC_MESSAGE           = 101,    //!< This message is sent on the sync channel. It is not necessary to request this
                                    //!< message explicitly, it is always returned as first message before others get
                                    //!< demodulated.
   EVDO_QUICK_CONFIG      = 201,    //!< 1xEVDO - quick config message
   EVDO_SYNC              = 202,    //!< 1xEVDO - sync message
   EVDO_SECTOR_PARAMETERS = 203,    //!< 1xEVDO - sector parameters message
   EVDO_ACCESS_PARAMETERS = 204,    //!< 1xEVDO - access parameters message
   UNKNOWN                = SDefs::INVALID_DWORD_VALUE
};};

/**
Detail information on what kind of channel was used to demodulate a message, or which 
shall be used to demodulate one. This depends on whether this is used in the measurement
setup or in the result structures.

@see PduSpec for further details
\@attribute[type="serializable"]
*/
struct ChannelType { enum Type {
   INVALID  = 0x0,            //!< No Channel Type, for EVDO usage of PduSpec
   BCCH     = 0x2,            //!< Used the BCCH for message demodulation
   PCH      = 0x1,            //!< Used the PCH for message demodulation
   SYNC     = 0x4,            //!< The sync channel has been used for demodulation
   CCSYN    = 0x8,            //!< The 1xEVDO Synchronous Control Channel was used for message demodulation
     
     
   UNKNOWN  = 0xffff,          //!< No further information on what kind of channel has been used to demodulate result

   ALL	   = 0xffff			    //!< This can be used to enable decoding of all channel types		
};};

/**
Detail information about the EVDO Protocol ID

@see PduSpec for further details
\@attribute[type="serializable"]
*/
struct EvdoProtocol { enum Type {
   INVALID               = 0,     //!< No EVDO Protocol ID, for CDMA2000 usage of PduSpec
   PHY_Layer             = 10,    //!< Physical Layer
   CTRL_CH_MAC           = 11,    //!< Control Channel MAC
   ACCESS_CH_MAC         = 12,    //!< Access Channel MAC
   FWD_TRAF_CH_MAC       = 13,    //!< Forward Traffic Channel MAC
   REV_TRAF_CH_MAC       = 14,    //!< Reverse Traffic Channel MAC
   KEY_EXCHANGE          = 15,    //!< Key Exchange
   AUTHENT               = 16,    //!< Authentication
   ENCRYPTION            = 17,    //!< Encryption
   SECURITY              = 18,    //!< Security
   PACK_CONSOL           = 19,    //!< Packet Consolidation
   AIR_LINK_MAN          = 20,    //!< Air-Link Management
   INIT_STATE            = 21,    //!< Initialization State
   IDLE_STATE            = 22,    //!< Idle State
   CONNECTED_STATE       = 23,    //!< Connected State
   ROUTE_UPDATE          = 24,    //!< Route Update
   OVHD_MSG              = 25,    //!< Overhead Messages
   SESSION_MAN           = 26,    //!< Session Management
   ADDRESS_MAN           = 27,    //!< Address Management
   SESSION_CFG           = 28,    //!< Session Configuration
   STREAM                = 29,    //!< Stream
   STREAM_0_APP          = 30,    //!< Stream 0 App
   STREAM_1_APP          = 31,    //!< Stream 1 App
   STREAM_2_APP          = 32,    //!< Stream 2 App
   STREAM_3_APP          = 33,    //!< Stream 3 App
   VIRTUAL_STREAM        = 34,    //!< Virtual Stream
   VIRTUAL_STREAM_APP    = 35,    //!< Virtual Stream App
   MULTI_MODE_CAP_DIS    = 36,    //!< Multi Mode Capability Disc.
   UNKNOWN               = 0xffff //!< No kind of Protocol ID has been specified in the EVDO request
};};

/**
This enumeration defines modes for the demodulation.
Use this in the SDemodRequest structure to specify when the demodulation 
of this PDU shall be done.
\@attribute[type="serializable"]
*/
struct DemodMode { enum Type{
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
This structure can be used to define a message type related filter for channels and to specify details
on demodulated messages. So it is used as in- and output structure. 

@note
    When this is used as input parameter to specify the demodulation requests, then the ChannelType
    information is not evaluated inside the ViCom. Settings made that are ignored completely.

When a message has been demodulated successfully, this structure specifies the demodulation details.
The channel information then shows from what channel the message has been demodulated.
\@attribute[type="serializable"]
*/
struct SPduSpec
{
   SPduSpec()
      : ePdu( Pdu::NONE )
      , eChannelType( ChannelType::UNKNOWN )
      , eEvdoProtocol( EvdoProtocol::INVALID )
   {}

   Pdu::Type            ePdu;          //!< Specifies the message type (PDU ID)
   ChannelType::Type    eChannelType;  //!< In output mode: Specifies the channel type that has been used to gain the result.
                                       //!< Only one bit is set according to the enumeration type, or all bits are set if no information is available.
   EvdoProtocol::Type   eEvdoProtocol; //!< EVDO Protocol ID
};

/**
This structure is used to configure message demodulation requests.
It is used in start measurement requests and it is used to issue BCH demodulation requests during measurement,
e.g. demodulation is requested on demand or to reset previous intermediate results of demodulation.
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
   There must be one instance of this structure for each BCH demodulation request,
   therefore different message ids can be measured on different channels.

   Example for start measurement:
            On the first channel the system parameters and the channel list messages shall be decoded, on the the 
            second channel the neighbor list additionally. For the channel with index 1,
            the channel list shall be decoded repeatedly every 10 seconds (100*100 ms) and the neighborlist shall be decoded on demand.
            This requires five structures containing the following parameter sets:
   @code
            {0, { Pdu::SYS_PARAMS, C2K_CH_TYPE_ALL }, ONCE, 0, 0},
            {0, { Pdu::CHAN_LIST, C2K_CH_TYPE_ALL }, ONCE, 0, 0},
            {1, { Pdu::SYS_PARAMS, C2K_CH_TYPE_ALL }, ONCE, 0, 0},
            {1, { Pdu::CHAN_LIST, C2K_CH_TYPE_ALL }, REPETITION, 100, 0},
            {1, { Pdu::NEIGHBOR_LIST, C2K_CH_TYPE_ALL }, ON_CMD, 0, 0},
            {2, { Pdu::SYNC, C2K_CH_TYPE_ALL }, ONCE, 0, 0},
   @endcode
   Example during measurement:
            On channel 0 the system parameter message demodulation shall be completely reset for the BTS with the ID 260
            and on the the second channel the neighbor list demodulation shall be decoded for the BTS identified by ID 116 with 
            a timeout for the request of 1 sec (10*100ms):
   @code
            {0, { Pdu::SYS_PARAMS, C2K_CH_TYPE_ALL }, BTS_RESET, 0, 260},
            {1, { Pdu::NEIGHBOR_LIST, C2K_CH_TYPE_ALL }, BTS, 10, 116}
   @endcode
   @note
   You are responsible to manage the memory when you use SetDemodulationSettings().
   All parameters which are not necessary for a certain request shall be set to 0.
   */
   struct SDemodRequest
   {
      SDemodRequest()
         : dwChannelIndex( 0 )
         , eDemodulationMode( DemodMode::ONCE )
         , wRepetitionDelayIn100ms( 0 )
         , wBtsId( 0 )
      {}

      DWORD                               dwChannelIndex;         //!< Index of the frequency in SChannelSettings::pdTableOfFrequencyInMHz.
      SPduSpec                            PduSpec;                //!< Specifies which messages shall be demodulated on which channel
      DemodMode::Type                     eDemodulationMode;      //!< The mode used for demodulation.
      WORD                                wRepetitionDelayIn100ms;//!< Repeat measurement after this delay time, if eDemodulationMode is REPETITION. 
                                                                  //!< Timeout for request, if eDemodulationMode is ON_CMD.
      WORD                                wBtsId;				      //!< This can be used to specify a specific basestation when the demodulation mode is set to
                                                                  //!< one of the following modes: DEMOD__BTS, BTS_FORCE, BTS_RESET, 
                                                                  //!< BTS_OLD, BTS_OLD_FORCE. In other words, this can only be used in conjunction
                                                                  //!< with the IssueRequest method, when requestes are specified for a special BTS. 
                                                                  //!< Attention: This is ignored during initial measurement setup.
   };

   /**
   \@attribute[type="array" size="dwCountOfRequests" sizeismember="true"]
   */
   SDemodRequest* pDemodRequests;
};

/**
This structure is used to configure the BCH demodulation process.
\@attribute[type="serializable"]
*/
struct SDemodulationSettings
{
   SDemodulationSettings()
      : lEcToIoThresholdInDB100_for_CDMA( 0 )
      , lEcToIoThresholdInDB100_for_EVDO( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /**
   Threshold for CDMA demodulation trials in 0.01dB.
   For default value and range see GetDemodThresholdLimits().
   */
   long                 lEcToIoThresholdInDB100_for_CDMA;

   /**
   Threshold for EVDO demodulation trials in 0.01dB.
   For default value and range see GetDemodThresholdLimits().
   */
   long                 lEcToIoThresholdInDB100_for_EVDO;

   /**
   Receiver front end to be used by the demodulator for this channel. 
   If no demodulation shall be provided, this shall be set to 0.
   Demodulation tasks are not allowed to contain multiple FE's.
   */
   DWORD                dwFrontEndSelectionMask;

   SDemodRequests       sStartMeasurementRequests;    //!< The requests for the general start measurement command.
};

/**
As an additional option, the CDMA 2000 interface can measure and demodulate EVDO channels as well. How this
is done can be controlled by the parameters made available in this structure.

Which EVDO channels shall be measured is specified as part of the SFrequencySetting structure vector
that is contained in the SChannelSettings structure.

@note
   To use EVDO scanning without scanning any CDMA channel, you must at least activate BCH Demodulation for EVDO_SYNC message.


\@attribute[type="serializable"]
*/
struct SEvdoControlSettings
{
   SEvdoControlSettings()
      : dwMeasRatePer1000Sec( 0 )
      , bStopCdma2000AfterSync( false )
      , dwFullSyncRatePer1000Sec( dwMaxFullSyncRatePer1000Sec )
      , dwShortSyncRatePer1000Sec( dwMaxShortSyncRatePer1000sec )
      , dwShortSyncRangeInChips( dwRecommendedDownTownSyncRangeInChips )
   {}

   /** 
   Measurement rate for EVDO measurements. 
   All channels together will be measured with this rate, i.e. one channel will be measured with an average
   of ChannelCount/dwMeasRatePer1000Sec measurements per 1000 sec. 
   
   The valid ranges are defined in the SMeasurementRate struct.
   */
   DWORD   dwMeasRatePer1000Sec;   

   /**
   If this is set to true, then the CDMA 2000 demodulation is stopped after the
   first complete demodulation of a sync channel. That helps saving time and it is not
   required. Once a synchronization is available, the time estimation can be performed.
   */
   bool    bStopCdma2000AfterSync;
      
   static const DWORD    dwMinFullSyncRatePer1000Sec  =  1000; //!< The minimum full sync rate is 1/sec
   static const DWORD    dwMaxFullSyncRatePer1000Sec  = 10000; //!< The maximum full sync rate is 10/sec

   /**
   When a full synchronization is performed, all the available data within a PN offset frame
   is searched for EVDO signals, and new and old basestations can be found/tracked.
   Since a lot of calculation has to be performed during such an operation, this is a 
   time consuming task. 
   To reduce the processing overhead introduced with that computation, the measurement rate 
   can be reduced to a much smaller amount than the original measurement rate.
   
   Attention: The settings must comply with the following relation:
              dwFullSyncRatePer1000Sec <= dwShortSyncRatePer1000Sec <= dwMeasRatePer1000Sec. 
   
   */
   DWORD   dwFullSyncRatePer1000Sec;

   static const DWORD    dwMinShortSyncRatePer1000sec  =  1000; //!< The minimum short sync rate is 1/sec
   static const DWORD    dwMaxShortSyncRatePer1000sec  = 10000; //!< The maximum short sync rate is 10/sec

   /**
   A short synchronization is normally done within shorter time intervals and not as time-consuming
   as the full synchronization, but it still needs processing time. So it should not be
   done as often as the normal demodulation.
   
   Compared with the full synchronization, during the short synchronization only a subrange of
   the measured signal is investigated. The size of the subrange is specified in the dwShortSyncRangeInChips
   parameter.
   
   Attention: The settings must comply with the following relation:
              dwFullSyncRatePer1000Sec <= dwShortSyncRatePer1000Sec <= dwMeasRatePer1000Sec. 
   
   */
   DWORD   dwShortSyncRatePer1000Sec;  


   static const DWORD    dwMinShortSyncRange  =    1; //!< At least one chip is searched (which is equal to ca 200m).
   static const DWORD    dwMaxShortSyncRange  = 1024; //!< Maximum chip count, round about 250 km radius. 
                                                      //!< If this is used, the short sync is equal to the full sync,
                                                      //!< since the signal investigates is the same in this case.
   static const DWORD    dwRecommendedDownTownSyncRangeInChips  = 160; //!< 160 chips is equal to 39 km
   static const DWORD    dwRecommendedUrbanSyncRangeInChips     = 320; //!< 320 chips is equal to 78 km

   /**
   This is the range the short sync will use to calculate the timing estimation when synchronizing
   with EVDO channels. During such a sync phase, synchronization will be performed around the expected
   point in time with the specified radius. 
   There are significant processing thresholds all 160 chips (39km). We recommend 160 chips (39km) 
   for downtown areas and 320 chips (78km) for urban areas. 
   */
   DWORD   dwShortSyncRangeInChips;
};


//** Request Settings **/
/**
By means of the functions: SetFrequencyTable() and SetMeasurementRate() internal
settings of the PN-Scanner may be changed and with the functions SelectReceiver(), SetResultBufferDepth(),
SetTimebaseSynchronisationMode() of CViComBasicInterfaceData change basic settings.
Except of frequency settings all other settings have default values.

This structure bundles these settings and will be returned as result from the GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   // basic settings
   DWORD                                                       dwReceiverIndex;              //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   TimeBaseSynchronisationMode::Type                           eTimeBaseSynchronisationMode; //!< Specifies with what device the internal clock shall be synchronized
   SResultBufferDepth                                          ResultBufferDepth;            //!< Number of results allowed in the result buffer

   // measurement specific settings
   SChannelSettings                                            ChannelSettings;              //!< Defines the measured frequencies.
   SPPSSettings                                                PPSSettings;                  //!< PPS Pulse
   SMaxVelocity                                                MaxVelocity;                  //!< Max speed of the test vehicle
   SyncChannelDemodulationMode::Type                           eSyncChannelDemodulationMode; //!< Sync Channel Demodulation Mode
   SDemodulationSettings                                       DemodulationSettings;		   //!< Settings that control the BCH demodulator
   SEvdoControlSettings                                        EvdoSettings;                 //!< Control settings how EVDO Channels shall be demodulated 
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
   The SExtendedPNOffset structure identifies a basestation in the system and provides PN-Offset measurement results for
   this basestation. 

   Measurement results with equal values {channel, ID} are very likely from the same physical BTS. As long as the arrival time of 
   a measured pilot signal fits the 80/3 ms frame pattern of a previously measured pilot signal the ID of the previously measured
   signal is reused for this measured signal. If two pilot signals transmitted from the same BTS have been measured at different 
   locations a new ID value is used for the recent signal if the previous pilot signal measurement was taken long ago or due to 
   strong reflections a fit of the frame timing is not assumed.

   In contrast to UMTS SC the PN-Offset in the measurement results is not used to indicate that results belong to
   the same BTS, as the PN-Offset is time-estimated or demodulated. E.g. two results for the same channel {ID = 156,
   PN-Offset = 30 (time estimated)} and {ID = 156, PN-Offset = 32 (demodulated)} belong to the same BTS. In this case
   the PN-Offset time estimation was wrong and may be corrected in a post process using this ID.
   \@attribute[type="serializable"]
   */
   struct SExtendedPNOffset
   {
      WORD           wPNOffset;     //!< PN offset: 0 .. 511, or SDefs::INVALID_WORD_VALUE if neither demodulation
                                    //!< nor time estimation could be performed successfully (normally the
                                    //!< case when a measurement is started).
      WORD           wIndicator;    //!< ID: 0,  1, ... see explanation above

      /**
      Source of the PN-Offset was a demodulation of the sync channel
      of this base station (even if it was long before we did this measurement). So the PN
      Offset value is correct.
      */
      bool           bOffsetSourceIsSyncChannelDemodulation;

      /**
      The PN-Offset was found by a time estimation. The circumstances
      (reliability etc.) of the time estimation are described in STimeEstimationResult. Only this
      or bOffsetSourceIsSyncChannelDemodulation is set.
      */
      bool           bOffsetSourceIsTimeEstimation;
   };

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

   //* PN-Scanner results common to all measurements:

   /**
   Channel index to which the measurement belongs.

   Each result is for only one of the selected RF channels. The channel index counts from 0 and corresponds to the
   array index in SChannelSettings::pdTableOfFrequencyInMHz.
   */
   DWORD             dwChannelIndex;

   /**
   The start time of the RF measurement which was used to produce the result.

   The dwPcTimeStampInMs is a conversion of the device time to the PC system time obtained with the 
   GetTickCount() function.
   The adjustment of possible time drift between internal device time and GetTickCount() as well as the resolution 
   of GetTickCount() causes a conversion error of a few milliseconds.
   dwPcTimeStampInMs may be used to synchronize PN-Scanner results with other measurement data in the system 
   or to determine the age of the PN-Scanner result = GetTickCount() - dwPcTimeStampInMs
   */
   DWORD             dwPcTimeStampInMs;

   /**
   If available, the device time in ns is reported, otherwise NULL.
   */
   ULONGLONG         u64DeviceTimeInNs;


   //* Channel impulse response (CIR) related measurement results.

   /**
   The CIR is given as a discrete power delay spectrum. The resolution (time distance of adjacent power values) is half the chip distance.
   This value is given as an integer constant in femto seconds (10^-15 sec). It is 10^15/(2 * 1228800) = 406901042.
   */
   static const DWORD  dwCirResolutionInFemtoSeconds = 4069010417L; //(DWORD)(1.0e15 / (2 * 1228800) + .5);

   /**
   Sometimes it is more convenient to operate with the reciprocal of the CIR resolution. This is the CIR sampling twice the chip rate.
   */
   static const DWORD  dwCirSamplingRateInHz = 2457600; // 2 * 1228800

   /**
   The power values of the CIR are given in 0.01dB resolution as 16 bit signed integer values in the range of (-327.67..327.67dBm).
   Some power values of a CIR may be below the noise limit or have not been measured. In this case the integer gets the invalid power value
   0x8000 = -32768 = -2^15 = -327.68dBm.
   */
   static const short  sInvalidPowerInDBm100 = - 32768;

   /**
   If a CIR measurement had no success (e.g. there is no CDMA2000 signal on air) then we get at least the received power of the
   specified channel.
   The pointer to this power value is valid (!= NULL) if a power measurement was performed.
   */
   short             *psAverageInbandPowerInDBm100;

   /**
   Tsmu Meas Time
   Exact time when the CIR measurement was taken. This can be used to analyze the fading situation.
   */
   double            *pdTsmuMeasTimeInSec;

   /**
   For most code power measurements and for F-PICH CIR measurements not only the absolute code power value (in 0.01dBm) is
   reported but in addition the absolute total inband power value (InbandPower) belonging to the code power measurement is reported.
   This makes it possible to calculate the Ec/Io for the measured code power by subtracting the InbandPower which belongs to the
   code power.
   \@attribute[type="serializable"]
   */
   struct SFPichCir
   {
      /**
      //!< The identifier for this measurement.
      */
      SExtendedPNOffset    ExtendedPNOffset;      

      /** The first CIR signal psCodePowerInDBm100[0] has arrived the receiver
      dwTimeDelayInCirSamples/dwCirSamplingRateInHz seconds delayed from u64DeviceTimeInNs
      this value can be used to display the time delay between different CIRs.
      */
      DWORD                dwTimeDelayInCirSamples;  

      /**
      The offset of the radio frame arrival time from the time it was sent from the base station.
      It uses the even GPS PPS pulse and the PN Offset value to determine the time offset.
      The radio frame beginning is calculated using the maximum CIR peak.

      3 CDMA radio frames build up one CDMA super frame with 80 ms length, 
      therefore one radio frame has a length of 80/3 ms = 26.6666666 ms.

      To determine the frame delay as value within +/- half of the radio length (+/- 26.666666 ms / 2),
      you must add this half RF length value, apply a full RF length modulo calculation and then subtract 
      the half length value:
      double dFrameDelay_ms = ( (1000*dFrameToaOffsetToPpsInSec  + 5 ms) modulo 10 ms ) - 5 ms

      In addition, it will take about 100 s from start of measurement, before the first results are provided.

      @note
      This feature requires a GPS antenna to be connected to the receiver.
      */
      float*               pfFrameToaOffsetToPpsInSec;

      /**
      The length of the CIR depends on distance between minimum and maximum delayed signal components.
      */
      DWORD                dwCountOfCodePowerValues;

      /**
      CIR power values in 0.01dBm in time distance dwCirResolutionInFemtoSeconds.
      \@attribute[type="array" size="dwCountOfCodePowerValues" sizeismember="true" filterable="true"]
      */
      short*               psCodePowerInDBm100;

      /**
      Power in the RF band during the CIR measurement.
      */
      short                sInbandPowerInDBm100;

      /**
      Integral measured power for the F-PICH signal (composition of all peaks).
      */
      short                sRSCPInDBm100;

      /**
      structure which contains results for separated peak values in the CIR
      \@attribute[type="serializable"]
      */
      struct SPeakInfo
      {
         /**
         The identifier for this measurement, NULL if not available.
         */
         SExtendedPNOffset*   pExtendedPNOffset;

         /**
         Signal of the peak has arrived the receiver dwTimeDelayInCirSamples/dwCirSamplingRateInHz seconds 
         delayed from u64DeviceTimeInNs.
         */
         DWORD                dwTimeDelayInCirSamples;  

         /**
         This power includes short term reflections and may be different to the power
         in the CIR graph at the same position.
         */
         short                sPeakPowerInDBm100;       

         /**
         The doppler shift. If invalid, the pointer is NULL;
         */
         short*               psDopplerShiftInHz;
      };

      /**
      Peak result list.
      */
      SViComList<SPeakInfo> ListOfPeaks;
   };

   /**
   Result list of F-PICH CIRs.
   */
   SViComList<SFPichCir> ListOfFPichCirs;


   /** Sync Channel Demodulation Result **/
   /**
   SyncChannelDemodulationResult:
   The contents of the synchronization message are described in standard TIA-2000.5-D.
   see spec C.S0005-D 3.7.2.3.2.26
   \@attribute[type="serializable"]
   */
   struct SSyncChannelDemodulationResult
   {
      BYTE    bP_REV;                  //!< Protocol revision level
      BYTE    bMIN_P_REV;              //!< Minimum protocol revision level
      WORD    wSID;                    //!< System identification
      WORD    wNID;                    //!< Network identification
      WORD    wPILOT_PN;               //!< Pilot PN sequence offset index
      LONGLONG  l64LC_STATE;           //!< Long code state
      ULONGLONG w64SYS_TIME;           //!< System time
      BYTE    bLP_SEC;                 //!< Number of leap seconds that have occurred since the start of the system time
      BYTE    bLTM_OFF;                //!< Offset of local time from system time
      BYTE    bDAYLT;                  //!< Daylight savings time indicator
      BYTE    bPRAT;                   //!< Paging channel data rate
      WORD    wCDMA_FREQ;              //!< Frequency assignment
      WORD    wEXT_CDMA_FREQ;          //!< Extended frequency assignment
      BYTE    bSR1_BCCH_NON_TD_INCL;   //!< SR1 Non-TD BCCH support indicator
      BYTE    bSR1_TD_INCL;            //!< SR1 TD BCCH support indicator
      BYTE    SR3_INCL;                //!< SR3 support indicator
      BYTE    bDS_INCL;                //!< Direct spread (DS) system and information available

      /**
      Bit error rate measured
      Given as Count of received bits with error and count of all received bits
      Can be invalid.
      */
      DWORD   dwCountOfReceivedBitsWithError;   //!< Count of received bits with error

      DWORD   dwCountOfAllReceivedBits;         //!< Count of all received bits, can be invalid, see below
      static const DWORD dwInvalidCountOfAllReceivedBits = 0xffffffff;
   }           *pSyncChannelDemodulationResult;
     

   /**
   If a message could be demodulated then the pointer to the MsgResult for this measurement is valid (!= NULL)
   \@attribute[type="serializable"]
   */
   struct SDemodResult
   {
      DWORD          dwChannelIndex;//!< Index of the channel used to measure the BTS.
      WORD           wBtsId;        //!< Unique identifier during the measurement for the measured station
      WORD           wFirstBtsId;   //!< The ExtendedSC contains an identifier for the BTS, but it may be the
                                    //!< case that the monitored BTS has been measured before. Only when a message
                                    //!< is decoded successfully, the id of the BTS can be determined uniquely.
                                    //!< For example, a BTS got the id 13 when detected for the first time. After
                                    //!< a while, the BTS is found again, but the id 49 is assigned to it since
                                    //!< the back-reference could not be surely made. After demodulation, it is clear
                                    //!< that the BTS was the same as before, then 13 is entered in this attribute.
                                    //!< If this is 0, then there is no such backreference.
      SPduSpec       PduSpec;       //!< Container holding information about the decoded message, i.e. which message id has been decoded and
                                    //!< from what type of channel the message has been retrieved.
      DWORD          dwBitCount;    //!< count of bits the demodulated message consists of
      BYTE           *pbBitStream;  //!< pointer to (dwBitCount + 7) / 8 bytes which contain the bits of the message. The first bit is stored in the
                                    //!< bit 1 of pbBitStream[0]. If dwBitCount is not a multiple of 8 then some tail bits in the last byte
                                    //!< pbBitStream[(dwBitCount + 7) / 8 - 1] should be ignored.
                                    //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
      DWORD          dwStartTime;   //!< Timestamp showing when the demodulation of the message started. Together with
                                    //!< dwStopTime this can be used to see how long the demodulation process took to
                                    //!< detect and interpret the data contained in the CDMA 2000 signal.
      DWORD          dwStopTime;    //!< Containing end time of demodulation (see dwStartTime).

   }                 *pDemodResult;

   //** Time Estimation
   /**
   Information for the time line estimation used for arrival times and time estimation of PN-Offset
   \@attribute[type="serializable"]
   */
   struct STimeEstimationResult
   {
      bool    bGpsPpsWasAvailable;        //!< TRUE if a PPS was available at the PN-scanner. It is necessary 
                                          //!< to use TimeBaseSynchronisationMode::PPS in CViComBasicInterface::SetTimebaseSynchronisationMode

      /**
      If the F-SYNC channel of any receiver pilot signal is not yet demodulated a time based PN-Offset estimation method is used 
      by the scanner. For this estimation the CDMA 2000 time line has to be synchronised with the internal receiver time. 
      The time based PN-Offset estimation needs at least one F-SYNC channel to be demodulated for both, the PPS synchronised 
      method and the CDMA2000 network synchronised method.

      The offset estimation method specifies how a time based PN offset measurement value was derived and how reliable the
      result is. 
      */
      enum etOffsetEstimationMethod
      {
         /**
         No F-Sync demodulation for any BTS was not yet successfully finished, therefore all reported PN-offset values are set 
         to wInvalidPNOffset. The transmit time is estimated by receive time by an internal algorithm. For a CDMA2000 channel 
         the scanner is in this state only short time normally.
         */
         OFFSET_ESTIMATION_METHOD_1 = 1,

         /**
         At least one F-Sync demodulation was successful but GPS PPS is not available or not used for time line estimation. 
         The PN offset values for pilot signals will be estimated from of the arrival time as long as no F-Sync demodulation 
         for this pilots are successful. The transmit time was estimated by several received pilots using internal algorithms.
         The scanner remains in this state if PPS synchronisation is not used for the receiver or if 
         SPPSSettings::iDelayOfPPSFallingEdgeIn100ns is not in a valid range or if no PPS pulses are fed in the receiver. 
         */
         OFFSET_ESTIMATION_METHOD_3 = 3,

         /**
         At least one F-Sync demodulation and GPS PPS were used for the CDMA2000 time line estimation. The PN offset values 
         for pilot signals can be estimated from arrival time as long as no F-Sync demodulation for this pilots are 
         successful.
         Transmit time has now GPS accuracy. To enable the GPS time based offset estimation method PPS synchronisation has to be used
         for the receiver and SPPSSettings::iDelayOfPPSFallingEdgeIn100ns is in a valid range and PPS pulses are fed in the receiver.
         The accuracy of the time line estimation directly depends on SPPSSettings::iDelayOfPPSFallingEdgeIn100ns.
         */
         OFFSET_ESTIMATION_METHOD_4 = 4
      }      eOffsetEstimationMethod;

      DWORD   dwAgeOfDemodulationInMs;    //!< Age of demodulation in ms or SDefs::INVALID_DWORD_VALUE if no demodulation was used.
                                          //!< Once a demodulation was successful, this increases with every measurement
                                          //!< (for one bastesation).

      DWORD   dwAgeOfGpsPpsInMs;          //!< Age of GPS PPS in ms or SDefs::INVALID_DWORD_VALUE if PPS not available

      static const long lInvalidPPSDelayInNs = 1000 * 1000 * 1000;   //!< invalid value: 1 sec (1e9 ns)

      /**
      The measured GPS PPS Pulse time difference in ns. This value is valid, if bGpsPpsWasAvailable is TRUE
      and if the eOffsetEstimationMethod is OFFSET_ESTIMATION_METHOD_3. The range is -1ms (-1000000).. 999ms (999000000).
      It shows the GPS arrival time for the case that the measurement results (delays) are correct.

      If a time delay of a BTS signal is known, than the measurement error of the PN-Scanner result can be calculated.
      The same error applies to this value. E.g. PN_Offest 271 was measured. The frame of this signal is
      transmitted 271 * 64/1228800sec =  14114.58us after a frame with offset 0. Assume the distance to the BTS is 5km.
      So a measurement result of 14114.58us + 5000m / (300m/us) = 14114.58us + 16.67us = 14131.31us is the
      expected measurement value. If for instance a delay of 14141.31us was measured as a delay value for this BTS
      then the measurement result has an error of +10us.

      Now we can reduce both the delay value and the PPS delay value by 10us to get correct results.

      The corrected value times 0.01 can be used to be set in the SPPSSettings::iDelayOfPPSFallingEdgeIn100ns in the start command
      to calibrate the GPS delay.
      */
      long    lMeasuredPPSDelayInNs;

   }           *pTimeEstimationResult;

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
      : dwBitCount( 0 )
      , pbBitStream( NULL )
   {}

   SPduSpec             PduSpec;           //!< Channel type and message id of the message stored in the bitstream
   DWORD                dwBitCount;        //!< Number of bits for this PDU.
   BYTE*                pbBitStream;       //!< Pointer to a buffer containing the message bits, organized in the same 
                                           //!< way as returned in the measurement result above (in terms of bit orientation).
                                           //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
};

/**
Result from the Layer 3 Decoder.
The string returned via this structure is formatted according to the appropriate CDMA / EVDO standard.
\@attribute[type="serializable"]
*/
struct SL3DecoderResult
{
   Pdu::Type            ePdu;             //!< PDU contained.
   DWORD                dwStringLength;   //!< Number of decoding result text bytes in the buffer (incl. terminating '\0').
   char*                pcPduText;        //!< Pointer to a buffer containing the decoded PDU text, as zero terminated ASCII string.
                                          //!< \@attribute[type="array" size="dwStringLength" sizeismember="true"]
};

} // namespace CDMA
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
