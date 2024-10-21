#ifndef IMGUI_VIEW
#define IMGUI_VIEW

#include <format>

#include "IconsFontAwesome6.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"

#ifdef __EMSCRIPTEN__
#include "imgui_impl_wgpu.h"
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>
#endif

#include <texture_helpers.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>


#include "./shared.h"

class ReactImgui;

using json = nlohmann::json;

class ImGuiView {
    protected:
        std::string m_rawFontDefs;

        ReactImgui* m_reactImgui;

        GLFWwindow* m_glfwWindow;

        const char* m_glWindowTitle;

        int m_initial_window_width = 400;
        int m_initial_window_height = 300;
        int m_window_width = m_initial_window_width;
        int m_window_height = m_initial_window_height;

        ImGuiContext* m_imGuiCtx;

        std::unordered_map<std::string, std::unordered_map<int, int>, StringHash, std::equal_to<>> m_fontDefMap;

        // static constexpr ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

        void LoadFontsFromDefs();

    #ifdef __EMSCRIPTEN__
        std::unique_ptr<char[]> m_canvasSelector;
        wgpu::Instance m_instance;
        WGPUColor m_clearColor;
        WGPUDevice m_device;
        WGPUQueue m_queue;
        WGPUSurface m_wgpu_surface;
        WGPUTextureFormat m_wgpu_preferred_fmt = WGPUTextureFormat_RGBA8Unorm;
        WGPUSwapChain m_wgpu_swap_chain;
        int m_wgpu_swap_chain_width = 0;
        int m_wgpu_swap_chain_height = 0;
    #else

    #endif

    public:
        ImGuiView(ReactImgui* reactImgui, const char* newWindowId, const char* newGlWindowTitle, std::string rawFontDefs);

        bool m_shouldLoadDefaultStyle;

        std::vector<ImFont*> m_loadedFonts;

        const char* m_windowId;

        bool LoadTexture(const void* data, int numBytes, Texture* texture);

        // virtual void PrepareForRender() = 0;
        // virtual void Render(int window_width, int window_height) = 0;

        void BeginRenderLoop();

        int GetFontIndex(const std::string& fontName, int fontSize);

        [[nodiscard]] bool IsFontIndexValid(int fontIndex) const;

        void SetFontDefault(int fontIndex) const;

        // Only call this after having verified that fontIndex is valid (use IsFontIndexValid())
        void PushFont(int fontIndex) const;

        void PopFont();

        ImGuiStyle& GetStyle();

        void SetCurrentContext() {
            ImGui::SetCurrentContext(m_imGuiCtx);
        }

        virtual void SetUp();

        void InitGlfw();

    #ifdef __EMSCRIPTEN__
        bool InitWGPU();
        void RenderDrawData(WGPURenderPassEncoder pass);
        void CreateSwapChain(int width, int height);
        void SetDeviceAndStart(WGPUDevice& cDevice);
        void RequestDevice(wgpu::Instance wgpuInstance, ImGuiView* glWasmInstance);
        virtual void Init(std::string& cs);
    #else
        void RenderDrawData();
        virtual void Init();
    #endif
        void HandleScreenSizeChanged();

        void PerformRendering();

        void CleanUp();

        void SetWindowSize(int width, int height);

        json GetAvailableFonts();
};

#endif