#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build-server"

mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR"

CMAKE_ARGS=(".." \
  "-DBUILD_CLIENT=OFF" \
  "-DBUILD_SERVER=ON" \
  "-DBUILD_DB_TEST=OFF")

if [[ -f /etc/os-release ]] && grep -q 'ID=ubuntu' /etc/os-release; then
  echo "Ubuntu detected. Adding PostgreSQL paths to CMake arguments."
  PG_INCLUDE_DIR=$(pg_config --includedir)
  PG_TYPE_INCLUDE_DIR=$(pg_config --includedir-server) # Use correct config option
  PG_LIB_DIR=$(pg_config --libdir)
  CMAKE_ARGS+=("-DPostgreSQL_INCLUDE_DIR=${PG_INCLUDE_DIR}")
  CMAKE_ARGS+=("-DPostgreSQL_TYPE_INCLUDE_DIR=${PG_TYPE_INCLUDE_DIR}")
  CMAKE_ARGS+=("-DPostgreSQL_LIBRARY=${PG_LIB_DIR}/libpq.so")
fi

echo "Running CMake with arguments: ${CMAKE_ARGS[@]}"
cmake "${CMAKE_ARGS[@]}" && \
  cmake --build . --target SwiftMessage_Server


if [ $? -eq 0 ]; then
  echo "✅ Сервер готов: $BUILD_DIR/SwiftMessage_Server"
else
  echo "❌ Ошибка сборки сервера."
  exit 1
fi