#Requires -Version 5.1
# Gran-Ornament local gate: builds the ARM firmware and builds+runs the host
# tests, mirroring .github/workflows/firmware.yml. A green run here means the CI
# will pass; a red run stops a `git push` (via the pre-push hook).
#
# Run manually:
#   powershell -NoProfile -ExecutionPolicy Bypass -File tools/pre-push.ps1 [-Config Debug|Release]

[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release')]
    [string]$Config = 'Release'
)

$ErrorActionPreference = 'Stop'

# Resolve the repo root (parent of tools/) from this script's location (works from any CWD).
if ($PSCommandPath) { $root = Split-Path -Parent (Split-Path -Parent $PSCommandPath) }
else               { $root = (Get-Location).Path }
Set-Location $root

$fw = Join-Path $root 'firmware\devboard\Gran_Ornament'
$ts = Join-Path $root 'firmware\tests'
$tb = Join-Path $root 'build-tests'

function Fail([string]$label) {
    Write-Host ''
    Write-Host ("FAILED: {0}" -f $label) -ForegroundColor Red
    Write-Host 'Gran-Ornament local gate: STOPPED - fix the error above, then push again.' -ForegroundColor Red
    exit 1
}

Write-Host '============================================================' -ForegroundColor Cyan
Write-Host ' Gran-Ornament local gate (build firmware + run tests)'      -ForegroundColor Cyan
Write-Host (' Config : {0}' -f $Config) -ForegroundColor Cyan
Write-Host (' Root   : {0}' -f $root)  -ForegroundColor Cyan
Write-Host '============================================================' -ForegroundColor Cyan

# --- 1. Firmware: configure ---------------------------------------------------
Write-Host ''
Write-Host ('[1/4] Firmware configure  (cmake --preset {0})' -f $Config) -ForegroundColor Yellow
Push-Location $fw
cmake --preset $Config
$p = $LASTEXITCODE
Pop-Location
if ($p -ne 0) { Fail "firmware configure (exit $p)" }

# --- 2. Firmware: build -------------------------------------------------------
Write-Host ''
Write-Host ('[2/4] Firmware build  (cmake --build --preset {0})' -f $Config) -ForegroundColor Yellow
Push-Location $fw
cmake --build --preset $Config
$p = $LASTEXITCODE
Pop-Location
if ($p -ne 0) { Fail "firmware build (exit $p)" }

# --- 2a. Verify the .elf artifact was produced -------------------------------
$elf = Join-Path $fw ("build\{0}\Gran_Ornament.elf" -f $Config)
if (-not (Test-Path -LiteralPath $elf)) { Fail "expected artifact missing: $elf" }
$sz = (Get-Item -LiteralPath $elf).Length
Write-Host ('      OK: {0} ({1:N0} bytes)' -f $elf, $sz) -ForegroundColor DarkGreen

# --- 3. Host tests: configure + build ----------------------------------------
Write-Host ''
Write-Host '[3/4] Host tests: configure + build' -ForegroundColor Yellow
cmake -S $ts -B $tb
if ($LASTEXITCODE -ne 0) { Fail 'tests configure (cmake -S firmware/tests -B build-tests)' }
cmake --build $tb --config $Config
if ($LASTEXITCODE -ne 0) { Fail 'tests build (cmake --build build-tests)' }

# --- 4. Host tests: run --------------------------------------------------------
Write-Host ''
Write-Host '[4/4] Host tests: run  (ctest)' -ForegroundColor Yellow
ctest --test-dir $tb -C $Config --output-on-failure
if ($LASTEXITCODE -ne 0) { Fail 'tests run (ctest)' }

Write-Host ''
Write-Host '============================================================' -ForegroundColor Green
Write-Host ' ALL PASSED: firmware built + tests green' -ForegroundColor Green
Write-Host '============================================================' -ForegroundColor Green
