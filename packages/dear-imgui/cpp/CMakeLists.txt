cmake_minimum_required(VERSION 3.8)

project(reactDearImgui)

add_definitions(-std=c++23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK_HOME}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(CMAKE_EXECUTABLE_SUFFIX ".mjs")

if (DEFINED EMSCRIPTEN)
	add_executable(reactDearImgui main.cpp
        plot.cpp
        widget.cpp
        reactimgui.cpp
        ${DEPS}/imgui/imgui.cpp
        ${DEPS}/imgui/imgui_draw.cpp
        ${DEPS}/imgui/imgui_tables.cpp
        ${DEPS}/imgui/imgui_widgets.cpp
        ${DEPS}/imgui/backends/imgui_impl_glfw.cpp
        ${DEPS}/imgui/backends/imgui_impl_wgpu.cpp
        ${DEPS}/implot/implot.cpp
        ${DEPS}/implot/implot_items.cpp
        ${DEPS}/wasm-glfw3-boilerplate/view.cpp
        ${DEPS}/wasm-glfw3-boilerplate/glwasm.cpp
        imguiview.cpp implotview.cpp
    )

    target_include_directories(reactDearImgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${DEPS}/imgui ${DEPS}/imgui/backends ${DEPS}/implot ${DEPS}/json/include ${DEPS}/wasm-glfw3-boilerplate)

	set_target_properties(reactDearImgui PROPERTIES COMPILE_FLAGS " -O3 -pthread -DIMGUI_DISABLE_FILE_FUNCTIONS -Wall -Wformat -s DISABLE_EXCEPTION_CATCHING=0")
    set_target_properties(reactDearImgui PROPERTIES LINK_FLAGS "-O3 -pthread -lembind --embind-emit-tsd interface.d.ts -s ENVIRONMENT='web,worker' -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s USE_GLFW=3 -s USE_WEBGPU=1 -s NO_FILESYSTEM=1 -s NO_EXIT_RUNTIME=0 -s STANDALONE_WASM=0 -s EXIT_RUNTIME=1 -s ASSERTIONS=1 -s STACK_OVERFLOW_CHECK=2 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s DISABLE_EXCEPTION_CATCHING=0 -sPTHREAD_POOL_SIZE=12 ")
    set_target_properties(reactDearImgui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../ts/src/lib/wasm)
endif()