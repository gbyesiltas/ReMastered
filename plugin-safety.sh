#!/usr/bin/env bash
set -euo pipefail

VST3="$HOME/Library/Audio/Plug-Ins/VST3/ReMastered.vst3"
AU="$HOME/Library/Audio/Plug-Ins/Components/ReMastered.component"
DISABLED_ROOT="$HOME/Library/Audio/Plug-Ins/_Disabled/ReMastered"
DISABLED_VST3="$DISABLED_ROOT/ReMastered.vst3"
DISABLED_AU="$DISABLED_ROOT/ReMastered.component"
STANDALONE="$(cd "$(dirname "$0")" && pwd)/build/ReMastered_artefacts/Release/Standalone/ReMastered.app"

usage() {
  cat <<USAGE
Usage: $(basename "$0") <command>

Commands:
  status       Show whether plugin is enabled/disabled
  disable      Move AU+VST3 out of active plugin folders (safe mode)
  enable       Restore AU+VST3 to active plugin folders
  standalone   Open standalone app for quick testing (no Ableton)
USAGE
}

cmd="${1:-}"

case "$cmd" in
  status)
    echo "VST3 active:    $([ -e "$VST3" ] && echo yes || echo no)"
    echo "AU active:      $([ -e "$AU" ] && echo yes || echo no)"
    echo "VST3 disabled:  $([ -e "$DISABLED_VST3" ] && echo yes || echo no)"
    echo "AU disabled:    $([ -e "$DISABLED_AU" ] && echo yes || echo no)"
    ;;
  disable)
    mkdir -p "$DISABLED_ROOT"
    [ -e "$VST3" ] && mv "$VST3" "$DISABLED_VST3"
    [ -e "$AU" ] && mv "$AU" "$DISABLED_AU"
    echo "ReMastered disabled. Restart Ableton."
    ;;
  enable)
    mkdir -p "$(dirname "$VST3")" "$(dirname "$AU")"
    [ -e "$DISABLED_VST3" ] && mv "$DISABLED_VST3" "$VST3"
    [ -e "$DISABLED_AU" ] && mv "$DISABLED_AU" "$AU"
    echo "ReMastered enabled. Restart/rescan in Ableton."
    ;;
  standalone)
    if [ ! -d "$STANDALONE" ]; then
      echo "Standalone app not built yet: $STANDALONE"
      exit 1
    fi
    open "$STANDALONE"
    ;;
  *)
    usage
    exit 1
    ;;
esac
