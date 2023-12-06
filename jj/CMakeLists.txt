cmake_minimum_required(VERSION 3.27)

project(aoc23)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_library(LIBASAN NAMES "asan" "libasan" "libasan.so" "libasan.so.8")
if(NOT LIBASAN)
	message(FATAL_ERROR "libasan not found!")
endif()

find_package(benchmark REQUIRED)

add_subdirectory(day02)
add_subdirectory(day03)
