// ViComAcdInterface.h
#pragma once
#include "ViComAcdInterfaceData.h"
#include "ViComAcdErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment

namespace RohdeSchwarz
{
namespace ViCom
{
namespace ACD
{

/**
@section acd Automatic channel detection
   The automatic channel detection feature allows to discover active channels
   without prior knowledge of the used frequencies. The user has to specify
   which technologies shall be evaluated as well as a list of bands in order
   the select the wanted frequency range. This is typically known from the
   spectrum allocation plan by the regulator.

   The supported modes of operation, covering both stationary and drive test
   scenarios, are presented in the next sections.

@subsection simple_mode Simple mode
   In simple mode the channel detection uses a brute force algorithm to simply
   scan all channels for the specified technologies and bands. This mode is most
   suitable to get a precise overview about active channels in the current
   location, e.g. it's best used in stationary scenarios.

@subsection smart_mode  Smart mode
   In smart mode the channel detection additionally utilizes spectrum scan data
   to optimize the discovery of active channels. It runs continuously and sends
   detection reports on a periodic basis which allows to track changes.

   This mode is most usefully in inhomogeneous networks where channel allocations
   change depending on the driven route or when frequency spectrum is shared among
   multiple technologies.

@section basic_apprach Basic approach
   The steps to setup a automatic channel detection measurement depend on the
   selected mode of operation. In smart mode there are more options to fine
   tune the detection process.

@subsection simple_mode_approach Simple mode approach
   To actually get channel detection results in for both modes a three step approach
   has to be taken:
   
   1. Configure the channel detector using CViComAcdInterface::SetAcdSettings() method.
   2. Start a detection process using.
   3. Retrieve periodic result reports about detected channels as long as all configured
      bands have been scanned.

   In contrast to the simple mode the periodic delivery of results (step 3) in smart mode
   does only stop if all candidate channels have been validated and there aren't any changes
   in the measured power spectrum.
*/


/**
This class can be used to automatically detect channels without knowing the corresponding radio frequencies.
\@attribute[type="interface"]
*/
class CViComAcdInterface
{
public:
   typedef etViComInterfaceVersion VERSION_TYPE;

public:
   /**
   The basic interface is a part of any application specific ViCom interface. It is used for different TSM applications.
   The purpose is to control basic TSM functions, which are not specific for a certain measurement.
   See CViComBasicInterface for specification of basic interface functions.


   @return
      reference to the BasicInterface object that can be used to control basic functions like
      StartMeasurement() and StopMeasurement() et al.
   */
   virtual CViComBasicInterface& GetBasicInterface( ) = 0; 

   /**
   Limits for Parameter SAcdSettings::dwCount

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the limits for this setting. In case of failure, NULL. The returned value is valid
      as long as no other interface method is called.
   */
   virtual const SRange<DWORD>* GetTechnologyCountLimits( CViComError& rViComError ) = 0;
   virtual const SRange<DWORD>* GetTechnologyCountLimits( )                          = 0; 
   //!< @throw CViComError @see GetTechnologyCountLimits( CViComError& )

   /**
   TODO
   SChannelSettings contains the count of channels or frequencies and the table of frequencies in MHz.
   The frequency or channel index is counted from zero and referenced in other data e.g. BCH demodulation settings.
   The selection of new frequencies causes default settings for the BCH demodulation.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rFrequencySettings
      Contains the definition which frequencies shall be measured. @see SChannelSettings for
      a more detailed description on those settings.

   @return
      in case of success true otherwise false
   */
   virtual bool SetAcdSettings( CViComError& rViComError, const SAcdSettings& rAcdSettings ) = 0;
   virtual void SetAcdSettings( const SAcdSettings& rAcdSettings )                           = 0;
   //!< @throw CViComError @see SetAcdSettings( CViComError&, SAcdSettings& )


   //** Request Settings **/

   /**
   By means of the function: SetAcdSettings() internal settings of the channel detector may be changed.
   The functions SelectReceiver(), SetResultBufferDepth() of CViComBasicInterface  change basic settings.
   By means of this function you can retrieve the settings which apply at the moment for measurements to 
   be started or during running measurements.
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
   //!< @throw CViComError @see GetSettings( CViComError& )


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
   static const char*      GetName() { return "ACD"; }
   static Technology::Type GetType() { return Technology::ACD; }

protected:
   //! \internal
   virtual ~CViComAcdInterface(){}
};

typedef CViComAcdInterface     INTERFACE_TYPE;

} // namespace ACD
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
