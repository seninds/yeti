cmake_minimum_required(VERSION 2.8)

project(yeti)

if(MINGW OR UNIX)
    add_definitions(-std=gnu++11)
endif()

if(MSVC)
    # d.polubotko (FIXME):
    #add_definitions(-std=c++11)
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE sources_yeti src/*.cc)
file(GLOB_RECURSE sources_yeti_h inc/yeti/*.h)

list(APPEND sources ${sources_yeti})
list(APPEND sources ${sources_yeti_h})

include_directories (inc)

if(MSVC)
    source_group("Source Files\\yeti" FILES ${sources_yeti})
    source_group("Header Files\\yeti" FILES ${sources_yeti_h})
endif()

add_library(yeti STATIC ${sources})

add_subdirectory (tests)


