#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${ROOT}/build"

cmake -S "${ROOT}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" --config Release -j"$(sysctl -n hw.ncpu)"

echo
echo "Build complete. Installed plugin locations:"
echo "  VST3: ~/Library/Audio/Plug-Ins/VST3/ReMastered.vst3"
echo "  AU:   ~/Library/Audio/Plug-Ins/Components/ReMastered.component"
