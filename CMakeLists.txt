cmake_minimum_required(VERSION 2.8)

project(yeti)

set(GTEST_DIR tests/gtest)

if(MINGW OR UNIX)
    set(CMAKE_CXX_FLAGS "-pthread -std=c++11 -Wall -Werror")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE sources_yeti src/*.cc)
file(GLOB_RECURSE headers_yeti inc/yeti/*.h)

include_directories (inc)
include_directories (.)

add_library(yeti STATIC ${sources_yeti} ${headers_yeti})

enable_testing()
add_subdirectory(tests)


