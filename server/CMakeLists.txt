cmake_minimum_required(VERSION 2.0)
project(server)

set(CMAKE_CXX_STANDARD 14)

set(
        SOURCE_FILES
        main.cpp
        src/config/config.cpp
        src/core/TCP/clientcontroller.cpp
        src/core/TCP/tcp.cpp
        src/core/TCP/tcp.h
        src/core/UDP/udp.cpp
        src/core/UDP/udp.h
        src/core/listener.h)

add_definitions(-std=c++14 -O0 -g)

add_executable(server ${SOURCE_FILES})
target_link_libraries(server pthread)

