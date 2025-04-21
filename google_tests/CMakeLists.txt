cmake_minimum_required(VERSION 3.15)
project(SwiftMessage_Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
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

# Определяем путь к корню проекта
get_filename_component(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

find_package(Boost REQUIRED COMPONENTS serialization)

# Включаем директорию с заголовочными файлами
include_directories(
    ${PROJECT_ROOT_DIR}/include 
    ${Boost_INCLUDE_DIRS}
)

# Тесты для общих сообщений
add_executable(messages_test messages_test.cpp)
target_link_libraries(messages_test gtest_main Boost::serialization)

# Тесты для менеджера базы данных (закомментировано, требуется PostgreSQL)
# Раскомментируйте эти строки, если установлен PostgreSQL и libpqxx
#find_package(PostgreSQL)
#find_library(PQXX_LIB pqxx)
#if(PostgreSQL_FOUND AND PQXX_LIB)
#  add_executable(database_manager_test database_manager_test.cpp 
#                                       ${PROJECT_ROOT_DIR}/src/server/database_manager.cpp)
#  target_link_libraries(database_manager_test gtest_main 
#                                           ${PQXX_LIB}
#                                           ${PostgreSQL_LIBRARIES})
#  gtest_discover_tests(database_manager_test)
#endif()

# Тесты для менеджера запросов (закомментировано, требуется PostgreSQL)
# Раскомментируйте эти строки, если установлен PostgreSQL и libpqxx
#if(PostgreSQL_FOUND AND PQXX_LIB)
#  add_executable(request_manager_test request_manager_test.cpp
#                                     ${PROJECT_ROOT_DIR}/src/server/request_manager.cpp)
#  target_link_libraries(request_manager_test gtest_main
#                                           Boost::system
#                                           Boost::thread
#                                           Boost::serialization
#                                           ${PQXX_LIB}
#                                           ${PostgreSQL_LIBRARIES})
#  gtest_discover_tests(request_manager_test)
#endif()

# Добавление в ctest
include(GoogleTest)
gtest_discover_tests(messages_test)
#gtest_discover_tests(database_manager_test)
#gtest_discover_tests(request_manager_test) 