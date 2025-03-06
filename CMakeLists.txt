cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    /opt/homebrew/Cellar/boost/1.87.0/include
)

file(GLOB_RECURSE SERVER_SOURCES  
    "src/server/*.cpp"
)

file(GLOB_RECURSE CLIENT_SOURCES  
    "src/client/*.cpp"
)

add_executable(${PROJECT_NAME}_Server ${SERVER_SOURCES})

add_executable(${PROJECT_NAME}_Client ${CLIENT_SOURCES})