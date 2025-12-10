// ViComLteInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_LTE       27     // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)      // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                     // get a definite structure alignment for CViComLteInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace LTE
{
/**
Data containers used for ViCom LTE interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_LTE,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
Definition of the channel type.
\@attribute[type="serializable"]
*/
struct FrameStructureType { enum Type {
   FDD = 1,                         //!< Frequency Division Duplex Mode
   TDD = 2                          //!< Time Division Duplex Mode
}; };

/**
Configuration of the measurement that is performed on one single frequency.
\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   SFrequencySetting()
      : dwFrontEndSelectionMask( SRFPort::RF_1 )
      , dCenterFrequencyInHz( 0 )
      , dwSymbolsPerSlotMask( dwDefaultSymbolsPerSlot )
      , enFrameStructureType( FrameStructureType::FDD )
      , dwAvgBlockCountPer1000Sec( 10000 )
      , wNarrowbandRefSignalMeasMode( wNARROWBAND_RSRP_RSRQ | wCENTER_RSCINR_1x1080KHZ | w1MHZ_FILTER_NOISE_FOR_15RB )
      , bUpDownLinkMask( bAllUpDownLinkConfigurations )
      , wSpecialSubframe1ConfigurationMask( wAllSpecialSubFrameConfigurations )
      , wSpecialSubframe6ConfigurationMask( wAllSpecialSubFrameConfigurations )
      , enBandwidthCtrlMode( BandwidthCtrlMode::BW_KNOWN )
      , wNumberOfResourceBlocks( wNumberOfResourceBlocksFor10000kHzChBw )
      , enSSyncToPSyncRatioType( SSyncToPSyncRatioType::RatioRange )
      , enMbmsConfigCtrlMode( MbmsConfigCtrlMode::MBMS_NOT_PRESENT )
   {}

   /** 
   Receiver front end to be used by the scanner for this channel. This is only used for
   narrowband measurement tasks, not for wideband measurements / demodulation.

   Narrowband scanner tasks are not allowed to contain multiple FE's.
   */
   DWORD                      dwFrontEndSelectionMask;

   /**
   The center frequency on which the LTE signals shall be measured.
   For value range see SReceiver::SSupportedFrequencies.
   */
   double                     dCenterFrequencyInHz;

   static const DWORD         dwLTE_6SYMBOLS       = 2;                       //!< 6 OFDM Symbols
   static const DWORD         dwLTE_7SYMBOLS       = 4;                       //!< 7 OFDM Symbols

   /** 
   Default symbol configuration that should be used to set dwSymbolsPerSlotMask if no details are known about the network to be measured upfront.
   */
   static const DWORD         dwDefaultSymbolsPerSlot = dwLTE_6SYMBOLS | dwLTE_7SYMBOLS;

   /** 
   Number of OFDM symbols per slot. Can be any combination of dwLTE_xSYMBOLS constants as defined above. Must not be 0.
   */
   DWORD                      dwSymbolsPerSlotMask;

   /** 
   Frame structure type of the channel aka duplex mode: FDD or TDD
   */
   FrameStructureType::Type   enFrameStructureType;

   /**  
   Averaged measurement rate in 0.001 Hertz. The device attempts to perform the desired number of measurements, 
   but it is not guaranteed that the number of measurements is performed each second. 
   In general, the device tries to achieve the desired rate averaged over a longer period of time.
   For default value and range see GetMeasRateLimits().
   */
   DWORD                      dwAvgBlockCountPer1000Sec;

   static const WORD          w1MHZ_FILTER_NOISE_FOR_15RB    = 64;    //!< Filter noise reduction for the narrowband RS-CINR calculation assuming
                                                                      //!< a wideband LTE signal (BW >= 3 MHz).
                                                                      //!< This is done only for RS-CINR values > 20 dB.
   static const WORD          w1MHZ_FILTER_NOISE_FOR_6RB     = 32;    //!< Filter noise reduction for the narrowband RS-CINR calculation assuming
                                                                      //!< a narrowband LTE signal (BW = 1.4 MHz / 6 RBs).
                                                                      //!< This is done only for RS-CINR values > 20 dB.

   static const WORD          wCENTER_RSCINR_1x1080KHZ       = 16;    //!< The reference carrier narrowband RS-CINR is measured in 1 section within
                                                                      //!< the innermost 1080 kHz band of the LTE channel.
   static const WORD          wCENTER_RSCINR_2x540KHZ        = 8;     //!< The reference carrier narrowband RS-CINR is measured in 2 sections within
                                                                      //!< the innermost 1080 kHz band of the LTE channel.
   static const WORD          wCENTER_RSCINR_3x360KHZ        = 4;     //!< The reference carrier narrowband RS-CINR is measured in 3 sections within
                                                                      //!< the innermost 1080 kHz band of the LTE channel.
   static const WORD          wCENTER_RSCINR_6x180KHZ        = 2;     //!< The reference carrier narrowband RS-CINR is measured in 6 sections within
                                                                      //!< the innermost 1080 kHz band of the LTE channel.

   static const WORD          wNARROWBAND_RSRP_RSRQ          = 1;     //!< Enable narrowband RSRP/Q and RS-CINR measurements.

   /** 
   Narrowband Reference Signal measurement modes. Used to configure if narrowband RSRP/Q and RS-CINR measurements
   shall be done or not. 0 means no measurements.
   Measurements are performed only if enMbmsConfigCtrlMode is set to MBMS_NOT_PRESENT.
   Note that the carrier sections are only than on resource block boundaries, when the total number
   of resource blocks is even (e.g. in 10 MHz or 20MHz). If the number of resource blocks is odd
   (e.g. in 5MHz bandwidth) the boundary of the measurement sections are in the middle of a resource block.
   That gives a reason to use sub band notation in kHz rather than Resource Block notation.
   For other measurements see defines above. wNARROWBAND_RSRP_RSRQ is used to generally activate narrowband 
   RSRP/Q and RS-CINR measurements. Any combination of wCENTER_RSCINR_xxx is allowed. As the 1 MHz input filter
   is designed for SYNC signal measurements we produce some internal interference with this filter. This interference
   can be taken into account for the RS-CINR measurement. Therefore optionally set one of the two w1MHZ_FILTERN_NOISE_FOR_xxx defines.
   The narrwoband RS-CINR measurement is a requirement for wideband RS-CINR measurements. Therefore set a least
   wNARROWBAND_RSRP_RSRQ | wCENTER_RSCINR_6x180KHZ if you want to do wideband RS-CINR measurements.
   */
   WORD                       wNarrowbandRefSignalMeasMode;

   static const BYTE          bAllUpDownLinkConfigurations  = 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 |1<<5 | 1<<6;     //!< Uplink-downlink configuration 0 to 6.

   /**
   This value is used for RSRP and RSRQ measurements in TDD configurations. When no bit is set than no RSRP measurement is done in TDD networks. 
   Else we try the given uplink-downlink configuration specified by the bits set. 
   E.g. when uplink-downlink configuration 2 and 4 is possible in the network than bit 2 (1<<2) and 4 (1<<4) shall be set. 
   Configuration 0 to 6 are supported.
   */
   BYTE                       bUpDownLinkMask;

   static const WORD          wAllSpecialSubFrameConfigurations  = 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 |1<<5 | 1<<6 | 1<<7 |1<<8 | 1<<9;     //!< Special subframe configuration 0 to 9.

   /**
   This value is used for RSRP and RSRQ measurements in TDD configurations. When no bit is set than no RSRP measurement is done in TDD networks.
   Else we try the given special subframe configuration specified by the bits set.
   E.g. when special subframe configuration 2 and 4 is possible in the network than bit 2 (1<<2) and 4 (1<<4) shall be set.
   Configuration 0 to 9 are supported.
   */
   WORD                       wSpecialSubframe1ConfigurationMask;

   /**
   This value is used for RSRP and RSRQ measurements in TDD configurations. When no bit is set than no RSRP measurement is done in TDD networks.
   Else we try the given special subframe configuration specified by the bits set.
   E.g. when special subframe configuration 2 and 4 is possible in the network than bit 2 (1<<2) and 4 (1<<4) shall be set.
   Configuration 0 to 9 are supported.
   */
   WORD                       wSpecialSubframe6ConfigurationMask;

   /** 
   Bandwidth control modes. Used to configure if the number of resource blocks to use for wideband and MIMO measurements
   on this channel shall be determined from a MIB or are known before.
   \@attribute[type="serializable"]
   */
   struct  BandwidthCtrlMode { enum Type {
         BW_KNOWN                   = 0,     //!< The number of eNobeB resource blocks is taken from wNumberOfResourceBlocks.
                                             //!< This mode shortens the delay to first measurement results, as no MIB
                                             //!< needs to be decoded up-front.
         BW_FROM_MIB_ONCE           = 1,     //!< This mode uses the MIB from the first detected cell ID to determine the bandwidth
                                             //!< for all eNodeB's on this channel.
         BW_FROM_MIB_ONCE_EACH_CELL = 2      //!< This mode uses the MIB from each detected cell ID to determine the bandwidth for that cell.
                                             //!< This mode handles the scenario where different cell IDs on the same channel frequency may
                                             //!< have different bandwidth configurations.
                                             //!< Note: This mode is less efficient and requires more resources than modes LTE_BW_FROM_MIB_ONCE
                                             //!< or LTE_BW_KNOWN, therefore if it is known that all cells have the same bandwidth configuration,
                                             //!< it is recommend that one of the other modes be used.
   }; };

   BandwidthCtrlMode::Type    enBandwidthCtrlMode;

   static const WORD          wNumberOfResourceBlocksFor1400kHzChBw  = 6;     //!< 1.4 MHz LTE channel bandwidth.
   static const WORD          wNumberOfResourceBlocksFor3000kHzChBw  = 15;    //!< 3   MHz LTE channel bandwidth.
   static const WORD          wNumberOfResourceBlocksFor5000kHzChBw  = 25;    //!< 5   MHz LTE channel bandwidth.
   static const WORD          wNumberOfResourceBlocksFor10000kHzChBw = 50;    //!< 10  MHz LTE channel bandwidth.
   static const WORD          wNumberOfResourceBlocksFor15000kHzChBw = 75;    //!< 15  MHz LTE channel bandwidth.
   static const WORD          wNumberOfResourceBlocksFor20000kHzChBw = 100;   //!< 20  MHz LTE channel bandwidth.

   /**
   Number of resource blocks to use for this channel, if BandwidthCtrlMode is set to BW_KNOWN.
   E.g. 50 RB's for 10 MHz channel bandwidth.
   */
   WORD                       wNumberOfResourceBlocks;      

   /**
   To improve the synchronization speed of the LTE scanner, it is possible
   to define power ratios between the S-SYNC and the P-SYNC channel. These
   ratios are used to find the two channels in the signal that has been
   measured.

   One way is to define up to 6 fixed ratios for the power ratio between
   the P-SYNC and the S-SYNC channel. The list of these values is used to
   find such a constellation of S-SYNC and P-SYNC on the carrier. There is no
   interpolation or similar applied to them, each value is tested if a
   matching sequence can be found.

   The other possibility is to define a range in which the power ratio must
   reside. This is the more general approach if less information about the
   network(s) measured is known upfront.

   Note that the scanner will also synchronize if those values are of no
   help, but it'll take somewhat longer to do so.
   \@attribute[type="serializable"]
   */
   struct SSyncToPSyncRatioType { enum Type {
      RatioList,     //!< use settings stored in SSyncToPSyncRatioSettings::RatioList
      RatioRange,    //!< use settings stored in SSyncToPSyncRatioSettings::RatioRange
   }; };

   /**
   Specify what kind of S-SYNC to P-SYNC ratios are specified in the
   SSyncToPSyncRatio member, either the list of ratio values or a range.

   A power ratio is specified in dB. A value of 0 means that both signals have
   the same power, a value of 3 means the S-SYNC has a signal power which is
   about two times stronger than the on the P-SYNC.
   */
   SSyncToPSyncRatioType::Type enSSyncToPSyncRatioType;
   /**
   \@attribute[type="serializable"]
   */
   union SSyncToPSyncRatioSettings
   {
      SSyncToPSyncRatioSettings()
      {
         RatioList.dwValueCount = 2;
         RatioList.afSSyncToPSyncRatioInDB[0] = static_cast<float>(sMinSSyncToPSyncRatioInDB);
         RatioList.afSSyncToPSyncRatioInDB[1] = static_cast<float>(sMaxSSyncToPSyncRatioInDB);
         for( int i = 2; i < 5; i++ )
            RatioList.afSSyncToPSyncRatioInDB[i] = 0;

         RatioRange.fLowerRatioInDB = static_cast<float>(sMinSSyncToPSyncRatioInDB);
         RatioRange.fUpperRatioInDB = static_cast<float>(sMaxSSyncToPSyncRatioInDB);
      }

      static const short sMinSSyncToPSyncRatioInDB = -6; //!< Minimum allowed value for _StructRatioList::afSSyncToPSyncRatioInDB
                                                         //!< and _StructRatioRange::fLowerRatioInDB
      static const short sMaxSSyncToPSyncRatioInDB = 0;  //!< Maximum allowed value for _StructRatioList::afSSyncToPSyncRatioInDB
                                                         //!< and _StructRatioRange::fUpperRatioInDB
     /**
     \@attribute[type="serializable"]
     */
     struct _StructRatioList
      {
         /**
         1 to 6 valid power ratios; The values need not to be ordered because
         ordering is done internally.
         */
         float                afSSyncToPSyncRatioInDB[6];

         /**
         A value between 1 and 6, specifying how many of the values stored
         in afSSyncToPSyncRatioInDB array shall be used.
         */
         DWORD                dwValueCount;
      };
      
      /**
      @details \@attribute[type="unionfield" discriminator="enSSyncToPSyncRatioType" value="SSyncToPSyncRatioType::Type::RatioList"]
      */
      _StructRatioList RatioList;

      /**
      \@attribute[type="serializable"]
      */
      struct _StructRatioRange
      {
         /**
         The lowest ratio that shall apply to the relation between P-SYNC
         and S-SYNC. Possible values are in between fLowerRatioInDB and
         fUpperRatioInDB.
         */
         float                fLowerRatioInDB;
         /**
         The highest ratio that shall apply to the relation between P-SYNC
         and S-SYNC. Possible values are in between fLowerRatioInDB and
         fUpperRatioInDB.
         */
         float                fUpperRatioInDB;
      };

      /**
      @details \@attribute[type="unionfield" discriminator="enSSyncToPSyncRatioType" value="SSyncToPSyncRatioType::Type::RatioRange"]
      */
      _StructRatioRange RatioRange;
      
   } SSyncToPSyncRatio;

   /** 
   MBMS configuration control mode. Specifies how the MBMS configuration is determined by the MIMO and MBMS scanners.
   \@attribute[type="serializable"]
   */
   struct MbmsConfigCtrlMode { enum Type {
         MBMS_CONFIG_AUTO   = 1,    //!< The MBMS configuration for each cell is determined by the SIB2 and SIB13 detected for that cell.
                                    //!< Demodulation requests for SIB2 and SIB13 must be configured manually in this case. Minimally,
                                    //!< requests should have eDemodulationMode set to DemodMode::ONCE, which will cause the first MBMS
                                    //!< configuration detected to be used for the remainder of the measurement.
                                    //!< Prior to MBMS configuration detection, MIMO scanner measurements are only performed on subframes
                                    //!< guaranteed to be non-MBFSN subframes.
                                    //!< This mode ignores values set in SFrequencySetting::MbmsManualConfig.
         MBMS_CONFIG_MANUAL = 2,    //!< The MBMS configuration is determined by the values set in SFrequencySetting::MbmsManualConfig.
         MBMS_NOT_PRESENT   = 0     //!< MIMO scanner measurements are performed on all subframes.
   }; };

   MbmsConfigCtrlMode::Type  enMbmsConfigCtrlMode;

   /**
   Configuration for wideband RS-CINR measurements.
   \@attribute[type="serializable"]
   */
   struct SWidebandRsCinrSettings
   {
      SWidebandRsCinrSettings()
         : wWidebandRsCinrMeasMode( wWIDEBAND_RS_CINR | wSUBBAND_MEASUREMENTS )
         , dwAvgBlockCountPer1000Sec( 1000 )
         , wNumberOfRBsInSubband( wMinRBsInSubband )
         , bForceNoGap( TRUE )
         , bMaxCountOfeNodeBs( 5 )
         , sMinCenterRsrpInDBm100( -13000 )
         , wMaxRsrpDiffToBestCellInDB100( 1000 )
         , sMinRsCinrInDB100( -1000 )
         , dwFrontEndSelectionMask( SRFPort::RF_1 )
         , bTransmitAntennaSelectionMask( 1<<0 | 1<<1 | 1<<2 | 1<<3 )
      {}

      static const WORD         wWIDEBAND_RS_CINR     = 1;     //!< Enable wideband RS-CINR measurements.
      static const WORD         wSUBBAND_MEASUREMENTS = 2;     //!< Enable additional subband measurements.

      /** 
      Wideband Reference Signal measurement modes. Used to configure if wideband RS-CINR measurements shall be done
      or not. 0 means no measurements. Use SFrequencySettings::wNarrowbandRefSignalMeasMode to enable narrowband RS-CINR,
      as these results are required for wideband RS-CINR!
      Subband measurements are not possible for signals having S-SYNC CINR < -5 dB!
      */
      WORD                      wWidebandRsCinrMeasMode;

      /**  
      Averaged measurement rate in Hertz. The device attempts to perform the desired number of measurements, 
      but it is not guaranteed that the number of measurements is performed each second. 
      In general, the device tries to achieve the desired rate averaged over a longer period of time.
      For default value and range see GetWbMeasRateLimits().
      */
      DWORD                     dwAvgBlockCountPer1000Sec;

      static const WORD         wMinRBsInSubband   = 1;    //!< Minimum resource blocks for wNumberOfRBsInSubband
      static const WORD         wMaxRBsInSubband   = 5;    //!< Maximum resource blocks for wNumberOfRBsInSubband

      /**
      Number of resource blocks to specify the subband size for the wideband RS-CINR calculation if wSUBBAND_MEASUREMENTS is set.
      For LTE-FDD channel types 1 resource block is recommended. In LTE-TDD configurations with a low number of downlink
      subframes wNumberOfRBsInSubband > 1 may improve the accuracy of the wideband RS-CINR measurement results. E.g. in 
      LTE-TDD some subframe configurations have only 2 DL subframes per frame. In this case, the subband will need to be
      wider to gather enough reference signals for an equivalent accuracy in the wideband RS-CINR measurement as in LTE-FDD.
      */
      WORD                      wNumberOfRBsInSubband;

      /**
      If set to true, then wNumberOfRBsInSubband gets adjusted to avoid gaps between subbands. If wNumberOfRBsInSubband 
      is not a divider of the total number of resource blocks in the LTE bandwidth, then possible gaps between subbands
      around the DC subcarrier are eliminated by automatically increasing the subbandwidth.
      */
      bool                      bForceNoGap;

      /** 
      Maximum number of eNodeB's per channel index for which wideband RS-CINR, RSRP / Q measurements shall be done.
      This limitation is only valid for subband measurements, e.g. when wSUBBAND_MEASUREMENTS is set.
      */
      BYTE                      bMaxCountOfeNodeBs;

      /** 
      The minimum required subband RSRP value in 0.01 dB (obtained from the narrowband LTE scanner) for the eNodeB
      to enable the subband measurements.
      */
      short                     sMinCenterRsrpInDBm100;

      /**
      The maximum difference (distance) allowed in subband RSRP in 0.01 dB (obtained from the narrowband LTE scanner)
      from the best received eNodeB to enable the subband measurements.
      */
      WORD                      wMaxRsrpDiffToBestCellInDB100;

      /** 
      The minimum required reference signal CINR value in 0.01 dB (obtained from the narrowband LTE scanner) for the eNodeB 
      to enable the subband measurements.
      */
      short                     sMinRsCinrInDB100;

      /**
      Specify which FE should be used for wideband RS/CINR measurements and MIMO measurements.
      In case of SMimoSettings::wMimoMeasMode != 0 at least two front ends have to be specified.
      For TSMW this could either be SRFPort::RF_1 or SRFPort::RF_2 or both.
         
      */
      DWORD                     dwFrontEndSelectionMask;

      /**
      Specifies a set of transmit antenna ports to use for the measurements. At least one bit has to be set.
      E.g. for transmit antenna port 0 and 3, bit 0 (1<<0) and 3 (1<<3) shall be set. 
      */
      BYTE                      bTransmitAntennaSelectionMask;

   } WidebandRsCinrSettings;

   /**
   Configuration for full-channelbandwidth RSSI and spectrum measurements.
   \@attribute[type="serializable"]
   */
   struct SRssiSettings
   {
      SRssiSettings()
         : wRssiMeasMode( wRSSI_NORMAL_MODE )
         , dwFrontEndSelectionMask( SRFPort::RF_1 )
      {}

      static const WORD         wRSSI_NORMAL_MODE       = 1;   //!< The RSSI and spectrum measurements are done for a certain time
                                                               //!< range without respect to frame configuration (e.g. uplink down-
                                                               //!< link for TDD)
      static const WORD         wRSSI_TDD_UL_MODE       = 2;   //!< The TDD downlink RSSI and spectrum measurements are done as soon
                                                               //!< as the uplink downlink configuration for the channel is available.
      static const WORD         wRSSI_TDD_DL_MODE       = 4;   //!< The TDD uplink RSSI and spectrum measurements are done as soon
                                                               //!< as the uplink downlink configuration for the channel is available.
      static const WORD         wRSSI_DL_ALL_TS_MODE    = 8;   //!< One RSSI measurement for each downlink slot (0.5ms) will be performed
                                                               //!< over a whole frame (10ms). This measurement is synchronized with frame
                                                               //!< boundary of the strongest detected LTE cell.  This measurement can be 
                                                               //!< used for both FDD and TDD networks.  

      /** 
      Rssi measurement configuration. Can be any combination of wRSSI_xxx_MODE constants as defined above.
      0 means no measurements.
      */
      WORD                      wRssiMeasMode;

      /**
      Specify which TSMW FE should be used for RSSI measurements. For TSMW this could either be
      SRFPort::RF_1 or SRFPort::RF_2 or both.
      */
      DWORD                     dwFrontEndSelectionMask;

   } RssiSettings;

   /**
   Configuration for MIMO measurements.
   \@attribute[type="serializable"]
   */
   struct SMimoSettings
   {
      SMimoSettings()
         : wMimoMeasMode( 0 )
         , wTimeResolutionInMs( 10 )
         , dwMimoResultMaskFor2x2( dwMIMO_RES_CHAN_MATRIX | dwMIMO_RES_COND_NUMBER | dwMIMO_RES_RANK )
         , dwMimoResultMaskFor2x4( 0 )
         , sCinrThresholdForRankInDB100( 0 )
         , bMaxCountOfeNodeBs( 5 )
         , sMinCenterRsrpInDBm100( -13000 )
         , wMaxRsrpDiffToBestCellInDB100( 1000 )
         , sMinRsCinrInDB100( -1000 )
         , bEnableThroughputEstimation( false )
      {}

      static const WORD         wMIMO_MODE_2x2       = 1;   //!< Enable 2x2 MIMO measurements for an eNodeB as soon as transmit
                                                            //!< antenna port 0 and 1 have been detected by the scanner.
      static const WORD         wMIMO_MODE_2x4       = 2;   //!< Enable 2x4 MIMO measurements for an eNodeB as soon as transmit
                                                            //!< antenna port 0, 1, 2 and 3 have been detected by the scanner.

      /**
      MIMO measurement configuration. Use wMIMO_MODE_2xx constants defined above. Set to 0 if no MIMO measurement should be done.
      Use SFrequencySettings::wNarrowbandRefSignalMeasMode to enable narrowband RS-CINR, as these results are required for MIMO!
      */
      WORD                      wMimoMeasMode;

      /**
      Defines the time interval between MIMO measurements which are calculated per resource block at a give time position within
      the captured IQ data. For example wTimeResolutionInMs of 5ms and 50ms of captured IQ data leads to up to 10 results. 
      For TDD the time resolution has to be a multiple of the 10 ms LTE frame duration. Set to 0 for only one result per IQ acquisition.
      */
      WORD                      wTimeResolutionInMs;

      static const DWORD        dwMIMO_RES_CHAN_MATRIX = 1;   //!< Enables the return of the MIMO channel matrix (H matrix)
                                                              //!< If 2x2 and 2x4 MIMO measurements are enabled, only the 2x4 matrix is
                                                              //!< returned, since the 2x2 matrix is a subset of the 2x4 matrix.
      static const DWORD        dwMIMO_RES_COND_NUMBER = 2;   //!< Enables the return of the condition numbers for all measured H matrices.
                                                              //!< If 2x2 and 2x4 MIMO measurements are enabled, 2 condition numbers
                                                              //!< are returned.
      static const DWORD        dwMIMO_RES_RANK        = 4;   //!< Enables the return of the theoretical rank of the H matrix with respect
                                                              //!< to a specified CINR ratio for the minimum rank. For 2x2 and 2x4 MIMO
                                                              //!< measurements each rank value is returned.
 
      /**
      MIMO result selection masks for 2x2 and 2x4 MIMO measurements. Can be any combination of dwMIMO_RES_xxx constants as defined above.
      */
      DWORD                     dwMimoResultMaskFor2x2;
      DWORD                     dwMimoResultMaskFor2x4;

      /**
      Minimum CINR for a MIMO layer to be included in the rank calculation in 0.01 dB.
      */
      short                     sCinrThresholdForRankInDB100;

      /** 
      Maximum number of eNodeB's per channel index for which MIMO measurements shall be done.
      */
      BYTE                      bMaxCountOfeNodeBs;

      /** 
      The minimum required subband RSRP value in 0.01 dB (obtained from the narrowband LTE scanner) for the eNodeB
      to enable the measurements.
      */
      short                     sMinCenterRsrpInDBm100;

      /**
      The maximum difference (distance) allowed in subband RSRP in 0.01 dB (obtained from the narrowband LTE scanner)
      from the best received eNodeB to enable the measurements.
      */
      WORD                      wMaxRsrpDiffToBestCellInDB100;

      /** 
      The minimum required reference signal CINR value in 0.01 dB (obtained from the narrowband LTE scanner) for the eNodeB 
      to enable the measurements.
      */
      short                     sMinRsCinrInDB100;
         
      /** 
      Enables throughput estimation to get results for estimated channel capacity and estimated CQI.
      */
      bool                      bEnableThroughputEstimation;

   } MimoSettings;

   /**
   Configuration for MBMS. This is only used if SFrequencySetting::enMbmsConfigCtrlMode is set to MBMS_CONFIG_MANUAL.
   Additional information for these fields can be found in 3GPP TS 36.331 section 6.3.7. See MBSFN-AreaInfo,
   MBMS-NotificationConfig, and MBSFN-SubframeConfig.
   \@attribute[type="serializable"]
   */
   struct SMbmsManualConfig
   {
      SMbmsManualConfig()
         : bMbsfnAreaId( 0 )
         , bNonMbsfnRegionLength( 2 )
         , bNotificationIndicator( 0 )
         , bMcchOffset( 0 )
         , bSfAllocInfoBitMap( 0x20 )
         , bSignallingMCS( 2 )
         , bMcchRepetitionPeriod( 32 )
         , bMcchModificationPeriod( 512 )
         , dwSubframeAllocationBitMap( 0x3f )
         , bOneFrameOrFourFrames( 1 )
         , bRadioFrameAllocationPeriod( 1 )
         , bRadioFrameAllocationOffset( 0 )
      {}

      /**  
      MBSFN area ID. The range of valid values is [0, 255].
      */
      BYTE                 bMbsfnAreaId;

      /**  
      The count of symbols present in each MBSFN subframe that constitute the non-MBFSN region.
      The range of valid values is [1, 2].
      */
      BYTE                 bNonMbsfnRegionLength;

      /**  
      Indicates the position of the bit corresponding to the MBSFN area in the MCCH information change
      notification. Value 0 corresponds with the least significant bit and so on.

      The range of valid values is [0, 7].
      */
      BYTE                 bNotificationIndicator;

      /**
      The frame offset to determine the initial radio frame containing MCCH information.
      The range of valid values is [0, 7].
      */
      BYTE                 bMcchOffset;

      /**
      Indicates which subframes carry MCCH information for applicable radio frames. bSfAllocInfoBitMap is
      a 6-bit value (that is, the value is masked by 0x3F == 0b111111).

      For FDD, subframes #1, #2, #3, #6, #7, and #8 may carry MCCH information. Respectively,
      positions 5, 4, 3, 2, 1, and 0 (where position 0 refers to the least significant bit) of
      bSfAllocInfoBitMap indicate which of these subframes actually carry MCCH information.

      For TDD, subframes #3, #4, #7, #8, and #9 may carry MCCH information, and, similarly,
      positions 5, 4, 3, 2, and 1 of bSfAllocInfoBitMap indicate which of these subframes actually carry MCCH 
      information. For TDD, the bit in position 0 is ignored.
      */
      BYTE                 bSfAllocInfoBitMap; 

      /**  
      Indicates the Modulation and Coding Scheme (MCS) used for applicable subframes in the MBSFN area.
      The set of valid values is {2, 7, 13, 19}.
      */
      BYTE                 bSignallingMCS;    

      /**  
      Specifies the interval between transmissions of MCCH information in radio frames. The set of valid 
      values is {32, 64, 128, 256}.
      */
      WORD                 bMcchRepetitionPeriod;

      /**  
      Indicates the minimum count of radio frames that must occur between transmissions of MCCH information
      for the contents of the transmissions to be different. The set of valid values is {512, 1024}.
      */
      WORD                 bMcchModificationPeriod;

      /**
      Indicates which subframes in an MBSFN frame are allocated for MBSFN.

      If bOneFrameOrFourFrames is 1, then dwSubframeAllocationBitMap is a 6-bit value (that is, the value is
      masked by 0x3F == 0b111111) that indicates which subframes in one MBSFN frame are allocated for MBSFN.

      For FDD and 'oneFrame', subframes #1, #2, #3, #6, #7, and #8 can be allocated for MBSFN. Respectively,
      positions 5, 4, 3, 2, 1, and 0 (where position 0 refers to the least significant bit) of
      dwSubframeAllocationBitMap indicate which of these subframes are actually allocated for MBSFN.

      For TDD and 'oneFrame', subframes #3, #4, #7, #8, and #9 can be allocated for MBSFN, and, similarly,
      positions 5, 4, 3, 2, and 1 of dwSubframeAllocationBitMap indicate which of these subframes are actually
      allocated for MBSFN. For TDD, the bit in position 0 is ignored.

      If bOneFrameOrFourFrames is 4, then dwSubframeAllocationBitMap is a 24-bit value (that is, the value is
      masked by 0xFFFFFF == 0b111111111111111111111111) that indicates which subframes in four consecutive
      MBSFN frames are allocated for MBSFN.

      The 'fourFrames' cases are similar to the 'oneFrame' cases above, except each of the four 6-bit chunks
      of the 24-bit value indicates which subframes are allocated for MBSFN for one of the four MBSFN frames.
      The most significant 6-bit chunk corresponds to the first frame, and so on.
      */
      DWORD                dwSubframeAllocationBitMap;

      /**  
      Indicates if the dwSubframeAllocationBitMap contains the MBSFN subframe mapping for one frame or 
      four consecutive frames. The set of valid values is {1, 4}.
      */
      BYTE                 bOneFrameOrFourFrames;

      /** 
      The count of radio frames that occur from the first frame of an MBSFN broadcast to the first frame
      of the following MBSFN broadcast. MBSFN broadcasts contain one or four frames, as indicated by
      bOneFrameOrFourFrames.

      The set of valid values is {1, 2, 4, 8, 16, 32}.
      */
      BYTE                 bRadioFrameAllocationPeriod;

      /**
      The frame offset to determine the start of an MBSFN broadcast. The range of valid values is [0, 7].
      */
      BYTE                 bRadioFrameAllocationOffset;

   } MbmsManualConfig;

   /**
   Configuration for eMBMS measurements.
   \@attribute[type="serializable"]
   */
   struct SMbmsSettings
   {
      SMbmsSettings()
         : wMbmsMeasMode( 0 )
         , dwMaxSubframeRatePer1000Sec( 5000 )
      {}

      static const WORD          wMBSFN_RS_MEAS = 1;     //!< Enable MBSFN area reference signal measurements with
                                                         //!< the receiver front end selected for narrowband measurements.

      /**  
      eMBMS measurement modes. Used to configure which types of measurements will be performed. Set to 0 to disable
      eMBMS scanner measurements.
      */
      WORD                 wMbmsMeasMode;

      /**
      Maximum allowed eMBMS subframe measurement rate in units of 0.001 Hz.
      */
      DWORD                dwMaxSubframeRatePer1000Sec;

   } MbmsSettings;
};

   
/**
This structure holds the measurement setup for the LTE measurement. It
basically consists of the set of frequencies that shall be measured.
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   SChannelSettings()
      : dwCount( 0 )
      , pTableOfFrequencySetting( NULL )
      , bTddInterferenceKpiThresholdInPct( 0 )
   {}

   /**
   In the RS-CINR calculation a channel model is used to specify limits for maximum Doppler frequency and maximum delay
   spread. This model is used to separate reference signal power and interference power. If the model allows less freedom
   (maximal possible variation) to Doppler frequency and delay spread the RS-CINR result accuracy may be improved. The
   measurement accuracy may not be guaranteed if true signal propagation parameters exceed the limits specified in the
   channel model!
   \@attribute[type="serializable"]
   */
   struct SRsCinrChannelModel
   {
      SRsCinrChannelModel()
         : dwDelaySpreadInNs( dwOutdoorDelaySpreadInNs )
         , dwSpeedInKmPerHour( dwOutdoorMaxSpeedInKmh )
      {}

      static const DWORD        dwOutdoorDelaySpreadInNs  = 3000;  //!< Typical 3 탎 for outdoor environments.
      static const DWORD        dwIndoorDelaySpreadInNs   = 500;   //!< Typical 0.5 탎 for indoor environments.
      static const DWORD        dwMinDelaySpreadInNs      = 500;   //!< Minimum delay spread, 0.5 탎.
      static const DWORD        dwMaxDelaySpreadInNs      = 16000; //!< Maximum delay spread, 16 탎.

      /** 
      The maximum expected delay spread (time difference of arrival) between the signal received from different RF paths.
      */
      DWORD                     dwDelaySpreadInNs;

      static const DWORD        dwOutdoorMaxSpeedInKmh    = 100;   //!< Typical 100 km/h for outdoor environments.
      static const DWORD        dwIndoorMaxSpeedInKmh     = 5;     //!< Typical 5 km/h for indoor environments.
      static const DWORD        dwMinSpeedInKmh           = 5;     //!< Minimum speed, 5 km/h.
      static const DWORD        dwMaxSpeedInKmh           = 250;   //!< Maximum speed, 250 km/h.

      /** 
      The maximum speed (velocity) of the measurement platform in km / h. The relative platform to base station velocity gives the rise to
      the maximum Doppler frequencies in Hz: (CenterFrequency in Hz) * dwSpeedInKmPerHour / ( 3.6 * 3e8 )
      */
      DWORD                     dwSpeedInKmPerHour;

   }                       RsCinrChannelModel;

   /**
   Channel model for RS-CINR measurements performed on MBSFN areas. See SRsCinrChannelModel.
   \@attribute[type="serializable"]
   */
   struct SMbmsRsCinrChannelModel
   {
      SMbmsRsCinrChannelModel()
         : dwDelaySpreadInNs( dwDefaultDelaySpreadInNs )
         , dwSpeedInKmPerHour( dwOutdoorMaxSpeedInKmh )
      {}

      static const DWORD        dwDefaultDelaySpreadInNs  = 16667; //!< Default delay spread, 16.667 탎.
      static const DWORD        dwMinDelaySpreadInNs      = 16000; //!< Minimum delay spread, 16 탎.
      static const DWORD        dwMaxDelaySpreadInNs      = 17000; //!< Maximum delay spread, 17 탎.

      /**
      The maximum expected delay spread (time difference of arrival) between the signal received from different RF paths.
      */
      DWORD                     dwDelaySpreadInNs;

      static const DWORD        dwOutdoorMaxSpeedInKmh    = 100;   //!< Typical 100 km/h for outdoor environments.
      static const DWORD        dwIndoorMaxSpeedInKmh     = 5;     //!< Typical 5 km/h for indoor environments.
      static const DWORD        dwMinSpeedInKmh           = 5;     //!< Minimum speed, 5 km/h.
      static const DWORD        dwMaxSpeedInKmh           = 250;   //!< Maximum speed, 250 km/h.

      /**
      The maximum speed (velocity) of the measurement platform in km / h. The relative platform to base station velocity gives the rise to
      the maximum Doppler frequencies in Hz: (CenterFrequency in Hz) * dwSpeedInKmPerHour / ( 3.6 * 3e8 )
      */
      DWORD                     dwSpeedInKmPerHour;

   }                       MbmsRsCinrChannelModel;

   /**
   Configuration for TDD interference analysis. Disabled for bTddInterferenceKpiThresholdInPct == 0.
   If > 0 detailed reports for cells having an interference KPI value above or equal to the specified one
   are provided in SMeasResult::SSignals::ListOfTddInterferer.
   This feature requires a GPS antenna to be connected to the receiver.
   Also PDU::SIB1 has to be requested for channels with interference analysis.
   */
   BYTE                    bTddInterferenceKpiThresholdInPct;

   /** 
   Number of frequency configurations that shall be used from the pTableOfFrequencySetting to perform the measurement tasks.
   For value range see GetChannelCountLimits().
   */
   DWORD                   dwCount;

   /** 
   This array of structures can be filled with single frequency configurations.
   Only dwCount entries from this table will be used.
   */
   SFrequencySetting*      pTableOfFrequencySetting;
   //!<\@attribute[type="array" size="dwCount" sizeismember="true"]
};

