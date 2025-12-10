% main.m
% Exemplo de varredura de espectro usando ViCom DLL e headers em MATLAB 2014

% Caminhos para a DLL e header ViCom
dllPath = fullfile('..', 'bin', 'ViComRFPOWERSCANw.dll');
hPath   = fullfile('..', 'inc', 'ViComRFPowerScanInterface.h');

% Carrega a biblioteca ViCom
if ~libisloaded('ViCom')
    loadlibrary(dllPath, hPath, 'alias', 'ViCom');
end

try
    % Inicializa o erro
    err = libstruct('CViComError');
    receptor = 1; % Exemplo: TSMW = 1, ajuste conforme enum Receiver::Type

    % Cria o loader da interface
    loader = calllib('ViCom', 'CViComLoader_CViComRFPowerScanInterface_new', receptor);

    % Conecta ao instrumento
    ipAddress = '192.168.0.2';
    bLoaded = calllib('ViCom', 'CViComLoader_Connect', loader, err, ipAddress);
    if ~bLoaded
        disp('No receiver found.');
        return;
    end
    disp('TSMW conectado.');

    % Obtém a interface de varredura
    myIF = calllib('ViCom', 'CViComLoader_GetInterface', loader, err);

    % Prepara as configurações do sweep
    sweepSettings = libstruct('SSweepSettings');
    sweepSettings.dwFrontEndSelectionMask = 1;
    sweepSettings.dStartFrequencyInHz = 87.8e6;
    sweepSettings.dStopFrequencyInHz  = 108.8e6;
    sweepSettings.bRequestRawData = false;
    sweepSettings.sSpectrumSettings.fMaxReportingRateInHz = 10.0;
    sweepSettings.sSpectrumSettings.fMaxDeviceMeasRateInHz = 10.0;
    sweepSettings.sSpectrumSettings.eWindowType = 1; % HANNING, ajuste conforme enum
    sweepSettings.sSpectrumSettings.eFFTSize = 1024;
    sweepSettings.sSpectrumSettings.bAutoBandwidth = true;
    sweepSettings.sSpectrumSettings.dwBandwidthInHz = uint32(20e6);
    sweepSettings.sSpectrumSettings.bLevelThreshold = false;
    sweepSettings.sSpectrumSettings.fThresholdInDbm = -100.0;
    sweepSettings.sSpectrumSettings.bPreamplifier = true;
    sweepSettings.sSpectrumSettings.bAutoAttenuation = true;
    sweepSettings.sSpectrumSettings.bAttenuationInDb = 0;

    % Envia as configurações para o instrumento
    ok = calllib('ViCom', 'CViComRFPowerScanInterface_SetSweepSettings', myIF, err, sweepSettings);
    if ~ok
        disp(['Erro ao configurar SweepSettings: ' err.m_acErrorString]);
    else
        % Inicia a medição
        basicIF = calllib('ViCom', 'CViComRFPowerScanInterface_GetBasicInterface', myIF);
        calllib('ViCom', 'CViComBasicInterface_StartMeasurement', basicIF);

        % Aguarda e obtém o resultado
        timeout = 5000;
        pResult = calllib('ViCom', 'CViComRFPowerScanInterface_GetResult', myIF, err, timeout);
        if ~isempty(pResult) && isfield(pResult, 'pSpectrumResult') && ~isempty(pResult.pSpectrumResult)
            disp('Resultado da varredura:');
            for i = 1:pResult.pSpectrumResult.dwCount
                fprintf('Linha %d: %.2f dBm\n', i-1, pResult.pSpectrumResult.pfSpectrumValuesInDBm(i));
            end
        else
            disp(['Falha ao obter resultado: ' err.m_acErrorString]);
        end

        % Para a medição
        calllib('ViCom', 'CViComBasicInterface_StopMeasurement', basicIF);
        calllib('ViCom', 'CViComBasicInterface_HasMeasurementStopped', basicIF);
    end

    % Desconecta
    calllib('ViCom', 'CViComLoader_Disconnect', loader, err);

catch ME
    disp(['Erro: ' ME.message]);
end

% Libera a biblioteca
if libisloaded('ViCom')
    unloadlibrary('ViCom');
end