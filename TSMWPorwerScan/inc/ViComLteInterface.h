// ViComLteInterface.h
#pragma once
#include "ViComLteInterfaceData.h"
#include "ViComLteErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment

namespace RohdeSchwarz
{
namespace ViCom
{
namespace LTE
{

/**
LTE measurement module for ViCom. Offers the configuration and result receiving methods for all LTE related
measurement tasks.
\@attribute[type="interface"]
*/
class CViComLteInterface
{
public:
   typedef etViComInterfaceVersion VERSION_TYPE;

   /**
   The basic interface is a part of any application specific ViCom interface. It is used for different device applications.
   The purpose is to control basic device functions, which are not specific for a certain measurement.
   See CViComBasicInterface for specification of basic interface functions.

   @return
      reference to the BasicInterface object that can be used to control basic functions like
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
   //!< @throw CViComError @see CViComLteInterface::GetChannelCountLimits( CViComError& )

   /**
   Limits for Parameter SFrequencySettings::dwAvgBlockCountPer1000Sec

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<DWORD>* GetMeasRateLimits( CViComError& rViComError ) = 0;
   virtual const SRange<DWORD>* GetMeasRateLimits( )                          = 0; 
   //!< @throw CViComError @see CViComLteInterface::GetMeasRateLimits( CViComError& )

   /**
   Limits for Parameter SWidebandRsCinrSettings::dwAvgBlockCountPer1000Sec

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<DWORD>* GetWbMeasRateLimits( CViComError& rViComError ) = 0;
   virtual const SRange<DWORD>* GetWbMeasRateLimits( )                          = 0; 
   //!< @throw CViComError @see CViComLteInterface::GetWbMeasRateLimits( CViComError& )

   /**
   Limits for Parameter SMbmsSettings::dwMaxSubframeRatePer1000Sec

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<DWORD>* GetMbmsMeasRateLimits( CViComError& rViComError ) = 0;
   virtual const SRange<DWORD>* GetMbmsMeasRateLimits( )                          = 0;
   //!< @throw CViComError @see CViComLteInterface::GetMbmsMeasRateLimits( CViComError& )

   /**
   Limits for Parameter SDemodulationSettings::sSINRThresholdDB100

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<short>* GetDemodThresholdLimits( CViComError& rViComError ) = 0;
   virtual const SRange<short>* GetDemodThresholdLimits( )                          = 0;
   //!< @throw CViComError @see CViComLteInterface::GetDemodThresholdLimits( CViComError& )

   /**
   CViComLteInterfaceData::SChannelSettings contains the count of channels or frequencies and the table of frequencies in MHz.
   The frequency or channel index is counted from zero and referenced in other data e.g. BCH demodulation settings.
   The selection of new frequencies causes default settings for the BCH demodulation.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rFrequencySettings
      Contains the definition which frequencies shall be measured. @see SChannelSettings for
      a more detailed description on those settings.

   @return
      in case of success true otherwise False
   */
   virtual bool SetFrequencyTable( CViComError& rViComError, const SChannelSettings& rchannelSettings ) = 0;
   virtual void SetFrequencyTable( const SChannelSettings& rchannelSettings )                           = 0;
   //!< @throw CViComError @see CViComLteInterface::SetFrequencyTable( CViComError&, SChannelSettings& )

   //** BCH demodulation **/

   /**
   Use this function to set the general BCH demodulation properties, including the LTE Layer 3 PDUs that should
   be demodulated. Different sets of PDUs may be read from different BCH channels.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rDemodulationSettings
      When the function is called, this pointer contains the values that the application wants to set,
      and when the function returns, it contains the values that were actually set.
      For details see SDemodulationSettings and SDemodRequests.

   @return
      in case of success true otherwise False
   */
   virtual bool SetDemodulationSettings( CViComError& rViComError, const SDemodulationSettings& rDemodulationSettings ) = 0;
   virtual void SetDemodulationSettings( const SDemodulationSettings& rDemodulationSettings )                           = 0;
   //!< @throw CViComError @see CViComLteInterface::SetDemodulationSettings( CViComError&, const CViComLteInterfaceData::SDemodulationSettings& )