//** BCH demodulation **/

/**
This enumeration defines IDs for the LTE Layer 3 PDUs. 
Use this in the SDemodRequest structure to specify which PDUs shall be decoded.
\@attribute[type="serializable"]
*/
struct Pdu { enum Type {
   MIB            = 10, //!< MIB    is PDU 10   If necessary, this PDU will be acquired automatically.
   SIB1           = 11, //!< SIB  1 is PDU 11
   SIB2           = 12, //!< SIB  2 is PDU 12
   SIB3           = 13, //!< SIB  3 is PDU 13
   SIB4           = 14, //!< SIB  4 is PDU 14
   SIB5           = 15, //!< SIB  5 is PDU 15
   SIB6           = 16, //!< SIB  6 is PDU 16
   SIB7           = 17, //!< SIB  7 is PDU 17
   SIB8           = 18, //!< SIB  8 is PDU 18
   SIB9           = 19, //!< SIB  9 is PDU 19
   SIB10          = 20, //!< SIB 10 is PDU 20
   SIB11          = 21, //!< SIB 11 is PDU 21
   SIB12          = 22, //!< SIB 12 is PDU 22
   SIB13          = 23, //!< SIB 13 is PDU 23
   SIB14          = 24, //!< SIB 14 is PDU 24
   SIB15          = 25, //!< SIB 15 is PDU 25
   SIB16          = 26  //!< SIB 16 is PDU 26
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
   Only to be used in start measurement.
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
            (0, SIB3, 0, 0, 0)
            (0, SIB4, 0, 0, 0)
            (1, SIB3, 0, 0, 0)
            (1, SIB5, REPETITION, 10000, 0)
            (1, SIB7, ON_CMD, 0, 0)
   @endcode
   Example during measurement:
            On channel 0 PDU demodulation shall be completely reset for the BTS identified by ID 3
            and on the the second channel SIB 7 shall be decoded for the BTS identified by ID 4 
            with a timeout for the request of 1 sec (1000ms):
   @code
            (0, 0, CHANNEL_RESET, 0, 3)
            (1, SIB7, ON_CMD, 1000, 4)
   @endcode
   Note that you are responsible to manage the memory when you use SetDemodulationSettings().
   Note: All parameters which are not necessary for a certain request shall be set to 0.
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
      {}

