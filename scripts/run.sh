SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"

CMAKE_COMMAND="cmake .."
MAKE_COMMAND="make"

# Если запускаемся в Ubuntu — настраиваем флаги для PostgreSQL
if [[ -f /etc/os-release ]]; then
  # Подключаем переменные из os-release
  . /etc/os-release
  if [[ "$ID" == "ubuntu" ]]; then
    echo "Detected Ubuntu—включаем PostgreSQL настройки..."
    
    PG_INCLUDE_DIR="$(pg_config --includedir)"
    PG_TYPE_INCLUDE_DIR="${PG_INCLUDE_DIR}/server"
    PG_LIB_DIR="$(pg_config --libdir)"

    CMAKE_COMMAND="cmake .. \
      -DPostgreSQL_INCLUDE_DIR=${PG_INCLUDE_DIR} \
      -DPostgreSQL_TYPE_INCLUDE_DIR=${PG_TYPE_INCLUDE_DIR} \
      -DPostgreSQL_LIBRARY=${PG_LIB_DIR}/libpq.so"
  fi
fi

# PG_INCLUDE_DIR=$(pg_config --includedir)
# PG_TYPE_INCLUDE_DIR="${PG_INCLUDE_DIR}/server"
# PG_LIB_DIR=$(pg_config --libdir)


# CMAKE_COMMAND="cmake .. \
# 	-DPostgreSQL_INCLUDE_DIR=${PG_INCLUDE_DIR} \
# 	-DPostgreSQL_TYPE_INCLUDE_DIR=${PG_TYPE_INCLUDE_DIR} \
# 	-DPostgreSQL_LIBRARY=${PG_LIB_DIR}/libpq.so"


# Проверка, запущен ли скрипт в GitHub Actions
if [ -n "$GITHUB_ACTIONS" ]; then
  echo "Запущено в GitHub Actions, использую CMakeLists.ci.txt..."
  if [ -f "$PROJECT_ROOT/CMakeLists.ci.txt" ]; then
    cp "$PROJECT_ROOT/CMakeLists.ci.txt" "$PROJECT_ROOT/CMakeLists.txt"
    echo "CMakeLists.txt успешно заменен на версию для CI"
  else
    echo "Предупреждение: CMakeLists.ci.txt не найден, использую стандартный CMakeLists.txt"
  fi
fi

if [ ! -d "$BUILD_DIR" ]; then
  echo "Папка $BUILD_DIR не найдена. Создаю..."
  mkdir -p "$BUILD_DIR"
fi
cd "$BUILD_DIR" || { echo "Ошибка: не удалось перейти в папку $BUILD_DIR"; exit 1; }
echo "Запуск CMake... plz"
$CMAKE_COMMAND || { echo "Ошибка: CMake завершился с ошибкой"; exit 1; }
echo "Запуск Make... plzplzplz"
$MAKE_COMMAND || { echo "Ошибка: Make завершился с ошибкой"; exit 1; }
echo "Сборка успешно завершена! toptoptop"
