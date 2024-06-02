#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <format>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>
#include <nlohmann/json.hpp>

#include "./view.cpp"

#pragma once

using json = nlohmann::json;

class ImGuiView : public View {
    protected:
        WGPUColor m_clearColor;

        ImGuiContext* m_imGuiCtx;

        ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

        std::vector<ImFont*> m_loadedFonts;

        std::unordered_map<std::string, std::unordered_map<int, int>> m_fontDefMap;
    public:

        ImGuiView(
            const char* newWindowId, 
            const char* newGlWindowTitle, 
            std::string& rawFontDefs) : View(newWindowId, newGlWindowTitle) {
            m_imGuiCtx = ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO(); (void)io;

            auto fontDefs = json::parse(rawFontDefs);

            if (fontDefs.is_object() && fontDefs.contains("defs")) {
                if (fontDefs["defs"].is_array()) {
                    for (auto& [key, item] : fontDefs["defs"].items()) {
                        if (item.is_object()) {
                            if (item.contains("name") && item.contains("size") && item["name"].is_string() && item["size"].is_number_unsigned()) {
                                auto fontName = item["name"].template get<std::string>();
                                auto pathToFont = std::format("fonts/{}.ttf", fontName.c_str());
                                auto fontSize = item["size"].template get<int>();

                                if (!m_fontDefMap.contains(fontName)) {
                                    m_fontDefMap[fontName] = std::unordered_map<int, int>();
                                }

                                if (!m_fontDefMap[fontName].contains(fontSize)) {
                                    m_fontDefMap[fontName][fontSize] = (int)m_loadedFonts.size();
                                }

                                m_loadedFonts.push_back(
                                    io.Fonts->AddFontFromFileTTF(
                                        pathToFont.c_str(), 
                                        fontSize
                                    )
                                );
                            }
                        }
                    }
                }

                if (fontDefs.contains("defaultFont") 
                    && fontDefs["defaultFont"].is_object() 
                    && fontDefs["defaultFont"]["name"].is_string()
                    && fontDefs["defaultFont"]["size"].is_number_unsigned()) {

                    auto defaultFontName = fontDefs["defaultFont"]["name"].template get<std::string>();
                    auto defaultFontSize = fontDefs["defaultFont"]["size"].template get<int>();

                    if (m_fontDefMap.contains(defaultFontName) && m_fontDefMap[defaultFontName].contains(defaultFontSize)) {
                        auto fontIndex = m_fontDefMap[defaultFontName][defaultFontSize];

                        SetFontDefault(fontIndex);
                    }
                }
            }

            ImVec4 v4 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            m_clearColor = { v4.x * v4.w, v4.y * v4.w, v4.z * v4.w, v4.w };
        }

        int GetFontIndex(std::string fontName, int fontSize) {
            if (m_fontDefMap.contains(fontName) && m_fontDefMap[fontName].contains(fontSize)) {
                return m_fontDefMap[fontName][fontSize];
            }

            return -1;
        }

        bool IsFontIndexValid(int& fontIndex) {
            return fontIndex >= 0 && fontIndex < m_loadedFonts.size();
        }

        void SetFontDefault(int fontIndex) {
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            if (IsFontIndexValid(fontIndex)) {
                io.FontDefault = m_loadedFonts[fontIndex];
            }
        }

        // Only call this after having verified that fontIndex is valid (use IsFontIndexValid())
        void PushFont(int fontIndex) {
            ImGui::PushFont(m_loadedFonts[fontIndex]);
        }

        void PopFont() {
            ImGui::PopFont();
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

