cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_NAME ${PROJECT_NAME}_Server)
set(CLIENT_NAME ${PROJECT_NAME}_Client)
set(DB_TEST_NAME ${PROJECT_NAME}_DbTest)
set(INTERFACE_TEST_NAME ${PROJECT_NAME}_InterfaceTest)

# Дефолтные значения тех, кого мы будем собирать
#option(BUILD_CLIENT "Build SwiftMessage client"    OFF)
#option(BUILD_SERVER "Build SwiftMessage server"    OFF)
#option(BUILD_DB_TEST "Build database tests"        OFF)
#option(BUILD_INTERFACE_TEST "Build interface test" ON)

find_package(Boost REQUIRED COMPONENTS system thread serialization)

include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

if (BUILD_SERVER OR BUILD_DB_TEST)
  find_package(PostgreSQL REQUIRED)
  find_library(PQXX_LIB pqxx)
endif()

if (BUILD_INTERFACE_TEST)
    find_package(Curses REQUIRED)
endif ()

if (BUILD_SERVER)
    add_executable(${SERVER_NAME}
        src/server/main.cpp
        src/server/database_manager.cpp
        src/server/connection_manager.cpp
        src/server/request_manager.cpp
        src/server/server.cpp
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
        src/client/main.cpp
        src/client/connection_manager.cpp
        src/client/client.cpp
        src/client/interface.cpp
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

if (BUILD_INTERFACE_TEST)
    add_executable(${INTERFACE_TEST_NAME}
        tests/screen_handler/test_interface.cpp
        src/screen_handler/interface.cpp
        src/screen_handler/sign_screen.cpp
        src/screen_handler/chat_screen.cpp
        src/screen_handler/greeting_screen.cpp
        src/screen_handler/abstract_screen.cpp
    )

    target_link_libraries(${INTERFACE_TEST_NAME}
        PRIVATE
        ${CURSES_LIBRARIES}
    )
endif ()
