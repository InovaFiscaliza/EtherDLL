// ViComBasicInterface.h
#pragma once
#include "ViComError.h"
#include "ViComBasicInterfaceData.h"
#include "ViComDataProcessor.h"

#pragma pack(push, 8) // get a definite structure alignment

namespace RohdeSchwarz
{
namespace ViCom
{

/**
@mainpage ViCom Interface for R&S TSMx devices

The ViCom API consists of a collection of header files that contain the interaction interfaces for a client application that
utilizes one or more TSM devices. Since the TSM provides several different feature sets, the header files can be grouped
into classes of such feature sets. For example, the RF Power Scan functionality is one such feature set. Besides the
specific interfaces, there is also a basic interface that defines the commonly shared methods and data units. Additionally,
there is also a set of helper classes, like the CViComLoader class that supports you when it comes to initializing
and using a specific ViCom interface.

All ViCom interfaces are organized in the same way. There are three header files for each specialized interface:

- One of them defines the data structures which serve as containers for settings and results.
- From this interface class, the actual ViCom object is inherited.
  This object uses the structures defined in its parent class.
- The last file contains error code and texts for that interface.

The data interface is always derived from the basic data interface. The actual working interfaces
inherit from the basic interface. This approach assures to share commonly used methods and structures
across different ViCom modules and to make moving from one interface to the next an easy task.

Some rules and guidelines that you should follow when using the interfaces are listed here:

- All methods take a ViCom error object as first parameter. Most methods return false when they fail.
  In such a case, the error object is filled with detailed information.
- Additionally every method is available in an exception based version which throws CViComError objects.
- If a method returns a pointer to a structure, the pointer is either NULL if the function failed, or
  it points to the result of the query. The result will get invalid as soon as the next interface method
  is called. So if you want to use the result over a longer period of time, you have to copy it.
- The basic interface methods can always be used from a specific ViCom interface class by using the
  GetBasicInterface() method.


Regardless of what special interface you use, the following approach should be used to initialize
your application:

@code

   //
   // Change this to switch to another technology resp. receiver type.
   //
   #include "ViComRFPowerScanInterface.h"

   using namespace RohdeSchwarz::ViCom;
   using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;

   typedef RFPOWERSCAN::INTERFACE_TYPE ViComInterface_type;
   Receiver::Type receiver = Receiver::TSMW;

   //
   // Implementation of the CViComResultDataProcessor interface to process results.
   //
   class CResultDataProcessor : public CViComResultDataProcessor<SMeasResult>
   {
   public:

      CResultDataProcessor( ViComInterface_type& rInterface )
         : m_rInterface( rInterface )
         , m_dwResultCounter( 0 )
      {}

   public:

      //* CViComResultDataProcessor interface implementation ************************************
      virtual void RegisterScannerId( WORD dwScannerDataId )
      {
         std::cout << m_rInterface.GetName() << " DataProcessor registered at scanner Id " << dwScannerDataId << std::endl;
      }

      virtual void RemoveScannerId( WORD dwScannerDataId )
      {
         std::cout << m_rInterface.GetName() << " DataProcessor unregistered at scanner Id " << dwScannerDataId << std::endl;
      }

      virtual void OnScannerDataMeasured( const ScannerData pData )
      {
         std::cout << "Got ViCom result #" << ++m_dwResultCounter << std::endl;

         if ( pData != NULL )
         {
            //
            // Do result processing.
            //
         }
      }

   private:

      ViComInterface_type&   m_rInterface;        //!< Pointer to the scanner interface
      DWORD                  m_dwResultCounter;   //!< Counter for the number of received results
   };


   int _tmain(int argc, _TCHAR* argv[])
   {
      //
      // Create a loader object for the selected technology and receiver type.
      //
      CViComLoader<ViComInterface_type> ViComLoader( receiver );

      try
      {
         //
         // Connect available receivers.
         //
         ViComLoader.Connect();

         //
         // Get a reference to the specific interface.
         //
         ViComInterface_type& rSpecificInterface = *ViComLoader.GetInterface();

         //
         // Get a reference to the basic interface.
         //
         CViComBasicInterface& rBasicInterface = rSpecificInterface.GetBasicInterface();

         //
         // Print version information.
         //
         std::cout << rBasicInterface.GetVersionInfoText() << std::endl;

         //
         // Create an instance of the result processor and register it as listener to the ViCom interface. 
         //
         CResultDataProcessor resultDataProcessor( rSpecificInterface );
         rSpecificInterface.RegisterResultDataListener( &resultDataProcessor );

      
         //
         // Setup the specific interface, do configuration tasks, etc.
         // This is the interface specific part which has to be handled separately for every technology.
         //
         // For more information see the sample applications in <VICOM_DIR>\src\SampleApplications\VicomSamples.sln
         //

         //
         // Now start the measurement. As soon as result are available,
         // these will be processed by the registered result processor.
         //
         rBasicInterface.StartMeasurement();
      
         //
         // Run as long as the user hits any key.
         //
         while ( true )
         {
            if ( _kbhit() )
               break;

            Sleep(10);
         }

         //
         // Stop the measurement and make sure it has really stopped within
         // SDefs::dwDefaultTimeOutInMs * 1e-3 seconds.
         //
         rBasicInterface.StopMeasurement();
         rBasicInterface.HasMeasurementStopped();

         //
         // Unregister the result processor to stop the processing of results.
         //
         rSpecificInterface.UnregisterResultDataListener( &resultDataProcessor );

         //
         // Release receivers associated with this loader.
         //
         ViComLoader.Disconnect();

         return 0;
      }
      catch(const CViComError& err)
      {
         //
         // Print error messages.
         //
         std::cout << "CViComError (" << err.GetErrorCode() << ") \"" << err.GetErrorString() << '\"' << std::endl;
      }

      return -1;
   }
@endcode
*/

/**
The functions in this section are accessed via a function called GetBasicInterface(), which is available from any ViCom interface object.
For example, to call the Basic interface function GetConnectedReceivers(), use the following code:

@code
myViComIF->GetBasicInterface().GetConnectedReceivers(myViComError);
@endcode

where myViComIF is a pointer to a ViCom interface object.

There are two kinds of interface functions. The first is called control function and does not return specific data. The
second kind is called request function which returns requested data.

The first parameter of both, control and request functions, is a reference to an instance of CViComError. In this instance possible
error information during the execution of the interface function is stored.

A control function returns a boolean value which is true if no error occurred. The return value of a request function is a pointer to
the requested data. This pointer is NULL if an error occurred. If no error occurred the data the pointer points to are valid as long as
no other interface function (control or request) is called. The data are for reading only.

This specification applies also for interface functions of CViComLoader and application specific interfaces e.g. CViComBasicInterface.
\@attribute[type="interface"]
*/
class CViComBasicInterface
{
public:
   //*** Interface functions ***/

