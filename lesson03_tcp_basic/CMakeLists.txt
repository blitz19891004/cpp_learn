cmake_minimum_required(VERSION 3.10)
project(lesson03_tcp_basic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lesson03 main.cpp)

if(WIN32)
    target_link_libraries(lesson03 ws2_32)
endif()

