cmake_minimum_required(VERSION 3.10)
project(lesson08_doip_flash)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lesson08 main.cpp)

if(WIN32)
    target_link_libraries(lesson08 ws2_32)
endif()