   //** Version information **/
   /**
   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return the version of the interface, or NULL if an error occurred. The result specifies the
      version of the ViCom interface that implements the interface.
   */
   virtual const SVersionInfo* GetVersionInfo( CViComError& rViComError ) = 0;
   virtual const SVersionInfo* GetVersionInfo( )                          = 0;    
   //!< @throw CViComError @see GetVersionInfo( CViComError& )

   //** Version information text **/
   /**
   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return the character based version information of the interface, or NULL if an error occurred. The result specifies the
      version of the ViCom interface that implements the interface.
   */
   virtual const char* GetVersionInfoText( CViComError& rViComError ) = 0;
   virtual const char* GetVersionInfoText( )                          = 0;    
   //!< @throw CViComError @see GetVersionInfoText( CViComError& )


   //** Searching connected receivers **/
   /**
   This function retrieves the receivers that were connected to the computer when the Connect() function returned
   successfully.
   
   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      a list of connected devices if successful, NULL otherwise. In that case, the rViComError is filled with
      more details.
   */
   virtual const SConnectedReceiverTable* GetConnectedReceivers( CViComError& rViComError ) = 0;
   virtual const SConnectedReceiverTable* GetConnectedReceivers( )                          = 0;  
   //!< @throw CViComError @see GetConnectedReceivers( CViComError& )

