cmake_minimum_required(VERSION 3.0)
project(dukext)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/duktape.cmake)

add_subdirectory(libs/uvext)
include_directories(${PROJECT_ROOT}/libs/uvext/src)

add_subdirectory(src)
include_directories(src)
add_subdirectory(example)
