// ViComGPSInterface.h
#pragma once
#include "ViComGpsInterfaceData.h"
#include "ViComGpsErrors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComGpsInterface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace GPS
{

/**
Description of the functions in CViComGpsInterface.
\@attribute[type="interface"]
*/
class CViComGpsInterface
{
public:
   typedef etViComInterfaceVersion VERSION_TYPE;

   /**
   Returns reference to basic interface object implementation.

   The basic interface is a part of any application specific ViCom interface.
   
   @see CViComBasicInterface for specification of basic interface functions.

   @return reference to the BasicInterface object that can be used to control basic functions like
      StartMeasurement() and StopMeasurement() et al.
   */
   virtual CViComBasicInterface& GetBasicInterface( ) = 0;

   //*** Interface functions ***//

   /**
   CViComGpsInterface::SetGPSDeviceSettings sets the configuration of the GPS device.

   @param[out] rViComError
   A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rGPSSettings
   Contains the wanted configuration for the GPS device.

   @return
   in case of success true otherwise false
   */
   virtual bool SetGPSDeviceSettings( CViComError& rViComError, const SGPSDeviceSettings& rGPSSettings ) = 0;
   virtual void SetGPSDeviceSettings( const SGPSDeviceSettings& rGPSSettings )                           = 0;
   //!< @throw CViComError @see CViComGpsInterface::SetGPSDeviceSettings( CViComError&, SGPSDeviceSettings& )
   
   /**
   Returns the settings which apply at the moment.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the current configuration. In case of failure, NULL. The returned value is valid 
      as long as no other interface method is called.
   */
   virtual const SSettings* GetSettings( CViComError& rViComError ) = 0;
   virtual const SSettings* GetSettings( )                          = 0;
   //!< @throw CViComError @see CViComGpsInterface::GetSettings( CViComError& )

   /**
   This function returns a GPS result from the result buffer or waits for the arrival of the next measurement 
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
      One result from the GPS receiver, if the function succeeded. Otherwise NULL.
      The returned structure is valid as long as no other interface method is called.
   */
   virtual const SMeasResult* GetResult( CViComError& rViComError, DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs ) = 0;
   virtual const SMeasResult* GetResult( DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs )                           = 0;
   //!< @throw CViComError @see CViComGpsInterface::GetResult( CViComError&, DWORD )

   /**
   Function to send a command to the GPS receiver. The command is forwarded without any validity check. Any error message 
   from the GPS receiver will be available through the GetResult command. 
   The function uses the same structure as GetResult. The timestamp parameters are not used.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rGPSFormat
      Format of the message to be transmitted. Either NMEA or UBLOX may be used.

   @param[in] rMessage
      The message to be forwarded to the GPS receiver. The timestamp information will not be used.

   @return
      true if successful otherwise false.
   */
   virtual bool SendMessage( CViComError& rViComError, const SGPSMessage& rMessage ) = 0;
   virtual void SendMessage( const SGPSMessage& rMessage )                           = 0;
   //!< @throw CViComError @see CViComGpsInterface::SendMessage( CViComError&, SGPSMessage& )


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
   static const char*      GetName() { return "GPS"; }
   static Technology::Type GetType() { return Technology::GPS; }

protected:
   //! \internal
   virtual ~CViComGpsInterface(){}
};

typedef CViComGpsInterface     INTERFACE_TYPE;

} // namespace GPS

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment