cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)

project(RaftOrderBook)

set(SOURCES
    src/main.cpp
    src/utils/parse_args.cpp
)

add_executable(raft_order_book ${SOURCES})
