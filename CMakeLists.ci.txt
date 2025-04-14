cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERVER_NAME ${PROJECT_NAME}_Server)
set(CLIENT_NAME ${PROJECT_NAME}_Client)

find_package(Boost REQUIRED COMPONENTS system thread serialization)

include_directories(${PROJECT_SOURCE_DIR}/include)

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
)

add_executable(${CLIENT_NAME}
    src/client/main.cpp
    src/client/connection_manager.cpp
    src/client/client.cpp
)

target_link_libraries(${CLIENT_NAME}
    PRIVATE
    Boost::system
    Boost::thread
    Boost::serialization
)