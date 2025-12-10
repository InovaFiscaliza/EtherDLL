// ViComBasicInterfaceData.h
#define VICOM_VERSION_MAJOR       15
#define VICOM_VERSION_MINOR       60
#define VICOM_VERSION_BUILD       0
#define VICOM_VERSION_IF_BASIC    27      // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)      // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                     // get a definite structure alignment for CViComBasicInterface Data

namespace RohdeSchwarz
{
namespace ViCom
{

//** Declarations used in CViComBasicInterface and other ViCom interfaces e.g. CViComWcdmaInterface.

/**
Template for a simple container which is a forward linked list of elements of type OBJECT
\@attribute[type="ignore"]
*/
template <class OBJECT> struct SViComList
{
   DWORD                dwCount;             //!< number of elements in list
   struct SLinkedObject: public OBJECT
   {
      SLinkedObject     *pNext;              //!< pointer to next element. If the current element is the
                                             //!< last in the list, this is NULL.

   }                    *pFirst;             //!< pointer to head element of the list
};

/**
Template for a simple container which defines a value range including a default value.
\@attribute[type="ignore"]
*/
template <typename T> 
struct SRange
{
   SRange()
   {}

   SRange( T minimumValue, T maximumValue, T defaultValue )
      : minimum( minimumValue )
      , maximum( maximumValue )
      , default( defaultValue )
   {}

   T     minimum;                            //!< Lower bound of the value range.
   T     maximum;                            //!< Upper bound of the value range.
   T     default;                            //!< Default value.
};


//** CViComBasicInterface declaration

/**
This class defines the basic data structures used in the specialized ViCom interfaces. All structures
are available in the technology implementations, such as version info, Receiver table etc.

@see CViComBasicInterface also for details on what to do with the data containers defined here.
*/

enum etBasicInterfaceVersion   { VICOM_BASIC_VERSION = VICOM_VERSION_IF_BASIC }; // do not edit

/**
* Definition of constants
* \@attribute[type="serializable"]
*/
struct SDefs
{
   /**
   Default value for all interface methods which require a timeout parameter.
   */
   static const DWORD dwDefaultTimeOutInMs = 15000;

   /**
   Invalid value marker for DWORD values.
   */
   static const DWORD INVALID_DWORD_VALUE = 0xFFFFFFFF;

   /**
   Invalid value marker for WORD values.
   */
   static const WORD INVALID_WORD_VALUE = 0xFFFF;

   /**
   Invalid value marker for short values.
   */
   static const short INVALID_SHORT_VALUE = 0x7FFF;

   /**
   Invalid value marker for BYTE values.
   */   
   static const BYTE INVALID_BYTE_VALUE = 0xFF;
};

/**
* Defines a data model to hold version information of the ViCom interface. This should reflect the
* DLL version information.
* File version number of DLL is:    dwVersionMajor.dwVersionMinor.dwVersionPatch.dwVersionBuild
* Product version number of DLL is: dwVersionMajor.dwVersionMinor.dwBasicInterfaceVersion.dwApplicationInterfaceVersion
* \@attribute[type="serializable"]
*/
struct SVersionInfo
{
   DWORD dwVersionMajor;                  //!< Major product version
   DWORD dwVersionMinor;                  //!< Minor product version
   DWORD dwBasicInterfaceVersion;         //!< Version number of the ViComBasicInterface
   DWORD dwApplicationInterfaceVersion;   //!< Version number of the ViComApplicationInterface, such as RF Power Scan etc
   DWORD dwVersionBuild;                  //!< Build number
};

//** Declarations used for connected receivers

/**
Different versions of the ViCom interface are suitable for different receiver types. This field gives the receiver type,
with the following values defined in the enum Receiver::Type:
\@attribute[type="serializable"]
*/
struct Receiver { enum Type { 
   TSMU           = 2,     //!< TSMU Model
   TSMU_H         = 3,     //!< TSMU Model with integrated RF-attenuators for processing of stronger power signals
   TSMQ           = 50,    //!< TSMQ Model
   TSMM           = 50,    //!< TSMM Model
   TSML_CW        = 15,    //!< TSML Model, Option CW
   TSML_W         = 11,    //!< TSML Model, Option WCDMA
   TSML_G         = 13,    //!< TSML Model, Option GSM
   TSML_C         = 12,    //!< TSML Model, Option CDMA
   TSML_E         = 16,    //!< TSML Model, Option EGPRS
   TSML_GW        = 20,    //!< TSML Model, Option GSM / WCDMA
   TSMW           = 300102,//!< TSMW
   TSME           = 150051,//!< TSME