   /**
   Use this function to issue commands (PDU requests, reset commands) for the BCH demodulator during measurement.

   PDUs may be acquired for certain eNodeBs. PDU demodulation for eNodeBs or for channels may be reset.

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
   //!< @throw CViComError @see CViComLteInterface::IssueDemodRequests( CViComError&, CViComLteInterfaceData::SDemodRequests& ) 

   //** Request Settings **/

   /**
   By means of the functions: SetFrequencyTable(), SetMeasurementMode() and SetDemodulationSettings() internal settings
   of the PN-Scanner may be changed. The functions SelectReceiver(), SetResultBufferDepth()of CViComBasicInterface 
   change basic settings. Except for frequency settings all other settings have default values.
   By means of this function you can retrieve the PN-Scanner settings which apply at the moment for measurements to be started
   or during running measurements.
   Note that the result, like all other results too, is only for reading.
   To change settings the functions mentioned above should be used.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the current device settings. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SSettings* GetSettings( CViComError& rViComError ) = 0;
   virtual const SSettings* GetSettings( )                          = 0; 
   //!< @throw CViComError @see CViComLteInterface::GetSettings( CViComError& )


   /**
   This function returns a measurement result from the result buffer or waits for the arrival of the next measurement
   result if the result buffer is empty.
   During the wait state the processor capacity is released for other threads of this process and other processes.
   If no result arrives within the timeout then the function returns with a NULL pointer and error VICEC_TIME_OUT.
   
   @note The result data are for reading only and are available only until the next interface function is called.

   @note Both possibilities to retrieve results are to be used exclusively. In case of ResultDataListener usage,
   it is not allowed to call CViComBasicInterface::GetResultCounters() or CViComLteInterface::GetResult() and vice versa.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] dwTimeOutInMs
      The time in milliseconds that the function should wait before returning an error, if the result buffer is empty.

   @return
      one measurement result, if the function succeeded. Otherwise NULL.
      The returned structure is valid as long as no other interface method is called.
   */
   virtual const SMeasResult* GetResult( CViComError& rViComError, DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs ) = 0;
   virtual const SMeasResult* GetResult( DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs )                           = 0; 
   //!< @throw CViComError @see CViComLteInterface::GetResult( CViComError&, DWORD )
   
   /**
   Issue a request to the Layer 3 Decoder to decode the PDU contained by rSL3DecoderRequest.
   If the data has been decoded successfully within the time given by dwTimeOutInMs,
   the method creates the SL3DecoderResult and the buffer within on the heap, fills in the Layer 3 Decoder result
   and returns a pointer to it.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rSL3DecoderRequest
      Contains the data that was the result of an LTE measurement and holds the PDU data that shall be decoded.

   @return
      one measurement result on the heap, if the function succeeded. Otherwise NULL.
      Attention: The returned pointer is only valid until this function is called again.
   */
   virtual const SL3DecoderResult*
      RetrieveTextForPDU( CViComError&             rViComError,
                          const SL3DecoderRequest& rSL3DecoderRequest,
                          DWORD                    dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs   ) = 0;
   virtual const SL3DecoderResult*
      RetrieveTextForPDU( const SL3DecoderRequest& rSL3DecoderRequest,
                          DWORD                    dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs   ) = 0;
   //!< @throw CViComError @see CViComLteInterface::RetrieveTextForPDU( CViComError&, CViComLteInterfaceData::SL3DecoderRequest&, DWORD )   


   //* CViComResultDataProcessor integration ************************************/
   /**
   Register a ScannerData listener that will be notified when new data is available. Note that it is
   important to Unregister the listener again once it is getting deleted, to avoid crashes.

   @note Both possibilities to retrieve results are to be used exclusively. In case of ResultDataListener usage,
   it is not allowed to call CViComBasicInterface::GetResultCounters() or CViComLteInterface::GetResult() and vice versa.

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



public:
   //! static functions
   static const char*      GetName() { return "LTE"; }
   static Technology::Type GetType() { return Technology::LTE; }

protected:
   //! \internal
   virtual ~CViComLteInterface() {}
};

   typedef CViComLteInterface      INTERFACE_TYPE;

} // namespace LTE

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment