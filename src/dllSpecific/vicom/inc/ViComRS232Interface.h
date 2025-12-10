// ViComRS232Interface.h
#pragma once
#include "ViComRS232InterfaceData.h"
#include "ViComRS232Errors.h"
#include "ViComBasicInterface.h"
#include "ViComLoader.h"

#pragma pack(push, 8) // get a definite structure alignment for CViComRS232Interface

namespace RohdeSchwarz
{
namespace ViCom
{
namespace RS232
{

/**
Explanation of RS232 Interface
\@attribute[type="interface"]
*/
class CViComRS232Interface
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


   //*** Interface functions ***//

   /** RS232 settings **/
   /**
   Sets the used RS232 baud rate.
   
   Unlike Silent Mode configuration this will NOT be restored on R&S TSM reboot
   and will only be used from call of StartMeasurement 
   to call of StopMeasurement.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] rConfiguration
      RS232 baud

   @return
      in case of success true otherwise false
   */
   virtual bool SetBaudRate( CViComError& rViComError, etBAUDRATE eBaudrate ) = 0;
   virtual void SetBaudRate( etBAUDRATE eBaudrate )                           = 0;
   //!< @throw CViComError @see SetBaudRate( CViComError&, etBAUDRATE )


   /** RS232 settings **/
   /**
   Changes the R&S TSMx Silent Mode configuration.

   The Silent Mode configuration will actually be stored on the device when
   calling StartMeasurement. Thus this will be restored
   on R&S TSMx reboot.

   A R&S TSMx device configured for Silent Mode will suppress all usual RS232 
   trace outputs. Otherwise these outputs 
   will hold onStartMeasurement
   and continue after StopMeasurement.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] eSilentMode
      R&S TSMx Silent Mode configuration

   @return
      in case of success true otherwise false
   */
   virtual bool SetSilentMode( CViComError& rViComError, etSILENT_MODE eSilentMode ) = 0;
   virtual void SetSilentMode( etSILENT_MODE eSilentMode )                           = 0;
   //!< @throw CViComError @see SetSilentMode( CViComError&, etSILENT_MODE )

   /**
   By means of the functions: SetBaudRate() and SetSilentMode() internal settings of the 
   RS232 Interface may be changed and with the functions SelectReceiver(),
   SetResultBufferDepth() change basic settings.
   All settings have default values.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @return
      pointer to a structure holding the current R&S TSMx settings. In case of failure, NULL. The returned value is valid 
      as long as no other interface method is called.
   */
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
   @param[out] rViComError
         This is filled with a detailed report what error occurred, if one occurred. Check return value for whether the execution+
         was successful or not.

   @param[in] rTxData
         refers to the characters that will be sent by the R&S TSMx to RS232.

   */
   virtual bool SendData( CViComError& rViComError, const SCharacterBuffer& rTxData ) = 0;
   virtual void SendData( const SCharacterBuffer& rTxData )                           = 0;
   //!< @throw CViComError @see SendData( CViComError&, SCharacterBuffer& )


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
   // This function is only used in the CViComLoader
   //! \internal
   static const char*      GetName() { return "RS232"; }
   static Technology::Type GetType() { return Technology::RS232; }

};

typedef CViComRS232Interface     INTERFACE_TYPE;

} // namespace RS232

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
