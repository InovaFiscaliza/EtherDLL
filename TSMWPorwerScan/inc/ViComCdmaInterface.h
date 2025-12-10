// ViComCdmaInterface.h
#pragma once
#include "ViComCdmaInterfaceData.h"
#include "ViComCdmaErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComCdmaInterface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace CDMA
{

/**
The following functions are specific to the ViComCdma interface, which is used when measuring CDMA2000 signals.
\@attribute[type="interface"]
*/
class CViComCdmaInterface
{
public:
   typedef etViComInterfaceVersion VERSION_TYPE;

   //** Basic interface **//

   /**
   The basic interface is a part of any application specific ViCom interface. It is used for different R&S TSM applications.
   The purpose is to control basic R&S TSM functions, which are not specific for a certain measurement.
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
   Limits for Parameter SDemodulationSettings::lEcToIoThresholdInDB100_for_CDMA
   and SDemodulationSettings::lEcToIoThresholdInDB100_for_EVDO

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
   Limits 
   The resulting meas rate limits are specified for the SMeasurementRate::dwValuePer1000Sec setting. The maxmimum rating
   for EvDo is equal to the returned default value of the SRange container object.

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
   Each frequency has a table of PN Offset Arbitrary Limitation settings attached.
   The frequency or channel index is counted from zero and is referenced in other data.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rFrequencySettings
      Contains the number of channels or frequencies and the table of frequencies in MHz.
      Each frequency has a table of PN Offset Arbitrary Limitation settings attached.
      The frequency or channel index is counted from zero and is referenced in other data.

   @return
      in case of success true otherwise False
   */
   virtual bool SetFrequencyTable( CViComError& rViComError, const SChannelSettings& rFrequencySettings ) = 0;
   virtual void SetFrequencyTable( const SChannelSettings& rFrequencySettings )                           = 0;
   //!< @throw CViComError @see SetFrequencyTable( CViComError&, SChannelSettings& )


   //** PPS Settings **/
   /**
   Delay of the PPS Pulse in sec. If this value is given, than the scanner uses the PPS for time synchronization. 
   If the value is invalid, than the PPS is not used for time synchronization even if a PPS is available.
   By this value the PPS propagation delay can be calibrated.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rPPSSettings
   rPPSSettings.pdDelayOfPPSFallingEdgeInSec can be either NULL (invalid) or a pointer to a double with the delay value.
   The value should be as exact as possible (at least 7 decimal places), because 0,1 µs corresponds to 30 m distance

   @return
   in case of success true, otherwise false
   */
   virtual bool SetPPSSettings( CViComError& rViComError, const SPPSSettings& rPPSSettings ) = 0;
   virtual void SetPPSSettings( const SPPSSettings& rPPSSettings )                           = 0;
   //!< @throw CViComError @see SetPPSSettings( CViComError&, SPPSSettings& )

   //** Max. Velocity **/
   /**
   Max speed of the test vehicle to adjust fading channel limitation in the measurement algorithms.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rMaxVelocity
   rMaxVelocity.dMaxVelocityInKmPerHour gives the maximum expected speed in the test drive 

   @return
   in case of success true, otherwise false
   */
   virtual bool SetMaxVelocity ( CViComError& rViComError, const SMaxVelocity& rMaxVelocity ) = 0;
   virtual void SetMaxVelocity ( const SMaxVelocity& rMaxVelocity )                           = 0;
   //!< @throw CViComError @see SetMaxVelocity( CViComError&, SMaxVelocity& )


   //** Sync Channel Demodulation Mode **/
   /**
   Determines how often the Scanner will demodulate the Forward Synchronization Channel (F-SYNC) and decode 
   the information elements in the synchronization message.
   The synchronization message is demodulated at least once per measured frequency. A smaller F-SYNC demodulation rate 
   has no impact on the measurement but can considerably reduce the amount of result data.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] eMode
   eMode can have the following values: ONCE, ALL or FAST.

   @return
   in case of success true, otherwise false
   */
   virtual bool SetSyncChannelDemodulationMode( CViComError& rViComError, SyncChannelDemodulationMode::Type eMode ) = 0;
   virtual void SetSyncChannelDemodulationMode( SyncChannelDemodulationMode::Type eMode )                           = 0;
   //!< @throw CViComError @see SetSyncChannelDemodulationMode( CViComError&, etSyncChannelDemodulationMode )
  
   //** BCH demodulation **/
   
   /**
   Use this function to set the general BCH demodulation properties, including the Message Identifiers that should 
   be read by the R&S TSM when the BCH is demodulated. Different sets of messages may be read from different BCH channels.

   Once a demodulation is requested, the R&S TSM device tries to serve the command as soon as possible. The
   demodulation settings can be changed during the measurement is active using the IssueDemodRequests() method
   below.

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
   Use this function to set the scan attributes for the EVDO channels. Note that these settings
   only apply if at least one of the channels has its EVDO scanning attribute set to TRUE.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rEvdoControlSettings
   When the function is called, this pointer contains the values that the application wants to set, 
   and when the function returns, it contains the values that were actually set.
   For details see SEvdoControlSettings .

   @return
   in case of success true otherwise False
   */
   virtual bool SetEvdoSettings( CViComError& rViComError, const SEvdoControlSettings& rEvdoControlSettings ) = 0;
   virtual void SetEvdoSettings( const SEvdoControlSettings& rEvdoControlSettings )                           = 0;
   //!< @throw CViComError @see SetEvdoSettings( CViComError&, SEvdoControlSettings& )

   /**
   Use this function to issue commands (specific demodulation requests, reset commands) for the BCH demodulator 
   during measurement. To enable or disable BTS related demodulation, this can only be achieved using this method.

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
   By means of the functions: SetFrequencyTable(), SetMeasurementMode(), SetPPSSettings(), SetMaxVelocity() 
   and SetSyncChannelDemodulationMode() internal settings of the PN-Scanner may be changed. 
   The functions SelectReceiver(), SetResultBufferDepth(), SetTimebaseSynchronisationMode()
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
   virtual const SMeasResult* GetResult( CViComError& rViComError, DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs ) = 0;
   virtual const SMeasResult* GetResult( DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs )                           = 0;
   //!< @throw CViComError @see GetResult( CViComError&, DWORD )

   /**
   Issue a request to the Layer 3 Decoder to decode the PDU contained by rSL3DecoderRequest.
   If the data has been decoded successfully within the time given by dwTimeOutInMs, 
   the method creates the SL3DecoderResult and the buffer within on the heap, fills in the Layer 3 Decoder result 
   and returns a pointer to it.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rSL3DecoderRequest
   Contains the data that was the result of a CDMA 2000 measurement and holds the message data that shall be decoded.

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
   static const char*      GetName() { return "CDMA"; }
   static Technology::Type GetType() { return Technology::CDMA; }

protected:
   //! \internal
   virtual ~CViComCdmaInterface(){}
};

typedef CViComCdmaInterface     INTERFACE_TYPE;

} // namespace CDMA

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
