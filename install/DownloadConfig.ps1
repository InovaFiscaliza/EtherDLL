param (
    [string]$SharedLink,
    [string]$TargetDirectory
)

if (!(Test-Path -Path $TargetDirectory)) {
    New-Item -ItemType Directory -Path $TargetDirectory | Out-Null
}

$FileName = "config.ovpn"
$TempFilePath = "$env:TEMP\$FileName"

try {
    Invoke-WebRequest -Uri $SharedLink -OutFile $TempFilePath
    Move-Item -Path $TempFilePath -Destination (Join-Path -Path $TargetDirectory -ChildPath $FileName) -Force
} catch {
    Write-Error "Error downloading file: $_"
    exit 1
}

if (Test-Path -Path $TempFilePath) {
    Remove-Item -Path $TempFilePath -Force
}
