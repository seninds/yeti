cmake_minimum_required(VERSION 2.8)

project(yeti_test)

if(MINGW OR UNIX)
    add_definitions(-std=gnu++11)
endif()

if(MSVC)
    # d.polubotko (FIXME):
    #add_definitions(-std=c++11)
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE sources_yeti_test ./*.cc)

list(APPEND sources ${sources_yeti_test})

include_directories (../inc) 

if(MSVC)
    source_group("Source Files\\yeti_test" FILES ${sources_yeti_test})
endif()


add_executable( yeti_test ${sources})

target_link_libraries(yeti_test yeti)




