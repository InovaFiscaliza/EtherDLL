// ViComGpsInterfaceData.h
#include "ViComBasicInterfaceData.h"
#define VICOM_VERSION_IF_GPS       10        // do not edit

#if !defined(VICOM_RESOURCE_INCLUDE)         // here stops the inclusion for resource files
#pragma once
#pragma pack(push, 8)                        // get a definite structure alignment for CViComGpsInterface


namespace RohdeSchwarz
{
namespace ViCom
{
namespace GPS
{
/**
Data containers used for ViCom GPS interface. Contains measurement configuration and result structures.
*/

//** ViCom Interface Version **//
enum etViComInterfaceVersion  {VICOM_INTERFACE_VERSION = VICOM_VERSION_IF_GPS,
                               VICOM_BASIC_VERSION     = VICOM_VERSION_IF_BASIC }; // do not edit

/**
Definition of the satellite navigation system type.
\@attribute[type="serializable"]
*/
struct SatNavSystem{ enum Type {
   GPS                           = 1,   //!< Global Positioning System (GPS).
   GLONASS                       = 2,   //!< GLObal NAvigation Satellite System (GLONASS) (not supported yet).
   GALILEO                       = 4    //!< Galileo satellite navigation system (not supported yet).
}; };

/**
Definition of the GPS message format (protocol type).
\@attribute[type="serializable"]
*/
struct GPSMessageFormat{ enum Type {
   VICOM_GPS_FORMAT_UNKNOWN      = 0,  //!< The format is not known yet (return value from the GetSettings() method).
   VICOM_GPS_FORMAT_NMEA         = 1,  //!< NMEA standard format.
   VICOM_GPS_FORMAT_UBLOX        = 2,  //!< UBLOX binary format.
   VICOM_GPS_FORMAT_BOTH         = 3,  //!< NMEA and UBLOX format.
}; };

/**
Definition of the GPS receiver reset modes.
\@attribute[type="serializable"]
*/
struct ResetMode{ enum Type {
   NONE,                               //!< No reset.
   HOTSTART,                           //!< Hotstart  (all gps datas are preserved).
   WARMSTART,                          //!< Warmstart (only Ephemeris is reset).
   COLDSTART                           //!< Coldstart (all gps information is reset).
}; };

/**
Configuration of the GPS device.
\@attribute[type="serializable"]
*/
struct SGPSDeviceSettings
{
   SGPSDeviceSettings()
      : enSatNavSystem( SatNavSystem::GPS )
      , enGPSMessageFormat( GPSMessageFormat::VICOM_GPS_FORMAT_UBLOX )
      , enResetMode( ResetMode::NONE )
   {}

   /**
   Satellite navigation system type.
   */
   SatNavSystem::Type         enSatNavSystem;

   /**
   GPS message format (protocol type).
   Decoding of GPS messages is not supported for NMEA protocol!
   */
   GPSMessageFormat::Type     enGPSMessageFormat;

   /**
   Reset mode which is applied to the GPS receiver on CViComBasicInteface::StartMeasurement().
   */
   ResetMode::Type            enResetMode;
};


/**
This structure holds the measurement setup that is configured for the connected GPS module. 
It is a result of the CViComGpsInterface::GetSettings() method.
\@attribute[type="serializable"]
*/
struct SSettings
{
   DWORD                    dwReceiverIndex;           //!< Index to SConnectedReceiverTable::Receiver of the currently selected receiver
   SResultBufferDepth       ResultBufferDepth;         //!< Number of results allowed in the result buffer.
   SGPSDeviceSettings       GpsDeviceSettings;         //!< Configuration of the GPS device.
};


/**
Container for messages from / to the GPS device.
\@attribute[type="serializable"]
*/
struct SGPSMessage
{
   /**
   Timestamp in ms of the host.
   */
   DWORD                   dwPcTimeStampInMs;
  
   /**
   Timestamp in ns of the receiver.
   */
   ULONGLONG               ul64DeviceTimeInNs;

   /**
   GPS message format (protocol type).
   */
   GPSMessageFormat::Type  enMessageFormat;
      
   /**
   Length of the message in bytes, i.e. length of the pbmessageText array if pbmessageText is not NULL.
   */
   DWORD                   dwMessageLength;

   /**
   Array of byte with the message content (either ASCII for NMEA or binary for UBLOX format).
   */
   BYTE*                   pbMessageText;
   //!< \@attribute[type="array" size="dwMessageLength" sizeismember="true"]

};

/**
Container for positions on the earth in Geo WGS84 representation.
\@attribute[type="serializable"]
*/
struct SGeoPosition
{
   static const int INVALID_VALUE = -99999; //!< invalid value for longitude, latitude, altitude and time values
      
   //* Default constructor presetting the correct defaults
   SGeoPosition() 
      : dLongitude( static_cast<double>(INVALID_VALUE) )
      , dLatitude( static_cast<double>(INVALID_VALUE) )
      , dAltitude( static_cast<double>(INVALID_VALUE) )
      , dTime( static_cast<double>(INVALID_VALUE) )
      , dGnssTime( static_cast<double>( INVALID_VALUE ) )
      , iLeapSeconds( INVALID_VALUE )
      , bTimeValid( false )
   {}

