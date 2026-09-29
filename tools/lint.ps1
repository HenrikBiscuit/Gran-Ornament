#Requires -Version 5.1
# Gran-Ornament lint: clang-format check + clang-tidy on our own code only.
#   - clang-format : firmware/devboard/*/App/** and firmware/tests/*.cpp
#   - clang-tidy   : firmware/devboard/Gran_Ornament/App/**/*.cpp (plus the App/
#                    headers they include), using the firmware compile database.
# CubeMX-generated Core/ and vendor Drivers/ are never touched.
#
# Needs a configured firmware build (cmake --preset <Config>) for
# compile_commands.json; run by tools/pre-push.ps1 after the firmware build and
# by the "lint" job in .github/workflows/firmware.yml.
#
# Run manually:
#   powershell -NoProfile -ExecutionPolicy Bypass -File tools/lint.ps1 [-Config Debug|Release] [-Fix]
#   -Fix rewrites files with clang-format instead of only checking them.

[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release')]
    [string]$Config = 'Release',
    [switch]$Fix
)

$ErrorActionPreference = 'Stop'

if ($PSCommandPath) { $root = Split-Path -Parent (Split-Path -Parent $PSCommandPath) }
else               { $root = (Get-Location).Path }
Set-Location $root

$fw     = 'firmware/devboard/Gran_Ornament'
$compDb = "$fw/build/$Config"

function Fail([string]$label) {
    Write-Host ''
    Write-Host ("LINT FAILED: {0}" -f $label) -ForegroundColor Red
    exit 1
}

foreach ($tool in 'clang-format', 'clang-tidy', 'arm-none-eabi-g++') {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) { Fail "$tool not found on PATH" }
}

# Tracked sources only, so build dirs and scratch files are never linted.
$files = @(git ls-files -- 'firmware/devboard/*/App/*' 'firmware/tests/*.cpp' |
           Where-Object { $_ -match '\.(c|cpp|h|hpp)$' })
if ($files.Count -eq 0) { Fail 'no source files found' }

# --- clang-format -------------------------------------------------------------
if ($Fix) {
    Write-Host ('clang-format: formatting {0} files' -f $files.Count) -ForegroundColor Yellow
    & clang-format -i $files
    if ($LASTEXITCODE -ne 0) { Fail "clang-format -i (exit $LASTEXITCODE)" }
} else {
    Write-Host ('clang-format: checking {0} files' -f $files.Count) -ForegroundColor Yellow
    & clang-format --dry-run --Werror $files
    if ($LASTEXITCODE -ne 0) { Fail 'clang-format (run tools/lint.ps1 -Fix to reformat)' }
}

# --- clang-tidy ---------------------------------------------------------------
if (-not (Test-Path -LiteralPath "$compDb/compile_commands.json")) {
    Fail "missing $compDb/compile_commands.json - run: cmake --preset $Config (in $fw)"
}

# Host clang does not know where the ARM GCC toolchain keeps libstdc++/newlib,
# so ask arm-none-eabi-g++ for its system include dirs and pass them through.
$ErrorActionPreference = 'Continue'   # g++ -v writes to stderr
$gccOut = @('' | & arm-none-eabi-g++ -xc++ -E -v - 2>&1 | ForEach-Object { "$_" })
$ErrorActionPreference = 'Stop'
$tidyArgs = @('-p', $compDb, '--quiet', '--extra-arg-before=--target=arm-none-eabi')
$inList = $false
$nDirs = 0
foreach ($line in $gccOut) {
    if ($line -like '#include <...>*')       { $inList = $true; continue }
    if ($line -like 'End of search list*')   { break }
    if ($inList) { $tidyArgs += "--extra-arg=-isystem$($line.Trim())"; $nDirs++ }
}
if ($nDirs -eq 0) { Fail 'could not read arm-none-eabi-g++ include dirs' }

# Host tests are covered by clang-format only; tidy runs on firmware sources.
$tidyFiles = @($files | Where-Object { $_ -like "$fw/App/*" -and $_ -match '\.(c|cpp)$' })
Write-Host ('clang-tidy: checking {0} files' -f $tidyFiles.Count) -ForegroundColor Yellow
& clang-tidy @tidyArgs $tidyFiles
if ($LASTEXITCODE -ne 0) { Fail "clang-tidy (exit $LASTEXITCODE)" }

Write-Host 'Lint passed.' -ForegroundColor Green