   UNKNOWN        = 0      //!< Unknown receiver, not supported!
};};

/**
Definitions for technology types.
\@attribute[type="serializable"]
*/
struct Technology { enum Type  { 
      UNKNOWN = 0,      //!< Type is not known
      GSM = 1,          //!< GSM
      WCDMA = 2,        //!< WCDMA
      CDMA = 3,         //!< CDMA2000
      EVDO = 4,         //!< EvDO
      LTE = 5,          //!< LTE
      WIMAX = 6,        //!< WIMAX
      TDSCDMA = 7,      //!< TD-SCDMA
      TETRA = 8,        //!< TETRA
      GPS = 9,          //!< GPS
      CW = 10,          //!< CW
      RFPS = 11,        //!< RF Power Scan
      RFSCAN = 12,      //!< RF Scan
      RS232 = 13,       //!< RS232
      ACD = 14,         //!< Automatic channel detection
};};


/**
When the system is asked for how many devices are connected, this structure is the result. This contains
- version information
- hardware information
- driver information
about every receiver that was connected before and are still active.

@note The table does not contain receivers of various types. E.g. if the system is asked for connected devices
on a TSMW, the connected receiver table will not contain any receivers other than TSMW!
\@attribute[type="serializable"]
*/
struct SConnectedReceiverTable
{
   /**
   There can not be more than 8 receivers connected to one PC at a time.
   */
   static const DWORD
                     dwMaxCountOfConnectedReceivers = 8;

   /**
   The number of R&S TSMx receivers which are connected to the PC.

   The theoretical maximum count is set to 8, but please note, that at the time of writing, the maximum tested value is 3.
   **/
   DWORD             dwCountOfReceivers;

   /**
   For each receiver connected to the system, one such structure is filled. dwCountOfReceivers contains
   how many instances of this structure are available.
   * \@attribute[type="serializable"]
   */
   struct SReceiver
   {
      /** TSMx receiver are connected in a chain from the PC. 1 means next to the PC. */
      DWORD          dw1394ChainPosition;

      /** Version of the used IEEE1394 firewire driver. */
      DWORD          dw1394DriverVersion;

      /**
      TSMW and TSME receiver are connected via IEEE 802.3 Ethernet bus.
      The address is formated in a dot-decimal like notation, e.g. the first array element
      is the first octet of the address.
      */
      BYTE           bIpAddress[4];

      /** Version of the internet protocol for TSMW and TSME. */
      BYTE           bIpVersion;

      /** Defines the receiver type. The command set of this interface is compatible with certain receiver types. */
      Receiver::Type eReceiver;

      /** Serial number of the receiver. */
      DWORD          dwSerialNumber;

      /** Model number of the receiver. */
      DWORD          dwModelNumber;

      /** Version of the application software, to be read as an 8 digit decimal integer. */
      DWORD          dwSoftwareVersion;

      /**
      Version of the installed hardware inclusive FPGA configuration code, to be read as an 8 digit decimal integer
      Example: TSMW FW Version 1.15.0.0 => dwHardwareVersion = 0x010F0000
      */
      DWORD          dwHardwareVersion;

      /**
      This defines what special capabilities the receiver has. This value is a or-combination of the
      constants defined below. For each capability, there is another application interface that can be
      used to access the specific functionality. For example, if dwFlagForRFPowerScan flag is set, then
      the ViComRFPowerScanInterface can be used for that receiver.
      */
      DWORD          dwAvailableMeasurementMask;

