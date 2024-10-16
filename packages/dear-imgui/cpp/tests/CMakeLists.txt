# 'Google_test' is the subproject name
cmake_minimum_required(VERSION 3.8)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_TOOLCHAIN_FILE ../deps/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_ARCHITECTURE "x64")

if (WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows")
else()
    set(VCPKG_TARGET_TRIPLET "x64-linux")
endif()

project(Google_tests)

set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/../deps)

find_package(imgui CONFIG REQUIRED)
find_package(implot REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Stb REQUIRED)
find_package(ada CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# 'lib' is the folder with Google Test sources
add_subdirectory(../deps/googletest tests-binary)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB YOGA_SRC CONFIGURE_DEPENDS
        ${DEPS}/yoga/yoga/*.cpp
        ${DEPS}/yoga/yoga/**/*.cpp)

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(Google_Tests_run
        ${DEPS}/css-color-parser-cpp/csscolorparser.hpp
        ${DEPS}/css-color-parser-cpp/csscolorparser.cpp

        ${YOGA_SRC}

        ../src/shared.cpp
        ../src/color_helpers.cpp
        ../src/yoga_helpers.cpp
        ../src/imgui_helpers.cpp
        ../src/element/layout_node.cpp

        shared_functions_test.cpp
        color_helpers_test.cpp
        yoga_helpers_test.cpp
        imgui_helpers_test.cpp
        element/layout_node_test.cpp
)

target_include_directories(
        Google_Tests_run PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
#        ${DEPS}/wasm-glfw3-boilerplate
#        ${DEPS}/IconFontCppHeaders
#        ${DEPS}/ReactivePlusPlus/src/rpp

#        ${Stb_INCLUDE_DIR}
        ${DEPS}/yoga
        ${DEPS}/css-color-parser-cpp

#        ${DEPS}/osm-static-map-generator/cpp
#        ${DEPS}/osm-static-map-generator/cpp/deps/leptonica/src
#        ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff
#        ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/port
)

target_link_libraries(Google_Tests_run gtest gmock gmock_main gtest_main imgui::imgui implot::implot nlohmann_json::nlohmann_json ada::ada)
