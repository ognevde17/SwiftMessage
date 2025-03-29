cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system thread serialization)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Server executable
add_executable(server
    src/server/main.cpp
    src/server/database_manager.cpp
    src/server/connection_manager.cpp
    src/server/request_manager.cpp
    src/server/server.cpp
)

# Link libraries for server
target_link_libraries(server
    PRIVATE
    Boost::system
    Boost::thread
    Boost::serialization
)

# Client executable
add_executable(client
    src/client/main.cpp
)

# Link libraries for client
target_link_libraries(client
    PRIVATE
    Boost::system
    Boost::thread
    Boost::serialization
)