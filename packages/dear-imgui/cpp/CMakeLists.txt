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

# add_definitions(-std=c++23)
set(CMAKE_CXX_STANDARD 23)

set(MLN_WITH_CORE_ONLY ON)
set(MLN_WITH_WERROR OFF)
set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/deps)

set(MAPLIBRE_BUILD_DIR ${DEPS}/maplibre-native-fork/build)
set(WEBP_BUILD_DIR ${DEPS}/maplibre-native-fork/vendor/libwebp/build)

set(MAPLIBRE_FILES "${MAPLIBRE_BUILD_DIR}/libmbgl-core.a ${MAPLIBRE_BUILD_DIR}/libmbgl-vendor-csscolorparser.a ${MAPLIBRE_BUILD_DIR}/libmbgl-vendor-icu.a ${MAPLIBRE_BUILD_DIR}/libmbgl-vendor-nunicode.a")

set(WEBP_FILES "${WEBP_BUILD_DIR}/libexampleutil.a ${WEBP_BUILD_DIR}/libextras.a ${WEBP_BUILD_DIR}/libimagedec.a ${WEBP_BUILD_DIR}/libimagedec.a ${WEBP_BUILD_DIR}/libimageenc.a ${WEBP_BUILD_DIR}/libimageioutil.a ${WEBP_BUILD_DIR}/libsharpyuv.a ${WEBP_BUILD_DIR}/libwebp.a ${WEBP_BUILD_DIR}/libwebpdecoder.a ${WEBP_BUILD_DIR}/libwebpdemux.a ${WEBP_BUILD_DIR}/libwebpmux.a")

find_package(OpenGL REQUIRED)
find_package(PkgConfig)
    
# pkg_check_modules(libunwind REQUIRED IMPORTED_TARGET GLOBAL libunwind)
    
# find_package(nlohmann_json REQUIRED)
# find_package(imgui REQUIRED)
# find_package(implot REQUIRED)
# find_package(RPP REQUIRED)

set(CMAKE_EXECUTABLE_SUFFIX ".mjs")

if (DEFINED EMSCRIPTEN)
	add_executable(reactDearImgui main.cpp
        ${DEPS}/imgui/imgui.cpp
        ${DEPS}/imgui/imgui_draw.cpp
        ${DEPS}/imgui/imgui_tables.cpp
        ${DEPS}/imgui/imgui_widgets.cpp
        ${DEPS}/imgui/backends/imgui_impl_glfw.cpp
        ${DEPS}/imgui/backends/imgui_impl_wgpu.cpp
        ${DEPS}/implot/implot.cpp
        ${DEPS}/implot/implot_items.cpp
        shared.cpp
        widget.cpp
        reactimgui.cpp
        ${DEPS}/wasm-glfw3-boilerplate/view.cpp
        ${DEPS}/wasm-glfw3-boilerplate/glwasm.cpp
        imguiview.cpp
        implotview.cpp
    )

    target_include_directories(reactDearImgui PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR} 
        ${DEPS}/imgui 
        ${DEPS}/imgui/backends
        ${DEPS}/implot 
        ${DEPS}/json/include
        ${DEPS}/wasm-glfw3-boilerplate 
        ${DEPS}/IconFontCppHeaders 
        ${DEPS}/libwebp/src
        ${DEPS}/ReactivePlusPlus/src/rpp 
        ${DEPS}/maplibre-native-fork/include 
        ${DEPS}/maplibre-native-fork/vendor/nunicode/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/cheap-ruler-cpp/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/geojson-vt-cpp/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/geojson-hpp/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/geometry.hpp/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/shelf-pack-cpp/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/supercluster.hpp/include
        ${DEPS}/maplibre-native-fork/vendor/mapbox-base/deps/variant/include
        ${DEPS}/maplibre-native-fork/platform/default/include
        # ${DEPS}/maplibre-native-fork/vendor/boost/include
        # ${DEPS}/unique_resource
        # ${DEPS}/maplibre-native/platform/default/include
    )

    # https://github.com/emscripten-core/emscripten/issues/18476
    # -c -fexperimental-library

    # target_compile_definitions(
    #     reactDearImgui
    #     PRIVATE EMCC_FORCE_STDLIBS=1
    # )

	# set_target_properties(reactDearImgui PROPERTIES COMPILE_FLAGS "-O3 -pthread -fPIC -Wno-error=deprecated-declarations -Wformat -s DISABLE_EXCEPTION_CATCHING=0 --use-port=libpng --use-port=sqlite3 --use-port=libjpeg --use-port=zlib")
    # set_target_properties(reactDearImgui PROPERTIES LINK_FLAGS "--no-heap-copy -pthread -fPIC --use-port=sqlite3 --use-port=libpng --use-port=libjpeg --use-port=zlib -O3 -lembind -lhtml5.js -lhtml5_webgl.js -lglfw.js -s DISABLE_EXCEPTION_CATCHING=0 -s ENVIRONMENT='web,worker' -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s USE_GLFW=3 -s USE_WEBGPU=1 -s NO_EXIT_RUNTIME=0 -s STANDALONE_WASM=0 -s EXIT_RUNTIME=1 -s ASSERTIONS=1 -s STACK_OVERFLOW_CHECK=2 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -sFULL_ES3 -s DISABLE_EXCEPTION_CATCHING=0 -s SINGLE_FILE=0 -sERROR_ON_UNDEFINED_SYMBOLS=0 -s EXPORT_ALL=1 ${CMAKE_CURRENT_SOURCE_DIR}/deps/maplibre-native-fork/build/libmbgl-core.a")

    set_target_properties(reactDearImgui PROPERTIES COMPILE_FLAGS "-O0 -pthread -Wall -Wformat -s DISABLE_EXCEPTION_CATCHING=0 --use-port=libpng --use-port=libjpeg --use-port=sqlite3")
    set_target_properties(reactDearImgui PROPERTIES LINK_FLAGS "-gsource-map --no-heap-copy -pthread --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../fonts@/fonts -O0 -lembind --use-port=libpng --use-port=libjpeg --use-port=sqlite3 --emit-tsd interface.d.ts -lhtml5.js -lhtml5_webgl.js -lglfw.js -s ENVIRONMENT='web,worker' -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=0 -sINITIAL_MEMORY=1024MB -s WASM=1 -s USE_GLFW=3 -s USE_WEBGPU=1 -s NO_EXIT_RUNTIME=0 -s STANDALONE_WASM=0 -s EXIT_RUNTIME=1 -s ASSERTIONS=2 -s STACK_OVERFLOW_CHECK=2 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s DISABLE_EXCEPTION_CATCHING=0 -s SINGLE_FILE=0 -sFULL_ES3 -sSTACK_SIZE=20MB -sOFFSCREENCANVAS_SUPPORT -sPTHREAD_POOL_SIZE_STRICT=0 ${MAPLIBRE_FILES} ${WEBP_FILES}")
    set_target_properties(reactDearImgui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ts/src/lib/wasm)

    set_target_properties(reactDearImgui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ts/src/lib/wasm)
    # target_link_libraries(reactDearImgui PRIVATE implot::implot nlohmann_json::nlohmann_json)

    # Reinstate line below once ReactivePlusPlus 2.1.2 gets released (preferably in the vcpkg repo)
    # target_link_libraries(reactDearImgui PRIVATE imgui::imgui implot::implot nlohmann_json::nlohmann_json RPP::rpp RPP::rppqt)

    # This currently breaks custom fonts - known issue
    # target_compile_definitions(reactDearImgui PUBLIC IMGUI_USE_WCHAR32)
endif()
