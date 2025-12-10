// ViComWcdmaInterface.h
#pragma once
#include "ViComWcdmaInterfaceData.h"
#include "ViComWcdmaErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComWcdmaInterface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace WCDMA
{

/**
WCDMA measurement module for ViCom. Offers the configuration and result receiving methods for all WCDMA related
measurement tasks.
\@attribute[type="interface"]
*/
class CViComWcdmaInterface
{
public:
   typedef etViComInterfaceVersion  VERSION_TYPE;

public:
   //** Basic interface **//

   /**
   The basic interface is a part of any application specific ViCom interface. It is used for different TSM applications.
   The purpose is to control basic TSM functions, which are not specific for a certain measurement.
   See CViComBasicInterface for specification of basic interface functions.


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
   Limits for Parameter SDemodulationSettings::sCINRThresholdInDB100.
   The resulting demod threshhold limits are specified for the MeasurementMode::HIGH_DYNAMIC setting. 
   The minimum limit for MeasurementMode::HIGH_SPEED is equal to the returned default value of the SRange container object.

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
   Limits for Parameter SChannelSettings::dwMeasRatePer1000sec.

   @note
      The resulting meas rate limits are specified for the MeasurementMode::HIGH_SPEED setting. The maxmimum rating
      for MeasurementMode::HIGH_DYNAMIC is equal to the returned default value of the SRange container object.

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


   //** BCH demodulation **/
   /**
   Use this function to set the general BCH demodulation properties, including the System Information Blocks (SIBs) that should
   be read by the R&S TSM when the BCH is demodulated. Different sets of SIBs may be read from different BCH channels.
   Note that SIB 3 must always be requested because it is needed to evaluate the Cell Identity (CI), to fill the CI table.
   The BCH Demodulator does not return any measurements until it has evaluated the Cell Identity.
   If the BCH demodulator is available and SetDemodulationSettings is not called, then the SIB 3 is automatically requested
   for all selected channels. This default is set on each call of SetFrequencyTable.

   Only the following SIB (System Information Block) PDU values may be requested:
   - MIB          -> PDU 14
   - SIB 1        -> PDU 15
   - SIB 2        -> PDU 16
   - SIB 3        -> PDU 17
   - SIB 4        -> PDU 18
   - SIB 5        -> PDU 19
   - SIB 5bis     -> PDU 53
   - SIB 6        -> PDU 20
   - SIB 7        -> PDU 21
   - SIB 11       -> PDU 25
   - SIB 11bis    -> PDU 61
   - SIB 12       -> PDU 26
   - SIB 13       -> PDU 27
   - SIB 13.1     -> PDU 28
   - SIB 13.2     -> PDU 29
   - SIB 13.3     -> PDU 30
   - SIB 13.4     -> PDU 31
   - SIB 14       -> PDU 32
   - SIB 15       -> PDU 33
   - SIB 15bis    -> PDU 67
   - SIB 15.1     -> PDU 34
   - SIB 15.1bis  -> PDU 68 
   - SIB 15.2     -> PDU 35
   - SIB 15.2bis  -> PDU 69
   - SIB 15.2ter  -> PDU 70
   - SIB 15.3     -> PDU 36
   - SIB 15.3bis  -> PDU 71
   - SIB 15.4     -> PDU 37
   - SIB 15.5     -> PDU 38
   - SIB 15.6     -> PDU 72
   - SIB 15.7     -> PDU 73
   - SIB 15.8     -> PDU 74
   - SIB 16       -> PDU 39
   - SIB 17       -> PDU 40
   - SIB 18       -> PDU 41
   - SIB 19       -> PDU 75
   - SIB 20       -> PDU 76

   SIB 3 must be requested for each measurement, to get SIB data, because it is necessary
   to evaluate the CI (Cell Identity), to fill the CI table.

   The BCH Demodulator doesn't return measurements until it has evaluated the CI.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rDemodulationSettings
      When the function is called, this pointer contains the values that the application wants to set,
      and when the function returns, it contains the values that were actually set.
      For details see SDemodulationSettings and
      SDemodRequests.

   @return
      in case of success true otherwise False
   */

   virtual bool SetDemodulationSettings( CViComError& rViComError, const SDemodulationSettings& rDemodulationSettings ) = 0;
   virtual void SetDemodulationSettings( const SDemodulationSettings& rDemodulationSettings )                           = 0;
   //!< @throw CViComError @see SetDemodulationSettings( CViComError&, SDemodulationSettings& )

   /**
   Use this function to issue commands (PDU requests, reset commands) for the BCH demodulator during measurement.

   SIBs may be acquired for certain node bs. PDU demodulation for node bs or for channels may be reset.

   Only the following SIB (System Information Block) PDU values may be requested:
   - MIB          -> PDU 14
   - SIB 1        -> PDU 15
   - SIB 2        -> PDU 16
   - SIB 3        -> PDU 17
   - SIB 4        -> PDU 18
   - SIB 5        -> PDU 19
   - SIB 6        -> PDU 20
   - SIB 7        -> PDU 21
   - SIB 8        -> PDU 22
   - SIB 9        -> PDU 23
   - SIB 11       -> PDU 25
   - SIB 12       -> PDU 26
   - SIB 13       -> PDU 27
   - SIB 13.1     -> PDU 28
   - SIB 13.2     -> PDU 29
   - SIB 13.3     -> PDU 30
   - SIB 13.4     -> PDU 31
   - SIB 14       -> PDU 32
   - SIB 15       -> PDU 33
   - SIB 15.1     -> PDU 34
   - SIB 15.4     -> PDU 37
   - SIB 15.5     -> PDU 38
   - SIB 17       -> PDU 40
   - SIB 18       -> PDU 41
   - SIB SB1      -> PDU 42
   - SIB SB2      -> PDU 43
   - SIB 5bis     -> PDU 53          applicable as of UMTS Rel. 6

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


   //** Request Settings **/

   /**
   By means of the functions: SetFrequencyTable(), SetMeasurementMode() and SetDemodulationSettings() internal settings
   of the PN-Scanner may be changed. The functions SelectReceiver(), SetResultBufferDepth(), SetTimebaseSynchronisationMode()
   of CViComBasicInterface change basic settings. Except for frequency settings all other settings have default values.
   By means of this function you can retrieve the PN-Scanner settings which apply at the moment for measurements to be started
   or during running measurements.
   Note that the result, like all other results too, is only for reading.
   To change settings the functions mentioned above should be used.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the current R&S TSM settings. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SSettings* GetSettings( CViComError& rViComError ) = 0;
   virtual const SSettings* GetSettings( )                          = 0;
   //!< @throw CViComError @see GetSettings( CViComError& )

   //** Measurement results **/
   /**
   This function returns a measurement result from the result buffer or waits for the arrival of the next measurement
   result if the result buffer is empty.
   During the wait state the processor capacity is released for other threads of this process and other processes
   (WaitForSingleObject call).
   If no result arrives within the timeout then the function returns with a NULL pointer and error VICEC_TIME_OUT.
   Note that the result data are for reading only and are available only until the next interface function is called.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] dwTimeOutInMs
      The time in milliseconds that the function should wait before returning an error, if the result buffer is empty.

   @return
      one measurement result, if the function succeeded. Otherwise NULL.
      The returned structure is valid as long as no other interface method is called.
   */
   virtual const SMeasResult* GetResult( CViComError& rViComError, DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs) = 0;
   virtual const SMeasResult* GetResult( DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs )                          = 0;
   //!< @throw CViComError @see GetResult( CViComError&, DWORD )


   /**
   Issue a request to the Layer 3 Decoder to decode the PDU contained by rSL3DecoderRequest.
   If the data has been decoded successfully within the time given by dwTimeOutInMs,
   the method creates the SL3DecoderResult and the buffer within on the heap, fills in the Layer 3 Decoder result
   and returns a pointer to it.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rSL3DecoderRequest
      Contains the data that was the result of a WCDMA measurement and holds the PDU data that shall be decoded.

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



public: // static functions
   //! \internal
   static const char*      GetName() {return "WCDMA";}
   static Technology::Type GetType() { return Technology::WCDMA; }

protected:
   //! \internal
   virtual ~CViComWcdmaInterface(){}
};

typedef CViComWcdmaInterface     INTERFACE_TYPE;

} // namespace WCDMA

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
