#include "EtherDLLUtils.h"

// Core processing functions
std::string processBITEResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* respdata);
std::string ProcessAntListResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
std::string processAutoViolateResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
std::string processMeasResponse(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ SEquipCtrlMsg::UBody* data);
std::string processDemodCtrlResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
std::string processPanResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
std::string processOccupancyResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
std::string processOccupancyDFResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
std::string ProcessRealTimeData(_In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data);

// Anxilary functions
nlohmann::json ProcessGpsData(SEquipCtrlMsg::SGpsResponse* gpsResponse);
int ScanDataExpand(int ninput, int* input, int noutput, int* output);
static const unsigned char* parsedBinData(const unsigned char* binData, unsigned short numBins);
std::string oleTimeToIsoFmt(double oleTime);

// Spectrum calculation utility
struct SpectrumInfo {
    double startFrequency;  // MHz
    double stopFrequency;   // MHz  
    double binSize;         // Hz
};

SpectrumInfo calculateSpectrumInfo(const SEquipCtrlMsg::SGetPanResp* panResponse);