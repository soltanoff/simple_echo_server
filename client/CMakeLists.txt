cmake_minimum_required(VERSION 2.0)
project(client)

set(CMAKE_CXX_STANDARD 14)

set(
        SOURCE_FILES
        main.cpp
        src/config/config.cpp
        src/core/TCP/tcp.cpp
        src/core/TCP/tcp.h
        src/core/UDP/udp.cpp
        src/core/UDP/udp.h
        src/core/client.cpp
        src/core/client.h)

add_definitions(-std=c++14 -O0 -g)

add_executable(client ${SOURCE_FILES})
