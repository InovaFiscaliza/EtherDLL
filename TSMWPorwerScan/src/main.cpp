#include "stdafx.h"
#include <atlstr.h>
#include "ViComRFPowerScanInterface.h"
#include "ViComRFPowerScanInterfaceData.h"
#include "ViComBasicInterface.h"
#include "ViComBasicInterfaceData.h"
#include "ViComLoader.h"

using namespace RohdeSchwarz::ViCom;
using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;

int _tmain(int argc, _TCHAR* argv[])
{

	try
	{
		// Procedimento para inicialiazar
		CViComError				err;
		bool						bLoaded;
		CString					ipAddressString;
		Receiver::Type receptor = Receiver::TSMW;

		// Atribuição do endereço IP do instrumento
		ipAddressString = _T("192.168.0.2");

		// Criação da interface
		CViComLoader<CViComRFPowerScanInterface> tsmwIFLoader(receptor);

		// Connect available receivers.

		bLoaded = tsmwIFLoader.Connect(err, ipAddressString);

		if (!bLoaded)
		{
			std::cout << "No receiver found." << std::endl;
			return 0;
		}
		else
		{
			std::cout << "TSMW conectado." << std::endl;
			CViComRFPowerScanInterface* myIF = tsmwIFLoader.GetInterface(err);
			if (myIF != NULL)
			{
				SSweepSettings sweepSettings;
				sweepSettings.dwFrontEndSelectionMask = 1;
				sweepSettings.dStartFrequencyInHz = 87.8e6;
				sweepSettings.dStopFrequencyInHz = 107.8e6;
				sweepSettings.bRequestRawData = FALSE;

				sweepSettings.sSpectrumSettings.fMaxReportingRateInHz = 10.0f;
				sweepSettings.sSpectrumSettings.fMaxDeviceMeasRateInHz = 10.0f;
				sweepSettings.sSpectrumSettings.eWindowType = SSpectrumSettings::RFPOWERSCAN_WINDOWTYPE_HANNING;
				sweepSettings.sSpectrumSettings.eFFTSize = SSpectrumSettings::RFPOWERSCAN_FFTSIZE_1024;
				sweepSettings.sSpectrumSettings.bAutoBandwidth = TRUE;
				sweepSettings.sSpectrumSettings.dwBandwidthInHz = (DWORD)20e6;
				sweepSettings.sSpectrumSettings.bLevelThreshold = FALSE;
				sweepSettings.sSpectrumSettings.fThresholdInDbm = -100.0f;
				sweepSettings.sSpectrumSettings.bPreamplifier = TRUE;

				/* set this to 1 to enable automatic attenuation management in the
				instrument. Other setting is expert mode.*/
				sweepSettings.sSpectrumSettings.bAutoAttenuation = TRUE;

				sweepSettings.sSpectrumSettings.bAttenuationInDb = 0;
				sweepSettings.sMeasurementTime.dwMeasTimeInNs = 1000000;
				sweepSettings.sMeasurementTime.eDetectorType = SMeasurementTime::RFPOWERSCAN_DETECTOR_TYPE_RMS;

				/* To void interpolation this value should not exceed (dStopFrequencyInHz -
					dStartFrequencyInHz)/[20MHz *1.15 /FFT size]. With our recommended
					settings this is (dStopFrequencyInHz - dStartFrequencyInHz)/22460 Hz .*/
				sweepSettings.sFrequencyDetector.dwCountOfLines = 890;

				sweepSettings.sFrequencyDetector.eDetectorType = SFrequencyDetector::RFPOWERSCAN_FREQDET_TYPE_RMS;
				sweepSettings.sTimeDetector.eDetectorType = STimeDetector::RFPOWERSCAN_TIMEDET_TYPE_RMS;
				sweepSettings.sTimeDetector.eDetectorIntervalType = STimeDetector::RFPOWERSCAN_TIMEDET_INTERVAL_TIMERANGE;
				sweepSettings.sTimeDetector.dwTimeParameterInMs = 1000;
				sweepSettings.sMarker.bUseMarker = 0;
				sweepSettings.sMarker.bReturnsPowerValues = 0;

				if (!myIF->SetSweepSettings(err, sweepSettings)) {
					std::cout << "Erro ao configurar SweepSettings: " << err.GetErrorString() << std::endl;
				}
				else {
					// Inicia a medição
					CViComBasicInterface& basicIF = myIF->GetBasicInterface();
					basicIF.StartMeasurement();

					// Aguarda e obtém o resultado
					const SMeasResult* pResult = myIF->GetResult(err, 5000); // timeout de 5 segundos
					if (pResult && pResult->pSpectrumResult) {
						std::cout << "Resultado da varredura:" << std::endl;
						for (DWORD i = 0; i < pResult->pSpectrumResult->dwCount; ++i) {
							std::cout << "Linha " << i << ": " << pResult->pSpectrumResult->pfSpectrumValuesInDBm[i] << " dBm" << std::endl;
						}
					}
					else {
						std::cout << "Falha ao obter resultado: " << err.GetErrorString() << std::endl;
					}

					// Para a medição
					basicIF.StopMeasurement();
					basicIF.HasMeasurementStopped();
				}
			}
			tsmwIFLoader.Disconnect(err);
		}





		////
		//// Unregister the result processor to stop the processing of results.
		////
		//rSpecificInterface.UnregisterResultDataListener( &resultDataProcessor );

	}
	catch (const CViComError& err)
	{
		//
		// Print error messages.
		//
		std::cout << "CViComError (" << err.GetErrorCode() << ") \"" << err.GetErrorString() << '\"' << std::endl;
	}

	return 0;
}

