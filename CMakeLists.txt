cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(RaftOrderBook)

# == Main Application ==

set(SOURCES
    src/main.cpp
    src/utils/parse_args.cpp
)

add_executable(raft_order_book ${SOURCES})

# == Testing ==
enable_testing()

set(SOURCES
    src/utils/parse_args.cpp
    tst/test_parse_args.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/35b75a2cba6ef72b7ce2b6b94b05c54ca07df866.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
    raft_order_book_test
    ${SOURCES}
)

target_link_libraries(
    raft_order_book_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(raft_order_book_test)