   //** Select the receiver **/
   /**
   When many receivers are connected to the control computer, an application can use this function to select which receiver
   is communicated.

   If SelectReceiver is not called, the receiver at the index 0 which is next to the computer is used by default.
   
   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] dwReceiverIndex
      The position of the connected R&S TSM, given by the SConnectedReceiverTable returned by GetConnectedReceivers(), above.
      dwReceiverIndex is the array index of SConnectedReceiverTable::Receiver[dwMaxCountOfConnectedReceivers].

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool SelectReceiver( CViComError& rViComError, DWORD dwReceiverIndex ) = 0;
   virtual void SelectReceiver( DWORD dwReceiverIndex )                           = 0;  
   //!< @throw CViComError @see SelectReceiver( CViComError&, DWORD )

   /**
   This function is the counterpart for SelectReceiver() and retrieves the index of the currently selected receiver.

   @param[out] rViComError
   In case of error, this is filled with the error code and an description string.

   @return
   receiver index if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual DWORD GetSelectedReceiverIndex( CViComError& rViComError) = 0;
   virtual DWORD GetSelectedReceiverIndex()                          = 0;
   //!< @throw CViComError @see GetSelectedReceiverIndex( CViComError& )


   //** Identify the receiver **/
   /**
   Not supported yet!
   When many receivers are connected to the control computer, an application can use this function to trigger the identify
   functionality which leads to flashing LEDs on the receiver front panel.

   If SelectReceiver was not called once before, the receiver at the index 0 is used by default.
   
   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool Identify( CViComError& rViComError ) = 0;
   virtual void Identify()                           = 0;  
   //!< @throw CViComError @see Identify( CViComError& )


   //** Transducer files **/
   /**
   Some receivers support the usage of transducer files to take antenna specific parameters into account during measurement.
   Basically these XML files contain tables of frequency dependent gain / attenuation values for the used antenna on a per
   front end basis. This means it's possible to specify the antenna parameters for every front end on every supported
   receiver device. Be aware that there's no synchronization mechanism with other ViCom instances using the same receiver device.
   It's totally up to the user to ensure that nobody changes the configuration in the meantime.

   @note
         It is crucial to load the transducer tables when the receiver is in idle state, e.g. before any call of the
      StartMeasurement() method.

      If SelectReceiver was not called once before, the receiver at the index 0 is used by default.

   @note
      Is only supported when working with TSMW. Otherwise, a VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE error is returned.

   @param[in] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] pcTransducerFilePath
      Path of the transducer XML file, e.g. "C:\RuS\ViCom\tools\TsmTransducerTables.xml".
      Use NULL to clear recently set tables.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool LoadTransducerFile( CViComError& rViComError, const char* pcTransducerFilePath = NULL ) = 0;
   virtual void LoadTransducerFile( const char* pcTransducerFilePath = NULL )                           = 0;
   //!< @throw CViComError @see  LoadTransducerFile( CViComError&, const char* )


   //** Configuration of multi purpose receiver ports **/
   /**
   Some receivers support user specific configuration of their external multi purpose ports. Be aware that there's
   no synchronization mechanism with other ViCom instances using the same receiver device. It's totally up to the
   user to ensure that nobody changes the configuration in the meantime. Each port has to be treated separately, 
   for example activating PPS output on receiver port x does not automatically switch off PPS output on the port
   which was configured before.

   @note
         Changing the port configuration is only possible in idle state, e.g. before any call of the
      StartMeasurement() method.

      If SelectReceiver was not called once before, the receiver at the index 0 is used by default.

   @note
      For unsupported ports / devices a VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE error is returned.

   @param[in] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] rPortConfiguration
      Desired port and configuration provided as STSMPort structure.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool SetPortConfiguration( CViComError& rViComError, const STSMPort& rPortConfiguration ) = 0;
   virtual void SetPortConfiguration( const STSMPort& rPortConfiguration )                           = 0;
   //!< @throw CViComError @see  SetPortConfiguration( CViComError&, STSMPort& )

   /** Not supported yet!
   This functions allows to retrieve the currently active state for any supported port by the connected receiver
   device. Be aware that there's no synchronization mechanism with other ViCom instances using the same receiver device.
   It's totally up to the user to ensure that nobody changes the configuration in the meantime.

   @note
         Reading the port configuration is only possible in idle state, e.g. before any call of the
      StartMeasurement() method.

   @note
      For unsupported ports / devices a VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE error is returned.

      If SelectReceiver was not called once before, the receiver at the index 0 is used by default.

   @param[in] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] enPort
      Selected port for the status query.

   @return
      the port configuration if operation succeeded. If an error occurred, the parameter 
      rViComError contains the error information and NULL is returned.
   */
   virtual const STSMPort* GetPortConfiguration( CViComError& rViComError ) = 0;
   virtual const STSMPort* GetPortConfiguration( )                          = 0;
   //!< @throw CViComError @see  GetPortConfiguration( CViComError& )


