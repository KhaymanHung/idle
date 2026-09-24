#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build/linux"

CANDIDATE_QT_PREFIXES=(
  "$ROOT_DIR/vendor/Qt/5.15.2/gcc_64"
  "$ROOT_DIR/vendor/Qt/5.15.2"
  "/usr/lib/x86_64-linux-gnu/cmake/Qt5"
  "/usr/lib/x86_64-linux-gnu"
)

QT_PREFIX="${QT_PREFIX:-}"
if [[ -z "$QT_PREFIX" ]]; then
  for candidate in "${CANDIDATE_QT_PREFIXES[@]}"; do
    if [[ -e "$candidate" ]]; then
      QT_PREFIX="$candidate"
      break
    fi
  done
fi

if [[ -z "$QT_PREFIX" ]]; then
  echo "找不到 Qt 5 目錄。請將 Qt 放在專案的 vendor/Qt/5.15.2/... 內，或設定 QT_PREFIX 環境變數。" >&2
  exit 1
fi

cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$QT_PREFIX"
cmake --build "$BUILD_DIR" --parallel
