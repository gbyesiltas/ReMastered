#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
JUCE_DIR="${ROOT}/../juce"
BUILD_DIR="${ROOT}/build"

if [[ ! -d "${JUCE_DIR}" ]]; then
  echo "JUCE not found at ${JUCE_DIR}"
  echo "Clone it first: git clone --depth 1 https://github.com/juce-framework/JUCE.git ${JUCE_DIR}"
  exit 1
fi

cmake -S "${ROOT}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" --config Release -j"$(sysctl -n hw.ncpu)"

echo
echo "Build complete. Installed plugin locations:"
echo "  VST3: ~/Library/Audio/Plug-Ins/VST3/ReMastered.vst3"
echo "  AU:   ~/Library/Audio/Plug-Ins/Components/ReMastered.component"
