// ViComCWInterface.h
#pragma once
#include "ViComCWInterfaceData.h"
#include "ViComCWErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for ViComCWInterface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace CW
{

/**
Interface to perform channel power measurements (also called CW measurements).

Channel power measurements do not perform any RAT specific measurements like demodulation or others. 
In this way they can easily be adopted to new technologies and can be used to perform the first, generic
measurements.
\@attribute[type="interface"]
*/
class CViComCWInterface
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

   /**
   Set the measurement mode to either Single Channel or Multi Channel measurement. In Single Channel 
   mode, the configured frequencies are measured sequentially. In Multi Channel measurement mode, frequencies
   might be grouped together to be measured in one row and to improve measurement performance.
   
   See the programming manual for more details on the different measurement modes.

   @param[out] rViComError
      A CViComError object passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] eMeasType
      Either MT_SingleChannel or MT_MultiChannel, depending if more accuracy or more performance is required.

   @return
      in case of success true otherwise false. If false, the rViComError parameter contains a description
      what went wrong.
   */
   virtual bool SetMeasurementType( CViComError& rViComError, etMeasType eMeasType ) = 0;
   virtual void SetMeasurementType( etMeasType eMeasType )                           = 0;
   //!< @throw CViComError @see ViComCWInterface::SetMeasurementType( CViComError&, etMeasType )

   /**
   SChannelSettings contains the number of channels and the table of frequencies.
   The frequency or channel index is counted from zero and referenced in other data e.g. the measurement result.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rFrequencySettings
      Contains the channel configuration that shall be measured. The configuration consists of a set of frequencies and
      bandwidths, for which a measurement time and attenuation is specified.

   @return
      in case of success true otherwise false. If false, the rViComError parameter contains a description
      what went wrong.
   */
   virtual bool SetFrequencyTable( CViComError& rViComError, const SChannelSettings& rFrequencySettings ) = 0;
   virtual void SetFrequencyTable( const SChannelSettings& rFrequencySettings )                           = 0;
   //!< @throw CViComError @see ViComCWInterface::SetFrequencyTable( CViComError&, SChannelSettings& )

   /**
   etAttenuationSwitchMode enumerates the possible modes how SFrequencySetting::eMinAttenuation is interpreted.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] eAttenuationSwitchMode
      The mode how SFrequencySetting::eMinAttenuation is interpreted.

   @return
      in case of success true otherwise false. If false, the rViComError parameter contains a description
      what went wrong.
   */
   virtual bool SetAttenuationSwitchMode( CViComError& rViComError, etAttenuationSwitchMode eAttenuationSwitchMode ) = 0;
   virtual void SetAttenuationSwitchMode( etAttenuationSwitchMode eAttenuationSwitchMode )                           = 0;
   //!< @throw CViComError @see ViComCWInterface::SetAttenuationSwitchMode( CViComError&, etAttenuationSwitchMode )

   /**
   Configures how a set of power values, that have been calculated from the raw measurement chunks, shall be
   aggregated to make up the final power value for a frequency in one measurement.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] eDetector
      One of TD_PEAK, TD_AVERAGE and TD_RMS, 
      specifying how the single power values created from the raw measurement chunks are aggregated.

   @return
      in case of success true otherwise false. If false, the rViComError parameter contains a description
      what went wrong.
   */
   virtual bool SetDetector( CViComError& rViComError, etDetector eDetector ) = 0;
   virtual void SetDetector( etDetector eDetector )                           = 0;
   //!< @throw CViComError @see ViComCWInterface::SetDetector( CViComError&, etDetector )

   /**
   There is the possibility to make the result creation depending on an external trigger device. Otherwise the Free Run mode is used.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rTriggerSettings
      Contains the type of the measurement scheduling (Free Run or External Trigger) and the trigger factor.

   @return
      in case of success true otherwise false. If false, the rViComError parameter contains a description
      what went wrong.
   */
   virtual bool SetTrigger( CViComError& rViComError, const STriggerSettings& rTriggerSettings ) = 0;
   virtual void SetTrigger( const STriggerSettings& rTriggerSettings )                           = 0;
   //!< @throw CViComError @see ViComCWInterface::SetTrigger( CViComError&, STriggerSettings& )


   /**
   Using the methods SetFrequencyTable(), SetMeasurementType() and SetDetector() the internal settings of
   the measurement device are changed. This function reads the current configuration from the internal
   software and returns all those settings for read-only access.

   All settings have default values. By means of this function you can retrieve the Channel Power Receiver
   settings which apply at the moment for measurements to be started or during running measurements.

   @note
      The result, like all other results too, is only for reading. 
      To change settings the functions mentioned above shall be used.

   @param[out] rViComError
      this is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
      was successful or not.

   @return 
      a structure filled with the currently valid values or NULL if the call was not processed successfully.
   **/
   virtual const SSettings* GetSettings( CViComError& rViComError ) = 0;
   virtual const SSettings* GetSettings( )                          = 0;
   //!< @throw CViComError @see ViComCWInterface::GetSettings( CViComError& )

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
   //!< @throw CViComError @see ViComCWInterface::GetResult( CViComError&, DWORD )


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
   static const char*      GetName() { return "CW"; }
   static Technology::Type GetType() { return Technology::CW; }
};

typedef CViComCWInterface     INTERFACE_TYPE;

} // namespace CW

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