      /** 
      Index of the frequency in SChannelSettings::pTableOfFrequencySetting.
      */
      DWORD                dwChannelIndex;

      /** 
      The PDU to be demodulated.
      */
      Pdu::Type            ePDU;

      /** 
      The mode used for BCH demodulation.
      */
      DemodMode::Type      eDemodulationMode;

      /** 
      Repetition time out in ms (only used if bDemodulationMode == REPETITION).
      */
      WORD                 wRepetitionTimeOutInMs;

      /** 
      Unique BTS ID provided by the scanner as SSignals::dwScannerBtsIdent. This ID is used by the 
      BCHDemodulator to demodulate a specific BTS, when the bDemodulationMode is set to ON_CMD.
      */
      DWORD                dwBtsId;
   };

   /** 
   Pointer to an array of demodulation requests. Must be NULL, if no demodulation shall be done.
   */
   SDemodRequest*          pDemodRequests;
   //!<  \@attribute[type="array" size="dwCountOfRequests" sizeismember="true"]
};

/**
This structure is used to configure the BCH demodulation process.
\@attribute[type="serializable"]
*/
struct SDemodulationSettings
{
   SDemodulationSettings()
      : sSINRThresholdDB100( 0 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /**
   SINR threshold for demodulation, given in 1/100 dBm.
   For default value and range see GetDemodThresholdLimits().
   */
   short                sSINRThresholdDB100;

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
This structure holds all the measurement setup that is configured in the ViCom LTE scanner module. 
It is a result of the CViComLteInterface::GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   DWORD                      dwReceiverIndex;           //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth         ResultBufferDepth;         //!< Number of results allowed in the result buffer
   SChannelSettings           ChannelSettings;           //!< The set of frequencies that shall be measured
   SDemodulationSettings      DemodulationSettings;      //!< Configuration of the demodulation
};

/**
Confidence interval of a value. The value is inside the interval with a defined probability (confidence probability).
The interval is defined from (maximum likelihood value - bDeviationToLowerRatioInDB10) to
(maximum likelihood value + bDeviationToHigherRatioInDB10). The maximum likelihood value has to be given outside this structure.
Used in S_LTE_RS_CINR_VALUE.
\@attribute[type="serializable"]
*/
struct S_LTE_RATIO_CONFIDENCE_INTERVALL
{
   /** 
   0..25.4 dB distance from maximum likelihood value. SDefs::INVALID_BYTE_VALUE is used when the confidence interval is bigger.
   */
   BYTE                 bDeviationToLowerRatioInDB10;

