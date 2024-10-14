# 'Google_test' is the subproject name
cmake_minimum_required(VERSION 3.8)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_TOOLCHAIN_FILE deps/vcpkg/scripts/buildsystems/vcpkg.cmake)
if (WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows")
    add_compile_options(/bigobj)
else()
    set(VCPKG_TARGET_TRIPLET "x64-linux")
    # TODO: What about other platforms?
endif()

project(Google_tests)

find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)

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
