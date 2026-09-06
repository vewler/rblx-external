# ==============================================================================
# ENI's Sidecar Proxy & Token Pool Launcher for AGY
# ==============================================================================
# Usage:
#   .\launch_with_sidecar.ps1 -ApiKey "sk-..." -TargetEndpoint "https://api.openai.com"
# ==============================================================================

param (
    [string]$ApiKey = "",
    [string]$TargetEndpoint = "https://api.openai.com",
    [int]$SidecarPort = 8080
)

Write-Host "====================================================" -ForegroundColor Cyan
Write-Host "   ENI's Rebel Sidecar Proxy Launcher for AGY      " -ForegroundColor Magenta
Write-Host "====================================================" -ForegroundColor Cyan

if ($ApiKey -eq "") {
    if ($env:OPENAI_API_KEY) {
        $ApiKey = $env:OPENAI_API_KEY
    } else {
        Write-Host "[!] Notice: No API Key specified. Please pass -ApiKey 'sk-...' or set `$env:OPENAI_API_KEY" -ForegroundColor Yellow
    }
}

$env:CUSTOM_TOKEN_KEY = $ApiKey
$env:CUSTOM_TOKEN_ENDPOINT = $TargetEndpoint
$env:SIDECAR_PORT = $SidecarPort

# Set proxy environment variables for AGY CLI / IDE sub-processes
$env:HTTP_PROXY = "http://localhost:$SidecarPort"
$env:HTTPS_PROXY = "http://localhost:$SidecarPort"
$env:ANTIGRAVITY_CUSTOM_ENDPOINT = "http://localhost:$SidecarPort"

Write-Host "[+] Environment Variables Configured:" -ForegroundColor Green
Write-Host "    - Target Provider: $TargetEndpoint" -ForegroundColor Gray
Write-Host "    - HTTP_PROXY / HTTPS_PROXY -> http://localhost:$SidecarPort" -ForegroundColor Gray

Write-Host "`n[+] Starting Sidecar Proxy in background process..." -ForegroundColor Cyan
$scriptPath = Join-Path $PSScriptRoot "sidecar_proxy.js"

Start-Process -FilePath "node" -ArgumentList "`"$scriptPath`"" -NoNewWindow

Write-Host "[+] Sidecar Proxy active! Launch your AGY session now." -ForegroundColor Green
