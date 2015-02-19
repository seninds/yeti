cmake_minimum_required(VERSION 2.8)

project(yeti_test)

if(MINGW OR UNIX)
    add_definitions(-pthread -std=gnu++11 -Wall -Werror)
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -std=gnu++11 -Wl,--no-as-needed")

include_directories (../inc)

###############################################################################
# GTest
ADD_SUBDIRECTORY (gtest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}) 


###############################################################################
# YETI_LOG_LEVEL Test
add_executable(yeti_log_level yeti_log_level.cc)
target_link_libraries(yeti_log_level yeti gtest_main pthread)
add_test(yeti_log_level ${CMAKE_BINARY_DIR}/tests/yeti_log_level)

add_executable(yeti_interface yeti_interface.cc)
target_link_libraries(yeti_interface yeti gtest_main pthread)
add_test(yeti_interface ${CMAKE_BINARY_DIR}/tests/yeti_interface)

add_executable(yeti_set_log_colored yeti_set_log_colored.cc)
target_link_libraries(yeti_set_log_colored yeti gtest_main pthread)
