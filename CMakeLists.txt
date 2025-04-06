cmake_minimum_required(VERSION 3.22)
project(C C)

set(CMAKE_C_STANDARD 99)

set(GCC_COVERAGE_COMPILE_FLAGS "-ansi -Wall -pedantic -std=gnu99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(OPU-20465-Ex14 main.c skeleton.h initial_run.c initial_run.h skeleton.h
        utilities.c utilities.h hashtable.c hashtable.h first_run.c first_run.h )