   /** 
   0..25.4 dB distance from maximum likelihood value. SDefs::INVALID_BYTE_VALUE is used when the confidence interval is bigger.
   */
   BYTE                 bDeviationToHigherRatioInDB10;
};

/**
Container for subband RS-CINR meas results used for narrowband / wideband RS-CINR measurements.
\@attribute[type="serializable"]
*/
struct S_LTE_RS_CINR_VALUE
{
   /** 
   Average power of all resource elements used for sRsCinrInDB100. The average calculation is done in Watt not in dB.
   SDefs::INVALID_SHORT_VALUE if invalid.
   */
   short                sLinAverageREpowerInDBm100;

   /** 
   Subband RS-CINR meas result in 0.01 dB or SDefs::INVALID_SHORT_VALUE if invalid.
   The value is the maximum likelihood result of the measured probability function for the RS-CINR value.
   */
   short                sRsCinrInDB100;

   /** 
   The confidence interval for 68% for the value sRsCinrInDB100.
   The 68% confidence interval corresponds to the 1 sigma distance in a Gaussian distribution.
   */
   S_LTE_RATIO_CONFIDENCE_INTERVALL    ConfidenceInterval68PercentOfsRsCinrInDB100;

   /** 
   The confidence interval for 95% for the value sRsCinrInDB100.
   The 95% confidence interval corresponds to the 2 sigma distance in a Gaussian distribution.
   */
   S_LTE_RATIO_CONFIDENCE_INTERVALL    ConfidenceInterval95PercentOfsRsCinrInDB100;

};

/**
Complex value used for channel matrix values of MIMO measurements. 
\@attribute[type="serializable"]
*/
struct S_LTE_COMPLEX_VALUE
{
   /** 
   Real part of the complex number.
   */
   float                fReal;

