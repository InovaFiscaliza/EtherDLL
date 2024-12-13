@echo off
set WIX_PATH="C:\Program Files (x86)\WiX Toolset v3.11\bin"

%WIX_PATH%\candle.exe installer.wxs product.wxs CustomActions.wxs
%WIX_PATH%\light.exe -ext WixUIExtension -out VPNConfigurator.msi installer.wixobj product.wixobj CustomActions.wixobj

echo Build completed.
