cmake_minimum_required(VERSION 3.16)
project(cpu-tracker)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -g")
add_executable(cpu-tracker main.c reader.c analyzer.c printer.c)