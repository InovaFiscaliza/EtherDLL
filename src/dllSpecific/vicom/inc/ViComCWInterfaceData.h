// ViComCWInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_CW      3        // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)      // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                     // get a definite structure alignment for CViComCWInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace CW
{
/**
Data containers used for ViCom CW interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_CW,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit
   
/**
* The measurement mode is one of the most important decisions to be made when CW measurements
* are configured. Both modes have advantages and disadvantages, and some of the boundaries
* defined in the constants in this interface are dependent of the mode used. 
\@attribute[type="serializable"]
*/
enum etMeasType
{
   /**
   * In the single channel mode, only one channel is measured at a time. This leads to
   * reduced influence of senders in the channels neighborhood, but also takes more
   * time and does not allow the smallest bandwidths.
   */
   MT_SingleChannel     = 0,

   /**
   * In this mode, the frequencies in the channels settings are grouped to measure
   * more than one channel at a time, if possible. This improves measurement speed by
   * order of magnitudes, because the amount of frequency changes is reduced, which
   * also minimizes the tuning times and pauses required between the changes.
   */
   MT_MultiChannel      = 1
};

/**
*\@attribute[type="serializable"]
*/
struct SFrequencySetting
{
   /**
   * Minimum frequency that can be measured. Specifying a smaller frequency leads to 
   * ViCom error VICEC_CW_FREQ_OUT_OF_RANGE.
   */
   static const DWORD            dwMinFreqInMHz             =    80;

   /**
   * Maximum frequency that can be measured with all device devices, except the TSML-CW.
   * Specifying a larger frequency leads to error VICEC_CW_FREQ_OUT_OF_RANGE.
   */
   static const DWORD            dwMaxFreqInMHz             =  3000;

   /**
   * The TSML-CW is a device specialized for CW measurements and can therefore measure
   * frequencies up to 6 GHz. This is useful in cases where no specialized scanners
   * are available and the channel power measurement is used in the initial phase.
   */
   static const DWORD            dwMaxFreqInMHz_TSML_CW     =  6000;

   /**
   Specifies the center frequency of the channel. All power within the frequency
   interval [ dCenterFrequencyInMHz - .5 * dBandwidthInHz; dCenterFrequencyInMHz + .5 * dBandwidthInHz ]
   will contribute to the overall result, resp. all spectrum lines from the FFT (in multi channel mode)
   that fall within that range will be measured.
   */
   double                        dCenterFrequencyInMHz;

   /**
   minimum bandwidth useable for Single Channel mode.
   */
   static const DWORD            dwMinBwInHz_SingleChannel  =      200000;
     
   /**
   minimum bandwidth useable for Single Channel mode.
   */
   static const DWORD            dwMinBwInHz_MultiChannel   =       12500;
      
   /**
   Overall maximum bandwidth. This is also the bandwidth that a channel group can span in Multi Channel
   measuremnt mode and is derived from the hardware constrains.
   */
   static const DWORD            dwMaxBwInHz                =     4000000;

   /**
   The bandwidth of the channel. Together with the dCenterFrequencyInMHz setting, this specifies an
   interval of frequencies that are selected from the FFT result resp. are configured in the measurement
   equipment to run the measurement.
   */
   double                        dBandwidthInHz;

   /**
   The meaning of this value can be changed with SetAttenuationSwitchMode.
   It specifies the minimum or fix attenuation that is used to measure the frequency.
   Refer to etAttenuationSwitchMode.
   */
   STSMxAttenuation::AttMode::Type   eMinAttenuation;

   static const DWORD            dwMinMeastimeIn_us         =         100;
   static const DWORD            dwMaxMeastimeIn_us         =     1000000;
   DWORD                         dwMeastimeIn_us;
};

/**
This structure contains the list of frequencies and the bandwidths that make up the channels
which shall be measured.
\@attribute[type="serializable"]
*/
struct SChannelSettings
{
   static const DWORD   dwMinCount                 = 1;    //!< minimum number of channels
   static const DWORD   dwMaxCount_SingleChannel   = 128;  //!< maximum number of channels for single channel measurements
   static const DWORD   dwMaxCount_MultiChannel    = 512;  //!< maximum number of channels for multi channel measurements
   static const DWORD   dwMaxCount                 = 512;  //!< maximum number

   /**
   The number of channel entries from TableOfFrequencySetting that shall be used to
   configure the measurement.
   */
   DWORD                dwCount;

   /**
   This static array can be filled with the specification which channels shall be measured.
   It contains up to dwMaxCount entries, but only those are used that are within the
   interval [0; dwCount[.
   \@attribute[type="array" size="dwCount" sizeismember="true"]
   */
   SFrequencySetting    TableOfFrequencySetting[dwMaxCount];
};

/**
Enumerates the modes of attenuation switching.
The choosen attenuation is influenced by the mode set with SetAttenuationSwitchMode 
which specifies how SFrequencySetting::eMinAttenuation is interpreted.
\@attribute[type="serializable"]
*/
enum etAttenuationSwitchMode
{
   /**
   * Let SFrequencySetting::eMinAttenuation
   * be the minimum attenuation chosen to measure the channel or channel group.
   * An attenuation switching to higher levels will be done automatically when necessary.
   */
   AS_AUTO = 0,

   /**
   * Let SFrequencySetting::eMinAttenuation
   * be the fix attenuation setting for single channel measurements.
   * During multi channel measurements the highest eMinAttenuation of each channel group
   * will be used as the fix attenuation for this channel group.
   * The actually used attenuation is returned with 
   * SMeasResult::SPowerResult::eAttMode
   * Because AS_FIX disables the switching to higher attenuation levels 
   * this could yield to unwanted ADC overflows.
   */
   AS_FIX  = 1
};

/**
* This enumeration defines the possible algorithms to combine several raw power values to
* one single power value for a channel that is calculated for the total measurement time.
*
* The raw power values are derived from data measured in a very short time interval. 
* For the complete time interval that a channel shall be measured a whole bunch of raw
* measurements is normally created.
\@attribute[type="serializable"]
*/
enum etDetector
{
   /**
   * If this mode is chosen, the highest power value is taken as overall result. 
   */
   TD_PEAK     = 1,
      
   /**
   * All the raw (linearized) power values are used to calculate the overall result.
   */
   TD_AVERAGE  = 2,

   /**
   * The result power value is calculated by using the root-mean-square method (see manual).
   */
   TD_RMS      = 4
};


/**
* There is the possibility to make the result creation depending on an external trigger device.
* This is enabled by setting eTriggerMode to TM_EXTERNAL.
\@attribute[type="serializable"]
*/
struct STriggerSettings
{
   enum etTriggerMode
   {
      TM_FREE_RUN = 0,                    //!< measure without external triggering
      TM_EXTERNAL = 1                     //!< use external trigger and trigger factor
   }              eTriggerMode;

   double         dTriggerFactor;         //!< The trigger factor maps external triggers to internal triggers. Refer to the manual.
};


/**
* This structure contains all the currently active settings stored in the ViCom CW interface, 
* which are used to actually perform the measurement. 
*
* All the content has either been set by related Set methods of the CViComCWInterface, or
* they are still in their default values.
*
* @note
*     The values in this structures cannot be changed, they are read-only. Even if they
*     are modified after the structure is requested by the GetSettings() method, the changes
*     will not be reflected in the internal settings.
\@attribute[type="serializable"]
*/
struct SSettings
{
   /**
   Index of the receiver unit to which the settings shall be applied. Starts with 0 and can be at most 31.
   */
   DWORD                                                       dwReceiverIndex;
      
   /**
   This structure defines how the result buffer shall be dimensioned.
   */
   SResultBufferDepth                                          ResultBufferDepth;

   /**
   Specifies with what device the internal clock shall be synchronized
   */
   TimeBaseSynchronisationMode::Type                           eTimeBaseSynchronisationMode;

   /**
   The current measurement mode used to measure the specified frequency.
   */
   etMeasType                                                  eMeasType;
      
   /**
   The list of channels, specified by center frequency, bandwidth et. al.
   */
   SChannelSettings                                            ChannelSettings;
      
   /**
   Specifies how SFrequencySetting::eMinAttenuation is interpreted.
   */
   etAttenuationSwitchMode                                     eAttenuationSwitchMode;

   /**
   Aggregation mode for power values created from raw measurement chunks.
   */
   etDetector                                                  eDetector;

   /**
   Contains the type of the measurement scheduling (Free Run or External Trigger) and the trigger factor.
   */
   STriggerSettings                                            TriggerSettings;
};


/**
This function returns a measurement result from the result buffer or waits for the arrival of the next measurement result if the result
buffer is empty.

During the wait state the processor capacity is released for other threads of this process and other processes (WaitForSingleObject call).
If no result arrives within the timeout then the function returns with a NULL pointer and error VICEC_TIME_OUT.

@note
   that the result data are for reading only and available until the next interface function is called.
   \@attribute[type="serializable"]
*/
struct SMeasResult
{
   /**
   For each result the device provides the device time of start of the CW measurement which was used to produce the result.

   The dwPcTimeStampInMs is a conversion of this device time to the PC system time obtained with the GetTickCount() function.
   The adjustment of possible time drift between device time and GetTickCount() as well as the resolution of GetTickCount() causes
   a conversion error of a few milliseconds.

   dwPcTimeStampInMs may be used to synchronize CW results with other measurement data in the system or to determine
   the age of the CW result = GetTickCount() - dwPcTimeStampInMs;
   **/
   DWORD       dwPcTimeStampInMs;

   /**
   TMSx internal timestamp, that has a 40ns resolution. From this value, the pc measurement time is created.
   This is the earliest time of all the time values in the power value results.
   */
   ULONGLONG   u64DeviceTimeIn40ns;

   /**
   All the resulting power values are returned as a list that contains instances of this structure.
   * \@attribute[type="serializable"]
   */
   struct SPowerResult
   {
      /**
      Index of the channel that was used to create the power value. Refers to 
      SChannelSettings::TableOfFrequencySetting.
      */
      WORD        wFrequencyIndex;          

      /**
      * Timestamp when the measurement has been finished for that power value.
      */
      ULONGLONG   u64DeviceTimeIn40ns;
         
      /**
      Measured power value, multiplied by 100. Divide this value by 100 to receive the power value in dBm.
      */
      short       sPowerInDBm100;                   

      /**
      Attenuation mode used to measure the power value.
      */
      STSMxAttenuation::AttMode::Type
                  eAttMode;

      /**
      This is true when an overflow has been detected and attenuation switching was not able to eliminate it.
      Also refer to etAttenuationSwitchMode::AS_FIX.
      */
      bool        bAdcOverflow;
   };

   /**
   The list of all power values that have been measured since the last result was created. This
   can contain more than one sweep and also partial data from sweeps that were not finished before
   resp are not completely measured yet.

   To iterate over the result data use code similar to the one shown below:
   @code
      SViComList<SMeasResult::SPowerResult>::SLinkedObject* 
         pcPowerResult = pcResult->ListOfPowerResults.pFirst;
      while ( pcPowerResult )
      {
         // Do something with pcPowerResult, it can be used as shown in SPowerResult,
         // for example:
         double fPowerIndBm = (float)pcPowerResult->sPowerInDBm100 / 100.0;

         pcPowerResult = pcPowerResult->pNext;
      }
   @endcode
   */
   SViComList<SPowerResult> ListOfPowerResults;
};

} // namespace CW
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop)                                                                   // reset your structure alignment

#endif                                                                              //VICOM_RESOURCE_INCLUDE