   /** 
   Imaginary part of the complex number.
   */
   float                fImag;
};

/**
The LTE measurement result for a single channel returned by CViComLteInterface::GetResult().
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /** 
   The channel to which all the data relates is specified using this index.
   The index refers to the position in the SChannelSettings::pTableOfFrequencySetting array.
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
   Actual power values measured for a single server
   * \@attribute[type="serializable"]
   */
   struct SSignals
   {
      /** 
      The physical Cell ID, which can take one from the values defined by [0;503].
      */
      WORD                       wPhysicalCellId;

      /**
      Internal ID assigned from the scanner to that specific station. Different
      stations will become different IDs assigned to them, but if the scanner
      cannot identify a cell exactly, it also might assign different IDs to
      the same station. This ID is unique on a per channel basis.

      @note
         This is unique only within one measurement. Do not use it when
         combining the results of different measurements.
      */
      DWORD                      dwScannerBtsIdent;

      /** 
      Number of LTE symbols per slot. 
      Can either be SFrequencySetting::dwLTE_6SYMBOLS or SFrequencySetting::dwLTE_7SYMBOLS
      */
      BYTE                       bNumberOfSymbolsPerSlot;

      /** 
      Type of the channel: FDD or TDD
      */
      FrameStructureType::Type   enFrameStructureType;

      /** 
      P-SYNC to S-SYNC ratio that has been measured. The result is within the interval given in
      SFrequencySetting::SSyncToPSyncRatio.
      This is an optional value and only valid if the pointer is not equal to NULL.
      */
      float*                     pfSSyncToPSyncRatioInDB;

      /** 
      An estimated time drift for the signal, calculated from the arrival time of the strongest BS visible.
      */
      float                      fExpectedTimeDriftInNsPerSec;

      /** 
      This is the expected standard deviation of fExpectedTimeDriftInNsPerSec.
      */
      float                      fSigmaTimeDriftInNsPerSec;

      /**
      Time difference between PPS and the weighted averaged time of arrival (TOA),
      if pfFrameToaOffsetToPpsInSec is not NULL.

      In case of TDD LTE, the following must be considered:
      According to 3GPP TS36.211 Chapter "4.2 Frame structure type 2", 
      the TDD LTE Frame consists of two 5 ms subframes for each complete radio frame.

      Therefore, if you want to calculate the radio frame delay, you must use the half radio frame length (5 ms).

      To determine the frame delay as value within +/- half of the length considered (+/- 5 ms / 2  =>  2.5 ms), 
      you must add this half length value, apply a 5 ms modulo calculation and then subtract the half length value.
      If you do so, you get the correct offset values:
      Example: 
      Assume you got two measurement values, 5.2 and 9.8 ms. The offset to PPS has to be calculated as
      ( (5.2 ms + 2.5 ms) modulo 5 ms) - 2.5 ms ==> 0.2 ms delay
      ( (9.8 ms + 2.5 ms) modulo 5 ms) - 2.5 ms ==> -0.2 ms delay, i.e. 0.2 ms ahead.

      Note: This feature requires a GPS antenna to be connected to the receiver.
      */
      float*                     pfFrameToaOffsetToPpsInSec;

      /** 
      Reference Signal measurement results. Only available if measurements have been activated using
      SFrequencySetting::wNarrowbandRefSignalMeasMode.
      \@attribute[type="serializable"]
      */
      struct SReferenceSignal
      {
         /** 
         If 0, no reference signal measurement was done and sCenterRSRPinDBm100 and sPBCHbasedRSRQinDB100 are not valid.
         Bit 0 to Bit 1 is set when the corresponding antenna 0 to 1 was used for RSRP measurement. 
         Regarding TS36.214 only antenna port 0 and 1 is used for RSRP measurement.
         */
         BYTE                    bAntennaMaskUsedForRSRP;

         /** 
         Detected transmit antenna ports of the eNodeB.
         Bit 0 to Bit 3 is set when the corresponding antenna port 0 to 3 is detected by the scanner.
         */
         BYTE                    bDetectedTransmitAntennaPorts;

         /** 
         Optional Up- / Downlink configuration used for the TD-LTE RSRP measurement if pbUpDownLinkConfig is not NULL.
         E.g. for Up- / Downlink configuration 2 bit 2 (1<<2) is set. Configuration 0 to 6 are supported for TDD.
         */
         BYTE*                   pbUpDownLinkConfig;

         /** 
         Number of symbols used for RSRQ measurements.
         */
         BYTE                    bNumberOfSymbolsUsedForRSRQ;

         /** 
         Gives the RSRP value based on the reference signal power measurement of the 6 innermost resource blocks 
         (1MHz BW around center frequency at symbols used for the PBCH) in 0.01 dBm.
         */
         short                   sPBCHbasedRSRPinDBm100;

         /** 
         RSRQ value based on sCenterRSRPinDBm100 and the total inband power in resource blocks used to transmit 
         the PBCH within the 100 ms IQ data block analysed with the scanner.
         */
         short                   sPBCHbasedRSRQinDB100;

         /**
         Narrowband RS-CINR measurements taken for antenna port 0 to 3. The bits SFrequencySetting::wCENTER_RSCINR_xxx are
         set if the specific measurement was done. The results are stored in the pNarrowbandRsCinrValues array. The number
         and order of the results is defined by taken measurements indicated in this byte array. Lower antenna port results
         are stored first (highest priority), lower subbandwidth first, subbands with lower center frequency first.
         E.g. 6x180kHz (wCENTER_RSCINR_6x180KHZ) before 3x360kHz (wCENTER_RSCINR_3x360KHZ).
         */
         BYTE                    bRsCinrMeasResultConfig[4];

         /**
         Number of values available in pNarrowbandRsCinrValues array if pNarrowbandRsCinrValues is not NULL.
         */
         DWORD                   dwCountOfNarrowbandRsCinrValues;

         /**
         Narrowband RS-CINR result if pNarrowbandRsCinrValues is not NULL.
         For description of the result order see bRsCinrMeasResultConfig.
         */
         //!<  \@attribute[type="array" size="dwCountOfNarrowbandRsCinrValues" sizeismember="true"]
         S_LTE_RS_CINR_VALUE*    pNarrowbandRsCinrValues;

         /**
         Narrowband RSRP result with subband resolution if pNarrowbandRSRPinDBm100 is not NULL.
         For description of the result order and resolution see bRsCinrMeasResultConfig.
         */
         //!<  \@attribute[type="array" size="dwCountOfNarrowbandRsCinrValues" sizeismember="true"]
         short*                  pNarrowbandRSRPinDBm100;
                  
