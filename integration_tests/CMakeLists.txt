cmake_minimum_required(VERSION 3.15)
project(SwiftMessage_Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Используем FetchContent для Google Test --- 
include(FetchContent)

FetchContent_Declare(
  googletest
  # Можно указать URL/GIT_REPOSITORY и GIT_TAG для конкретной версии
  # Например:
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0 # Corrected tag format (vX.Y.Z)
  # Или использовать стандартный способ поиска CMake, если GTest установлен
)

# Делаем GTest доступным (скачивает и настраивает при необходимости)
# GTEST_SKIP_INSTALL - не добавляет цель install для GTest
set(GTEST_SKIP_INSTALL ON CACHE BOOL \"\" FORCE)
FetchContent_MakeAvailable(googletest)
# --- Конец FetchContent --- 

# Определяем путь к корню проекта, если он не задан выше
if(NOT DEFINED PROJECT_ROOT_DIR)
  get_filename_component(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.. ABSOLUTE)
  message(STATUS \"Guessed PROJECT_ROOT_DIR: ${PROJECT_ROOT_DIR}\")
endif()

# Set policy for Boost (Хорошо, что это есть)
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# Находим нужные пакеты
find_package(Boost REQUIRED COMPONENTS system thread serialization)
find_package(PostgreSQL REQUIRED)
find_library(PQXX_LIB pqxx)
# find_library(PQXX_LIB pqxx) # find_package(libpqxx) может быть лучше, если используете pqxx
# Используем переменную из find_package(PostgreSQL ..)

# Включаем директории с заголовочными файлами
# FetchContent обычно добавляет include директории GTest автоматически
# Но можно добавить явно для надежности, если нужно
include_directories(
    ${PROJECT_ROOT_DIR}/include # Заголовки вашего проекта
    ${Boost_INCLUDE_DIRS}      # Заголовки Boost
    ${PostgreSQL_INCLUDE_DIRS} # Заголовки PostgreSQL
    # ${googletest_SOURCE_DIR}/googletest/include # Часто не нужно
    # ${googletest_SOURCE_DIR}/googlemock/include # Часто не нужно
)

# Собираем список исходных файлов сервера и клиента
# ПРЕДУПРЕЖДЕНИЕ: Лучше собирать server/client как библиотеки в их директориях
# и линковать их здесь, а не включать исходники напрямую.



# Включаем поддержку тестирования CTest
enable_testing()

# Интеграционный тест для проверки полного сценария
add_executable(${PROJECT_NAME} full_scenario_test.cpp ${SERVER_SOURCES} ${CLIENT_SOURCES})

# Explicitly add GTest include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${googletest_SOURCE_DIR}/googletest/include
    ${googletest_SOURCE_DIR}/googlemock/include # If using gmock
)

# Линкуем тест с GTest, вашими библиотеками/таргетами и зависимостями
target_link_libraries(${PROJECT_NAME} PRIVATE
    gtest_main           # GTest (включает main для тестов)
    Boost::system
    Boost::thread
    Boost::serialization
    ${PQXX_LIB}
    ${PostgreSQL_LIBRARIES}
    pthread              # Обычно требуется для GTest/std::thread
)

# Добавляем тест в CTest
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}) 