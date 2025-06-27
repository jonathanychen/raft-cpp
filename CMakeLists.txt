cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(RaftKeyValueStore)

# == Main Application ==

set(SOURCES
    src/main.cpp
    src/utils/parse_args.cpp
    src/replica/replica.cpp
)
include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(3700kvstore ${SOURCES})

target_link_libraries(3700kvstore PRIVATE nlohmann_json::nlohmann_json)

# == Testing ==
enable_testing()

set(SOURCES
    src/utils/parse_args.cpp
    tst/test_parse_args.cpp
)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/35b75a2cba6ef72b7ce2b6b94b05c54ca07df866.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
    3700kvstore_test
    ${SOURCES}
)

target_link_libraries(
    3700kvstore_test
    PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(3700kvstore_test)