      /** measurement capabilities of the device. */
      static const DWORD
                     dwFlagForWcdmaScanner             = 0x00000001,    //!< device supports CViComWcdmaInterface
                     dwFlagForWcdmaDemodulator         = 0x00000002,    //!< device supports BCH decoding for CViComWcdmaInterface
                     dwFlagForGsmScanner               = 0x00000004,    //!< device supports CViComGsmInterface
                     dwFlagForCdmaScanner              = 0x00000008,    //!< device supports CViComCdmaInterface for CDMA
                     dwFlagForEvdoScanner              = 0x00000010,    //!< device supports CViComCdmaInterface for EVDO
                     dwFlagForRFScan                   = 0x00000020,    //!< device supports CViComRFScanInterface
                     dwFlagForRFPowerScan              = 0x00000020,    //!< device supports CViComRFPowerScanInterface
                     dwFlagForCwMeasurement            = 0x00000040,    //!< device supports ViComCWInterface
                     dwFlagForRS232Tunneling           = 0x00000080,    //!< device supports CViComRS232Interface
                     dwFlagForRawDataHandlerWcdma      = 0x00000100,    //!< device supports raw data handler for WCDMA technology
                     dwFlagForLteScanner               = 0x00000200,    //!< device supports CViComLteInterface
                     dwFlagForRawDataHandlerCdma       = 0x00000400,    //!< device supports raw data handler for CDMA technology
                     dwFlagForRawDataHandlerGsm        = 0x00000800,    //!< device supports raw data handler for GSM technology
                     dwFlagForWimaxScanner             = 0x00001000,    //!< device supports CViComWimaxInterface
                     dwFlagForTetraScanner             = 0x00002000,    //!< reserved
                     dwFlagForRawDataHandlerLte        = 0x00004000,    //!< device supports raw data handler for LTE technology
                     dwFlagForRawDataHandlerWimax      = 0x00008000,    //!< device supports raw data handler for WIMAX technology
                     dwFlagForRawDataHandlerTetra      = 0x00010000,    //!< reserved
                     dwFlagForLteMimoScanner           = 0x00020000,    //!< device supports LTE WB / MIMO measurements
                     dwFlagForTdScdmaScanner           = 0x00040000,    //!< device supports CViComTdScdmaInterface
                     dwFlagForRawDataHandlerTdScdma    = 0x00080000,    //!< reserved
                     dwFlagForTetra2Scanner            = 0x00100000,    //!< reserved
                     dwFlagForTetraDMO                 = 0x00200000,    //!< reserved
                     dwFlagForRawDataHandlerGps        = 0x00400000,    //!< device supports raw data handler for GPS technology
                     dwFlagForRawDataHandlerAir        = 0x00800000,    //!< reserved
                     dwFlagForRemote                   = 0x02000000,    //!< device supports remote ViCom
                     dwFlagForAutomaticChannelDetection= 0x04000000,    //!< device supports automatic channel detection
                     dwFlagForLteMbmsScanner           = 0x08000000;    //!< device supports LTE MBSFN measurements

      /**
      Available RF front ends. This value is an or-combination of the SRFPort::RF_xx constants defined below.
      E.g. dwAvailableFrontEndMask & SRFPort::RF_1 equals true if front end 1 is available.
      */
      DWORD          dwAvailableFrontEndMask;

      /**
      List of frequency bands supported by the receiver. Each sub band is specified in Hz.
      */
      SViComList<SRange<double>> ListOfFrequencyBands;

      /**
      Receiver temperatures.
      \@attribute[type="serializable"]
      */
      struct STemperature
      {
         /**
         Specifies the type of a receiver temperature.
         \@attribute[type="serializable"]
         */
         struct TemperatureType { enum Type {
               ANALYZER        = 1,                         //!< Analyzer board temperature.
               CONTROLLER      = 2,                         //!< Controller board temperature.
               PROCESSOR_BOARD = 3,                         //!< Processor board temperature.
               RF1             = 4,                         //!< RF1 temperature.
               RF2             = 5                          //!< RF2 temperature.
         };};

         TemperatureType::Type   eType;                     //!< Type of the temperature value.
         short                   sTemperatureInDegreeC;     //!< Temperature value in degree Celsius.
      };

      /**
      List of receiver temperatures.
      */
      SViComList<STemperature> ListOfTemperatures;
  };
   
   /**
   \@attribute[type="array" size="dwCountOfReceivers" sizeismember="true"]
   */
   SReceiver Receivers[dwMaxCountOfConnectedReceivers];

};

//** Declaration used in function SetResultBufferDepth **/
/**
This structure defines the size of the internal result buffer that will be used to cache results from the
specific interfaces. The default and the maximum is 1024 the minimum is 1.
\@attribute[type="serializable"]
*/
struct SResultBufferDepth
{
   SResultBufferDepth()
      : dwValue( dwMax )
   {}

