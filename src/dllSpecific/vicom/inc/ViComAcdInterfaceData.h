// ViComAcdInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_ACD    3         // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)      // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                     // get a definite structure alignment for CViComAcdInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace ACD
{
/**
Data containers used for ViCom ACD interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_ACD,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
Configuration of the technologies used for channel detection.
\@attribute[type="serializable"]
*/
struct STechnologySettings
{
   STechnologySettings()
      : enTechnology( Technology::UNKNOWN )
      , u64BandIdMask( 0 )
   {}

   /** 
   Technology type for channel detection.
   */
   Technology::Type           enTechnology;

   /** 
   Bit mask for the configuration of frequency bands that shall be be evaluated.
   E.g. when Band 1 and 3 shall be evaluated bit 1 (1<<1) and bit 3 (1<<3) shall be set.
   */
   ULONGLONG                  u64BandIdMask;
};

   
/**
This structure holds the measurement setup for the automatic channel detection. It
basically consists of the set of technologies which should be detected and some
elements to control the detection speed and quality.
\@attribute[type="serializable"]
*/
struct SAcdSettings
{
   SAcdSettings()
      : dwCount( 0 )
      , paTechnologies( NULL )
      , enMeasurementMode( MeasurementMode::SMART )
      , enSensitivity( Sensitivity::GOOD )
      , dwNumberOfTrialsPerChannel( 1 )
      , dwMinimumDetectedBwInHz( 1400000 )
      , dwFrontEndSelectionMask( SRFPort::RF_1 )
   {}

   /** 
   Number of technology configurations that shall be used from the paTechnologies.
   */
   DWORD                   dwCount;

   /** 
   This array of structures can be filled with technology specific settings.
   Only dwCount entries from this array will be used.
   \@attribute[type="array" size="dwCount" sizeismember="true"]
   */
   STechnologySettings*    paTechnologies;

   /** 
   Definition of measurement modes.
   \@attribute[type="serializable"]
   */
   struct  MeasurementMode { enum Type {
                              SIMPLE     = 1,        //!< This mode does a brute force evaluation of all channels in the specified bands.
                                                     //!< The measurement is stopped after all channels have been scanned.
                                                     //!< ACD reports (SMeasResult) are available for the user as long as the scanning
                                                     //!< has finished.
                              SMART      = 2         //!< This modes incorporates spectrum measurements to do some advanced analysis to
                                                     //!< detect channels, thus the receiver has to support RF Power Scan measurements.
                                                     //!< In contrast to the SIMPLE mode ACD reports a generated periodically to allow
                                                     //!< tracking of the specified bands. Results are provided as long as the user
                                                     //!< stops the measurement.
   }; };

   /** 
   Measurement mode. Used to switch between modes suitable for stationary and drive test measurements.
   \@attribute[type="serializable"]
   */
   MeasurementMode::Type   enMeasurementMode;
   
   /** 
   Definition of sensitivity modes which controls the signal detection for RF Power Scan data.
   \@attribute[type="serializable"]
   */
   struct  Sensitivity { enum Type {
                              FAIR      = 1,         //!< This mode has a very fast detection rate and a fair sensitivity.
                              GOOD      = 2,         //!< This mode has a fast detection rate and a good sensitivity.
                              EXCELLENT = 3          //!< This mode has the highest sensitivity and a fair detection rate.
   }; };

   /**
   Sensitivity mode. Used to switch between modes having high sensitivity at a fair speed and vice versa.
   Only availabe in MeasurementMode::SMART.
   */
   Sensitivity::Type       enSensitivity;

   /** 
   Number of attempts to find a signal on a channel. Increasing this number will help to find weaker cells and increase the
   detection probability, but reduce the detection speed.
   */
   DWORD                   dwNumberOfTrialsPerChannel;

   /** 
   Minimum detected bandwidth in Hz. Only availabe in MeasurementMode::SMART.
   */
   DWORD                   dwMinimumDetectedBwInHz;

   /** 
   Receiver front ends to be used by the automatic channel detection.
   */
   DWORD                   dwFrontEndSelectionMask;
};

//** Request Settings **/
/**
This structure holds the measurement setup that is configured in the ViCom ACD module. 
It is a result of the CViComAcdInterface::GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   SResultBufferDepth         ResultBufferDepth;         //!< Number of results allowed in the result buffer
   SAcdSettings               AcdSettings;               //!< Set of channels that shall be evaluated.
};

/**
The ACD measurement result returned by CViComAcdInterface::GetResult().
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /** 
   PC local time when the ACD result was processed.
   */
   DWORD       dwPcTimeStampInMs;

   /**
   Container for channel specific information.
   \@attribute[type="serializable"]
   */
   struct SChannel
   {
      /** 
      Channel frequency in Hz.
      */
      ULONGLONG                 u64FrequencyInHz;

      /** 
      Definition of channel states.
      \@attribute[type="serializable"]
      */
      struct  State { enum Type {
                                 PENDING      = 1,      //!< This channel is scheduled to being scanned.
                                 SCANNING     = 2,      //!< This channel is currently scanned.
                                 DETECTED     = 3       //!< A technology has been detected on this channel.
                                                        //!< Not all specific information might be available yet.
      }; };

      /**
      Channel state.
      */
      State::Type               enState;

      /**
      Technology type the channel. Only available if penTechnology is not NULL.
      */
      Technology::Type*         penTechnology;

      /** 
      Frequency band id of the channel. Only available if pwBandId is not NULL.
      */
      WORD*                     pwBandId;

      /**
      Bandwidth of the channel, in Hz. Only available if pdwBandwidthInHz is not NULL.
      */
      DWORD*                    pdwBandwidthInHz;

      /**
      True if the bandwidth of the channel has been confirmed by demodulation, otherwise false. Only valid if pdwBandwidthInHz is not NULL.
      */
      bool                      bBandwidthConfirmed;

      /** 
      Last measured RSSI on this channel. Only available in MeasurementMode::SMART.
      Only available if psRssiInDBm is not NULL.
      */
      float*                    pfRssiInDBm;

      /** 
      PC local time when this channel was measured the last time. Only available if pdwPcTimeStampInMs is not NULL.
      */
      DWORD*                    pdwPcTimeStampInMs;

   };

   /**
   The actual set of evaluated channels.
   */
   SViComList<SChannel>    ListOfChannels;
};

} // namespace ACD
} // namespace ViCom
} // namespace RohdeSchwarz


#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