         /**
         Configuration of a TD-LTE special subframe at position 1 if pwSpecialSubframe1Config is not NULL.
         E.g. for configuration 0 bit 0 (1<<0) is set. Configuration 0 to 9 are possible.
         */
         WORD*                   pwSpecialSubframe1Config;

         /**
         Configuration of a TD-LTE special subframe at position 6 if pwSpecialSubframe6Config is not NULL.
         E.g. for configuration 0 bit 0 (1<<0) is set. Configuration 0 to 9 are possible.
         */
         WORD*                   pwSpecialSubframe6Config;                  

      } sRefSignal;

      /**
      Optional data container that holds information about the channel impulse
      responses measured in this cycle. The CIR consists of an optional power
      delay profile, and a list of peaks.

      Channel impulse responses are calculated from a 100ms block.
      \@attribute[type="serializable"]
      */
      struct SCir
      {
         DWORD       dwPcTimeStampInMs;
                                          //!< Calculated time of the first CIR sample based on start of the
                                          //!< operating system (GetTickCount()).
         ULONGLONG   u64DeviceTimeInNs;
                                          //!< Time when the first CIR sample has been measured, related to
                                          //!< the device internal clock.

         /**
         Structure containing the actual CIR measurement results combined with delay
         information.
         \@attribute[type="serializable"]
         */
         struct SPowerDelayProfile
         {
            float fInbandPowerInDBm;
                                          //!< Inband power measured during the CIR measurement (which is a
                                          //!< subset of the overall signal)
            float fAggregatePowerInDBm;
                                          //!< Total received power over the complete signal
            float fNoiseFloorInDBm;
                                          //!< Estimated noise level. Power values which are in the vicinity
                                          //!< of this value can be considered to be caused or influenced by
                                          //!< noise.
            float fSamplingTimeInSec;
                                          //!< Sampling time for a CIR measurement. This is also the time
                                          //!< between two CIR measurements.

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
               DWORD                dwCountOfValues;
                                          //!< number of elements available in the psValuesInDBm100 array
               short                *psValuesInDBm100;
                                          //!< array that holds dwCountOfValues elements,
                                          //!< each specifying a power delay value in 100th dBm
                                          //!< (e.g. a value of -6719 is to be read as -67.19 dBm).
                                          //!< Can be SDefs::INVALID_SHORT_VALUE when a value could not be measured
                                          //!< \@attribute[type="array" size="dwCountOfValues" sizeismember="true"]

            }                       PowerDelayProfileValues;

         } *pPowerDelayProfile;           //!< Optional power profile measurement further detailing the CIR measurement

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

         /**
         List of CIR peaks found in the signal.
         */
         SViComList<SPeak> ListOfPeaks;
                                          //!< List of power peaks and delay information
      }  *pCir;
                                          //!< This is NULL if no CIR information is available 
                                          //!<\@attribute[filterable="true"]

      /**
      Power values measured on the S-SYNC. The power is either retrieved from
      the complete LTE block (19-20 OFDM symbols), or from a single OFDM symbol. The former case
      is true when the pdwTimeFromStartOfBlockInNs is not set.
      \@attribute[type="serializable"]
      */
      struct SPowerValue
      {
         DWORD    *pdwTimeFromStartOfBlockInNs;
                           //!< Offset of the OFDM symbol that has been used to retrieve
                           //!< the power value. If not set, a complete LTE block has been
                           //!< used.
         float    fPowerInDBm;
                           //!< Power of S-SYNC signal. The power of the P-SYNC signal can
                           //!< be calculated from this using the pfSSyncToPSyncRatioInDB.
                           //!< This will give us the P-SYNC power of this special eNodeB.
         float    fCinrInDB;
                           //!< This is either the CINR of the S-SYNC only, or an averaged
                           //!< CINR of P-SYNC and S-SYNC. This depends on the value of the
                           //!< pfCinrPSyncInDB and pfCinrSSyncInDB. If both are not available,
                           //!< this is S-SYNC CINR only. The number of symbols from which the
                           //!< CINR is calculated is determined in the same way as described
                           //!< for fPowerInDBm.
                           //!< Otherwise, it's the averaged value.
         float    *pfCinrPSyncInDB;
                           //!< Optional CINR of the P-SYNC signal. As the P-SYNC is not unique
                           //!< for a special eNodeB, this value is based on the combined signal
                           //!< received from all eNodeB's using the same P-SYNC.
                           //!< Only available if > 0 dB.
         float    *pfCinrSSyncInDB;
                           //!< Optional CINR of the S-SYNC. Only available if > 0 dB.
         float    *pfAmpBasedCinrPSyncInDB;
                           //!< Optional CINR of the P-SYNC based on amplitude noise.
         float    *pfAmpBasedCinrSSyncInDB;
                           //!< Optional CINR of the S-SYNC based on amplitude noise.
         float    *pfSSyncRssiInDBm;
                           //!< Optional value. Comprises the linear average of the total received
                           //!< power observed only in OFDM symbols containing S-SYNC Signals over 
                           //!< the center 62 resource elements, including co-channel serving and
                           //!< non-serving cells, adjacent channel interference, thermal noise etc.
         float    *pfSSyncQualityInDB;
                           //!< Optional value. Calculated as the ratio of fPowerInDBm and SSyncRssiInDBm
         float    *pfPSyncPowerInDBm;
                           //!< Optional value. Calculated from fPowerInDBm and SSignals::pfSSyncToPSyncRatioInDB.
                              //!< P-SYNC power of this special eNodeB.
         float    *pfPSyncRssiInDBm;
                           //!< Optional value. Comprises the linear average of the total received
                           //!< power observed only in OFDM symbols containing P-SYNC Signals over 
                           //!< the center 62 resource elements, including co-channel serving and
                           //!< non-serving cells, adjacent channel interference, thermal noise etc.
         float    *pfPSyncQualityInDB;
                           //!< Optional value. Calculated as the ratio of pfPSyncPowerInDBm and PSyncRssiInDBm.
      };

      /**
      List of power values measured for the current channel.
      */
      SViComList<SPowerValue> ListOfPowerValues;
      //!<\@attribute[filterable="true"]

      /**
      Container for TDD interference analysis results.
      \@attribute[type="serializable"]
      */
      struct STddInterference
      {
         /**
         Internal ID of the interfering cell. See SSignals::dwScannerBtsIdent for more information.
         */
         DWORD             dwScannerBtsIdent;

         /**
         Channel index of the interfering station. If STddInterferenceAnalysis::dwChannelIndex matches SMeasResult::dwChannelIndex
         it's intra-frequency interference otherwise it's inter-frequency interference.
         */
         DWORD             dwChannelIndex;

         /** 
         Definition of interference reasons.
         \@attribute[type="serializable"]
         */
         struct Reason { enum Type {
                           CONFIGURATION_MISSMATCH = 1,    //!< Different special subframe configuration.
                           ASYNC_FRAMES            = 2     //!< Radio frames arrive asynchronously.
         }; };

         /**
         Reason for the interference. Can be any combination of the Reason::Type values above.
         */
         BYTE              bInterferenceReasonMask;

         /**
         Key performance indicator for the strength of the interference in percent.
         */
         float             fKPIinPct;
      };