   // limits for dwValue
   static const DWORD
                     dwMin =    1,  //!< at least one element must be storable in the buffer
                     dwMax = 1024;  //!< to limit the memory consumption, at most 1024 can be stored in the result buffer.

   /** number of results that will be stored at most in the buffer. Must be within [ dwMin; dwMax ].**/
   DWORD             dwValue;
};

//** Declarations used to control and to read the time base synchronisation of TSMx devices **/

/**
The TSM receiver types have an internal time base. This time base has to be synchronized to another reference.
Note that not all modes are supported in some measurement.
\@attribute[type="serializable"]
*/
struct TimeBaseSynchronisationMode { enum Type {
   INTERNAL              = 0,  //!< For all measurements, the TSM is not synchronised by another source
   GSM                   = 1,  //!< For measurements, R&S TSM is synchronised by GSM signals over the air.
   PPS_WATCHED_BY_GSM    = 2,  //!< For all measurements, the TSM is synchronised by the PPS pulse. If the PPS
                               //!< pulse is not available or incorrect then GSM is used
   PPS                   = 3,  //!< For reports in STimeBaseSynchronisation, the R&S TSM is synchronised by the PPS pulse.
   WCDMA                 = 4,  //!< For the WCDMA scanner only, the average of measured CPICH signals is used
   CDMA                  = 5,  //!< For CDMA and EvDo scanner only.
   UNKNOWN               = 256 //!< Used for reports if the sync source information is not available.
};};


//** Declaration to store additional TSM specific messages. **/
/**
Structure that describes a message from a sub-system. A message can be either just an info or even an error message.
Used in ViComBasicInterface::GetMessagesDuringStartMeasurement().
\@attribute[type="serializable"]
*/
struct SMessage
{
   /** Specifies the severity levels of a message 
   \@attribute[type="serializable"]
   */
   struct MessageType { enum Type {
      INFO      = 1,                       //!< Information is something the user should be noticed of
      WARNING   = 2,                       //!< Warning message means that something is wrong but the function may continue
      ERROR_    = 3,                       //!< Error messages contain severe problem that stop something from working.
      UNDEFINED = 0
   };};
   
   MessageType::Type eMessageType;         //!< The actual message type.
   const char        *pcszMessageText;     //!< The actual message what has happened.
};


//** Declaration to store result availability information **/
/**
This can be used to report the current fill state of the result buffer and a history how many results had
to be removed from the buffer before they could be processed.
\@attribute[type="serializable"]
*/
struct SResultCounters
{
   DWORD             dwCountOfBufferedResults;  //!< Number of results available in the ViCom result buffer. There may be some exceptions
                                                //!< that some results in the ViCom buffer are not relevant for the host application.
                                                //!< To improve performance this results are counted here and sorted out in the GetResult()
                                                //!< function of the specific interface.
   DWORD             dwCountOfThrashedResults;  //!< Number of results overwritten in the ViCom buffer. This happens only on buffer overrun.
};

//** Declaration for substructures of result data. **/
/**
If attenuation is introduced in the RF path, then the signal to noise ratio will be lower. If a preamplifier is used,
this improves (reduces) the noise figure.

To assess the impact on the noise figure, R&S TSMx devices therefore report attenuation settings and preamplifier
usage for RF measurements.

STSMxAttenuation is a sub-structure of SMeasResult.
\@attribute[type="serializable"]
**/
struct STSMxAttenuation
{
   /**
   Type and power of attenuation.

   The meaning of the enumeration values can be derived using the look-up table shown below:

   PA_ON, PA_OFF: preamplifier on/off
   RFx:           nominal x dB RF attenuation (the true value is calibrated and may vary from the nominal value)
   IFx:           nominal IF attenuation (does not influence the noise figure)
   \@attribute[type="serializable"]
   */
   struct AttMode { enum Type {
      PA_ON_RF0_IF0          = 0,  //!< preamplifier on, 0 dB RF attenuation, 0 db IF attenuation
      PA_OFF_RF0_IF0         = 1,  //!< preamplifier off, 0 dB RF attenuation, 0 db IF attenuation
      PA_OFF_RF0_IF10        = 2,  //!< preamplifier off, 0 dB RF attenuation, 10 db IF attenuation
      PA_OFF_RF0_IF15        = 3,  //!< preamplifier off, 0 dB RF attenuation, 15 db IF attenuation
      TSMXH_PA_ON_RF0_IF0    = 10, //!< TSMx-H preamplifier on, 0 dB RF attenuation, 0 db IF attenuation
      TSMXH_PA_OFF_RF0_IF0   = 11, //!< TSMx-H preamplifier off, 0 dB RF attenuation, 0 db IF attenuation
      TSMXH_PA_OFF_RF10_IF0  = 12, //!< TSMx-H preamplifier off, 10 dB RF attenuation, 0 db IF attenuation
      TSMXH_PA_OFF_RF20_IF0  = 13, //!< TSMx-H preamplifier off, 20 dB RF attenuation, 0 db IF attenuation
      TSMXH_PA_OFF_RF30_IF0  = 14, //!< TSMx-H preamplifier off, 30 dB RF attenuation, 0 db IF attenuation
      TSMXH_PA_OFF_RF30_IF10 = 15, //!< TSMx-H preamplifier off, 30 dB RF attenuation, 10 db IF attenuation
      TSMXH_PA_OFF_RF30_IF15 = 16  //!< TSMx-H preamplifier off, 30 dB RF attenuation, 15 db IF attenuation
   };};
   
