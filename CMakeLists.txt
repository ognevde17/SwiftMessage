cmake_minimum_required(VERSION 3.10)

project(SwiftMessage)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SERVER_NAME ${PROJECT_NAME}_Server)
set(CLIENT_NAME ${PROJECT_NAME}_Client)

# Настройка Boost
set(BOOST_ROOT "/home/dan/project/boost_1_87_0")
set(Boost_INCLUDE_DIR "/home/dan/project/boost_1_87_0")
set(Boost_LIBRARY_DIR "/home/dan/project/boost_1_87_0/stage/lib")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_SYSTEM_PATHS ON)

# Добавить недостающие компоненты
find_package(Boost REQUIRED COMPONENTS system thread serialization chrono atomic)

# Включить директории Boost в пути включения
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
)

add_executable(${SERVER_NAME}
    src/server/main.cpp
    src/server/database_manager.cpp
    src/server/connection_manager.cpp
    src/server/request_manager.cpp
    src/server/server.cpp
)

target_link_libraries(${SERVER_NAME}
    PRIVATE
    ${Boost_LIBRARY_DIR}/libboost_system.a
    ${Boost_LIBRARY_DIR}/libboost_thread.a
    ${Boost_LIBRARY_DIR}/libboost_serialization.a
    ${Boost_LIBRARY_DIR}/libboost_chrono.a
    ${Boost_LIBRARY_DIR}/libboost_atomic.a
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