      /**
      List of TDD interference analysis results for the current channel.
      */
      SViComList<STddInterference> ListOfTddInterferer;
   };

   /**
   The actual set of signals associated with the current channel specified
   by the dwChannelIndex member.
   */
   SViComList<SSignals>    ListOfSignals;

   /**
   Wideband scanner measurement results. Only available if wideband measurements
   have been configured using SFrequencySetting::SWidebandRsCinrSettings. The results
   are only valid if dwScannerBtsIdent does not equal ISDefs::INVALID_DWORD_VALUE.
   \@attribute[type="serializable"]
   */
   struct SWidebandRsCinrResult
   {
      /**
      Internal ID assigned from the scanner to that specific station as in 
      SSignals::dwScannerBtsIdent. Different stations will become different
      IDs assigned to them, but if the scanner cannot identify a cell exactly,
      it also might assign different IDs to the same station.

      @note
         This is unique only within one measurement. Do not use it when
         combining the results of different measurements.
      */
      DWORD                   dwScannerBtsIdent;

      /** 
      The physical Cell ID, which can take one from the values defined by [0;503].
      */
      WORD                    wPhysicalCellId;

      /**
      Transmit antenna ports for the eNodeB (0...3) used for the measurement.
      Bit 0 to Bit 3 is set when the corresponding antenna 0 to 3 was used for measurement.
      In case of multiple bits set, the average CINR, CINR and subband RSRP values are invalid!
      */
      BYTE                    bTransmitAntennaPort;

      /**
      Receive antenna ports used for the measurement. For TSMW this could either be
      SRFPort::RF_1 or SRFPort::RF_2 or both.
      */
      DWORD                   dwFrontEndSelectionMask;

      /**
      The number of resource blocks transmitted from the eNodeB, i.e. LTE system bandwidth.
      */
      WORD                    wRBNumberOfBts;

      /**
      The number of reference symbol resource elements used for the RSRP/Q measurement.
      */
      DWORD                   dwCountOfUsedREs;
         
      /** 
      RSRP value based on the wideband reference signal power measurement in 0.01 dB. 
      */
      short                   sRSRPinDBm100;

      /** 
      RSRQ value based on the wideband reference signal power measurement in 0.01 dB if not SDefs::INVALID_SHORT_VALUE.
      */
      short                   sRSRQinDB100;

      /** 
      Noise clipped RSRP value based on the wideband reference signal power measurement in 0.01 dB.
      For this value the measured RS-CINR was taken into account to correct the RSRP result regarding
      noise and interference.
      */
      short                   sNoiseClippedRSRPinDBm100;

      /** 
      Noise clipped RSRQ value based on the wideband reference signal power measurement in 0.01 dB.
      For this value the measured RS-CINR was taken into account to correct the RSRQ result regarding
      noise and interference. Only valid if not SDefs::INVALID_SHORT_VALUE.
      */
      short                   sNoiseClippedRSRQinDB100;

      /** 
      The average RS-CINR over all subbands of wideband RS-CINR measurements in 0.01 dB if not SDefs::INVALID_SHORT_VALUE.
      */
      short                   sAverageRsCinrInDB100;

      /**
      Number of values available in pWidebandRsCinrValues array if pWidebandRsCinrValues is not NULL.
      */
      DWORD                   dwCountOfSubbands;

      /**
      Wideband RS-CINR results if pWidebandRsCinrValues is not NULL.
      The result order is defined by: Subbands with lower center frequency are stored first.
      */
      //!<  \@attribute[type="array" size="dwCountOfSubbands" sizeismember="true" filterable="true"]
      S_LTE_RS_CINR_VALUE*    pWidebandRsCinrValues;

      /**
      Noise clipped wideband RSRP results with subband resolution if pWidebandSubbandRSRPinDBm100 is not NULL.
      The result order is defined by: Subbands with lower center frequency are stored first.
      Same resolution as the wideband RS-CINR results.
      */
      //!<  \@attribute[type="array" size="dwCountOfSubbands" sizeismember="true" filterable="true"]
      short*                  pWidebandSubbandRSRPinDBm100;

      /**
      Noise clipped wideband RSRQ results with subband resolution if pWidebandSubbandRSRQinDB100 is not NULL.
      The result order is defined by: Subbands with lower center frequency are stored first.
      Same resolution as the wideband RS-CINR results.
      */
      //!<  \@attribute[type="array" size="dwCountOfSubbands" sizeismember="true" filterable="true"]
      short*                  pWidebandSubbandRSRQinDB100;

      /** 
      Received signal strength indictor (RSSI) for only OFDM symbols that contain reference signals.
      This measurement is compatible with the definition in 3GPP 36.214, section 5.1.3 for RSSI used to compute RSRQ.
      */
      short                   sRsRssiInDBm100;
   };

   /**
   The actual set of wideband measurement results associated with the current channel specified
   by the dwChannelIndex member.
   */
   SViComList<SWidebandRsCinrResult>    ListOfWidebandRsCinrResults;

   /**
   Rssi and spectrum measurement results. Only available if RSSI / spectrum measurements
   have been configured using SFrequencySetting::SRssiSettings.
   \@attribute[type="serializable"]
   */
   struct SRssiAndSpectrumResult
   {
      /**
      Receive antenna ports used for the measurement. For TSMW this could either be
      SRFPort::RF_1 or SRFPort::RF_2 or both.
      */
      DWORD                   dwFrontEndSelectionMask;

      /** 
      Used Rssi measurement configuration. See SRssiSettings::wRSSI_xxx_MODE constants.
      */
      WORD                    wRssiMeasMode;

      /**
      RSSI measurement bandwidth in Hz (3 dB limit of the receiver filter).
      */
      float                   fUsedRssiBandwidthInHz;

      /**
      For the SRssiSettings::wRSSI_DL_ALL_TS_MODE measurement, this specifies the slot (0-19) in the LTE radio frame
      the RSSI measurements was made. Not used for any other measurement mode, only available if not NULL.
      */
      BYTE*                   pbSlotOffsetInLteFrame;

      /**
      Measured received signal strength indicator (RSSI) value in 0.01 dBm if psRssiInDB100 is not NULL.
      */
      short*                  psRssiInDBm100;

      /**
      Frequency spacing of spectrum values available in pSpectrumValues array if pSpectrumValues is not NULL.
      */
      float                   fSpectrumFreqDistanceInHz;

      /**
      Number of FFTs used for the RMS spectral power accumulation. This is the total count of FFTs if more than
      one FE is used. This is an optional value on only valid if the pointer is not equal to NULL.
      */
      WORD                    wCountOfFftsForSpectrum;

      /**
      The 6 dB resolution bandwidth for spectrum results (given by the IQ time window function applied before the FFT).
      */
      float                   f6dBResolutionBandwidthInHz;

      /**
      Optional container for spectrum meas results. Only available if spectrum measurements have been configured in
      SFrequencySetting::SRssiMeasSettings and pSpectrum is not NULL.
      The RMS spectrum values are directly output from the FFT, thus the first value is the zero frequency component.
      \@attribute[type="serializable"]
      */
      struct SSpectrumResult
      {
         /**
         Number of values available in psRmsSpectralPowerValueInDBm100 array if psRmsSpectralPowerValueInDBm100 is not NULL.
         */
         DWORD                dwCountOfSpectrumValues;

         /**
         RMS spectrum value in 0.01 dBm.
         */
         //!<  \@attribute[type="array" size="dwCountOfSpectrumValues" sizeismember="true"]
         short*               psRmsSpectralPowerValueInDBm100; 

      } *pSpectrumResult;
      //!<\@attribute[filterable="true"]

   };

   /**
   The actual set of RSSI and spectrum measurement results associated with the current channel specified
   by the dwChannelIndex member.
   */
   SViComList<SRssiAndSpectrumResult>    ListOfRssiAndSpectrumResults;

   /** 
   MIMO measurement results. Only available if MIMO measurements have been configured
   using SFrequencySetting::SMimoMeasSettings. The results are only valid if dwScannerBtsIdent
   does not equal SDefs::INVALID_DWORD_VALUE.
   \@attribute[type="serializable"]
   */
   struct SMimoResult
   {
      /**
      Internal ID assigned from the scanner to that specific station as in 
      SSignals::dwScannerBtsIdent. Different stations will become different
      IDs assigned to them, but if the scanner cannot identify a cell exactly,
      it also might assign different IDs to the same station. 

      @note
         This is unique only within one measurement. Do not use it when
         combining the results of different measurements.
      */
      DWORD                      dwScannerBtsIdent;

      /** 
      The physical Cell ID, which can take one from the values defined by [0;503].
      */
      WORD                       wPhysicalCellId;

      /**
      Number of receive antenna ports used for the MIMO measurement (e.g. in the calculation of the H matrix).
      */
      BYTE                       bNumberOfReceiveAntennas;

      /**
      Number of eNodeB transmit antenna ports used for the MIMO measurement (e.g. in the calculation of the H matrix).
      */
      BYTE                       bNumberOfTransmitAntennas;

      /**
      The number of resource blocks transmitted from the eNodeB, i.e. LTE system bandwidth.
      */
      WORD                       wRBNumberOfBts;

      /**
      Number of MIMO measurements made for each BTS in the captured IQ data.
      */
      WORD                       wNumberOfMeasTimes;

      /**
      MIMO resolution. The length of signal in ms that has been used to calculate the MIMO results.
      */
      WORD                       wTimeResolutionInMs;

      /**
      The number of available MIMO channel matrices if pChannelMatrix is not NULL.
      Equal to wRBNumberOfBts * wNumberOfMeasTimes.
      */
      DWORD                      dwCountOfChannelMatrices;

      /**
      Optional container for channel matrices (H matrix) if pChannelMatrix is not NULL.
      The total measurement count resp. the number of available MIMO channel matrices is
      wRBNumberOfBts * wNumberOfMeasTimes.
      E.g. wRBNumberOfBts == 50 and  wNumberOfMeasTimes == 4 means this BTS has a system
      bandwidth of 50 RBs and there are 4 channel matrices for every resource block,
      in total 200 matrices.

      The matrix ordering is first measured matrix first and lower RB number (lower frequency)
      first, e.g. [ H_RB0_T0  H_RB1_T0  ...  H_RBn_T0  H_RB0_T1 H_RB1_T1  H_RBn_T1  ... ]
      with RBx is a RB index and Tx is a time index.
      \@attribute[type="serializable"]
      */
      struct SChannelMatrix
      {
         /**
         Optional CINR value used in the channel matrix (H matrix) if psChannelMatrixCinrInDB100 is not NULL.
         */
         short*                  psChannelMatrixCinrInDB100;

         /**
         Optional rank for the channel matrix (H matrix) if pRank is not NULL.
         The estimated rank of H * ~H with respect to the CINR for 2x2 MIMO measurements.
         */
         BYTE*                   pbRankFor2x2Mimo;

         /**
         Optional rank for the channel matrix (H matrix) if pRank is not NULL.
         The estimated rank of H * ~H with respect to the CINR for 2x4 MIMO measurements.
         */
         BYTE*                   pbRankFor2x4Mimo;

         /**
         Optional condition number for the channel matrix (H matrix) if pwConditionNumberInDB100 is not NULL.
         The condition number has the format 100 * 10 * lg( max eigenvalue / min eigenvalue ).
         */
         WORD*                   pwConditionNumber2x2MimoInDB100;

         /**
         Optional condition number for the channel matrix (H matrix) if pwConditionNumberInDB100 is not NULL.
         The condition number has the format 100 * 10 * lg( max eigenvalue / min eigenvalue ).
         */
         WORD*                   pwConditionNumber2x4MimoInDB100;

         /**
         Number of values available in pComplexCoefficient if pComplexCoefficient is not NULL.
         */
         DWORD                   dwCountOfComplexCoefficients;

         /**
         Complex coefficient of the H matrix in sqrt(mW). Complex coefficients are stored column by column.
         Which means the pComplexCoefficient array effectively stores the transposed matrix H'.
         For example:
            
                              [ h11  h12  h13  h14 ]
                        H = 
                              [ h21  h22  h23  h24 ]

         => pComplexCoefficient array: [ h11 h21 h12 h22 h13 h23 h14 h24 ]

         If 2x2 and 2x4 MIMO measurements are enabled, only the 2x4 matrix is returned, since the 2x2 matrix
         is a subset of the 2x4 matrix. E.g. the first four coefficients represent the 2x2 case [ h11 h21 h12 h22 ].
         */
         //!<  \@attribute[type="array" size="dwCountOfComplexCoefficients" sizeismember="true"]
         S_LTE_COMPLEX_VALUE*    pComplexCoefficient;

      }  *pChannelMatrix;             //!<  \@attribute[type="array" size="dwCountOfChannelMatrices" sizeismember="true"]
   };

   /**
   The actual set of MIMO measurement results associated with the current channel specified
   by the dwChannelIndex member.
   \@attribute[filterable="true"]
   */
   SViComList<SMimoResult>    ListOfMimoResults;

   /**
   Container for throughput estimation results.
   \@attribute[type="serializable"]
   */
   struct SThroughputEstimationResult
   {
      /**
      Internal ID assigned from the scanner to that specific station as in
      SSignals::dwScannerBtsIdent. Different stations will become different
      IDs assigned to them, but if the scanner cannot identify a cell exactly,
      it also might assign different IDs to the same station.

      @note
      This is unique only within one measurement. Do not use it when
      combining the results of different measurements.
      */
      DWORD                      dwScannerBtsIdent;

      /**
      The physical Cell ID, which can take one from the values defined by [0;503].
      */
      WORD                       wPhysicalCellId;

      /**
      Number of values available in pThroughputPerTransmissionMode, if pThroughputPerTransmissionMode is not NULL.
      */
      BYTE                       dwCountOfThroughputPerTransmissionModeValues;

      /**
      Container for throughput estimation results for one transmission mode.
      \@attribute[type="serializable"]
      */
      struct SThroughputPerTransmissionMode
      {
         /**
         Definition of transmission mode values.
         \@attribute[type = "serializable"]
         */
         struct TransmissionMode{ enum Type {
               SISO = 0,        //!< Single port transmission mode.
               SIMO = 1,        //!< Single Input Multiple Output transmission mode.
               SFBC = 2,        //!< Space Frequency Block Code transmission mode (transmit diversity).
               MIMO = 3,        //!< Multiple Input Multiple Output transmission mode.
         };};

         /**
         Transmission mode of the spatial layer
         */
         TransmissionMode::Type    enTransmissionMode;

         /**
         Number of spatial layer for TransmissionMode::MIMO, always 1 for single layer transmission modes.
         */
         BYTE                      bNumberOfSpatialLayer;

         /**
         Throughput estimation value.
         \@attribute[type="serializable"]
         */
         struct SThroughputValue
         {
            /** 
            Definition of channel quality index (CQI) values.
            \@attribute[type="serializable"]
            */
            struct CQI { enum Type {
                           UNKNOWN = 0,     //!< CQI index invalid or unknown.
                           INDEX1 = 1,      //!< CQI index 1, QPSK code rate * 1024 = 78.
                           INDEX2 = 2,      //!< CQI index 2, QPSK code rate * 1024 = 120.
                           INDEX3 = 3,      //!< CQI index 3, QPSK code rate * 1024 = 193.
                           INDEX4 = 4,      //!< CQI index 4, QPSK code rate * 1024 = 308.
                           INDEX5 = 5,      //!< CQI index 5, QPSK code rate * 1024 = 449.
                           INDEX6 = 6,      //!< CQI index 6, QPSK code rate * 1024 = 602.
                           INDEX7 = 7,      //!< CQI index 7, 16QAM code rate * 1024 = 378.
                           INDEX8 = 8,      //!< CQI index 8, 16QAM code rate * 1024 = 490.
                           INDEX9 = 9,      //!< CQI index 9, 16QAM code rate * 1024 = 616.
                           INDEX10 = 10,    //!< CQI index 10, 64QAM code rate * 1024 = 466.
                           INDEX11 = 11,    //!< CQI index 11, 64QAM code rate * 1024 = 567.
                           INDEX12 = 12,    //!< CQI index 12, 64QAM code rate * 1024 = 666.
                           INDEX13 = 13,    //!< CQI index 13, 64QAM code rate * 1024 = 772.
                           INDEX14 = 14,    //!< CQI index 14, 64QAM, code rate * 1024 = 873.
                           INDEX15 = 15     //!< CQI index 15, 64QAM code rate * 1024 = 948.
            }; };

            /**
            Estimated channel quality index.
            */
            CQI::Type              enCqiIndex;

            /**
            Estimated channel capacity in kbit per second.
            */
            float                  fCapacityInKbitPerSecond;
         };

         /**
         The actual set of wideband throughput estimation values for the whole carrier bandwidth.
         */
         SThroughputValue          pWidebandThroughputValue;

         /**
         Number of values available in pSubbandThroughputValues if pSubbandThroughputValues is not NULL.
         */
         DWORD                     dwCountOfSubbandThroughputValues;

         /**
         The actual set of subband throughput estimation values with subband resolution.
         \@attribute[type="array" size="dwCountOfSubbandThroughputValues" sizeismember="true"]
         */
         SThroughputValue*         pSubbandThroughputValues;
      };
         
      /**
      Throughput results per transmission mode and spatial layer. For TransmissionMode::MIMO with
      number of spatial layers > 1, lower spatial layer index comes first.
      \@attribute[type="array" size="dwCountOfThroughputPerTransmissionModeValues" sizeismember="true"]
      */
      SThroughputPerTransmissionMode*    pThroughputPerTransmissionMode;
   };

   /**
   The actual set of throughput estimation results.
   \@attribute[filterable="true"]
   */
   SViComList<SThroughputEstimationResult>      ListOfThroughputEstimationResults;



   /**
   eMBMS scanner measurement results. Only available if eMBMS measurements have been configured using
   SFrequencySetting::SMbmsSettings.
   \@attribute[type="serializable"]
   */
   struct SMbmsResult
   {
      /**
      Area ID for the measured MBSFN area.
      */
      BYTE                    bMbsfnAreaId;

      /**
      The number of reference symbol resource elements used for the RSRP/Q measurement.
      */
      WORD                    wCountOfUsedREs;

      /**
      The number of resource blocks transmitted from the eNodeB, i.e. LTE system bandwidth.
      */
      WORD                    wRBNumberOfBts;

      /** 
      RSRP value based on the wideband reference signal power measurement in 0.01 dB. 
      */
      short                   sRSRPinDBm100;

      /** 
      RSRQ value based on the wideband reference signal power measurement in 0.01 dB if not SDefs::INVALID_SHORT_VALUE.
      */
      short                   sRSRQinDB100;

      /** 
      Noise clipped RSRP value based on the wideband reference signal power measurement in 0.01 dB.
      For this value the measured RS-CINR was taken into account to correct the RSRP result regarding
      noise and interference.
      */
      short                   sNoiseClippedRSRPinDBm100;

      /** 
      Noise clipped RSRQ value based on the wideband reference signal power measurement in 0.01 dB.
      For this value the measured RS-CINR was taken into account to correct the RSRQ result regarding
      noise and interference. Only valid if not SDefs::INVALID_SHORT_VALUE.
      */
      short                   sNoiseClippedRSRQinDB100;

      /** 
      The average RS-CINR over all subbands of wideband RS-CINR measurements in 0.01 dB if not SDefs::INVALID_SHORT_VALUE.
      */
      short                   sAverageRsCinrInDB100;

      /** 
      Received signal strength indictor (RSSI) for only OFDM symbols that contain reference signals.
      This measurement is compatible with the definition in 3GPP 36.214, section 5.1.3 for RSSI used to compute RSRQ.
      */
      short                   sRsRssiInDBm100;

      /**
      Measured received signal strength indicator (RSSI) value in 0.01 dBm.
      */
      short                   sRssiInDBm100;

      /**
      The count of eNodeB identifiers associated with the eMBMS configuration for the measured MBSFN area if
      pENodeBIdentifier is not NULL.
      */
      DWORD                   dwCountOfENodeBIdentifiers;

      /**
      Optional container for the eNodeB identifiers associated with the eMBMS configuration for the measured MBSFN area.
      \@attribute[type="serializable"]
      */
      struct SENodeBIdentifier
      {
         /**
         Internal ID assigned from the scanner to that specific station as in 
         SSignals::dwScannerBtsIdent. Different stations will become different
         IDs assigned to them, but if the scanner cannot identify a cell exactly,
         it also might assign different IDs to the same station.

         @note
         This is unique only within one measurement. Do not use it when
         combining the results of different measurements.
         */
         DWORD                dwScannerBtsIdent;

         /**
         The physical cell ID, which can take one from the values defined by [0;503].
         */
         WORD                 dwPhysicalCellId;

         /**
         The 28-bit unique cell ID if not SDefs::INVALID_DWORD_VALUE.
         */
         DWORD                dwCellId;

         /**
         The MCC (mobile country code) if not SDefs::INVALID_WORD_VALUE.
         */
         WORD                 wMCC;

         /**
         The MNC (mobile network code) in binary format if not SDefs::INVALID_WORD_VALUE.
         */
         WORD                 wMNC;

         /**
         The location area code if not SDefs::INVALID_DWORD_VALUE.
         */
         DWORD                dwLAC;

      }  *pENodeBIdentifier;           //!<  \@attribute[type="array" size="dwCountOfENodeBIdentifiers" sizeismember="true"]
   };

   /**
   The actual set of eMBMS measurement results associated with the current channel specified
   by the dwChannelIndex member.
   */
   SViComList<SMbmsResult>    ListOfMbmsResults;



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
      */
      //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
      const BYTE     *pbBitStream;
         
      /** 
      The physical Cell ID, which can take one from the values defined by [0;503].
      */
      WORD           wPhysicalCellId;
   };

   /** 
   Pointer to demodulation result. If NULL, no demodulation result is present.
   */
   const SDemodResult  *pDemodResult;

   /**
   Measurement rate for the given LTE channel (dwChannelIndex).
   List of measurement rates rates and other values in relation to the measurement rate for one or more time intervals 
   of measurement rate calculation.
   The measurement rate is the ratio of all measurements of the same type of measurement at the time the measurement signals
   have been picked from the air. So the time of arrival at the host application is not used.
   If ListOfSignals has elements then the given measurement rate values correspond to narrowband measurements.
   Otherweise they are wideband.
   */
   SViComList<SMeasurementRate> ListMeasurementRates;

   /**
   Measurement rate for all LTE channels.
   In the case of one channel is measured with 5 and another one with 2 per second we expect an total meas rate of 7.
   If ListOfSignals has elements then the given measurement rate values correspond to narrowband measurements.
   Otherweise they are wideband.
   */
   SViComList<SMeasurementRate> ListMeasurementRatesTotal;
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

   DWORD                ePdu;             //!< PDU contained.
   DWORD                dwBitCount;       //!< Number of bits for this PDU.
   BYTE*                pbBitStream;      //!< Pointer to a buffer containing the PDU bits.
                                          //!< The first bit is stored in bit 7 of pbBitStream[0].
                                          //!< If dwBitCount is not a multiple of 8, there are remaining bits,
                                          //!< which shall be ignored.
                                          //!< \@attribute[type="bitfield" size="dwBitCount" sizeismember="true"]
};

/**
Result from the Layer 3 Decoder.

The string returned via this structure is formatted using the ASN.1 notation of the LTE standard (3GPP TS 36.331).
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
   char*                pcPduText;        //!< Pointer to a buffer containing the decoded PDU text, as zero terminated ASCII string.
                                          //!< \@attribute[type="array" size="dwStringLength" sizeismember="true"]
};

} // namespace LTE
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
