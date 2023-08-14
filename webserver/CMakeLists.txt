cmake_minimum_required(VERSION 3.25)
project(webserver)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

include_directories(.)

add_executable(webserver
        http_conn.cpp
        http_conn.h locker.h main.cpp threadpool.h)
