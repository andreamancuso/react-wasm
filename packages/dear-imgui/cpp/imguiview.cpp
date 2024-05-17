#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

#include "./imguiview.h"


WGPUColor ImGuiView::GetClearColor() {
    return m_clearColor;
};