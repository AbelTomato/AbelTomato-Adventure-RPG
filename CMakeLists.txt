cmake_minimum_required(VERSION 3.10)
project(TomatoHeroAbel VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")

add_executable(TomatoHeroAbel ${SOURCES})