cmake_minimum_required(VERSION 3.10)

project(爱吃番茄的勇者Abel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.cpp")

add_executable(爱吃番茄的勇者Abel ${SOURCES})

if(MSVC)
    add_compile_options(/utf-8)
endif()