   /**
   Latitude in WGS84 datum; positive values: North, negative values: South
   */
   double               dLatitude;

   /**
   Longitude in WGS84 datum; positive values: East, negative values: West
   */
   double               dLongitude;

   /**
   Altitude in WGS84 datum in meters above sea level.
   */
   double               dAltitude;

   /**
   Timestamp of WGS84 datum in seconds since midnight (UTC time).
   */
   double               dTime;

   /**
   Timestamp of WGS84 datum, GNSS time of week.
   */
   double               dGnssTime;

   /**
   Leap seconds, e.g. time difference of GNSS time and UTC time. GNSS time is ahead of UTC.
   */
   double               iLeapSeconds;

   /**
   Validity flag for UTC time parameter dTime.
   */
   bool                 bTimeValid;
};

/**
Definition of the position fix types.
\@attribute[type="serializable"]
*/
struct PositionFix { enum Type {
   NONE,                               //!< No fix.
   FIX_2D,                             //!< 2D fix.
   FIX_3D,                             //!< 3D fix.
   TIME_ONLY,                          //!< Time only fix.
   UNKNOWN,                            //!< Unknown.
};};

/**
Definition of the antenna state values.
\@attribute[type="serializable"]
*/
struct AntennaState{ enum Type{
   INIT,               //!< Antenna initiation.
   SHORT,              //!< Antenna port is short circuit.
   OPEN,               //!< Antenna is open / not connected.
   OK,                 //!< Antenna is OK.
   UNKNOWN             //!< Antenna state is unknown.
};};

/**
Definition of the antenna power state values.
\@attribute[type="serializable"]
*/
struct AntennaPowerState{ enum Type{
   OFF,           //!< Power OFF.
   ON,            //!< Power ON.
   UNKNOWN        //!< Power state unknown.
};};

/**
Container for receiver information.
\@attribute[type="serializable"]
*/
struct SReceiverInfo
{
   /**
   Antenna power status.
   */
   AntennaPowerState::Type    enAntennaPowerState;

   /**
   Antenna status.
   */
   AntennaState::Type         enAntennaState;

   /*
   Receiver info text. 
   **/
   char*                      pcReceiverInfoText;
};

/**
Container for satellite information.
\@attribute[type="serializable"]
*/
struct SSatInfo
{
   /**
   Satellite ID.
   */
   BYTE                 bSatID;

   /**
   Signal quality indicator, e.g. Carrier to Noise ratio C/N0 in dB-Hz. Range: 0...99
   */
   BYTE                 bSignalQuality;
};

/**
Container for velocity information.
\@attribute[type="serializable"]
*/
struct SVelocity
{
   /**
   Ground speed in meter per second.
   */
   double               dGroundSpeedInMPerS;

   /**
   Heading of motion in degree (2D).
   */
   double               d2DHeadingInDegree;

   /**
   Speed in meter per second (3D).
   */
   double               d3DSpeedInMPerS;
};

/**
Container for accuracy statistics and information about the position fix.
\@attribute[type="serializable"]
*/
struct SStatistics
{
   /**
   Horizontal dilution of precision.
   */
   float                fhDOP;

   /**
   Vertical dilution of precision.
   */
   float                fvDOP;

   /**
   Position dilution of precision.
   */
   float                fpDOP;

   /**
   2D position accuracy estimate in meter.
   */
   float                f2DPositionAccuracyEstimate;

   /**
   3D position accuracy estimate in meter.
   */
   float                f3DPositionAccuracyEstimate;

   /**
   3D speed accuracy estimate in meter per second.
   */
   float                f3DSpeedAccuracyEstimate;

   /**
   3D speed accuracy estimate in meter per second.
   */
   PositionFix::Type    enPositionFix;

   /**
   Number of satellites used for position fix.
   */
   BYTE                 bUsedSatellites;
};

/**
GPS result returned by CViComGpsInterface::GetResult().
\@attribute[type="serializable"]
*/
struct SMeasResult
{
   /**
   PC local time when the data has been processed on the computer.
   */
   DWORD                   dwPcTimeStampInMs;

   /**
   List of raw messages contributing to the meas result.
   \@attribute[filterable="true"]
   */
   SViComList<SGPSMessage> ListOfMessages;

   /**
   Position information in WGS84 representation.
   */
   SGeoPosition            sPosition;

   /**
   List of information about visible satellites.
   */
   SViComList<SSatInfo>    ListOfSatInfo;

   /**
   Velocity information.
   */
   SVelocity               sVelocity;

   /**
   Accuracy statistics and information about the position fix.
   */
   SStatistics             sStatistics;

   /**
   Information about the GPS receiver.
   */
   SReceiverInfo           sReceiverInfo;
};
  
} // namespace GPS
} // namespace ViCom
} // namespace RohdeSchwarz

#pragma pack(pop) // reset your structure alignment

#endif //VICOM_RESOURCE_INCLUDE
