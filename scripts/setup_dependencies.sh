#!/bin/bash
# setup_dependencies.sh - Fetches and sets up external dependencies for Guitar Amp Simulator
# Run this script once before building the project.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
EXTERNAL_DIR="$PROJECT_ROOT/external"

echo "=== Guitar Amp Simulator - Dependency Setup ==="
echo "Project root: $PROJECT_ROOT"

mkdir -p "$EXTERNAL_DIR"

# --- Dear ImGui ---
IMGUI_DIR="$EXTERNAL_DIR/imgui"
IMGUI_VERSION="v1.90.4"

if [ ! -d "$IMGUI_DIR" ]; then
    echo ""
    echo "Fetching Dear ImGui $IMGUI_VERSION..."
    git clone --depth 1 --branch "$IMGUI_VERSION" https://github.com/ocornut/imgui.git "$IMGUI_DIR"
    echo "Dear ImGui fetched successfully."
else
    echo "Dear ImGui already present, skipping."
fi

# --- Install system dependencies ---
echo ""
echo "Checking system dependencies..."

install_deps() {
    if command -v apt-get &> /dev/null; then
        echo "Detected Debian/Ubuntu. Installing dependencies..."
        sudo apt-get update
        sudo apt-get install -y \
            build-essential cmake pkg-config \
            libportaudio2 portaudio19-dev \
            libsdl2-dev \
            libgl1-mesa-dev
    elif command -v dnf &> /dev/null; then
        echo "Detected Fedora/RHEL. Installing dependencies..."
        sudo dnf install -y \
            gcc-c++ cmake pkg-config \
            portaudio-devel \
            SDL2-devel \
            mesa-libGL-devel
    elif command -v pacman &> /dev/null; then
        echo "Detected Arch Linux. Installing dependencies..."
        sudo pacman -S --noconfirm \
            base-devel cmake pkg-config \
            portaudio \
            sdl2 \
            mesa
    elif command -v brew &> /dev/null; then
        echo "Detected macOS with Homebrew. Installing dependencies..."
        brew install cmake portaudio sdl2
    else
        echo "WARNING: Could not detect package manager."
        echo "Please install manually: cmake, portaudio, sdl2, opengl dev headers"
    fi
}

read -p "Install system dependencies? [y/N] " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    install_deps
fi

echo ""
echo "=== Setup Complete ==="
echo "Build with:"
echo "  mkdir -p build && cd build && cmake .. && make -j\$(nproc)"
