#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build-client"

mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR"

cmake .. \
  -DBUILD_CLIENT=ON \
  -DBUILD_SERVER=OFF \
  -DBUILD_DB_TEST=OFF \
  $([[ -f /etc/os-release ]] && . /etc/os-release && [[ "$ID" == "ubuntu" ]] \
    && printf "-DPostgreSQL_INCLUDE_DIR=%s -DPostgreSQL_TYPE_INCLUDE_DIR=%s/server -DPostgreSQL_LIBRARY=%s/libpq.so" \
         "$(pg_config --includedir)" "$(pg_config --includedir)" "$(pg_config --libdir)") \
&& cmake --build . --target SwiftMessage_Client

echo "✅ Клиент готов: $BUILD_DIR/SwiftMessage_Client"
