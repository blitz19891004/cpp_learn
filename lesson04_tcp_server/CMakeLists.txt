cmake_minimum_required(VERSION 3.10)
project(lesson04_tcp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lesson04 main.cpp)

if(WIN32)
    target_link_libraries(lesson04 ws2_32)
endif()

