cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_NAME ${PROJECT_NAME}_Server)
set(CLIENT_NAME ${PROJECT_NAME}_Client)
set(DB_TEST_NAME ${PROJECT_NAME}_DbTest)

# Дефолтные значения тех, кого мы будем собирать
option(BUILD_CLIENT "Build SwiftMessage client"    ON)
option(BUILD_SERVER "Build SwiftMessage server"    ON)
option(BUILD_DB_TEST "Build database tests"        OFF)

find_package(Boost REQUIRED COMPONENTS system thread serialization)
find_package(Curses REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

if (BUILD_SERVER OR BUILD_DB_TEST)
  find_package(PostgreSQL REQUIRED)
  find_library(PQXX_LIB pqxx)
endif()

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
#        src/client/main.cpp
#        src/client/connection_manager.cpp
#        src/client/client.cpp
#        src/client/interface.cpp
        src/client/test.cpp
        src/screen_handler/interface.cpp
        src/screen_handler/sign_screen.cpp
        src/screen_handler/chat_screen.cpp
        src/screen_handler/greeting_screen.cpp
        src/screen_handler/abstract_screen.cpp
    )

    target_link_libraries(${CLIENT_NAME}
        PRIVATE
        Boost::system
        Boost::thread
        Boost::serialization
        ${CURSES_LIBRARIES}
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
