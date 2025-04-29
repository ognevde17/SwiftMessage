cmake_minimum_required(VERSION 3.15)

# ─────────────────────────────── Google Test ────────────────────────────────
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(GTEST_SKIP_INSTALL ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ────────────────────────── бинарь с интеграционными тестами ───────────────
add_executable(SwiftMessage_Tests
    full_scenario_test.cpp
    ${SERVER_SOURCES}
    ${CLIENT_SOURCES}
)

target_include_directories(SwiftMessage_Tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(SwiftMessage_Tests PRIVATE
    GTest::gtest_main
    Boost::system Boost::thread Boost::serialization
    ${PQXX_LIB} ${PostgreSQL_LIBRARIES}
    pthread)

# ─────────────────────────── регистрация в CTest ───────────────────────────
enable_testing()
include(GoogleTest)
gtest_discover_tests(SwiftMessage_Tests)
