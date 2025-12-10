/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2005 TCI. ALL RIGHTS RESERVED                                 *
**************************************************************************/
#ifndef GPSFORMATS_H
#define GPSFORMATS_H

typedef struct
{
    unsigned SatLock  : 1;    // 0..1   (1 bit)  = L,U
    unsigned Accuracy : 2;    // 0..3   (2 bits) = A,B,C,D
	unsigned Mode	  : 2;    // 0..3	(2 bits) = T,A,S,D
	unsigned NumSats  : 3;	  // 0..6   (3 bits) = 0,1,...,6
	unsigned TimSrce  : 2;    // 0..3	(2 bits) = G,F,I,N
    unsigned PLLSynth : 1;    // 0..1   (1 bit)	 = -,P
    unsigned Antenna  : 2;    // 0..2   (2 bits) = -,O,S
	unsigned Receiver : 1;	  // 0..1	(1 bit)  = -,R
	unsigned NVRam    : 1;    // 0..1	(1 bit)  = -,N
	unsigned TimErr1  : 1;    // 0..1	(1 bit)  = -,U
	unsigned TimErr2  : 1;    // 0..1	(1 bit)  = -,U
	unsigned Tracking : 1;    // 0..1	(1 bit)  = -,T
	unsigned OscVolt  : 1;    // 0..1	(1 bit)  = -,X
	unsigned BatVolt  : 1;    // 0..1	(1 bit)  = -,B
	unsigned LockHist : 2;    // 0..2	(2 bits) = -,a,A
} GPS_STATUS;

char getChar(int pos, char* string);
int CharPos(char* string, char c);
void statusFormat(long GPSStatus, char* string);
int stringToLatLon(char* in_str, double& latitude, double& longitude);
void latlonFormat(double latitude, double longitude, char* string);

#endif