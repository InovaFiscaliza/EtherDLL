// ViComGsmInterface.h
#pragma once
#include "ViComGsmInterfaceData.h"
#include "ViComGsmErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComGsmInterface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace GSM
{

/**
GSM measurement module for ViCom. Offers the configuration and result receiving methods for all GSM related
measurement tasks.
\@attribute[type="interface"]
*/
class CViComGsmInterface
{
public:
   typedef etViComInterfaceVersion VERSION_TYPE;

   /**
   Returns reference to basic interface object implementation.

   The basic interface is a part of any application specific ViCom interface. It is used for different R&S TSM applications.
   The purpose is to control basic R&S TSM functions, which are not specific for a certain measurement.
   
   @see CViComBasicInterface for specification of basic interface functions.

   @return reference to the BasicInterface object that can be used to control basic functions like
      StartMeasurement() and StopMeasurement() et al.
   */
   virtual CViComBasicInterface& GetBasicInterface( ) = 0;

   //** Request technology an device specific parameter limits **/

   /**
   Limits for Parameter SChannelSettings::dwCount

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<DWORD>* GetChannelCountLimits( CViComError& rViComError ) = 0;
   virtual const SRange<DWORD>* GetChannelCountLimits( )                          = 0; 
   //!< @throw CViComError @see GetChannelCountLimits( CViComError& )

   /**
   Limits for Parameter SDemodulationSettings::lTotalPowerOffsetInDB10

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<long>* GetDemodThresholdLimits( CViComError& rViComError ) = 0;
   virtual const SRange<long>* GetDemodThresholdLimits( )                          = 0;
   //!< @throw CViComError @see GetDemodThresholdLimits( CViComError& )

   /**
   Limits for Parameter SMeasurementRate::dwValuePer1000Sec.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<DWORD>* GetMeasRateLimits( CViComError& rViComError ) = 0;
   virtual const SRange<DWORD>* GetMeasRateLimits( )                          = 0; 
   //!< @throw CViComError @see GetMeasRateLimits( CViComError& )

   //*** Interface functions ***//

   /** Frequency settings **/
   /**
   SChannelSettings contains the count of channels or frequencies and the table of frequencies in MHz.
   The frequency or channel index is counted from zero and referenced in other data e.g. BCH demodulation settings.
   The selection of new frequencies causes default settings for the BCH demodulation.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rFrequencySettings
   Contains the number of channels or frequencies and the table of frequencies in MHz.
   The frequency or channel index is counted from zero and referenced in other data e.g. BCH demodulation settings.
   The selection of new frequencies also changes the default settings for the BCH demodulation.

   @return
   in case of success true otherwise False
   */
   virtual bool SetFrequencyTable( CViComError& rViComError, const SChannelSettings& rChannelSettings ) = 0;
   virtual void SetFrequencyTable( const SChannelSettings& rChannelSettings )                           = 0;
   //!< @throw CViComError @see SetFrequencyTable( CViComError&, SChannelSettings& )

   //** Measurement details setting**/
   /**
   With this function the definition of measurement details can be requested on start of scanning.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] SMeasurementDetails
   Structure with detail info for
   a) Spectrum measurements (valid for all channels)
   b) Channel power measurements (valid for all channels)
   c) start/stop of measurement modes on individual channels

   @return
   in case of success true, otherwise false
   */
   virtual bool SetMeasurementDetails( CViComError& rViComError, const SMeasurementDetails& rMeasurementDetails ) = 0;
   virtual void SetMeasurementDetails( const SMeasurementDetails& rMeasurementDetails )                           = 0;
   //!< @throw CViComError @see SetMeasurementDetails( CViComError&, SMeasurementDetails& )


   //** Request redefinition of measurement details during scanning**/
   /**
   With this function the redefinition of measurement details can be requested during scanning.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] SMeasurementDetails
   Structure with detail info for
   a) Spectrum measurements (valid for all channels)
   b) Channel power measurements (valid for all channels)
   c) start/stop of measurement modes on individual channels

   @return
   in case of success true, otherwise false
   */
   virtual bool RedefineMeasurementDetails( CViComError& rViComError, const SMeasurementDetails& rMeasurementDetails ) = 0;
   virtual void RedefineMeasurementDetails( const SMeasurementDetails& rMeasurementDetails )                           = 0;
   //!< @throw CViComError @see RedefineMeasurementDetails( CViComError&, SMeasurementDetails& )


   //** BCH System Information Types demodulation **/
   /**
   Use this function to set the general BCH System Information Types demodulation properties, 
   including the list of System Information Types that will ever be requested on the channels. 
   Different sets of System Information Types may be defined for different BCH channels.
   Note that SI Type 3 must always be requested because it is needed to evaluate the Cell Identity (CI), to fill the CI table. 
   The BCH Demodulator does not return any measurements until it has evaluated the Cell Identity.
   If SetDemodulationSettings is not called, then the auto demodulation of SI Type 3 is automatically requested 
   for all selected channels. This default is set on each call of SetFrequencyTable.
   
   @warning
   System Information Types demodulation (SDemodulationSettings) cannot be used together 
   with auto demodulation of ST1-4 (see SetFrequencyTable())
   or with the demodulation of ST1-4 during scanning (see RequestDemodulationOfST1To4()).

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rDemodulationSettings
   When the function is called, this pointer contains the values that the application wants to set, 
   and when the function returns, it contains the values that were actually set.
   For details see CViComWcdmaInterfaceData::SDemodulationSettings and 
   CViComWcdmaInterfaceData::SDemodRequests.

   @return
   in case of success true otherwise False
   */
   virtual bool SetDemodulationSettings( CViComError& rViComError, const SDemodulationSettings& rDemodulationSettings ) = 0;
   virtual void SetDemodulationSettings( const SDemodulationSettings& rDemodulationSettings )                           = 0;
   //!< @throw CViComError @see SetDemodulationSettings( CViComError&, SDemodulationSettings& )

   /**
   Use this function to issue commands (demodulation requests, reset commands) for the System Information Type demodulator 
   during measurement.

   SI Types may be acquired for certain BTSs. Demodulation for BTSs or for channels may be reset.
   
   Any SI Type requested here for a specific channel must have been registered for that channel before start of measurement
   in the SetDemodulationSettings() method, for example with mode SITYPE_DEMOD_ON_CMD.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rDemodRequests
   When the function is called, this pointer contains the request parameters.
   For details see SDemodRequests.

   @return
   in case of success true otherwise False
   */
   virtual bool IssueDemodRequests( CViComError& rViComError, const SDemodRequests& rDemodRequests ) = 0;
   virtual void IssueDemodRequests( const SDemodRequests& rDemodRequests )                           = 0;
   //!< @throw CViComError @see IssueDemodRequests( CViComError&, SDemodRequests& )
   

   /**
   By means of the functions:
   SetFrequencyTable(), SetMeasurementRate() and SetAutoDemodulationSettings()
   internal settings of the GSM Network-Scanner may be changed and with the functions 
   SelectReceiver(), SetResultBufferDepth() of CViComBasicInterface 
   basic settings are changed. 
   
   All settings have default values. By means of this function you can retrieve the GSM Network-Scanner
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

   /**
   Issue a request to the Layer 3 Decoder to decode the System Information contained by rSL3DecoderRequest.
   If the data has been decoded successfully within the time given by dwTimeOutInMs, 
   the method creates the SL3DecoderResult and the buffer within on the heap, fills in the Layer 3 Decoder result 
   and returns a pointer to it.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rSL3DecoderRequest
   Contains the data that was the result of a GSM measurement and holds the System Information data that shall be decoded.

   @return
   one measurement result on the heap, if the function succeeded. Otherwise NULL.
   Attention: The returned pointer is only valid until this function is called again.
   */
   virtual const SL3DecoderResult* 
      RetrieveTextForPDU( CViComError&             rViComError, 
                          const SL3DecoderRequest& rSL3DecoderRequest, 
                          DWORD                    dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs  ) = 0;
   virtual const SL3DecoderResult* 
      RetrieveTextForPDU( const SL3DecoderRequest& rSL3DecoderRequest, 
                          DWORD                    dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs  ) = 0;
   //!< @throw CViComError @see RetrieveTextForPDU( CViComError&, SL3DecoderRequest&, DWORD )


   //* CViComResultDataProcessor integration ************************************/
   /**
   Register a ScannerData listener that will be notified when new data is available. Note that it is
   important to Unregister the listener again once it is getting deleted, to avoid crashes.

   @note Both possibilities to retrieve results are to be used exclusively. In case of ResultDataListener usage,
   it is not allowed to call CViComBasicInterface::GetResultCounters() or GetResult() and vice versa.

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
   static const char*      GetName() {return "GSM";}
   static Technology::Type GetType() { return Technology::GSM; }

protected:
   //! \internal
   virtual ~CViComGsmInterface(){}
};

typedef CViComGsmInterface     INTERFACE_TYPE;

} // namespace GSM

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
