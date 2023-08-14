cmake_minimum_required(VERSION 3.25)
project(chapter_5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(CGI)

add_executable(chapter_5
        CGI/process_poll.h)
