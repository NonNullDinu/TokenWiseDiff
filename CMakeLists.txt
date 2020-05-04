cmake_minimum_required(VERSION 3.14)
project(twdiff C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

file(GLOB twdiff_sources src/*.c)
file(GLOB twdiff_headers include/*.h)

add_executable(twdiff
        ${twdiff_sources}
        ${twdiff_headers})

target_include_directories(twdiff PRIVATE include)