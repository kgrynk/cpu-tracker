cmake_minimum_required(VERSION 3.0)
project(cpu-tracker)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
add_executable(cpu-tracker main.c reader/reader.c analyzer/analyzer.c printer/printer.c)