cmake_minimum_required(VERSION 3.8)

set(CMAKE_GENERATOR "Ninja")
set(CMAKE_TOOLCHAIN_FILE deps/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET "wasm32-emscripten")


if(NOT DEFINED ENV{EMSDK})
    message(FATAL_ERROR "Environment variable EMSDK is not defined. Follow the README steps and source emsdk_env.* from the EMSDK root directory in this shell before running CMake")
endif()


set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

execute_process(
        COMMAND "npm install"
        WORKING_DIRECTORY "$ENV{EMSDK}/upstream/emscripten"
)

project(reactDearImgui LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(Stb REQUIRED)
find_package(ada CONFIG REQUIRED)
# find_package(RPP REQUIRED)

set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(CMAKE_EXECUTABLE_SUFFIX ".mjs")

file(GLOB LEPTONICA_SRC "${DEPS}/osm-static-map-generator/cpp/deps/leptonica/src/*.c")

set(OSM_STATIC_MAP_GENERATOR_SRC
    ${DEPS}/osm-static-map-generator/cpp/shared.cpp
    ${DEPS}/osm-static-map-generator/cpp/tiledownloader.cpp
    ${DEPS}/osm-static-map-generator/cpp/mapgenerator.cpp
)

set(LIBTIFF_SRC
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_aux.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_close.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_codec.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_color.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_compress.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_dir.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_dirinfo.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_dirread.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_dirwrite.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_dumpmode.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_error.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_extension.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_fax3.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_fax3sm.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_flush.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_getimage.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_hash_set.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_jbig.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_jpeg.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_jpeg_12.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_lerc.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_luv.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_lzma.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_lzw.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_next.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_ojpeg.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_open.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_packbits.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_pixarlog.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_predict.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_print.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_read.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_strip.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_swab.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_thunder.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_tile.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_version.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_warning.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_webp.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_write.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_zip.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_zstd.c
    ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff/tif_unix.c
)

if (DEFINED EMSCRIPTEN)
    # Copied from yoga's CMakeLists.txt 
    file(GLOB YOGA_SRC CONFIGURE_DEPENDS
        ${DEPS}/yoga/yoga/*.cpp
        ${DEPS}/yoga/yoga/**/*.cpp)

	add_executable(reactDearImgui
        src/main.cpp
        src/shared.cpp
        src/element/layout_node.cpp
        src/element/element.cpp
        src/widget/widget.cpp
        src/widget/styled_widget.cpp
        src/widget/button.cpp
        src/widget/checkbox.cpp
        src/widget/child.cpp
        src/widget/clipped_multi_line_text_renderer.cpp
        src/widget/collapsing_header.cpp
        src/widget/combo.cpp
        src/widget/group.cpp
        src/widget/image.cpp
        src/widget/input_text.cpp
        src/widget/item_tooltip.cpp
        src/widget/map_view.cpp
        src/widget/multi_slider.cpp
        src/widget/plot_line.cpp
        src/widget/plot_candlestick.cpp
        src/widget/separator.cpp
        src/widget/separator_text.cpp
        src/widget/slider.cpp
        src/widget/table.cpp
        src/widget/tabs.cpp
        src/widget/text.cpp
        src/widget/text_wrap.cpp
        src/widget/tree_node.cpp
        src/widget/window.cpp
        src/reactimgui.cpp
        src/imguiview.cpp
        src/implotview.cpp

        ${DEPS}/wasm-glfw3-boilerplate/view.cpp
        ${DEPS}/wasm-glfw3-boilerplate/glwasm.cpp
        ${OSM_STATIC_MAP_GENERATOR_SRC}
        ${LEPTONICA_SRC}
        ${LIBTIFF_SRC}
        ${YOGA_SRC}
        ${DEPS}/css-color-parser-cpp/csscolorparser.hpp
        ${DEPS}/css-color-parser-cpp/csscolorparser.cpp
    )

    target_include_directories(
        reactDearImgui PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${DEPS}/wasm-glfw3-boilerplate 
        ${DEPS}/IconFontCppHeaders 
        ${DEPS}/ReactivePlusPlus/src/rpp

        ${Stb_INCLUDE_DIR}
        ${DEPS}/yoga
        ${DEPS}/css-color-parser-cpp

        ${DEPS}/osm-static-map-generator/cpp
        ${DEPS}/osm-static-map-generator/cpp/deps/leptonica/src
        ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/libtiff
        ${DEPS}/osm-static-map-generator/cpp/deps/libtiff/port
    )

    # https://github.com/emscripten-core/emscripten/issues/18476
    # -c -fexperimental-library

	set_target_properties(reactDearImgui PROPERTIES COMPILE_FLAGS "-O3 -Wall -Wformat  -Wno-implicit-int -s DISABLE_EXCEPTION_CATCHING=0 --use-port=libjpeg --use-port=libpng")
    set_target_properties(reactDearImgui PROPERTIES LINK_FLAGS "--no-heap-copy --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../assets@/assets -O3 -lembind --emit-tsd interface.d.ts --use-port=libjpeg --use-port=libpng -lhtml5.js -lhtml5_webgl.js -lglfw.js -s ENVIRONMENT='web' -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s USE_GLFW=3 -s USE_WEBGPU=1 -sFETCH -s NO_EXIT_RUNTIME=0 -s STANDALONE_WASM=0 -s EXIT_RUNTIME=1 -s ASSERTIONS=1 -s STACK_OVERFLOW_CHECK=2 -s DISABLE_EXCEPTION_CATCHING=0 -s SINGLE_FILE=1")
    set_target_properties(reactDearImgui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ts/src/lib/wasm)
    target_link_libraries(reactDearImgui PRIVATE imgui::imgui implot::implot nlohmann_json::nlohmann_json ada::ada)

    # Reinstate line below once ReactivePlusPlus 2.1.2 gets released (preferably in the vcpkg repo)
    # target_link_libraries(reactDearImgui PRIVATE imgui::imgui implot::implot nlohmann_json::nlohmann_json RPP::rpp RPP::rppqt)

    # This currently breaks custom fonts - known issue
    # target_compile_definitions(reactDearImgui PUBLIC IMGUI_USE_WCHAR32)
endif()