   AttMode::Type     eMode;  //!< Actual mode

   /**
   Signalizes why attenuation is used or suppressed.

   Normally attenuation is introduced if we get IF overflows at the RF AD converters. Then IF_OVERFLOW is reported.
   But there exists a special scanning mode to estimate intermodulation products. This mode is only active for GSM scanning.
   When RF measurement may be affected by intermodulation it bridges the preamplifier (PA_OFF) and reports INTERMODULATION.
   * \@attribute[type="serializable"]
   */
   struct AttReason { enum Type {
      IF_OVERFLOW                = 0,  //!< Overflow in  RF AD converter
      INTERMODULATION            = 1   //!< Pre-Amplifier is bridged, intermodulation detected.
   };};
   
   AttReason::Type   eReason; //!< Actual reason
};

/**
The R&S TSMx receiver types have an internal time base, that must be synchronized to another reference. This structure
contains information about synchronizing the R&S TSM internal time base.

The external reference may be read from a commercial network GSM or UMTS signal, or gained from another source via the
PPS cable in the rear panel of the TSM.
\@attribute[type="serializable"]
*/
struct STimeBaseSynchronisation
{
   TimeBaseSynchronisationMode::Type
                     eTimeBaseSynchronisationMode;                //!< @see etTimeBaseSynchronisationMode declaration above
   long              lEstimatedTimeBaseCorrectionInNanoSecPerSec; //!< Correction applied to the internal time base, in nanoseconds/second.
                                                                  //!< It is assumed that the internal time is linear to the correction
                                                                  //!< The correction is an average over different receiver devices.
   long              lPcTimeStampOfLastChangeInMs;                //!< Time when the last time base correction was applied
                                                                  //!< (related to GetTickCount() system time).
                                                                  //!< Note that this value may be negative if the PC was started after the last correction.
   long              lReceiverTemperatureInDegree100;             //!< TSMx receiver temperature in 0.01°C
};

//** Declaration for substructures of multi purpose receiver port configuration. **/
/**
The multi purpose receiver I/O connectors can be used for application specific requirements like external trigger signals
or timing reference. The configuration has to be done by specifying a port and the wanted mode. Used in
ViComBasicInterface::SetPortConfiguration() method.
\@attribute[type="serializable"]
*/
struct STSMPort
{
   /**
   Port of the receiver device.
   \@attribute[type="serializable"]
   */
   struct Port { enum Type {
      TSMW_PULSE1       = 0,  //!< Pulse 1 port of TSMW receiver.
      TSMW_PULSE2       = 1,  //!< Pulse 2 port of TSMW receiver.
      TSMW_EXT_REF      = 2   //!< External 10 MHz reference port of TSMW receiver.
   };};
   
   Port::Type     ePort;      //!< Port.

