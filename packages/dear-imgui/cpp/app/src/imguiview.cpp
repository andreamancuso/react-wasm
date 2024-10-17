#include <webgpu/webgpu.h>

#include "./imguiview.h"


WGPUColor ImGuiView::GetClearColor() {
    return m_clearColor;
};