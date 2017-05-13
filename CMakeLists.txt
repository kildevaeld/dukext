cmake_minimum_required(VERSION 3.0)
project(dukext)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Os -fomit-frame-pointer -fstrict-aliasing")


set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

option(DUKEXT_NODE_COMPAT "Node" off)
option(DUKEXT_CRYPTO "Crypto" off)
option(DUKEXT_DEBUG "Debug" off)

include(cmake/duktape.cmake)

add_subdirectory(libs/uvext)
include_directories(${PROJECT_ROOT}/libs/uvext/src)
include_directories(${PROJECT_ROOT}/libs/uvext/libs/boringssl/include)

if (${DUKEXT_DEBUG})
    message(STATUS "Not in debug")
else()
    add_definitions(-DNDEBUG)

endif(${DUKEXT_DEBUG})

if (${DUKEXT_NODE_COMPAT})
    message(STATUS "Using node comp")
    add_definitions(-DDUKEXT_NODE_COMPAT)
endif(${DUKEXT_NODE_COMPAT})

if (${DUKEXT_CRYPTO})
    message(STATUS "Using node crypto")
    add_definitions(-DDUKEXT_CRYPTO)
endif(${DUKEXT_CRYPTO})

add_subdirectory(src)
include_directories(src)
add_subdirectory(example)
