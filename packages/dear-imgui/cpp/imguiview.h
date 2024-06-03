#include "IconsMaterialDesignIcons.h"
// #include "IconsFontAwesome6.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include <format>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <webgpu/webgpu_cpp.h>
#include <nlohmann/json.hpp>

#include "./shared.h"
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

        // static constexpr ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    public:

        ImGuiView(
            const char* newWindowId, 
            const char* newGlWindowTitle, 
            std::string& rawFontDefs) : View(newWindowId, newGlWindowTitle) {
            m_imGuiCtx = ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.Fonts->TexDesiredWidth = 4096;

            auto fontDefs = json::parse(rawFontDefs);

            // static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
            static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };

            

            if (fontDefs.is_object() && fontDefs.contains("defs") && fontDefs["defs"].is_array()) {
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

                            float iconFontSize = fontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly
                            ImFontConfig icons_config; 
                            icons_config.MergeMode = true; 
                            icons_config.PixelSnapH = true; 
                            icons_config.GlyphMinAdvanceX = iconFontSize;
                            icons_config.OversampleH = 1;
                            // auto pathToFaFontFile = std::format("fonts/{}", FONT_ICON_FILE_NAME_FAS);
                            auto pathToMdiFontFile = std::format("fonts/{}", FONT_ICON_FILE_NAME_MDI);
                            
                            // io.Fonts->AddFontFromFileTTF(pathToFaFontFile.c_str(), iconFontSize, &icons_config, icons_ranges);
                            io.Fonts->AddFontFromFileTTF(pathToMdiFontFile.c_str(), fontSize, &icons_config, icons_ranges);
                        }
                    }
                }

                io.Fonts->Build();

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

            // If not custom fonts defined, ensure font-awesome are still available
            // if (m_loadedFonts.size() == 0) {
            //     io.Fonts->AddFontDefault();
            //     float baseFontSize = 13.0f; // 13.0f is the size of the default font.
            //     float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

            //     ImFontConfig icons_config; 
            //     icons_config.MergeMode = true; 
            //     icons_config.PixelSnapH = true; 
            //     icons_config.GlyphMinAdvanceX = iconFontSize;
            //     auto pathToFaFontFile = std::format("fonts/{}", FONT_ICON_FILE_NAME_FAS);
                
            //     m_loadedFonts.push_back(
            //         io.Fonts->AddFontFromFileTTF(pathToFaFontFile.c_str(), iconFontSize, &icons_config, icons_ranges)
            //     );

            //     io.Fonts->Build();

            //     SetFontDefault(0);
            // }

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

