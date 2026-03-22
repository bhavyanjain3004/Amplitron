# setup_dependencies.ps1 - Fetches and sets up external dependencies for Guitar Amp Simulator
# Run this script once before building the project.

$ErrorActionPreference = "Stop"

$PROJECT_ROOT = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$EXTERNAL_DIR = Join-Path $PROJECT_ROOT "external"

Write-Host "=== Guitar Amp Simulator - Dependency Setup ===" -ForegroundColor Cyan
Write-Host "Project root: $PROJECT_ROOT"

# Create external directory
if (-Not (Test-Path $EXTERNAL_DIR)) {
    New-Item -ItemType Directory -Path $EXTERNAL_DIR | Out-Null
}

# --- Dear ImGui ---
$IMGUI_DIR = Join-Path $EXTERNAL_DIR "imgui"
$IMGUI_VERSION = "v1.90.4"

if (-Not (Test-Path $IMGUI_DIR)) {
    Write-Host "`nFetching Dear ImGui $IMGUI_VERSION..." -ForegroundColor Yellow
    git clone --depth 1 --branch $IMGUI_VERSION https://github.com/ocornut/imgui.git $IMGUI_DIR
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Failed to clone ImGui" -ForegroundColor Red
        exit 1
    }
    Write-Host "Dear ImGui fetched successfully." -ForegroundColor Green
} else {
    Write-Host "Dear ImGui already present, skipping." -ForegroundColor Green
}

# --- Check for PortAudio ---
Write-Host "`nChecking for PortAudio..." -ForegroundColor Yellow
$pa_found = $false

# Check vcpkg
if (Get-Command vcpkg -ErrorAction SilentlyContinue) {
    Write-Host "vcpkg found. You can install PortAudio with: vcpkg install portaudio" -ForegroundColor Cyan
    $pa_found = $true
}

# Check common paths
$pa_paths = @(
    "C:\Program Files\portaudio",
    "C:\vcpkg\installed\x64-windows",
    "$env:VCPKG_ROOT\installed\x64-windows"
)
foreach ($p in $pa_paths) {
    if (Test-Path (Join-Path $p "include\portaudio.h")) {
        Write-Host "PortAudio found at: $p" -ForegroundColor Green
        $pa_found = $true
        break
    }
}

if (-Not $pa_found) {
    Write-Host @"

PortAudio not found. Install it via one of these methods:
  1. vcpkg:   vcpkg install portaudio:x64-windows
  2. Manual:  Download from http://www.portaudio.com/download.html
              Build and install to C:\Program Files\portaudio
"@ -ForegroundColor Red
}

# --- Check for SDL2 ---
Write-Host "`nChecking for SDL2..." -ForegroundColor Yellow
$sdl_found = $false

$sdl_paths = @(
    "C:\Program Files\SDL2",
    "C:\vcpkg\installed\x64-windows",
    "$env:VCPKG_ROOT\installed\x64-windows"
)
foreach ($p in $sdl_paths) {
    if ((Test-Path (Join-Path $p "include\SDL2\SDL.h")) -or (Test-Path (Join-Path $p "include\SDL.h"))) {
        Write-Host "SDL2 found at: $p" -ForegroundColor Green
        $sdl_found = $true
        break
    }
}

if (-Not $sdl_found) {
    Write-Host @"

SDL2 not found. Install it via one of these methods:
  1. vcpkg:   vcpkg install sdl2:x64-windows
  2. Manual:  Download from https://github.com/libsdl-org/SDL/releases
              Extract to C:\Program Files\SDL2
"@ -ForegroundColor Red
}

Write-Host "`n=== Setup Complete ===" -ForegroundColor Cyan
if ($pa_found -and $sdl_found) {
    Write-Host "All dependencies found! You can now build with:" -ForegroundColor Green
    Write-Host "  mkdir build; cd build; cmake ..; cmake --build . --config Release" -ForegroundColor White
} else {
    Write-Host "Some dependencies are missing. Please install them before building." -ForegroundColor Yellow
}
