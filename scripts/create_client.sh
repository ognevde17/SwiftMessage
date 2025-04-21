#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build-client"

mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR"

CMAKE_ARGS=(".." \
  "-DBUILD_CLIENT=ON" \
  "-DBUILD_SERVER=OFF" \
  "-DBUILD_DB_TEST=OFF")

echo "Running CMake with arguments: ${CMAKE_ARGS[@]}"
cmake "${CMAKE_ARGS[@]}" && \
  cmake --build . --target SwiftMessage_Client

if [ $? -eq 0 ]; then
  echo "✅ Клиент готов: $BUILD_DIR/SwiftMessage_Client"
else
  echo "❌ Ошибка сборки клиента."
  exit 1
fi
