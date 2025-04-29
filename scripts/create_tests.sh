#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build-tests"

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

CMAKE_ARGS=(
  ".."
  "-DCMAKE_BUILD_TYPE=Release"
  "-DBUILD_CLIENT=OFF"
  "-DBUILD_SERVER=OFF"
  "-DBUILD_DB_TEST=OFF"
  "-DBUILD_INTEGRATION_TESTS=ON"
)

if [[ -f /etc/os-release ]] && grep -q 'ID=ubuntu' /etc/os-release; then
  echo "Ubuntu detected. Adding PostgreSQL paths to CMake arguments."
  PG_INCLUDE_DIR=$(pg_config --includedir)
  PG_TYPE_INCLUDE_DIR=$(pg_config --includedir-server)
  PG_LIB_DIR=$(pg_config --libdir)

  CMAKE_ARGS+=(
    "-DPostgreSQL_INCLUDE_DIR=${PG_INCLUDE_DIR}"
    "-DPostgreSQL_TYPE_INCLUDE_DIR=${PG_TYPE_INCLUDE_DIR}"
    "-DPostgreSQL_LIBRARY=${PG_LIB_DIR}/libpq.so"
  )
fi

echo "Running CMake with arguments:"
printf '  %q\n' "${CMAKE_ARGS[@]}"

cmake "${CMAKE_ARGS[@]}"
cmake --build . --target SwiftMessage_Tests --parallel

echo "✅ Тестовый бинарь готов: ${BUILD_DIR}/integration_tests/SwiftMessage_Tests"
