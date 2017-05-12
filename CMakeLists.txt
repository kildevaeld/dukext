cmake_minimum_required(VERSION 3.0)
project(dukext)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Os -fomit-frame-pointer -fstrict-aliasing")


set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/duktape.cmake)

add_subdirectory(libs/uvext)
include_directories(${PROJECT_ROOT}/libs/uvext/src)

add_subdirectory(src)
include_directories(src)
add_subdirectory(example)
