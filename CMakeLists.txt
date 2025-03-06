cmake_minimum_required(VERSION 3.10)
project(SwiftMessage)  # Замените "MyProject" на имя вашего проекта

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление путей к заголовкам
include_directories(
    /opt/homebrew/Cellar/boost/1.87.0/include
)

# Сбор исходных файлов
file(GLOB_RECURSE SOURCES  
    "src/server/*.cpp"
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})