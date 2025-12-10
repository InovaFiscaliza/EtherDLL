// ViComRS232InterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_RS232            1     // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)            // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                           // get a definite structure alignment for CViComRS232Interface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace RS232
{
/**
Data containers used for ViCom RS232 interface. Contains measurement configuration and result structures.
*/
   
//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_RS232,
                                 VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
RS232 baud rate
\@attribute[type="serializable"]
**/
enum etBAUDRATE
{
   BAUDRATE_4800                 = 4800,     //!< set to 4800 Bd
   BAUDRATE_9600                 = 9600,     //!< set to 9600 Bd
   BAUDRATE_14400                = 14400,    //!< set to 14400 Bd
   BAUDRATE_19200                = 19200,    //!< set to 19200 Bd
   BAUDRATE_38400                = 38400,    //!< set to 38400 Bd
   BAUDRATE_57600                = 57600,    //!< set to 57600 Bd
   BAUDRATE_115200               = 115200    //!< set to 115200 Bd
};

/**
The device can be switched to "Silent Mode" to suppress usual trace output on RS232.
\@attribute[type="serializable"]
**/
enum etSILENT_MODE
{
   SILENTMODE_LEAVE_UNCHANGED    = 0,        //!< leave Silent Mode configuration unchanged
   SILENTMODE_SWITCH_OFF         = 2,        //!< switch OFF Silent Mode
   SILENTMODE_SWITCH_ON          = 4         //!< switch ON Silent Mode
};


/**
Used to refer input and output characters.
\@attribute[type="serializable"]
*/
struct SCharacterBuffer
{
   static const DWORD   dwMaxTxCharCount = 0x10000;   //!< maximum count of characters to send using one call of SendToRS232

   DWORD                dwCharCount;                  //!< count of characters in the buffer
   const char           *pcCharacters;                //!< pointer to the data received or the data to send
                                                      //!< \@attribute[type="array" size="dwCharCount" sizeismember="true"]
};

//** Request Settings **/
/**
By means of the functions: SetBaudRate() and SetSilentMode() internal settings of the 
RS232 Interface may be changed and with the functions SelectReceiver(),
SetResultBufferDepth() change basic settings.
All settings have default values.

This structure bundles these settings and will be returned as result from the GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   // basic settings
   DWORD                            dwReceiverIndex;              //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth               ResultBufferDepth;            //!< Number of results allowed in the result buffer
   // RS232 settings
   etBAUDRATE                       eBaudRate;                    //!< Specifies the baudrate the device will be configured with.
                                                                  //!< @see SetBaudRate
   etSILENT_MODE                    eSilentMode;                  //!< Specifies the device Silent Mode
                                                                  //!< @see SetSilentMode
};


//** Measurement results **/
/**
This structure holds a measurement result from the result buffer. Please refer to the manual for a detailed discussion of
how to interpret the results.
@note
   that the result data are for reading only and available until the next interface function is called.

\@attribute[type="serializable"]
**/
struct SMeasResult
{
   struct SHardwareErros
   {
      bool           bFrameError;         //!< indicates a RS232 frame error
      bool           bOverrunError;       //!< indicates a receive buffer overrun
   }                 *pHardwareErros;     //!< indicates detected RS232 erros
                                          //!< This is NULL if no erros are noticed

   SCharacterBuffer  *pReceiveData;       //!< refers to the characters that have been received by the device on RS232
                                          //!< This is NULL if no received data is available.
};

} // namespace RS232
} // namespace ViCom
} // namespace RohdeSchwarz
#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
