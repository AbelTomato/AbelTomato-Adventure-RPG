$ErrorActionPreference = "Stop"

$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptPath "..")
$backendDir = Join-Path $repoRoot "backend"
$backendPython = Join-Path $backendDir ".venv\Scripts\python.exe"
$coreExe = Join-Path $repoRoot "build-ninja\abel_core_cli.exe"
$frontendDir = Join-Path $repoRoot "frontend"
$hostAddress = "127.0.0.1"
$port = 8000

function Write-Step {
    param([string]$Message)
    Write-Host "[phase6] $Message"
}

Write-Step "Repository: $repoRoot"

if (-not (Test-Path -LiteralPath $backendPython)) {
    Write-Error @"
Backend virtual environment was not found:
  $backendPython

Create it first:
  cd "$backendDir"
  python -m venv .venv
  .\.venv\Scripts\python.exe -m pip install -r requirements.txt
"@
}

if (-not (Test-Path -LiteralPath $coreExe)) {
    Write-Error @"
C++ CLI executable was not found:
  $coreExe

Build it first:
  cd "$repoRoot"
  cmake --build build-ninja --target abel_core_cli
"@
}

if (-not (Test-Path -LiteralPath (Join-Path $frontendDir "node_modules"))) {
    Write-Step "Frontend dependencies are missing. Install them in another terminal before starting React:"
    Write-Host "  pnpm -C frontend install"
}

$portInUse = Get-NetTCPConnection -LocalAddress $hostAddress -LocalPort $port -State Listen -ErrorAction SilentlyContinue
if ($portInUse) {
    Write-Error @"
Port $port is already in use on $hostAddress.

This usually means FastAPI is already running. Either keep using the existing backend terminal, or stop it with Ctrl+C before starting this script.
"@
}

$env:USE_MOCK_CORE = "false"

Write-Step "USE_MOCK_CORE=false"
Write-Step "Starting FastAPI on http://${hostAddress}:$port"
Write-Step "Keep this terminal open while testing React or Godot."
Write-Step "React dev server command for another terminal:"
Write-Host "  pnpm -C frontend dev"
Write-Host ""

Set-Location -LiteralPath $backendDir
& $backendPython -m uvicorn app.main:app --host $hostAddress --port $port
