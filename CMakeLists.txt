cmake_minimum_required(VERSION 3.14)
project(twdiff C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(twdiff
        main.c
        syntax.h
        comparator.h comparator.c
        comparing_algorithm.h comparing_algorithm.c
        tokens.h syntax.c)