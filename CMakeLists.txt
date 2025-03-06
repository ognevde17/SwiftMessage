cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    /opt/homebrew/Cellar/boost/1.87.0/include
)

file(GLOB_RECURSE SOURCES  
    "src/server/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})