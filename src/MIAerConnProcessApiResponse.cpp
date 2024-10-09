#include "MIAerConnProcessApiResponse.h"

std::string processBITEResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* respdata)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);
	if (respType == ECSMSDllMsgType::GET_BIST ||
		respType == ECSMSDllMsgType::GET_BIST_RESULT)
	{
		SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
		jsonObj["SGetBistResp"]["last"] = BITEResponse->last;
		jsonObj["SGetBistResp"]["result"] = BITEResponse->result;
		jsonObj["SGetBistResp"]["text"] = BITEResponse->text;
		jsonObj["SGetBistResp"]["textLen"] = BITEResponse->textLen;
	}
	else if (respType == ECSMSDllMsgType::GET_DIAGNOSTICS)
	{
		SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
		jsonObj["SGetBistResp"]["last"] = BITEResponse->last;
		jsonObj["SGetBistResp"]["result"] = BITEResponse->result;
		jsonObj["SGetBistResp"]["text"] = BITEResponse->text;
		jsonObj["SGetBistResp"]["textLen"] = BITEResponse->textLen;
	}
	else
	{
	}
	return jsonObj.dump();

}
std::string ProcessAntListResponse(_In_ SEquipCtrlMsg::UBody* data)
{
	SEquipCtrlMsg::SAntInfoListResp* antListResponse = (SEquipCtrlMsg::SAntInfoListResp*)data;
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	jsonObj["SAntInfoListResp"]["ant"]["ant"] = antListResponse->ant->ant;
	jsonObj["SAntInfoListResp"]["ant"]["antName"] = antListResponse->ant->antName;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthStart"] = antListResponse->ant->azimuthStart;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthStep"] = antListResponse->ant->azimuthStep;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthStop"] = antListResponse->ant->azimuthStop;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthTolerance"] = antListResponse->ant->azimuthTolerance;
	jsonObj["SAntInfoListResp"]["ant"]["aziOrientation"] = antListResponse->ant->aziOrientation;
	jsonObj["SAntInfoListResp"]["ant"]["elevationStart"] = antListResponse->ant->elevationStart;
	jsonObj["SAntInfoListResp"]["ant"]["elevationStep"] = antListResponse->ant->elevationStep;
	jsonObj["SAntInfoListResp"]["ant"]["elevationStop"] = antListResponse->ant->elevationStop;
	jsonObj["SAntInfoListResp"]["ant"]["elevationTolerance"] = antListResponse->ant->elevationTolerance;
	jsonObj["SAntInfoListResp"]["ant"]["highFreq"]["internal"] = antListResponse->ant->highFreq.internal;
	jsonObj["SAntInfoListResp"]["ant"]["lowFreq"]["internal"] = antListResponse->ant->lowFreq.internal;
	jsonObj["SAntInfoListResp"]["ant"]["polarization"] = antListResponse->ant->polarization;
	jsonObj["SAntInfoListResp"]["ant"]["rotatorId"] = antListResponse->ant->rotatorId;
	jsonObj["SAntInfoListResp"]["numAnt"] = antListResponse->numAnt;

	return jsonObj.dump();
}
std::string processAutoViolateResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	SEquipCtrlMsg::SOccupancyHeader* pOccHdr = nullptr;
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	if (respType == ECSMSDllMsgType::AVD_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE)
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = Response->completionTime;
		jsonObj["SStateResp"]["state"] = Response->state;
	}
	else if (respType == ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL)
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
		jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = Response->frequencies->internal;
		jsonObj["SFrequencyVsChannelResp"]["hostName"] = Response->hostName;
		jsonObj["SFrequencyVsChannelResp"]["numBands"] = Response->numBands;
		jsonObj["SFrequencyVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = Response->occPrimaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = Response->occSecondaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = Response->saveIntermediateData;
		jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = Response->selectedAntenna;
		jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = Response->useSecondaryThreshold;
	}
	else if (respType == ECSMSDllMsgType::AVD_FREQ_MEAS)
	{
		SEquipCtrlMsg::SAvdMeasureResult* Response = (SEquipCtrlMsg::SAvdMeasureResult*)data;
		SSmsMsg::SAvdMeasureResultV1::SMeasureData arrayMeasData[1000];
		// TODO - montar array de measData
		//jsonObj["SAvdMeasureResult"]["measData"] = Response->measData;
		jsonObj["SAvdMeasureResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["status"] = Response->occHdr.status;
	}
	else if (respType == ECSMSDllMsgType::AVD_BW_MEAS)
	{
		SEquipCtrlMsg::SAvdMeasureResult* Response = (SEquipCtrlMsg::SAvdMeasureResult*)data;
		// TODO - montar array de measData
		//jsonObj["SAvdMeasureResult"]["measData"] = Response->measData;
		jsonObj["SAvdMeasureResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["status"] = Response->occHdr.status;
	}
	else if (respType == ECSMSDllMsgType::AVD_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;

	}
	else if (respType == ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"]["avg"] = Response->resultData->avg;
		jsonObj["SOccResult"]["resultData"]["avg"] = Response->resultData->max;
	}
	else	// not AUTOVIOLATE_RESPONSE
	{
		//tempstr.Format(_T("unexpected AVD message %u"),
			//respType);
	}

	return jsonObj.dump();
}
std::string processMeasResponse(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	if (respType == ECSMSDllMsgType::VALIDATE_MEAS)
	{
		SEquipCtrlMsg::SValidateMeasurementResp* DwellResponse = (SEquipCtrlMsg::SValidateMeasurementResp*)data;
		jsonObj["SValidateMeasurementResp"]["bwDwellTime"] = DwellResponse->bwDwellTime;
		jsonObj["SValidateMeasurementResp"]["dfDwellTime"] = DwellResponse->dfDwellTime;
		jsonObj["SValidateMeasurementResp"]["fieldStrengthDwellTime"] = DwellResponse->fieldStrengthDwellTime;
		jsonObj["SValidateMeasurementResp"]["freqDwellTime"] = DwellResponse->freqDwellTime;
		jsonObj["SValidateMeasurementResp"]["modulationDwellTime"] = DwellResponse->modulationDwellTime;
		jsonObj["SValidateMeasurementResp"]["status"] = DwellResponse->status;
		jsonObj["SValidateMeasurementResp"]["totalTime"] = DwellResponse->totalTime;
	}
	else if (respType == ECSMSDllMsgType::GET_MEAS)
	{
		SEquipCtrlMsg::SGetMeasResp* MeasResponse = (SEquipCtrlMsg::SGetMeasResp*)data;
		jsonObj["SGetMeasResp"]["amProbDist"] = MeasResponse->amProbDist;
		jsonObj["SGetMeasResp"]["bwBinData"] = MeasResponse->bwBinData;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaBw"] = MeasResponse->bwResponse.betaBw;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaFreqHigh"] = MeasResponse->bwResponse.betaFreqHigh;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaFreqLow"] = MeasResponse->bwResponse.betaFreqLow;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaStdDev"] = MeasResponse->bwResponse.betaStdDev;
		jsonObj["SGetMeasResp"]["bwResponse"]["binSize"]["internal"] = MeasResponse->bwResponse.binSize.internal;
		jsonObj["SGetMeasResp"]["bwResponse"]["freq"]["internal"] = MeasResponse->bwResponse.freq.internal;
		jsonObj["SGetMeasResp"]["bwResponse"]["numBins"] = MeasResponse->bwResponse.numBins;
		jsonObj["SGetMeasResp"]["bwResponse"]["repeatCount"] = MeasResponse->bwResponse.repeatCount;
		jsonObj["SGetMeasResp"]["bwResponse"]["status"] = MeasResponse->bwResponse.status;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1Bw"] = MeasResponse->bwResponse.x1Bw;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1FreqHigh"] = MeasResponse->bwResponse.x1FreqHigh;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1FreqLow"] = MeasResponse->bwResponse.x1FreqLow;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1StdDev"] = MeasResponse->bwResponse.x1StdDev;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2Bw"] = MeasResponse->bwResponse.x2Bw;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2FreqHigh"] = MeasResponse->bwResponse.x2FreqHigh;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2FreqLow"] = MeasResponse->bwResponse.x2FreqLow;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2StdDev"] = MeasResponse->bwResponse.x2StdDev;
	}
	return jsonObj.dump();
}
std::string processDemodCtrlResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	if (respType == ECSMSDllMsgType::SET_RCVR)
	{
		SEquipCtrlMsg::SGenericResp* RcvrResponse = (SEquipCtrlMsg::SGenericResp*)data;
		jsonObj["SGenericResp"]["status"] = RcvrResponse->status;
	}
	else if (respType == ECSMSDllMsgType::SET_PAN_PARAMS)
	{
		SEquipCtrlMsg::SGenericResp* PanParaResponse = (SEquipCtrlMsg::SGenericResp*)data;
		jsonObj["SGenericResp"]["status"] = PanParaResponse->status;
	}
	else if (respType == ECSMSDllMsgType::SET_AUDIO_PARAMS)
	{
		SEquipCtrlMsg::SAudioParamsResp* AudioResponse = (SEquipCtrlMsg::SAudioParamsResp*)data;
		jsonObj["SAudioParamsResp"]["channel"] = AudioResponse->channel;
		jsonObj["SAudioParamsResp"]["ipAddress"] = AudioResponse->ipAddress;
		jsonObj["SAudioParamsResp"]["status"] = AudioResponse->status;
	}
	else if (respType == ECSMSDllMsgType::FREE_AUDIO_CHANNEL)
	{
		SEquipCtrlMsg::SGenericResp* FreeAudioResponse = (SEquipCtrlMsg::SGenericResp*)data;
		jsonObj["SGenericResp"]["status"] = FreeAudioResponse->status;
	}
	else
	{
		//tempstr.Format(_T("unexpected Demod Control message subType %lu"),
		//	m_Response.hdr.msgSubType);
	}
	return jsonObj.dump();
}
std::string processPanResponse(_In_ SEquipCtrlMsg::UBody* data)
{
	//error codes:
	// 0 = no error
	// 1 = server timeout
	// 2 = Carrier signal not detected
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);
	SEquipCtrlMsg::SGetPanResp* PanResponse = (SEquipCtrlMsg::SGetPanResp*)data;
	for (size_t i = 0; i < PanResponse->nActiveAudioChannels; ++i) {
		jsonObj["SGetPanResp"]["audioPower"][i]["powerdBm"] = PanResponse->audioPower[i].powerdBm;
	}
	jsonObj["SGetPanResp"]["binData"] = std::string(reinterpret_cast<char*>(PanResponse->binData), PanResponse->numBins);
	jsonObj["SGetPanResp"]["audioPower"]["active"] = PanResponse->audioPower->active;
	jsonObj["SGetPanResp"]["binSize"]["internal"] = PanResponse->binSize.internal;
	jsonObj["SGetPanResp"]["conversionFactorForFS"] = PanResponse->conversionFactorForFS;
	jsonObj["SGetPanResp"]["dateTime"] = PanResponse->dateTime;
	jsonObj["SGetPanResp"]["freq"]["internal"] = PanResponse->freq.internal;
	jsonObj["SGetPanResp"]["nActiveAudioChannels"] = PanResponse->nActiveAudioChannels;
	jsonObj["SGetPanResp"]["numBins"] = PanResponse->numBins;
	jsonObj["SGetPanResp"]["powerDbm"] = PanResponse->powerDbm;
	jsonObj["SGetPanResp"]["rcvrAtten"] = PanResponse->rcvrAtten;
	jsonObj["SGetPanResp"]["status"] = PanResponse->status;
	return jsonObj.dump();
}
std::string processOccupancyResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);
	if (respType == ECSMSDllMsgType::OCC_MSGLEN_DIST_RESPONSE)
	{
		SEquipCtrlMsg::SMsgLengthDistributionResp* Response = (SEquipCtrlMsg::SMsgLengthDistributionResp*)data;
		jsonObj["SMsgLengthDistributionResp"]["histData"]["channel"] = Response->histData->channel;
		jsonObj["SMsgLengthDistributionResp"]["histData"]["length"] = Response->histData->length;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
	}
	else if (respType == ECSMSDllMsgType::OCC_SPECTRUM_RESPONSE)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	else if (respType == ECSMSDllMsgType::OCC_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::OCC_SOLICIT_STATE_RESPONSE)
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = (unsigned long)Response->completionTime;
		jsonObj["SStateResp"]["state"] = (unsigned long)Response->state;
	}
	else if (respType == ECSMSDllMsgType::OCC_FREQ_VS_CHANNEL)
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
		jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = Response->frequencies->internal;

		jsonObj["SFrequencyVsChannelResp"]["hostName"] = Response->hostName;
		jsonObj["SFrequencyVsChannelResp"]["numBands"] = Response->numBands;
		jsonObj["SFrequencyVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = Response->occPrimaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = Response->occSecondaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = Response->saveIntermediateData;
		jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = Response->selectedAntenna;
		jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = Response->useSecondaryThreshold;

	}
	else if (respType == ECSMSDllMsgType::OCC_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	else if (respType == ECSMSDllMsgType::OCC_EFLD_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	else if (respType == ECSMSDllMsgType::OCC_TIMEOFDAY_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	else if (respType == ECSMSDllMsgType::OCC_MSGLEN_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	else if (respType == ECSMSDllMsgType::OCC_EFLD_TIMEOFDAY_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	else if (respType == ECSMSDllMsgType::OCC_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;
	}
	else if (respType == ECSMSDllMsgType::VALIDATE_OCCUPANCY)
	{
		SEquipCtrlMsg::SValidateOccupancyResp* Response = (SEquipCtrlMsg::SValidateOccupancyResp*)data;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["ant"] = Response->occCmd.ant;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["channelBandwidth"]["internal"] = Response->occCmd.band[0].channelBandwidth.internal;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["exclude"] = Response->occCmd.band[0].exclude;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["highFrequency"]["internal"] = Response->occCmd.band[0].highFrequency.internal;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["lowFrequency"]["internal"] = Response->occCmd.band[0].lowFrequency.internal;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["horizPol"] = (unsigned long)Response->occCmd.band[0].sType.signalType.horizPol;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["narrow"] = (unsigned long)Response->occCmd.band[0].sType.signalType.narrow;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["unused0"] = (unsigned long)Response->occCmd.band[0].sType.signalType.unused0;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["unused1"] = (unsigned long)Response->occCmd.band[0].sType.signalType.unused1;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["confidenceLevel"] = Response->occCmd.confidenceLevel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["desiredAccuracy"] = Response->occCmd.desiredAccuracy;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["durationMethod"] = Response->occCmd.durationMethod;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["measurementTime"] = Response->occCmd.measurementTime;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["numBands"] = Response->occCmd.numBands;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["occPrimaryThreshold"] = Response->occCmd.occPrimaryThreshold;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["occSecondaryThreshold"] = Response->occCmd.occSecondaryThreshold;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["occupancyMinGap"] = Response->occCmd.occupancyMinGap;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["eFieldVsChannel"] = Response->occCmd.output.eFieldVsChannel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["msglengthDistribution"] = Response->occCmd.output.msglengthDistribution;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["msglengthVsChannel"] = Response->occCmd.output.msglengthVsChannel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["adjustAgcThres"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.adjustAgcThres;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["eFieldVsTOD"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.eFieldVsTOD;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["gainMode"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.gainMode;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["spurTest"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.spurTest;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["unused"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.unused;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["zeroFlag"] = (unsigned char)Response->occCmd.output.occupancyFlag.zeroFlag;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyVsChannel"] = (unsigned char)Response->occCmd.output.occupancyVsChannel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyVsTimeOfDay"] = (unsigned char)Response->occCmd.output.occupancyVsTimeOfDay;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["spectrogram"] = (unsigned char)Response->occCmd.output.spectrogram;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["timegram"] = (unsigned char)Response->occCmd.output.timegram;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["saveIntermediateData"] = Response->occCmd.saveIntermediateData;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["storageTime"] = Response->occCmd.storageTime;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["thresholdMethod"] = Response->occCmd.thresholdMethod;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["useSecondaryThreshold"] = Response->occCmd.useSecondaryThreshold;
		jsonObj["SValidateOccupancyResp"]["status"] = Response->status;

	}
	else	// not OCCUPANCY_RESPONSE
	{
		//tempstr.Format(_T("unexpected occupancy message %u"),
			//respType);
	}
	return jsonObj.dump();
}
std::string processOccupancyDFResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);

	if (respType == ECSMSDllMsgType::OCCDF_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE)
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = Response->completionTime;
		jsonObj["SStateResp"]["state"] = Response->state;
	}
	else if (respType == ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL)
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
		jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = Response->frequencies->internal;

		jsonObj["SFrequencyVsChannelResp"]["hostName"] = Response->hostName;
		jsonObj["SFrequencyVsChannelResp"]["numBands"] = Response->numBands;
		jsonObj["SFrequencyVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = Response->occPrimaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = Response->occSecondaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = Response->saveIntermediateData;
		jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = Response->selectedAntenna;
		jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = Response->useSecondaryThreshold;
	}
	else if (respType == ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL)
	{
		SEquipCtrlMsg::SScanDfVsChannelResp* Response = (SEquipCtrlMsg::SScanDfVsChannelResp*)data;
		jsonObj["SScanDfVsChannelResp"]["aveFldStr"] = Response->aveFldStr;
		jsonObj["SScanDfVsChannelResp"]["aveRange"] = Response->aveRange;
		jsonObj["SScanDfVsChannelResp"]["numAzimuths"] = Response->numAzimuths;
		jsonObj["SScanDfVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SScanDfVsChannelResp"]["scanDfData"] = Response->scanDfData;
	}
	else if (respType == ECSMSDllMsgType::OCCDF_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;
	}
	else	// not OCCUPANCYDF_RESPONSE
	{
		//tempstr.Format(_T("unexpected occupancyDF message %u"),
		//	respType);
	}
	return jsonObj.dump();
}
std::string ProcessRealTimeData(_In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);
	switch (respType)
	{
	case ECSMSDllMsgType::RT_SPECTRUM_START:
	{
		const SSmsRealtimeMsg::SStartV2* pData = (SSmsRealtimeMsg::SStartV2*)data;
		jsonObj["SStartV2"]["band"]["chanSize"]["internal"] = pData->band->chanSize.internal;
		jsonObj["SStartV2"]["band"]["firstChanFreq"]["internal"] = pData->band->firstChanFreq.internal;
		jsonObj["SStartV2"]["band"]["numChan"] = pData->band->numChan;
		jsonObj["SStartV2"]["numBands"] = pData->numBands;
		jsonObj["SStartV2"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_STOP:
	{
		const SSmsRealtimeMsg::SStop* pData = (const SSmsRealtimeMsg::SStop*)data;
		jsonObj["SStop"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_V1RESPONSE:
	{
		const SSmsRealtimeMsg::SSpectrum* pData = (SSmsRealtimeMsg::SSpectrum*)data;
		jsonObj["SSpectrum"]["bandIndex"] = pData->bandIndex;
		jsonObj["SSpectrum"]["chanData"] = pData->chanData;
		jsonObj["SSpectrum"]["chanSize"] = pData->chanSize;
		jsonObj["SSpectrum"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SSpectrum"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SSpectrum"]["numChan"] = pData->numChan;
		jsonObj["SSpectrum"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_V2RESPONSE:
	{
		const SSmsRealtimeMsg::SSpectrumV2* pData = (SSmsRealtimeMsg::SSpectrumV2*)data;
		jsonObj["SSpectrumV2"]["bandIndex"] = pData->bandIndex;
		jsonObj["SSpectrumV2"]["chanData"] = pData->chanData;
		jsonObj["SSpectrumV2"]["chanSize"]["internal"] = pData->chanSize.internal;
		jsonObj["SSpectrumV2"]["firstChanFreq"]["internal"] = pData->firstChanFreq.internal;
		jsonObj["SSpectrumV2"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SSpectrumV2"]["numChan"] = pData->numChan;
		jsonObj["SSpectrumV2"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_RESPONSE:
	{
		const SSmsRealtimeMsg::SSpectrumV3* pData = (SSmsRealtimeMsg::SSpectrumV3*)data;
		jsonObj["SSpectrumV3"]["bandIndex"] = pData->bandIndex;
		jsonObj["SSpectrumV3"]["chanData"] = pData->chanData;
		jsonObj["SSpectrumV3"]["chanSize"]["internal"] = pData->chanSize.internal;
		jsonObj["SSpectrumV3"]["firstChanFreq"]["internal"] = pData->firstChanFreq.internal;
		jsonObj["SSpectrumV3"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SSpectrumV3"]["numChan"] = pData->numChan;
		jsonObj["SSpectrumV3"]["taskId"] = pData->taskId;
		jsonObj["SSpectrumV3"]["efield"] = pData->efield;
		jsonObj["SSpectrumV3"]["zeroVal"] = pData->zeroVal;
		break;

	}
	case ECSMSDllMsgType::RT_DF_START:
	{
		const SSmsRealtimeMsg::SStartV2* pData = (SSmsRealtimeMsg::SStartV2*)data;
		jsonObj["SStartV2"]["band"]["chanSize"]["internal"] = pData->band->chanSize.internal;
		jsonObj["SStartV2"]["band"]["firstChanFreq"]["internal"] = pData->band->firstChanFreq.internal;
		jsonObj["SStartV2"]["band"]["numChan"] = pData->band->numChan;
		jsonObj["SStartV2"]["numBands"] = pData->numBands;
		jsonObj["SStartV2"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_STARTV1:
	{
		const SSmsRealtimeMsg::SStart* pData = (SSmsRealtimeMsg::SStart*)data;
		jsonObj["SStart"]["chanSize"] = pData->chanSize;
		jsonObj["SStart"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SStart"]["MAX_OCCBANDS"] = pData->MAX_OCCBANDS;
		jsonObj["SStart"]["numBands"] = pData->numBands;
		jsonObj["SStart"]["numChan"] = pData->numChan;
		jsonObj["SStart"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_STOP:
	{
		const SSmsRealtimeMsg::SStop* pData = (SSmsRealtimeMsg::SStop*)data;
		jsonObj["SStop"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_DATAV1:
	{
		const SSmsRealtimeMsg::SDfData* pData = (SSmsRealtimeMsg::SDfData*)data;
		jsonObj["SDfData"]["bandIndex"] = pData->bandIndex;
		jsonObj["SDfData"]["chanData"]["azimData"] = pData->chanData->azimData;
		jsonObj["SDfData"]["chanData"]["specData"] = pData->chanData->specData;
		jsonObj["SDfData"]["chanSize"] = pData->chanSize;
		jsonObj["SDfData"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SDfData"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SDfData"]["numChan"] = pData->numChan;
		jsonObj["SDfData"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_DATAV2:
	{
		const SSmsRealtimeMsg::SDfDataV2* pData = (SSmsRealtimeMsg::SDfDataV2*)data;
		jsonObj["SDfDataV2"]["bandIndex"] = pData->bandIndex;
		jsonObj["SDfDataV2"]["chanData"]["azimData"] = pData->chanData->azimData;
		jsonObj["SDfDataV2"]["chanData"]["specData"] = pData->chanData->specData;
		jsonObj["SDfDataV2"]["chanSize"] = pData->chanSize;
		jsonObj["SDfDataV2"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SDfDataV2"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SDfDataV2"]["numChan"] = pData->numChan;
		jsonObj["SDfDataV2"]["taskId"] = pData->taskId;
		jsonObj["SDfDataV2"]["horizPol"] = pData->horizPol;
		break;
	}
	case ECSMSDllMsgType::RT_DF_DATA:
	{
		const SSmsRealtimeMsg::SDfDataV3* pData = (SSmsRealtimeMsg::SDfDataV3*)data;
		jsonObj["SDfDataV3"]["bandIndex"] = pData->bandIndex;
		jsonObj["SDfDataV3"]["chanData"]["azimData"] = pData->chanData->azimData;
		jsonObj["SDfDataV3"]["chanData"]["specData"] = pData->chanData->specData;
		jsonObj["SDfDataV3"]["chanSize"]["internal"] = pData->chanSize.internal;
		jsonObj["SDfDataV3"]["firstChanFreq"]["internal"] = pData->firstChanFreq.internal;
		jsonObj["SDfDataV3"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SDfDataV3"]["numChan"] = pData->numChan;
		jsonObj["SDfDataV3"]["taskId"] = pData->taskId;
		jsonObj["SDfDataV3"]["horizPol"] = pData->horizPol;
		break;
	}
	case ECSMSDllMsgType::RT_IQ_DATA:
	{
		SSmsRealtimeMsg::SIqDataV4* iqData = (SSmsRealtimeMsg::SIqDataV4*)data; // v5 should be sent to rds app
		jsonObj["SIqDataV4"]["actualBW"]["internal"] = iqData->actualBW.internal;
		jsonObj["SIqDataV4"]["actualSampleRate"] = iqData->actualSampleRate;
		jsonObj["SIqDataV4"]["dataType"] = iqData->dataType;
		jsonObj["SIqDataV4"]["ddcChannel"] = iqData->ddcChannel;
		jsonObj["SIqDataV4"]["EOS"] = iqData->EOS;
		jsonObj["SIqDataV4"]["freq"]["internal"] = iqData->freq.internal;
		jsonObj["SIqDataV4"]["inputPort"] = iqData->inputPort;
		jsonObj["SIqDataV4"]["MAX_SAMPLES"] = iqData->MAX_SAMPLES;
		jsonObj["SIqDataV4"]["numSamples"] = iqData->numSamples;
		jsonObj["SIqDataV4"]["rxAtten"] = iqData->rxAtten;
		jsonObj["SIqDataV4"]["sampleOffset"] = iqData->sampleOffset;
		jsonObj["SIqDataV4"]["scaleFactor"] = iqData->scaleFactor;
		jsonObj["SIqDataV4"]["seqNumber"] = iqData->seqNumber;
		jsonObj["SIqDataV4"]["SIq"]["samplesFloat32"]["im"] = iqData->SIq.samplesFloat32->im;
		jsonObj["SIqDataV4"]["SIq"]["samplesFloat32"]["re"] = iqData->SIq.samplesFloat32->re;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt16"]["im"] = iqData->SIq.samplesInt16->im;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt16"]["re"] = iqData->SIq.samplesInt16->re;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt32"]["im"] = iqData->SIq.samplesInt32->im;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt32"]["re"] = iqData->SIq.samplesInt32->re;
		jsonObj["SIqDataV4"]["streamID"] = iqData->streamID;
		jsonObj["SIqDataV4"]["streamStartTime"]["timestamp"] = iqData->streamStartTime.timestamp;

		break;
	}
	default:
	{
		break;
	}
	}
	return jsonObj.dump();
}