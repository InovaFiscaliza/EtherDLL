param (
    [string]$ResultFilePath
)

# Check if OpenVPN service is running
$serviceName = "OpenVPNService"
$service = Get-Service -Name $serviceName -ErrorAction SilentlyContinue

if ($null -eq $service) {
    Write-Host "OpenVPN service not found."
    Out-File -FilePath $ResultFilePath -InputObject "ERROR: Service not found" -Force
    exit 1
}

if ($service.Status -ne "Running") {
    try {
        Start-Service -Name $serviceName
        Start-Sleep -Seconds 5
        $service.Refresh()
    } catch {
        Write-Host "Failed to start OpenVPN service."
        Out-File -FilePath $ResultFilePath -InputObject "ERROR: Failed to start service" -Force
        exit 1
    }
}

# Retrieve TUN interface number
$interface = Get-NetAdapter | Where-Object { $_.InterfaceDescription -like "*TAP-Windows Adapter*" }

if ($null -eq $interface) {
    Write-Host "TUN interface not found."
    Out-File -FilePath $ResultFilePath -InputObject "ERROR: TUN interface not found" -Force
    exit 1
}

# Return interface number
$interfaceIndex = $interface.ifIndex
Write-Host "TUN interface number: $interfaceIndex"
Out-File -FilePath $ResultFilePath -InputObject $interfaceIndex -Force
exit 0