   //** SetResultBufferDepth **/
   /**
   By means of this function you can set the depth of result buffer (FIFO). The default and the maximum is 1024 the minimum is 1.
   If the result buffer overruns then old result data are trashed.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] rResultBufferDepth
      A pointer to the desired buffer depth. When the function returns, this pointer contains the buffer depth that was
      actually set.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool SetResultBufferDepth( CViComError& rViComError, const SResultBufferDepth& rResultBufferDepth ) = 0;
   virtual void SetResultBufferDepth( const SResultBufferDepth& rResultBufferDepth )                           = 0;  
   //!< @throw CViComError @see  SetResultBufferDepth( CViComError& , const SResultBufferDepth& )


   //** Synchronisation of R&S TSMx **/
   /**
   The R&S TSMx receiver types have an internal time base. This time base has to be synchronized to another reference. By means of this
   function you can set the specific synchronization mode. Note that not all modes are supported in some measurement.
   
   If SelectReceiver was not called once before, the receiver at the index 0 is used by default.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] eMode
      The synchronization mode to be used, defined in the enum TimeBaseSynchronisationMode::Type.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool SetTimebaseSynchronisationMode( CViComError& rViComError, TimeBaseSynchronisationMode::Type eMode ) = 0;
   virtual void SetTimebaseSynchronisationMode( TimeBaseSynchronisationMode::Type eMode )                           = 0;  
   //!< @throw CViComError @see SetTimebaseSynchronisationMode( CViComError&, TimeBaseSynchronisationMode::Type )

   /**
   // Not supported yet!
   In order to synchronize the measurements of several TSME receivers it's possible to configure the used timebase.
   Every receiver having the same timebase identifier operates on the same timebase, thus these receivers are
   synchronized. It is required to manually connect the different receivers with aux cables to allow the synchronization.

   If SelectReceiver was not called once before, the receiver at the index 0 is used by default.
   
   @note
      is not supported when working with TSMx / TSMW. In such a case, a VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE error is returned.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] dwTimebaseIdentifier
      The arbitrarily chosen timebase identifier.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool SetReceiverTimebase( CViComError& rViComError, DWORD dwTimebaseIdentifier ) = 0;
   virtual void SetReceiverTimebase( DWORD dwTimebaseIdentifier )                           = 0;  
   //!< @throw CViComError @see SetReceiverTimebase( CViComError&, DWORD )


   /**
   // Not supported yet!
   In order to synchronize the measurements of several TSME receivers it's possible to configure the used timebase.
   Every receiver having the same timebase identifier operates on the same timebase, thus these receivers are
   synchronized.

   If SelectReceiver was not called once before, the receiver at the index 0 is used by default.
   
   @note
      is not supported when working with TSMx / TSMW. In such a case, a VICEC_FUNCTION_NOT_SUPPORTED_BY_DEVICE error is returned.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      the timebase identifier if operation succeeded. If an error occurred, the parameter 
      rViComError contains the error information and NULL is returned.
   */
   virtual const DWORD* GetReceiverTimebase( CViComError& rViComError ) = 0;
   virtual const DWORD* GetReceiverTimebase( )                          = 0;  
   //!< @throw CViComError @see GetReceiverTimebase( CViComError& )


   //** Start a measurement **/

