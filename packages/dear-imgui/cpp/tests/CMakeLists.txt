# 'Google_test' is the subproject name
cmake_minimum_required(VERSION 3.8)

project(Google_tests)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# 'lib' is the folder with Google Test sources
add_subdirectory(../deps/googletest tests-binary)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(Google_Tests_run hello_test.cc)
target_link_libraries(Google_Tests_run gtest gtest_main)