   /**
   Operating mode of the receiver port. Only a subset of the following settings might be supported by specific receiver ports
   in STSMPort::ePort.
   \@attribute[type="serializable"]
   */
   struct Mode { enum Type {
      NOT_USED        = 0,  //!< Disabled port, not used.
      ACTIVE          = 1,  //!< Enabled port. E.g. used to enable the ext. reference port of TSMW receivers.
      TRIGGER_OUT     = 2,  //!< Trigger output mode. Not supported yet!
      TRIGGER_IN      = 3,  //!< Trigger input mode. Not supported yet!
      PPS_OUT         = 4,  //!< PPS output mode. 
                            //!< Used to route the GPS PPS signal of the receiver internal GPS device to the selected port.
                            //!< Only possible with connected GPS antenna and sufficient GPS signal reception.
      PPS_IN          = 5   //!< PPS input mode. Not supported yet!
   };};
   
   Mode::Type  eMode;       //!< Mode.
};


//** Declaration of constants for receiver front end selection. **/
/**
The available RF front ends of a connected receiver are listed in SReceiver::dwAvailableFrontEndMask
which is an or-combination of the following constants.
\@attribute[type="serializable"]
*/
struct SRFPort{ enum Type {
   RF_1  = 1<<0,            //!< RF front end 1
   RF_2  = 1<<1,            //!< RF front end 2
   RF_3  = 1<<2,            //!< RF front end 3
   RF_4  = 1<<3,            //!< RF front end 4
   RF_5  = 1<<4,            //!< RF front end 5
   RF_6  = 1<<5,            //!< RF front end 6
   RF_7  = 1<<6,            //!< RF front end 7
   RF_8  = 1<<7,            //!< RF front end 8
   RF_9  = 1<<8,            //!< RF front end 9
   RF_10 = 1<<9,            //!< RF front end 10
   RF_11 = 1<<10,           //!< RF front end 11
   RF_12 = 1<<11,           //!< RF front end 12
   RF_13 = 1<<12,           //!< RF front end 13
   RF_14 = 1<<13,           //!< RF front end 14
   RF_15 = 1<<14,           //!< RF front end 15
   RF_16 = 1<<15,           //!< RF front end 16
   RF_17 = 1<<16,           //!< RF front end 17
   RF_18 = 1<<17,           //!< RF front end 18
   RF_19 = 1<<18,           //!< RF front end 19
   RF_20 = 1<<19,           //!< RF front end 20
   RF_21 = 1<<20,           //!< RF front end 21
   RF_22 = 1<<21,           //!< RF front end 22
   RF_23 = 1<<22,           //!< RF front end 23
   RF_24 = 1<<23,           //!< RF front end 24
   RF_25 = 1<<24,           //!< RF front end 25
   RF_26 = 1<<25,           //!< RF front end 26
   RF_27 = 1<<26,           //!< RF front end 27
   RF_28 = 1<<27,           //!< RF front end 28
   RF_29 = 1<<28,           //!< RF front end 29
   RF_30 = 1<<29,           //!< RF front end 30
   RF_31 = 1<<30,           //!< RF front end 31
   RF_32 = 1<<31            //!< RF front end 32
}; };

//** Declaration of a result structures for measurement rates. **/
/**
Measurement rates result structure for specific ViComs.
\@attribute[type="serializable"]
*/
struct SMeasurementRate
{
   /**
   Rate type
   \@attribute[type="serializable"]
   */
   struct MeasRateType { enum Type {
      /**
      Invalid rate type.
      */
      INVALID = 0,

      /**
      From first to last available measurement.
      */
      TOTAL = 1,

      /**
      All measurements from the first measurement 2 minutes after the first measurement to the last measurement.
      */
      SUB_TOTAL = 2,

      /**
      All measurements taken in the last minute. Within the first minute after the 
      first measurement the interval is shorter.
      */
      RECENT = 3

   };};
   
   MeasRateType::Type   eMeasRateType;

   /**
   Count of measurements in the time interval defined below.
   */
   DWORD    dwCountOfMeasurements;

   /**
   Time of the first measurement the meas rate has been calculated in seconds.
   */
   double   dStartTimeInSec; 

   /**
   Time of the last measurement (or time next to the last measurement) taken for meas rate calculation in seconds.
   */
   double   dEndTimeInSec;

   /**
   Measurement rate in 0.001 per second.
   */
   DWORD    dwMeasRateInMilliHz;
};

/**
Complex float value.
\@attribute[type="serializable"]
*/
struct SComplex
{
   /**
   Real part of the value.
   */
   float fRe;

   /**
   Imaginary part of the value.
   */
   float fIm;
};

} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment
#endif //VICOM_RESOURCE_INCLUDE
