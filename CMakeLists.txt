cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_NAME ${PROJECT_NAME}_Server)
set(CLIENT_NAME ${PROJECT_NAME}_Client)
set(DB_TEST_NAME ${PROJECT_NAME}_DbTest)
set(INTERFACE_TEST_NAME ${PROJECT_NAME}_InterfaceTest)

# Дефолтные значения тех, кого мы будем собирать
option(BUILD_CLIENT "Build SwiftMessage client"    ON)
option(BUILD_SERVER "Build SwiftMessage server"    ON)
option(BUILD_DB_TEST "Build database tests"        OFF)
option(BUILD_INTEGRATION_TESTS "Build integration tests" OFF)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED CONFIG COMPONENTS system thread serialization)

include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

if (BUILD_SERVER OR BUILD_DB_TEST OR BUILD_INTEGRATION_TESTS)
  find_package(PostgreSQL REQUIRED)
  find_library(PQXX_LIB pqxx)
  include_directories(${PostgreSQL_INCLUDE_DIRS})
endif()

if (BUILD_INTERFACE_TEST)
    find_package(Curses REQUIRED)
endif ()

# ── сервер ─────────────────────────────────────────────
set(SERVER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/server/server.cpp
    ${PROJECT_SOURCE_DIR}/src/server/connection_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/server/database_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/server/request_manager.cpp
)

# ── клиент ─────────────────────────────────────────────
set(CLIENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/client/client.cpp
    ${PROJECT_SOURCE_DIR}/src/client/connection_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/client/interface.cpp
)

if (BUILD_SERVER)
    add_executable(${SERVER_NAME}
        ${PROJECT_SOURCE_DIR}/src/server/main.cpp
        ${SERVER_SOURCES}
    )

    target_link_libraries(${SERVER_NAME}
        PRIVATE
        Boost::system
        Boost::thread
        Boost::serialization
        ${PQXX_LIB}
        ${PostgreSQL_LIBRARIES}
    )
endif()

if (BUILD_CLIENT)
    add_executable(${CLIENT_NAME}
        ${PROJECT_SOURCE_DIR}/src/client/main.cpp
        ${CLIENT_SOURCES}
    )

    target_link_libraries(${CLIENT_NAME}
        PRIVATE
        Boost::system
        Boost::thread
        Boost::serialization
    )
endif()


if (BUILD_DB_TEST)
    add_executable(${DB_TEST_NAME}
        tests/database/test_database_manager.cpp
        src/server/database_manager.cpp
    )

    target_link_libraries(${DB_TEST_NAME}
        PRIVATE
        ${PQXX_LIB}
        ${PostgreSQL_LIBRARIES}
    )
endif()

# Добавление интеграционных тестов
if (BUILD_INTEGRATION_TESTS)
    add_subdirectory(integration_tests)
endif()

if (BUILD_INTERFACE_TEST)
    add_executable(${INTERFACE_TEST_NAME}
        tests/screen_handler/test_interface.cpp
        src/screen_handler/interface.cpp
        src/screen_handler/sign_screen.cpp
        src/screen_handler/send_screen.cpp
        src/screen_handler/chat_screen.cpp
        src/screen_handler/greeting_screen.cpp
        src/screen_handler/abstract_screen.cpp
    )

    target_link_libraries(${INTERFACE_TEST_NAME}
        PRIVATE
        ${CURSES_LIBRARIES}
    )
endif ()
