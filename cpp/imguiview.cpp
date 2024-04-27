#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>

#include "./view.cpp"

class ImGuiView : public View {
    protected:
        WGPUColor clearColor;

        ImGuiContext* imGuiCtx;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

    public:
        ImGuiView(const char* newWindowId, const char* newGlWindowTitle) : View(newWindowId, newGlWindowTitle) {
            imGuiCtx = ImGui::CreateContext();

            ImVec4 v4 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            clearColor = { v4.x * v4.w, v4.y * v4.w, v4.z * v4.w, v4.w };
        }

        WGPUColor GetClearColor() {
            return clearColor;
        }

        void SetCurrentContext() {
            ImGui::SetCurrentContext(imGuiCtx);
        }

        void SetUp(std::string canvasSelector, WGPUDevice device, GLFWwindow* glfwWindow, WGPUTextureFormat wgpu_preferred_fmt) {
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
            ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback(canvasSelector.c_str());
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
            ImGui::DestroyContext(imGuiCtx);

            ImGui_ImplWGPU_Shutdown();
            ImGui_ImplGlfw_Shutdown();
        }
};

