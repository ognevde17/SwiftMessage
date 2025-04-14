SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
CMAKE_COMMAND="cmake .." 
MAKE_COMMAND="make"

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