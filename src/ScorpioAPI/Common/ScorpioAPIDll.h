/**************************************************************************
*                           CONFIDENTIAL                                  *
* Unauthorized access to, copying, use of or disclosure of this software, *
* or any of its features, is strictly prohibited. Your access to or       *
* possession of a copy of this software is persuant to a limited license; *
* ownership of the software and any associated media remains with TCI.    *
*                                                                         *
* Copyright 2021-2021 TCI International, Inc. All rights reserved         *
**************************************************************************/
#pragma once
//typedef double DATE;

#include <string>
#include <vector>
//#include "GeoServerTypes.h"
#include "EquipCtrlMsg.h"
#include "SmsRealtimeMsg.h"
#include "Units.h"
#include "ScorpioAPITypes.h"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCORPIO_APIDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCORPIOAPICLIENTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef SCORPIO_APIDLL_EXPORTS
#define SCORPIO_APIDLL __declspec(dllexport)
#else
#define SCORPIO_APIDLL __declspec(dllimport)
#endif

#define NETTED_SOURCE 5000

//extern "C"
//{

//}

// Callbacks
//typedef void(*BistCB) (_In_  unsigned long serverId, _In_ const SScorpioAPIText& text);
typedef void(*DataCB)(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, unsigned long destAddr, _In_ SEquipCtrlMsg::UBody* data);
typedef void(*ErrorCB)(_In_  unsigned long serverId, _In_ const std::wstring& errorMsg);
typedef void(*RealTimeDataCB)(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data);

//obsolete, used with virtual function callback
/*typedef void(*OnBist) (_In_ const SScorpioAPIText& text);
typedef void(*OnError)(_In_ const std::wstring& errorMsg);
typedef void(*OnData)(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
typedef void(*OnRealTimeData)(_In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data);
*/

extern "C"
{
	// Connect/Disconnect
	SCORPIO_APIDLL ERetCode ScorpioAPICreate(unsigned long & serverId, const SScorpioAPIClient& params,
		ErrorCB OnErrFunc, DataCB OnDataFunc, RealTimeDataCB OnRealtimeDataFunc);
	SCORPIO_APIDLL ERetCode Connect(unsigned long & serverId, const SScorpioAPIClient& params,
		ErrorCB OnErrFunc, DataCB OnDataFunc, RealTimeDataCB OnRealtimeDataFunc);
	SCORPIO_APIDLL ERetCode Disconnect(_In_ unsigned long serverId);

	// Handle tasks
	SCORPIO_APIDLL ERetCode TerminateTask(_In_ unsigned long serverId, _In_ unsigned long reqID);
	SCORPIO_APIDLL ERetCode SuspendTask(_In_ unsigned long serverId, ECSMSDllMsgType taskType, _In_ unsigned long reqID);
	SCORPIO_APIDLL ERetCode ResumeTask(_In_ unsigned long serverId, ECSMSDllMsgType taskType, _In_ unsigned long reqID);
	SCORPIO_APIDLL ERetCode RequestTaskStatus(_In_ unsigned long serverId, _In_ unsigned long reqID);
	SCORPIO_APIDLL ERetCode RequestTaskState(_In_ unsigned long serverId, ECSMSDllMsgType taskType, _In_ unsigned long reqID);
	// Measuremements
	SCORPIO_APIDLL ERetCode RequestOccupancy(_In_ unsigned long serverId, _In_ SOccupReqData * occupancyParams, _Inout_ unsigned long* reqID);
	SCORPIO_APIDLL ERetCode RequestOccupancyDF(_In_ unsigned long serverId, _In_ SOccDFReqData * occupancyParams, _Inout_ unsigned long* reqID);
	SCORPIO_APIDLL ERetCode RequestAVD(_In_ unsigned long serverId, _In_ SAVDReqData * avdParams, _Inout_ unsigned long* reqID);
	SCORPIO_APIDLL ERetCode RequestMeasurement(_In_ unsigned long serverId, _In_ SMeasReqData * measParams, _Inout_ unsigned long* reqID);
	SCORPIO_APIDLL ERetCode RequestBist(_In_ unsigned long serverId, _In_ EBistScope scope, _Inout_  unsigned long * reqID);

	// capabiliities
	SCORPIO_APIDLL ERetCode RequestCapabilities(_In_ unsigned long serverId, _Out_ SCapabilities& capabilities);
	SCORPIO_APIDLL ERetCode RequestAntennaListInfo(_In_ unsigned long serverId, _Inout_ unsigned long* reqID);

	SCORPIO_APIDLL ERetCode SetPanParams(_In_ unsigned long serverId, _In_  SPanParams panParams, _Inout_ unsigned long* reqID);
	SCORPIO_APIDLL ERetCode RequestPan(_In_ unsigned long serverId, _In_ SGetPanParams panParams, _Inout_ unsigned long* reqID);

	SCORPIO_APIDLL ERetCode SetAudio(_In_ unsigned long serverId, _In_ SAudioParams audioParams, _Inout_ unsigned long* reqID);
	SCORPIO_APIDLL ERetCode FreeAudio(_In_ unsigned long serverId, _In_ unsigned long channel, _Inout_ unsigned long* reqID);


	SCORPIO_APIDLL ERetCode RequestRealTime(_In_ unsigned long serverId, _In_ bool on, _Inout_ unsigned long* reqID);

	//SCORPIOAPICLIENTDLL_API ERetCode ValidateOccupancy(_In_ unsigned long serverId, _In_ SOccupReqData* occupancyParams);
	//SCORPIOAPICLIENTDLL_API ERetCode RequestTaskInfo(_In_ unsigned long reqID, _Out_ unsigned long taskId, _Out_ unsigned long taskKey);

}
