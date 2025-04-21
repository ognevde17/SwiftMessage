cmake_minimum_required(VERSION 3.15)
project(SwiftMessage_IntegrationTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем путь к корню проекта
get_filename_component(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

# Загрузка и распаковка googletest во время настройки
configure_file(
    ${PROJECT_ROOT_DIR}/google_tests/CMakeLists.txt.in 
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-download/CMakeLists.txt
)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Set policy for Boost
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# Находим нужные пакеты
find_package(Boost REQUIRED COMPONENTS system thread serialization)
find_package(PostgreSQL REQUIRED)
find_library(PQXX_LIB pqxx)

# Включаем директории с заголовочными файлами
include_directories(
    ${PROJECT_ROOT_DIR}/include 
    ${Boost_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
)

# Соберем список исходных файлов сервера и клиента
set(SERVER_SOURCES
    ${PROJECT_ROOT_DIR}/src/server/database_manager.cpp
    ${PROJECT_ROOT_DIR}/src/server/connection_manager.cpp
    ${PROJECT_ROOT_DIR}/src/server/request_manager.cpp
    ${PROJECT_ROOT_DIR}/src/server/server.cpp
)

set(CLIENT_SOURCES
    ${PROJECT_ROOT_DIR}/src/client/connection_manager.cpp
    ${PROJECT_ROOT_DIR}/src/client/client.cpp
)

# Интеграционный тест для проверки полного сценария
add_executable(full_scenario_test full_scenario_test.cpp ${SERVER_SOURCES} ${CLIENT_SOURCES})
target_link_libraries(full_scenario_test 
    gtest_main
    Boost::system
    Boost::thread
    Boost::serialization
    ${PQXX_LIB}
    ${PostgreSQL_LIBRARIES}
)

# Добавление в ctest
include(GoogleTest)
gtest_discover_tests(full_scenario_test) 