   /**
   This is possible if all preconditions have been met e.g frequencies have been selected, and a previous measurement has been stopped
   successfully. A sucessful stop can be checked with the HasMeasurementStopped() function below.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool StartMeasurement( CViComError& rViComError ) = 0;
   virtual void StartMeasurement( )                          = 0;  
   //!< @throw CViComError @see StartMeasurement( CViComError& )

   /**
   If StartMeasurement fails then additional messages received from the receiver can be retrieved with
   GetMessagesDuringStartMeasurement().


   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      Pointer to a list of SMessage objects, containing message information if operation succeeded.
      If an error occurred, the parameter rViComError contains the error information and NULL is returned.
   */
   virtual SViComList<SMessage>* GetMessagesDuringStartMeasurement( CViComError& rViComError ) = 0;
   virtual SViComList<SMessage>* GetMessagesDuringStartMeasurement( )                          = 0;  
   //!< @throw CViComError @see GetMessagesDuringStartMeasurement( CViComError& )

   //** Ask for result availability **/
   /**
   This basic function returns the number of available results which are waiting to be fetched with the GetResult()
   function of the specific interface as well as the number of results already overwritten.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      Pointer to a struct containing result availability information if operation succeeded.
      If an error occurred, the parameter rViComError contains the error information and NULL is returned.
   */
   virtual const SResultCounters* GetResultCounters( CViComError& rViComError ) = 0;
   virtual const SResultCounters* GetResultCounters( )                          = 0;  
   //!< @throw CViComError @see GetResultCounters( CViComError& )


   /** Stop the measurement */

   /**
   This function stops R&S TSMx scanner measurement. To confirm that measurement has been successfully stopped,
   HasMeasurementStopped() should be called.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @return
      true if operation succeeded. If an error occurred, the parameter rViComError contains the error information.
   */
   virtual bool StopMeasurement( CViComError& rViComError ) = 0;
   virtual void StopMeasurement( )                          = 0;  
   //!< @throw CViComError @see StopMeasurement( CViComError& )

   /**
   This function checks to see if measurement really has stopped.

   @param[out] rViComError
      In case of error, this is filled with the error code and an description string.

   @param[in] dwTimeOutInMs
      The maximum time that this function should delay, before it returns. If the measurements are successfully
      stopped before the timeout, then the function will return immediately.

   @note
      The general recommendation for the dwTimeOutInMs values is 15 seconds.

   @return
      If the measurement has been successfully stopped, this function returns true.
      If the measurement has not successfully stopped, it returns false.
   */
   virtual bool HasMeasurementStopped( CViComError& rViComError, DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs ) = 0;
   virtual bool HasMeasurementStopped( DWORD dwTimeOutInMs = SDefs::dwDefaultTimeOutInMs )                           = 0; 
   //!< @throw CViComError @see HasMeasurementStopped( CViComError&, DWORD )


   //* CViComRawDataProcessor integration ************************************/

   /**
   Register a RawScannerData listener that will be notified when new raw data is available. Note that it is
   important to Unregister the listener again once it is getting deleted, to avoid crashes.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] pListener
      The object that shall be notified when new raw measurement data is available. This must not be
      null. If the listener has already been registered to the interface, it will not be
      registered a second time. Instead, an error will be indicated.

   @return
      if listener has been appended to the list of registered listeners or not
   */
   virtual bool RegisterRawDataListener( CViComError& rViComError, CViComRawDataProcessor* pListener ) = 0;
   virtual void RegisterRawDataListener( CViComRawDataProcessor* pListener )                           = 0; 
   //!< @throw CViComError @see RegisterRawDataListener( CViComError&, CViComRawDataProcessor* )


   /**
   Unregister a formerly registered RawScannerData listener to remove it from the list of objects that
   shall get notification about new measurement data.

   @param[out] rViComError
      A CViComError object is passed by reference, and in case of error, is filled with the error code and text string.

   @param[in] pListener
      The listener that shall be unregistered. If NULL or if the listener is not found in the list of registered
      objects, an error is indicated.

   @return
      if listener has been removed from the list of registered listeners or not
   */
   virtual bool UnregisterRawDataListener( CViComError& rViComError, CViComRawDataProcessor* pListener ) = 0;
   virtual void UnregisterRawDataListener( CViComRawDataProcessor* pListener )                           = 0; 
   //!< @throw CViComError @see UnregisterRawDataListener( CViComError&, CViComRawDataProcessor* )


protected:
   virtual ~CViComBasicInterface() {}

};

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
