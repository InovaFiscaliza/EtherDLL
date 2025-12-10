// ViComRFPowerScanInterface.h
#pragma once
#include "ViComRFScanInterfaceData.h"
#include "ViComRFScanErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComRFScanInterface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace RFSCAN
{

/**
Explanation of RF-Power Scan measurements

The RF Power scan measurements are derived from a set of successive fundamental spectrum measurements. To make a difference between this
fundamental R&S TSMx spectrum measurements and results derived from this measurements the fundamental R&S TSMx spectrum measurements are called
RF sweeps.

The RF sweeps have a variable start and stop frequency but fix values for sweep speed,
resolution bandwidth and sampling rate. The sweep speed is 1 GHz / 0.1 sec, the resolution bandwidth 30 kHz and the sampling rate (25/32) MHz
which gives a frequency shift of 12.8 kHz between adjacent power values of the RF sweep. E.g an RF sweep from 200MHz to 700MHz evaluates
the antenna signal from 200 MHz to 700 MHz for 50 ms and produces a power spectrum with 39063 power values.
The small K Factor of 0.09 is realized by digital signal processing in the FPGA of the R&S TSMx.

Once started the measurement with the StartMeasurement function of the CViComBasicInterface the R&S TSMx carries out RF sweeps and collects
the most recent RF sweeps within an internal buffer until they will expire after an requested time. From the measurements in this buffer a
variety of different specific results can be requested and will be calculated in a post process. This is outlined below.

To get ViCom results from RF sweeps other specific request have to be dispatched via the ViCom interface. Such a request may produce
one result or more results with an constant rate. Examples for requests are:

- Sections of the RF-Sweep with a user defined resolution at the frequency axis.
- Calculated power values for defined RF channels.
- Max peak markers within a small frequency interval of the RF sweep.

The number of ViCom request running in parallel is limited to 32. Each ViCom request may be stopped before the number of desired results
was responded. To identify results from certain requests a request specific identifier is used. If a new ViCom request is posted but there
is still one active using the same identifier then the active request with the same identifier will be stopped immediately and the new one
is placed.
\@attribute[type="interface"]
*/
class CViComRFScanInterface
{
public:
   typedef etViComInterfaceVersion VERSION_TYPE;

public:
   /**
   Returns reference to basic interface object implementation.

   The basic interface is a part of any application specific ViCom interface. It is used for different R&S TSM applications.
   The purpose is to control basic R&S TSM functions, which are not specific for a certain measurement.
   
   @see CViComBasicInterface for specification of basic interface functions.

   @return reference to the BasicInterface object that can be used to control basic functions like
      StartMeasurement() and StopMeasurement() et al.
   */
   virtual CViComBasicInterface& GetBasicInterface( ) = 0;

   /**
   Set sweep range (min and max frequency).

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.
   @param[in] rRfSweepRange
         contains min and max frequency in which the frequency sweep shall take place

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetRfSweepFrequencySpan( CViComError& rViComError, const SRfSweepRange& rRfSweepRange ) = 0;
   virtual void SetRfSweepFrequencySpan( const SRfSweepRange& rRfSweepRange )                           = 0;
   //!< @throw CViComError @see SetRfSweepFrequencySpan( CViComError&, SRfSweepRange& )


   /** @brief Set RF sweep parameters related attenuation handling in case of an ADC overflow.

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.
   @param[in] rRfSweepSectionRelation
         this contains settings to control how to handle the attenuation the ADC overflow handling

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetRfSweepSectionRelation( CViComError& rViComError, const SRfSweepSectionRelation& rRfSweepSectionRelation ) = 0;
   virtual void SetRfSweepSectionRelation( const SRfSweepSectionRelation& rRfSweepSectionRelation )                           = 0;
   //!< @throw CViComError @see SetRfSweepSectionRelation( CViComError&, SRfSweepSectionRelation& )


   /** @brief Define how an overflow during ADC shall be handled

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.
   @param[in] rRfSweepAdcOverflowAction
         this contains settings to control the ADC overflow handling

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetRfSweepAdcOverflowAction( CViComError& rViComError, const SRfSweepAdcOverflowAction& rRfSweepAdcOverflowAction ) = 0;
   virtual void SetRfSweepAdcOverflowAction( const SRfSweepAdcOverflowAction& rRfSweepAdcOverflowAction )                           = 0;
   //!< @throw CViComError @see SetRfSweepAdcOverflowAction( CViComError&, SRfSweepAdcOverflowAction& )
   
   /**
   Defines how often a sweep shall be performed.

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.
   @param[in] rRfSweepRate
         this structure contains the settings that shall be applied to the interface object.

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetRfSweepRate( CViComError& rViComError, const SRfSweepRate& rRfSweepRate ) = 0;
   virtual void SetRfSweepRate( const SRfSweepRate& rRfSweepRate )                           = 0;
   //!< @throw CViComError @see SetRfSweepRate( CViComError&, SRfSweepRate& )


   /**
   Set RF sweep Parameters

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.
   @param[in] rRfSweepBuffer
         this structure contains the settings that shall be applied to the interface object. Specifies the size of the post-process
         result buffer.

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetRfSweepBufferTime( CViComError& rViComError, const SRfSweepBuffer& rRfSweepBuffer ) = 0;
   virtual void SetRfSweepBufferTime( const SRfSweepBuffer& rRfSweepBuffer )                           = 0;
   //!< @throw CViComError @see SetRfSweepBufferTime( CViComError&, SRfSweepBuffer& )


   /**
   Set RF sweep Parameters

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.
   @param[in] rRfSweepAttenuationLimits
         this structure contains the settings that shall be applied to the interface object.

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetRfSweepAttenuationLimits( CViComError& rViComError, const SRfSweepAttenuationLimits& rRfSweepAttenuationLimits ) = 0;
   virtual void SetRfSweepAttenuationLimits( const SRfSweepAttenuationLimits& rRfSweepAttenuationLimits )                           = 0;
   //!< @throw CViComError @see SetRfSweepAttenuationLimits( CViComError&, SRfSweepAttenuationLimits& )

   /**
   This function returns NULL on error or the maximum dwMaxCountOfRfSweepsPer1000Sec for the frequency span which is active at the moment.
   The value dwMaxCountOfRfSweepsPer1000Sec can be achieved if no other measurement, e.g. another RF Sweep measurement, is running at the
   R&S TSMx. Especially if the requested RF sweep rate is high than the R&S TSMx has to collect some RF sweeps before transferring it through the
   Fire Wire. 
   
   For the value dwMaxCountOfRfSweepsPer1000Sec the minimum dwRfSweepCollectionCount is returned with this function.
   A smaller dwRfSweepCollectionCount may be used (as defined in SRfSweepRate) when a smaller RF sweep rate is requested.

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.

   @return a structure filled with the currently valid values or NULL if the call was not processed successfully.
   **/
   virtual const SRfSweepRate* GetMaxRfSweepRate( CViComError& rViComError ) = 0;
   virtual const SRfSweepRate* GetMaxRfSweepRate( )                          = 0;
   //!< @throw CViComError @see GetMaxRfSweepRate( CViComError& )

   /**
   By means of the functions:
   SetRfSweepFrequencySpan(), SetRfSweepSectionRelation(), SetRfSweepAdcOverflowAction() and SetRfSweepRate()
   internal settings of the RF-Power-Scanner may be changed and with the functions SelectReceiver(), SetResultBufferDepth() of CViComBasicInterface basic
   settings are changed. 
   
   All settings have default values. By means of this function you can retrieve the RF-Power-Scanner
   settings which apply at the moment for measurements to be started or during running measurements.
   
   @note
      The result, like all other results too, is only for reading. To change settings the functions mentioned above shall be used.

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.

   @return 
      a structure filled with the currently valid values or NULL if the call was not processed successfully.
   **/
   virtual const SSettings* GetSettings( CViComError& rViComError ) = 0;
   virtual const SSettings* GetSettings( )                          = 0;
   //!< @throw CViComError @see GetSettings( CViComError& )

   /**
   To receive the results from the R&S TSMx not per frequency, but as power values of channels, a channel filter has to be
   specified. The R&S TSMx uses a symmetric channel filter to convert raw measurement values into channel powers.

   To use a channel filter set with this function, you have to set the SSpecificParameters::eFreqPostProcess value to CHANNEL_SEQUENCE 
   and provide the SSpecificParameters::UFreqPostProcess::SChannelSequence::wFilterTag when calling RequestDerivedResults(). 

   @note 
         that this function might fail if you try to set a filter that has been defined before. Filters are recognized
         using the wFilterTag. 

   @param[out] rViComError
         this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.

         You might encounter one of these error codes:
         - 11012: you already set a channel filter with the given tag. You cannot define a filter twice, so use another
               channel filter.
         - 11013: check if one of the following conditions is true:
               -# is dFrequencySpacingInHz within dwMinFrequencySpacingInHz and dwMaxFrequencySpacingInHz ?
               -# is dwCountOfInterpolatingPoints within dwMinCountOfInterpolatingPoints and dwMaxCountOfInterpolatingPoints ?
               -# is bandwidth = 2 * (dwCountOfInterpolatingPoints - 1) * dFrequencySpacingInHz within the range dwMinBandwidthInHz to dwMaxBandwidthInHz ?
         - 11014: each value in SChannelFilter::fPositiveLobePowerScaling must be within dwMinPowerScaling and
               dwMaxPowerScaling range

   @param[in] rChannelFilter
         Structure that defines the parameters of the channel filter. The channel filter is defined as function from
         the center frequency to some greater frequency.

   @see SChannelFilter for more details on the calculation algorithm.

   @return 
         true if values from input structure could be applied, false otherwise
   **/
   virtual bool SetChannelFilter( CViComError& rViComError, const SChannelFilter& rChannelFilter ) = 0;
   virtual void SetChannelFilter( const SChannelFilter& rChannelFilter )                           = 0;
   //!< @throw CViComError @see SetChannelFilter( CViComError&, SChannelFilter& )

   /**
   Use this function to start calculation of a spectrum. The given SSpecificParameters define the
   post process configuration that will lead to the final results.

   When this function returns true, results can be queried using the GetResult() function. Note that not all results will be available
   as soon as the function returns. This heavily depends on how many measurement values are requested and how much delay between
   the measurement results is desired.

   Since this function returns immediately and processing is performed in the background, multiple requests can be run in parallel.
   The different requests are distinguished using the wRequestIdentifier value in the input structure.
   
   @note The results of this function are stored in the result buffer. Be sure to only request as much results at a time that can
      be kept in the result buffer, otherwise you may loose values if you don't get the results in time.

   @param rViComError
         [out] this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution
         was successful or not.

   @param[in] rSpecificParameters
         this parameter configures the post-process that is applied to convert the raw measurement data received from the
         R&S TSMx device into a frequency spectrum.
         If the same wRequestIdentifier in the structure has been requested before and calculation has not finished for that request,
         the old processing is stopped and the new request starts processing.

   @return true if values from input structure could be applied, false otherwise
   **/
   virtual bool RequestDerivedResult( CViComError& rViComError, const SSpecificParameters& rSpecificParameters ) = 0;
   virtual void RequestDerivedResult( const SSpecificParameters& rSpecificParameters )                           = 0;
   //!< @throw CViComError @see RequestDerivedResult( CViComError&, SSpecificParameters& )

   /**
   This function returns a measurement result from the result buffer or waits for the arrival of the next measurement result if the result
   buffer is empty.
   During the wait state the processor capacity is released for other threads of this process and other processes (WaitForSingleObject call).
   If no useful result arrives within the timeout then the function returns with a NULL pointer and error VICEC_TIME_OUT.
   
   @note that the result data are for reading only and available until the next interface function is called.

   @note that the result might come from any request that has been started and not yet finished without any particular order.
      So if you want to process the results depending on the request identifier, you have to copy store the results in some
      data container and index them using the request identifier (using a dictionary, for example).  It is important to
      copy the data in that case, otherwise the next time this function is called the old values are lost.

   @param[out] rViComError
         This is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.

   @param[in] dwTimeOutInMs
         Number of milliseconds the caller will be blocked if the method has to wait for additional results. If there
         are enough results available in the buffer, then this is ignored.

   @return A structure filled with the requested measurement results or NULL if the call was not processed successfully.
         If not NULL, the result given is removed from the result buffer. The result is only valid as long as the function
         is not called a second time.
   */
   virtual const SMeasResult* GetResult( CViComError& rViComError, DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs ) = 0;
   virtual const SMeasResult* GetResult( DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs )                           = 0;
   //!< @throw CViComError @see GetResult( CViComError&, DWORD )


   //* CViComResultDataProcessor integration ************************************/
   /**
   Register a ScannerData listener that will be notified when new data is available. Note that it is
   important to Unregister the listener again once it is getting deleted, to avoid crashes.

   @note Both possibilities to retrieve results are to be used exclusively. In case of ResultDataListener usage,
   it is not allowed to call GetResultCounters() or GetResult() and vice versa.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] pListener
      The object that shall be notified when new measurement data is available. This must not be
      null. If the listener has already been registered to the interface, it will not be
      registered a second time. Instead, an error will be indicated.

   @return
      if listener has been appended to the list of registered listeners or not
   */
   virtual bool RegisterResultDataListener( CViComError& rViComError, CViComResultDataProcessor<SMeasResult>* pListener ) = 0;
   virtual void RegisterResultDataListener( CViComResultDataProcessor<SMeasResult>* pListener )                           = 0; 
   //!< @throw CViComError @see RegisterResultDataListener( CViComError&, CViComResultDataProcessor* )


   /**
   Unregister a formerly registered ScannerData listener to remove it from the list of objects that
   shall get notification about new measurement data.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] pListener
      The listener that shall be unregistered. If NULL or if the listener is not found in the list of registered
      objects, an error is indicated.

   @return
      if listener has been removed from the list of registered listeners or not
   */
   virtual bool UnregisterResultDataListener( CViComError& rViComError, CViComResultDataProcessor<SMeasResult>* pListener ) = 0;
   virtual void UnregisterResultDataListener( CViComResultDataProcessor<SMeasResult>* pListener )                           = 0; 
   //!< @throw CViComError @see UnregisterResultDataListener( CViComError&, CViComResultDataProcessor<>* )



public: // static functions
   // This function is only used in the CViComLoader
   //! \internal
   static const char*      GetName() {return "RFSCAN";}
   static Technology::Type GetType() { return Technology::RFSCAN; }
};

typedef CViComRFScanInterface     INTERFACE_TYPE;

} // namespace RFSCAN

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
