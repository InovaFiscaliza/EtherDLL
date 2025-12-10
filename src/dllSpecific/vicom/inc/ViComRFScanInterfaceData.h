// ViComRFScanInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_RFSCAN     2  // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)   // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                  // get a definite structure alignment for CViComRFScanInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace RFSCAN
{
/**
Data containers used for ViCom RFSCAN interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_RFSCAN,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
@brief Contains the lower and upper bound of the sweep range.

The sweep range structure can be used to determine the bandwidth that shall be scanned. The raw data is then measured
as specified in this structure.

The range is specified with a lower and an upper border.
\@attribute[type="serializable"]
*/
struct SRfSweepRange
{
   SRfSweepRange()
      : dMinFrequencyInHz( 0 )
      , dMaxFrequencyInHz( 0 )
   {}

   /** This is the minimum difference of dMaxFrequencyInHz and dMinFrequencyInHz **/
   static const DWORD   dwMinSpanInHz           = 1000000;

   /** 
   Lower bound of sweep range.
      
   The lower bound of the scan range interval. Specified in Hz.
   Minimum value is dwMinFreqInHz.
   **/
   double               dMinFrequencyInHz;

   /** @brief Upper bound of sweep range.
   *
   * The upper bound of the scan range interval. Specified in Hz.
   *
   * Max value allowed depends on the device used.
   **/
   double               dMaxFrequencyInHz;

   /**
   Transition frequencies between RF Sweep Sections (informative and for estimation of max. sweep rate).
   1st transition happens at 1 GHz
   **/
   static const DWORD   dwSweepSection12TransitionIn10Hz = 100000000;
   /**
   Transition frequencies between RF Sweep Sections (informative and for estimation of max. sweep rate).
   2nd transition happens at 2 GHz
   **/
   static const DWORD   dwSweepSection23TransitionIn10Hz = 200000000;
   /**
   Transition frequencies between RF Sweep Sections (informative and for estimation of max. sweep rate).
   3rd transition happends at 3 GHz
   **/
   static const DWORD   dwSweepSection34TransitionIn10Hz = 300000000;
   /**
   Transition frequencies between RF Sweep Sections (informative and for estimation of max. sweep rate).
   4th transition happends at 4.8 GHz
   **/
   static const DWORD   dwSweepSection45TransitionIn10Hz = 483024480;
};

/**
The sweep rate is the number of RF sweeps within a given time. The maximum sweep rate is limited by the start and stop frequency.
To estimate the maximum RF sweep rate the following parameters have to be taken into account:
The RF sweep speed is fixed to 10 kHz per microsecond. But for each sweep section transition defined in SRfSweepRange another
14 ms are needed.

A fix time is used for the return of the frequency synthesizer from dMaxFrequencyInHz to dMinFrequencyInHz. This time is 5 ms for
spans smaller than 50 MHz, 10 ms for spans greater than 50 MHz or 14 ms if the span crosses at least one of the different sweep
sections defined in SRfSweepRange.

If the sweep rate is lower than the calculated maximum sweep rate then the receiver may be used for other measurements in between
the RF sweeps, e.g. other RF sweeps with other spans and sweep rates.
\@attribute[type="serializable"]
*/
struct SRfSweepRate
{
   SRfSweepRate()
      : dwMaxCountOfRfSweepsPer1000Sec( 1 )
      , dwRfSweepCollectionCount( dwMaxRfSweepCollectionCount )
   {}

   /** 10 MHz / ms fixed RF sweep speed **/
   static const DWORD   dwSweepSpeedInHzPerMs            = 10000000;

   /** 50 MHz for different return times **/
   static const DWORD   dwReturnTimeTresholdInHz         = 50000000;

   /** 5 ms to return to start of sweep if the span is less than 50 MHz **/
   static const DWORD   dwLessThan50MHzReturnTimeInMs    = 5;

   /** 10 ms to return to start of sweep if the span is greater than 50 MHz **/
   static const DWORD   dwMoreThan50MHzReturnTimeInMs    = 10;

   /**
   14 ms for crossing a sweep section transition during sweep or
   return. For the return only one transition is counted.
   **/
   static const DWORD   dwTransitionTimeInMs             = 14;

   /**
   This is the definition of the average sweep rate the device shall not exceed. A calculation of the maximum value may be done with the
   interface function GetMaxRfSweepRate once the Rf Sweep start and stop frequency have been passed to the scanner.
   **/
   DWORD                dwMaxCountOfRfSweepsPer1000Sec;

   /**
   To reduce processing time and fire wire protocol overhead it is possible to collect some RF sweeps within the device. This may reduce
   the device and the processor load especially if the count of sweeps per second is higher than 10.

   A good compromise between processing load and response speed is reached if
   dwRfSweepCollectionCount == dwMaxCountOfRfSweepsPer1000Sec / 5000. So the collection time intervals are between 50ms and 100ms
      
   The collection count is limited by dwMaxRfSweepCollectionCount and by the memory needed for collected RF sweeps. This limitation is
   defined by the equation:
      
   (dMaxFrequencyInMHz - dwMinFreqInMHz) * dwRfSweepCollectionCount <= dwMaxProductCollectionCountTimesSpanInMHz
   **/
   static const DWORD   dwMaxRfSweepCollectionCount               = 100;

   /** This is 5 GHz **/
   static const DWORD   dwMaxProductCollectionCountTimesSpanInMHz = 5000;

   /**
   If the RF sweep span is small than the sweep rate may be dwMaxCountOfRfSweepsPer1000Sec <= 196078 for 1MHz span with 5 ms return
   time and 0.1ms sweep time. The result rate shall be limited to a maximum value dwMaxResultRatePer1000Sec which is 25 per second.
   In this case we have to collect at least 8 RF sweeps to produce one result.
   **/
   static const DWORD   dwMaxResultRatePer1000Sec = 25000;

   /**
   This the count of RF sweeps collected before transferring it through the fire wire.
   **/
   DWORD                dwRfSweepCollectionCount;
};

/**
For calculation of power average values in time, histograms or simply for collecting RF sweeps the sweeps can be buffered in the PC.
The buffer timeout is defined in the following structure.
\@attribute[type="serializable"]
*/
struct SRfSweepBuffer
{
   SRfSweepBuffer()
      : dwBufferTimeInMs( dwMaxBufferTimeInMs )
   {}

   /** Maximum value for dwBufferTimeInMs **/
   static const DWORD   dwMaxBufferTimeInMs = 60000;

   /** Minimum value for dwBufferTimeInMs **/
   static const DWORD   dwMinBufferTimeInMs = 100;

   /** A RF sweep will expire when RF sweep time in ms + dwBufferTimeInMs < GetTickCount(); **/
   DWORD                dwBufferTimeInMs;
};

/**
During the RF sweep measurement the device normally is used in the most sensitive attenuation for the measurement without usage of the
preamplifier.

If the measurement should operate in limited attenuation range, or with preamplifier usage, than it is possible to change both, the
maximum possible gain and the maximum possible attenuation. Only the most sensitive attenuation (TSMX_ATT_PA_ON_RF0_IF0 or
TSMXH_ATT_PA_ON_RF0_IF0) will use the preamplifier.
\@attribute[type="serializable"]
*/
struct SRfSweepAttenuationLimits
{
   /**
   This is the minimum attenuation that shall be used. This value must be more sensitive or equal to eMaxAttenuationMode.
   Sensivity is defined by the order in which the enum values in STSMxAttenuation::AttMode::Type
   are specified

   @note
      If the eMinAttenuationMode is set to TSMX_ATT_PA_ON_RF0_IF0 for a TSMx device
      or to TSMXH_ATT_PA_ON_RF0_IF0 for a TSMx_H device then the preamplifier is switched off.
   */
   STSMxAttenuation::AttMode::Type eMinAttenuationMode;

   /**
   Upper limit of attenuation mode. When device tries to extend attenuation because of ADC overflows for example, it
   will try no mode that has a higher value assigned than the value specified here.
   */
   STSMxAttenuation::AttMode::Type eMaxAttenuationMode;
};

/**
If an ADC overflow occurs during an RF Sweep the device has multiple options how to proceed with the measurement.
The behavior after an overflow is defined in the following structure:
\@attribute[type="serializable"]
*/
struct SRfSweepAdcOverflowAction
{
   SRfSweepAdcOverflowAction()
      : bRepeateOnAdcOverflow( true )
      , dwCountOfInsensitiveRfSweepsAfterOverflow( dwMaxCountOfInsensitiveRfSweepsAfterOverflow )
   {}

   /**
   On overflow:
   If bRepeateOnAdcOverflow is TRUE and the current attenuation is smaller than maximum attenuation defined in SRfSweepAttenuationLimits
   than the RF Sweep is repeated with the next higher attenuation setting. If bRepeateOnAdcOverflow is FALSE the RF sweep results will be
   transferred to the PC. 
     
   Note that it is possible to mark power result values contaminated by ADC overflows. 
   @see SSpecificParameters::SPowerValueFormatSpec::etPowerWithOverflowFormat 
   **/
   bool                 bRepeateOnAdcOverflow;

   /**
   The dwCountOfInsensitiveRfSweepsAfterOverflow value is used to define the measurements after overflow occurence. To avoid measurement
   repetitions it is possible to define the count of RF sweeps which are measured with the next higher allowed attenuation setting
   after an overflow. Especially if the RF sweep rate is high, a higher value for insensitive RF sweeps after overflow reception
   is appropriate to get the receiver the optimum attenuation range.
   **/
   static const DWORD   dwMaxCountOfInsensitiveRfSweepsAfterOverflow = 100;

   /**
   This specifies how many insensitive sweeps shall be done after an overflow happened.

   Max value is dwMaxCountOfInsensitiveRfSweepsAfterOverflow
   **/
   DWORD                dwCountOfInsensitiveRfSweepsAfterOverflow;
};

/**
The next definition deals with the ADC overflow actions described in SRfSweepAdcOverflowAction if the RF Sweep range touches more than one
RF Sweep section.

The different RF sweep sections are defined in structure SRfSweepRange and for each section the optimum attenuation settings may be
different.

This structure allows to define wether different attenuation settings or different preamplifier usage per section shall be enabled or not.
\@attribute[type="serializable"]
*/
struct SRfSweepSectionRelation
{
   /**
   If ePreamplifierUsage = EQUAL_PREAMP_USAGE_FOR_ALL_SECTIONS than all sections have the same preamplifier usage per RF sweep.
   As soon as the preamplifier usage would caus ADC overflows in one of the sections than the preamplifier is not used in
   neither section. If ePreamplifierUsage = MOST_SENSITIVE_PREAMP_USAGE_PER_SECTION the need to switch off the preamplifier in one section
   does not affect the preamplifier usage in another section.
   **/
   enum etPreamplifierUsage
   {
      MOST_SENSITIVE_PREAMP_USAGE_PER_SECTION   = 0,
      EQUAL_PREAMP_USAGE_FOR_ALL_SECTIONS       = 1

   }                    ePreamplifierUsage;

   /**
   This in general the same behavior as explained for the preamplifier usage but it applies not only for the preamplifier but for
   all other attenuation settings in the same sense.
   **/
   enum etAttenuation
   {
      MIN_ATT_FOR_ALL_SECTIONS                  = 0,
      EQUAL_ATT_FOR_ALL_SECTIONS                = 1

   }                    eAttenuation;
};

/**
By means of the functions: SetRfSweepFrequencySpan(), SetRfSweepSectionRelation(), SetRfSweepAdcOverflowAction() and SetRfSweepRate()
internal settings of the RF-Scanner may be changed and with the functions SelectReceiver(), SetResultBufferDepth() of
CViComBasicInterfaceData basic settings will be changed. Except of frequency settings all other settings have default values.
   
By means of this function you can retrieve the RF-Scanner settings which apply at the moment for measurements to be started or during
running measurements.

@note
   that the result, like all other results too, is only for reading. To change settings the functions mentioned above should be used.

\@attribute[type="serializable"]
*/
struct SSettings
{
   // basic settings
   DWORD                             dwReceiverIndex;             //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth                ResultBufferDepth;           //!< This structure defines how the result buffer shall be dimensioned.

   // measurement specific settings
   SRfSweepRange                    RfSweepRange;                 //!< defines the minimum and maximum limits for a sweep
   SRfSweepRate                     RfSweepRate;                  //!< defines the sweep frequency
   SRfSweepBuffer                   RfSweepBuffer;                //!< defines the size of the result cache
   SRfSweepAttenuationLimits        RfSweepAttenuationLimits;     //!< defines minimum and maximum attenuation mode used to ease some error conditions.
   SRfSweepAdcOverflowAction        RfSweepAdcOverflowAction;     //!< defines handling of ADC overflows
   SRfSweepSectionRelation          RfSweepSectionRelation;       //!< defines how to handle the attenuation in the different frequency sections
};

/**
This structure contains the positive lobe of a symmetrical channel filter.

The channel power is calculated as c * Sum[p(f) * pfPositiveLobe(|f|)] with the internal correction factor c which corrects the
filter bandwidth and frequency sampling distance of the RF sweep in a way that the channel filter will return the power of an
applied CW (continuous wave) carrier, when the transfer function in pfPositiveLobe is constant 1 within the frequency section the
CW carrier is received.

The sample below shows how to define a filter using a power scaling function for GSM channels:
@code
const float cfPositiveLobePowerScaling[] = { 
      1.000, //       0 Hz
      0.988, //   20000 Hz
      0.951, //   40000 Hz
      0.891, //   60000 Hz
      0.809, //   80000 Hz
      0.707, //  100000 Hz
      0.588, //  120000 Hz
      0.454, //  140000 Hz
      0.309, //  160000 Hz
      0.156, //  180000 Hz
      0.000  //  200000 Hz
   };

SChannelFilter cChannelFilter;
cChannelFilter.wFilterTag = 0;
cChannelFilter.dFrequencySpacingInHz = 20000;
cChannelFilter.dwCountOfInterpolatingPoints = (sizeof(cfPositiveLobePowerScaling)) / (sizeof(float));

memcpy(cChannelFilter.fPositiveLobePowerScaling, cfPositiveLobePowerScaling, 
   cChannelFilter.dwCountOfInterpolatingPoints);
@endcode

\@attribute[type="serializable"]
*/
struct SChannelFilter
{
   /**
   The filter may be referenced later by means of its tag. Only one filter per tag may be defined.
   **/
   WORD                       wFilterTag;

   /**
   Lower limit for the frequency spacing of the interpolating function
   **/
   static const DWORD         dwMinFrequencySpacingInHz = 1000;

   /**
   Upper limit for the frequency spacing of the interpolating function
   **/
   static const DWORD         dwMaxFrequencySpacingInHz = 10000000;

   /**
   This defines the distance between each of the points in the power scaling and interpolation function.

   Minimum is dwMinFrequencySpacingInHz, the maximum is dwMaxFrequencySpacingInHz.
   **/
   double                     dFrequencySpacingInHz;

   /** Minimum value allowed for dwCountOfInterpolatingPoints **/
   static const DWORD         dwMinCountOfInterpolatingPoints = 2;

   /** Maximum value allowed for dwCountOfInterpolatingPoints **/
   static const DWORD         dwMaxCountOfInterpolatingPoints = 3000;

   /**
   The symmetric filter is defined by one lobe as a linear interpolation function with dwMinCountOfInterpolatingPoints to
   dwMaxCountOfInterpolatingPoints equidistant interpolating points.
   **/
   DWORD                      dwCountOfInterpolatingPoints;

   /** Each value in the fPositiveLobePowerScaling must be greater than or equal to this value **/
   static const DWORD         dwMinPowerScaling = 0;

   /** Each value in the fPositiveLobePowerScaling must be less than or equal to this value **/
   static const DWORD         dwMaxPowerScaling = 2;

   /**
   For each interpolation point this specifies the positive power scaling. Each of that interpolated points
   is multiplied by the according power scaling value when the overall channel power is calculated. 
   The frequencies shown below are added or subtracted to the center frequency of the channel.

   - fPositiveLobePowerScaling[0] scaling at 0 Hz,
   - fPositiveLobePowerScaling[1] scaling at +- dFrequencySpacingInHz,
   - fPositiveLobePowerScaling[2] scaling at +-2 * dFrequencySpacingInHz,
   - fPositiveLobePowerScaling[k] scaling at +-k * dFrequencySpacingInHz, 0 <= k < dwCountOfInterpolatingPoints

   @note 
      If k * dFrequencySpacingInHz is smaller than the channel bandwidth, not all raw measurement values of the 
      channel might be used. On the other hand, if k * dFrequencySpacingInHz is greater then the channel bandwidth,
      measurement values from other channels could influence the result.
   **/
   float                      fPositiveLobePowerScaling[dwMaxCountOfInterpolatingPoints];

   /**
   The minimum bandwidth for which the love power scaling function must be defined.

   The bandwidth is 2 * (dwCountOfInterpolatingPoints - 1) * dFrequencySpacingInHz and should be in the range
   dwMinBandwidthInHz to dwMaxBandwidthInHz.
   **/
   static const DWORD         dwMinBandwidthInHz = 100000;

   /**
   The maximum bandwidth of the lobe function can be 100 MHz.
   @see dwMinBandwidthInHz
   */
   static const DWORD         dwMaxBandwidthInHz = 100000000;
};

/**
After one or more RF sweeps are running in the device, specific post processed information may be retrieved from the RF sweeps.
The post process and a repetition of post processes is defined in SSpecificParameters:
\@attribute[type="serializable"]
**/
struct SSpecificParameters
{
   /**
   @brief Maximum number of post processes.

   Up to 32 post processes may run simultaneous. Each post process has a tag 0 <= wRequestIdentifier < 32
   **/
   static const WORD          wMaxRequestIdentifier = 31;

   /**
   @brief Id of the request

   Since up to 32 different requests can be active for one scanner interface, the results might come in any order when
   requested using the ViComRFPowerScannerInterface::GetResult() method. To distinguish which result belongs to which
   request, the request identifier is stored in both structures.
   **/
   WORD                       wRequestIdentifier;

   /**
   Number of request repetitions. Note that if this is greater than 1, then one call to 
   RequestDerivedResult() will result in a behavior similar to dwCountOfRequests calls
   to the function internally, each request at least dwTimeBetweenRequestsInMs after the one before.

   Defines the count of repetitions. 0 means a stop of the previous request.
   **/
   DWORD                      dwCountOfRequests;

   /**
   The longest turn between two measurements can be 1 minute.
   */
   static const DWORD         dwMaxTimeBetweenRequestsInMs = 60000;

   /**
   The time between requests is limited to the reciprocal of the current max result rate of the RF Sweep which is
   (1000000 * SRfSweepRate::dwRfSweepCollectionCount / SRfSweepRate::dwMaxCountOfRfSweepsPer1000Sec) and 1 minute.

   In the case that only one specific result is requested the lower limit of dwTimeBetweenRequestsInMs is 0.
   For the first post process the most recent RF sweeps not older than dwTimeBetweenRequestsInMs in the RF sweep buffer are used to
   calculate the first result.

   After the first result, the value specified here is used to trigger an internal timer, that results in repeated
   result requests equivalent to the first one.
   **/
   DWORD                      dwTimeBetweenRequestsInMs;

   /**
   There are two different post process types with respect to the frequency axis implemented.
   **/
   enum etFreqPostProcess
   {
      SPECTRUM_DISPLAY        = 0,     //!< use UFreqPostProcess::SSpectrumDisplay to specify further details for post-processing
      CHANNEL_SEQUENCE        = 1      //!< use UFreqPostProcess::SChannelSequence to specify further details for post-processing
   }                          eFreqPostProcess;

   /**
   This union containes the exclusive possibilities to get results regarding frequency domain power data calculations. Either the
   classical spectrum display is prepaired (eFreqPostProcess = SPECTRUM_DISPLAY) or a channel power values are calculated
   (eFreqPostProcess = CHANNEL_SEQUENCE).
   * \@attribute[type="serializable"]
   **/
   union UFreqPostProcess
   {
      /**
      A spectrum display is the graphical result for a spectrum measurement. The graphical result has less or equal information
      than the result from the RF sweep. Respectively it has less data. 
         
      Normally the graphical result has less vertical display lines than power measurements have been taken in the RF sweep.
      In this case there are more power measurements per display line available.
      The frequency detector (eFreqDetector) defines, how to get a display from the set of RF sweep measurements per display line.
     * \@attribute[type="serializable"]
      **/
      struct SSpectrumDisplay
      {
         /**
         The lowest frequency that will be delivered in the result.

         @note
            dMinFrequencyInHz and dMaxFrequencyInHz have to be a subsection of the RF sweep frequency range.
         **/
         double               dMinFrequencyInHz;

         /**
         The highest frequency that will be delivered in the result.

         @note
            dMinFrequencyInHz and dMaxFrequencyInHz have to be a subsection of the RF sweep frequency range.
         **/
         double               dMaxFrequencyInHz;

         /**
         This informative constant value gives the native frequency resolution.
         **/
         static const DWORD   dwRFSweepFrequencyResolutionInHz = 12800;

         static const DWORD   dwMinCountOfDisplayLines = 1;          //!< Lower limit for dwCountOfDisplayLines
         static const DWORD   dwMaxCountOfDisplayLines = 3000000;    //!< Upper limit for dwCountOfDisplayLines

         /**
         The number of available display lines. For some post process calculation the selection of the native resolution may be
         helpful. This is achieved by setting

         dwCountOfDisplayLines = (dMaxFrequencyInHz - dMinFrequencyInHz) / dwRFSweepFrequencyResolutionInHz.
         **/
         DWORD                dwCountOfDisplayLines;

         /**
         * Define how to calculate a characteristic value for frequency if more than one value is available for that frequency.
         * This is the case for example if 
         */
         enum etFreqDetector
         {
            FD_PEAK           = 0,     //!< The maximum measured power value per display line is used.
            FD_RMS            = 1,     //!< The root mean sqare value calculated from the power values per display line in Watt is used.
            FD_AUTO_PEAK      = 2      //!< The maximum and the minimum measured power value per display line are used.
         }                    eFreqDetector;
      };

      /**
      SChannelFilterSequence defines the center frequencies of channels using one channel filter. The channels may be equally spaced or at
      arbitrary frequencies or a mixture of both.

      If some of the channels are equally spaced then this channels may be collected in subsequences. If some channels are not equally
      spaced than one subsequence has to be defined for each of it.
     * \@attribute[type="serializable"]
      **/
      struct SChannelSequence
      {
         /** Same as used in SChannelFilter to identify the channel filter. **/
         WORD                 wFilterTag;

         /** lower limitation of subsequences that can be stored **/
         static const DWORD   dwMinCountOfSubSequences = 1;
         /** upper limitation of subsequences that can be stored **/
         static const DWORD   dwMaxCountOfSubSequences = 500;

         /** Number of channel filter sequences **/
         DWORD                dwCountOfSubSequences;

         /** lower limitation of channels in different subsequences **/
         static const DWORD   dwMinChannelCountInSubsequence = 1;
         /** upper limitation of channels in different subsequences **/
         static const DWORD   dwMaxCountOfChannels = 1000;

         /** 
         Each subsequence defines a set of equally spaced channel filters of the same type (wFilterTag) 

         There are two possible approaches to set this properly: 
            
         Either the dwChannelCount is set to one, in which case the dChannelSpacingInHz is ignored, or the dwChannelCount
         is set to a value greater than one. In the latter case, the sub-sequence defines a set of channels and 
         dChannelSpacing is the size of one channel. 
            
         In total, the covered frequency range is:

         [ dFirstCenterFreqInHz - dChannelSpacingInHz/2; 
            dFirstCenterFreqInHz + dwChannelCount * dChannelSpacingInHz - dChannelSpacingInHz/2 ]

         @note
            If you specify a non-uniformly distributed channel sequence, the marker tool will not work correctly.
            This is considered as a bug and will be fixed in one of the next releases.
      * \@attribute[type="serializable"]
         **/
         struct SSubSequence
         {
            /** Number of channel filters to be used. **/
            DWORD             dwChannelCount;
            /** Center frequency of first channel. **/
            double            dFirstCenterFreqInHz;
            /** Should be set to 0 if dwChannelCount == 1. Else it should be > 0. **/
            double            dChannelSpacingInHz;

         };

         /**
            * @details \@attribute[type="array" size="dwCountOfSubSequences" sizeismember="true"]
            */
         SSubSequence SubSequence[dwMaxCountOfSubSequences];
      };

      /**
         * @details \@attribute[type="unionfield" discriminator="eFreqPostProcess" value="etFreqPostProcess::SPECTRUM_DISPLAY"]
         */
      SSpectrumDisplay SpectrumDisplay;

      /**
         * @details \@attribute[type="unionfield" discriminator="eFreqPostProcess" value="etFreqPostProcess::CHANNEL_SEQUENCE"]
         */
      SChannelSequence ChannelSequence;

   }                          FreqPostProcess;

   /**
   For post processing in time domain different detectors may be used. The Detector applies either for the same frequency display line or
   for the same channel. It is applied after the frequency detector or channel filter.
   **/
   enum etTimeDetector
   {
      TD_AUTO_PEAK            = 2,        //!< The minimum and the maximum value is returned.
      TD_MAX_PEAK             = 0,        //!< The maximum value is returned.
      TD_MIN_PEAK             = 3,        //!< The minimum value is returned. 
      TD_RMS                  = 1,        //!< The average power in Watt is calculated.
      TD_ALL                  = 5,        //!< All power values after the frequency detector of the channel filter are returned.
      TD_HISTOGRAM            = 4         //!< A histogram is returned, showing which frequency/power-value pair occured how often. 
                                          //!< @see SMeasResult::pwPowerWithOverflowValues for details
   }                          eTimeDetector;

   /**
   The result values are in integer format. The integer is between 0 and wMaxPowerInteger. Whereby both limits 0 and wMaxPowerInteger
   may be caused by saturation, e.g. the measured power is lower than or greater then the measured power.
      
   An integer wPower means that the measured power was

   Power/dBm = fMinPowerValueInDBm + wPower * fPowerResolutionInDB +/- 0.5 * fPowerResolutionInDB;
      
   For eTimeDetector = TD_HISTOGRAM a histogram with

         (wMaxPowerInteger + 1) * dwCountOfDisplayLines or

         (wMaxPowerInteger + 1) * Sum(dwChannelCount)

   counter values is returned.
   * \@attribute[type="serializable"]
   */
   struct SPowerValueFormatSpec
   {
      /** at least 5 differnt power values should be possible **/
      static const WORD       wMinOfMaxPowerInteger = 5;

      /** 
      This defines the upper limit that will be returned in the result list of power values.
      */
      WORD                    wMaxPowerInteger;

      /**
      min value for fMinPowerValueInDBm
      */ 
      static const long       lMinPowerValueIndBm = -150;

      /**
      max value for fMinPowerValueInDBm
      */ 
      static const long       lMaxPowerValueIndBm = 50;

      /**
      lower limit value for (fMinPowerValueInDBm + wMaxPowerInteger) - fMinPowerValueInDBm 
      */ 
      static const long       lMinPowerValueRangeIndB = 10;

      /** 
      Base Power Value to which all result values reference.

      This is the base power value that is used as a base level for all power values. If there is a power
      value smaller that this lower threshold, then the value will be stored as this minimum value. All
      result value greater this have to be calculated as       
          
      Power/dBm = fMinPowerValueInDBm + wPower * fPowerResolutionInDB +/- 0.5 * fPowerResolutionInDB;
      **/
      float                   fMinPowerValueInDBm;

      /** 
      Resolution of the result values.

      This is the maximum resolution for the power values. The resolution is specified as 10^-x, where x is the
      number of non-integer digits. Must be less than (lMaxPowerValueIndBm - fMinPowerValueInDBm) / wMaxPowerInteger.
                       
      @see fMinPowerValueInDBm
      **/
      float                   fPowerResolutionInDB;

      /**
      Defines whether the result values stored in SMeasResult::pwPowerWithOverflowValues contain some indication
      of an ADC overflow and how this is encoded in the result. 
         
      Possible values are:
      - no overflow information, i.e. the power value can be directly used.
      - lowest bit contains overflow flag, i.e. to get power value do a right shift by 1.
      - highest bit contains overflow flag, i.e. do power & 0xfffe to get power value.
      */
      enum etPowerWithOverflowFormat
      {
         POWER_ONLY = 0,                              //!< the pwPowerWithOverflowValues[] contains only the power values
         POWER_VALUE_LEFT_SHIFT1_OV_IN_BIT0 = 1,      //!< the pwPowerWithOverflowValues[] contains 2 * power | ov_bit
         POWER_VALUE_OV_IN_BIT0_OVERWRITE = 2         //!< the pwPowerWithOverflowValues[] contains power & 0xfffe | ov_bit
      }                       ePowerWithOverflowFormat;

   }                          PowerValueFormatSpec;

   /**
   If this is set to true, then the result will be filtered by the marker tool. The interpretation of the SMeasResult::pwPowerWithOverflowValues
   is then different. The field SMeasResult::pMarkerResult will also only be filled if this is true.
   */
   bool                       bUseRequestForMarker;
};

/**
This function returns a measurement result from the result buffer or waits for the arrival of the next measurement result if the result
buffer is empty.

During the wait state the processor capacity is released for other threads of this process and other processes (WaitForSingleObject call).
If no result arrives within the timeout then the function returns with a NULL pointer and error VICEC_TIME_OUT.

@note
   that the result data are for reading only and available until the next interface function is called.

\@attribute[type="serializable"]
**/
struct SMeasResult
{
   /**
   For each result the device provides the device time of start of the RF measurement which was used to produce the result. If more than
   one RF sweep was used to produce the result than this time stamp corresponds to the start time of the last RF sweep.

   The dwPcTimeStampInMs is a conversion of this device time to the PC system time obtained with the GetTickCount() function.
   The adjustment of possible time drift between device time and GetTickCount() as well as the resolution of  GetTickCount() causes
   a conversion error of a few milliseconds.

   dwPcTimeStampInMs may be used to synchronise RF-Power-Scanner results with other measurement data in the system or to determine
   the age of the RF-Power-Scanner result = GetTickCount() - dwPcTimeStampInMs;
   **/
   DWORD             dwPcTimeStampInMs;

   /**
   The request structure for the post process remains stored in the ViCom interface DLL. Each meas result contains the pointer to the
   request structure the result belongs to.
   **/
   SSpecificParameters
                     *pSpecificParameters;

   /**
   This specifies the number of values that can be accessed in the pwPowerWithOverflowValues result array.
   The highest value accessible is therefore pwPowerWithOverflowValues[ dwCountOfPowerWithOverflowValues - 1 ].
   */
   DWORD             dwCountOfPowerWithOverflowValues;

   /**
   The main output of the RF-Power-Scanner measurements are power values. The usage of power values is explained first for normal
   request when bUseRequestForMarker = FALSE. Then the power values for either the frequency lines
   or the channels are stored. The count of values and its order depends on the used detectors. If one of the detectors
   FD_AUTO_PEAK or TD_AUTO_PEAK is used then value pairs {Max Power, Min Power} are stored. If the time
   detector is not TD_ALL or TD_HISTOGRAM than for each frequency line of for each channel a power value pair or a single power value is
   stored.

   If the time detector is TD_ALL than for each RF Sweep within the time interval of the post process a separate line with single or pair
   values for frequency lines or channels is stored. The first line belongs to the oldest RF sweep and the last line belongs to the most
   recent measured RF sweep.

   By means of ePowerWithOverflowFormat it is possible to use one bit in pwPowerWithOverflowValues[] to indicate ADC overflows
   occurred during the calculation of the corresponding power value. After the integer wPower is recovered by removing the
   overflow bit, the power in dBm can be calculated by:

   @code
   Power/dBm = fMinPowerValueInDBm + wPower * fPowerResolutionInDB; with a numerical uncertainty of +/- 0.5 * fPowerResolutionInDB.
   @endcode
      
   A special case is the TD_HISTOGRAM. For TD_HISTOGRAM pwPowerWithOverflowValues points rather to counter values than to power values.
   For SPECTRUM_DISPLAY pwPowerWithOverflowValues[p * dwCountOfDisplayLines + f] contains the number of power measurements at
   frequency display line f within the power range defined by p:
   from fMinPowerValueInDBm + p * fPowerResolutionInDB to fMinPowerValueInDBm + (p + 1) * fPowerResolutionInDB.

   When bUseRequestForMarker = TRUE the power values are stored for the marker frequency line or the marked channel only. Always
   one single (FD_PEAK, FD_RMS) value or pair (FD_AUTO_PEAK) of values is stored per RF sweep.

   \@attribute[type="array" size="dwCountOfPowerWithOverflowValues" sizeismember="true"]
   **/
   WORD              *pwPowerWithOverflowValues;

   /**
   When the result was used to get a marker information the pointer pMarkerResult is not NULL and points to the marker result.
   The marker is set to the frequency or channel with the maximum power measured after the time detector has been applied.
   * \@attribute[type="serializable"]
   **/
   struct            SMarkerResult
   {
      /**
      The marker frequency after the frequency detector for SPECTRUM_DISPLAY or the channel center frequency for
      CHANNEL_SEQUENCE.
      **/
      double         dMarkerFrequencyInHz;

      /**
      For FD_AUTO_PEAK and/or TD_AUTO_PEAK a minimum and a maximum power value may be calculated. 
      Else both values are equal.
      **/
      WORD           wMinResultValue;

      /**
      For FD_AUTO_PEAK and/or TD_AUTO_PEAK a minimum and a maximum power value may be calculated. 
      Else both values are equal.
      **/
      WORD           wMaxResultValue;

   }                 *pMarkerResult;

   /**
   This is true if any of the power values in the result was hit by an ADC overflow.
   **/
   bool              bOverflowIndicator;
};

} // namespace RFSCAN
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
