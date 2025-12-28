cmake_minimum_required(VERSION 3.10)
project(lesson01_basics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lesson01 main.cpp)

if(WIN32)
    target_compile_options(lesson01 PRIVATE /utf-8)
endif()

