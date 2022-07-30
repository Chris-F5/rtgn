cmake_minimum_required(VERSION 3.10)

project(rtgn)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(test)
add_subdirectory(examples/top_down_shooter)

add_library(rtgn STATIC
    src/temp.c
    )

target_include_directories(rtgn
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
