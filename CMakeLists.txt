cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak -fno-sanitize-recover=all -g -lm")

find_package(Boost REQUIRED)
find_package(Curses REQUIRED)

file(GLOB_RECURSE SERVER_SOURCES  
    "src/server/*.cpp"
)

file(GLOB_RECURSE CLIENT_SOURCES  
    "src/client/main.cpp"
)

file(GLOB_RECURSE FRONT_SOURCES
    "src/screens/test.cpp"
)

add_executable(${PROJECT_NAME}_Server ${SERVER_SOURCES})
target_link_libraries(${PROJECT_NAME}_Server ${CURSES_LIBRARIES})

add_executable(${PROJECT_NAME}_Client ${CLIENT_SOURCES})
target_link_libraries(${PROJECT_NAME}_Client ${CURSES_LIBRARIES})