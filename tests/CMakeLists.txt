cmake_minimum_required(VERSION 2.8)

project(yeti_test)

if(MINGW OR UNIX)
    set(CMAKE_CXX_FLAGS "-pthread -std=c++11 -Wall -Werror")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -std=c++11 -Wl,--no-as-needed")

include_directories (../inc)

###############################################################################
# GTest
ADD_SUBDIRECTORY (gtest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


###############################################################################
# YETI_LOG_LEVEL Test
add_executable(test_environment test_environment.cc)
target_link_libraries(test_environment yeti gtest_main pthread)
add_test(test_environment ${CMAKE_BINARY_DIR}/tests/test_environment)

add_executable(test_common_interface test_common_interface.cc)
target_link_libraries(test_common_interface yeti gtest_main pthread)
add_test(test_common_interface ${CMAKE_BINARY_DIR}/tests/test_common_interface)

add_executable(test_colors test_colors.cc)
target_link_libraries(test_colors yeti gtest_main pthread)

add_executable(test_performance test_performance.cc)
target_link_libraries(test_performance yeti gtest_main pthread)
