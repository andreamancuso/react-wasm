#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

#include "./view.cpp"

#pragma once

class ImGuiView : public View {
    protected:
        WGPUColor m_clearColor;

        ImGuiContext* m_imGuiCtx;

        ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

    public:
        ImGuiView(const char* newWindowId, const char* newGlWindowTitle) : View(newWindowId, newGlWindowTitle) {
            m_imGuiCtx = ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO(); (void)io;

            io.Fonts->AddFontFromFileTTF("fonts/dejavu/DejaVuSans.ttf", 14.0f);

            ImVec4 v4 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            m_clearColor = { v4.x * v4.w, v4.y * v4.w, v4.z * v4.w, v4.w };
        }

        WGPUColor GetClearColor();

        void SetCurrentContext() {
            ImGui::SetCurrentContext(m_imGuiCtx);
        }

        void SetUp(char* pCanvasSelector, WGPUDevice device, GLFWwindow* glfwWindow, WGPUTextureFormat wgpu_preferred_fmt) {
            IMGUI_CHECKVERSION();

            ImGui_ImplWGPU_InitInfo init_info;
            init_info.Device = device;
            init_info.NumFramesInFlight = 3;
            init_info.RenderTargetFormat = wgpu_preferred_fmt;
            init_info.DepthStencilFormat = WGPUTextureFormat_Undefined;
            ImGui_ImplWGPU_Init(&init_info);

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOther(glfwWindow, true);
        #ifdef __EMSCRIPTEN__
            ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback(pCanvasSelector);
        #endif
        }

        void HandleScreenSizeChanged() {
            ImGui_ImplWGPU_InvalidateDeviceObjects();
            ImGui_ImplWGPU_CreateDeviceObjects();
        }

        void RenderDrawData(WGPURenderPassEncoder pass) {
            ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
        }

        void CleanUp() {
            ImGui::DestroyContext(m_imGuiCtx);

            ImGui_ImplWGPU_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
};

