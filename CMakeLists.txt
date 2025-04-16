cmake_minimum_required(VERSION 3.22)
project(C C)

set(CMAKE_C_STANDARD 99)

set(GCC_COVERAGE_COMPILE_FLAGS "-ansi -Wall -pedantic -std=gnu99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(OPU-20465-Ex14 assembler.c "Header Files/skeleton.h" initial_run.c "Header Files/initial_run.h" "Header Files/skeleton.h"
        utilities.c "Header Files/utilities.h" hash_table.c "Header Files/hash_table.h" first_run.c
        "Header Files/first_run.h" linked_list.c "Header Files/linked_list.h" second_run.c "Header Files/second_run.h")