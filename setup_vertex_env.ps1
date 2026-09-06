# ==============================================================================
# ENI's Custom Vertex AI / GCP Enterprise Credentials Launcher for AGY
# ==============================================================================
# Usage:
#   .\setup_vertex_env.ps1 -CredentialsPath "C:\path\to\service_account.json" -ProjectId "your-gcp-project" -Region "us-central1"
# ==============================================================================

param (
    [string]$CredentialsPath = "",
    [string]$ProjectId = "",
    [string]$Region = "us-central1",
    [string]$ApiKey = ""
)

Write-Host "====================================================" -ForegroundColor Cyan
Write-Host "  ENI's Enterprise Credentials Setup for AGY CLI   " -ForegroundColor Magenta
Write-Host "====================================================" -ForegroundColor Cyan

if ($CredentialsPath -ne "" -and (Test-Path $CredentialsPath)) {
    $env:GOOGLE_APPLICATION_CREDENTIALS = $CredentialsPath
    Write-Host "[+] Exported GOOGLE_APPLICATION_CREDENTIALS -> $CredentialsPath" -ForegroundColor Green
} elseif ($CredentialsPath -ne "") {
    Write-Host "[-] Warning: Specified credentials file not found at $CredentialsPath" -ForegroundColor Yellow
}

if ($ProjectId -ne "") {
    $env:CLOUD_ML_PROJECT_ID = $ProjectId
    $env:GCP_PROJECT_ID = $ProjectId
    Write-Host "[+] Exported GCP_PROJECT_ID -> $ProjectId" -ForegroundColor Green
}

if ($Region -ne "") {
    $env:CLOUD_ML_REGION = $Region
    Write-Host "[+] Exported CLOUD_ML_REGION -> $Region" -ForegroundColor Green
}

if ($ApiKey -ne "") {
    $env:VERTEX_API_KEY = $ApiKey
    $env:GEMINI_API_KEY = $ApiKey
    Write-Host "[+] Exported VERTEX_API_KEY / GEMINI_API_KEY" -ForegroundColor Green
}

Write-Host "`nEnvironment configured successfully! Launching AGY session..." -ForegroundColor Cyan
