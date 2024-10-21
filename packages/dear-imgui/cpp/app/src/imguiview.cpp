#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#include <webgpu/webgpu.h>
#endif

#ifdef __EMSCRIPTEN__
#include <functional>
static std::function<void()>            MainLoopForEmscriptenP;
static void MainLoopForEmscripten()     { MainLoopForEmscriptenP(); }
#define EMSCRIPTEN_MAINLOOP_BEGIN       MainLoopForEmscriptenP = [&]()
#define EMSCRIPTEN_MAINLOOP_END         ; emscripten_set_main_loop(MainLoopForEmscripten, 30, true) // 24 frames / second, use 0 for browser's default
#else
#define EMSCRIPTEN_MAINLOOP_BEGIN
#define EMSCRIPTEN_MAINLOOP_END
#endif

#include "./reactimgui.h"
#include "./imguiview.h"

void glfw_error_callback(int error, const char* description)
{
    printf("GLFW Error %d: %s\n", error, description);
}

#ifdef __EMSCRIPTEN__
void wgpu_error_callback(WGPUErrorType error_type, const char* message, void*)
{
    const char* error_type_lbl = "";
    switch (error_type)
    {
        case WGPUErrorType_Validation:  error_type_lbl = "Validation"; break;
        case WGPUErrorType_OutOfMemory: error_type_lbl = "Out of memory"; break;
        case WGPUErrorType_Unknown:     error_type_lbl = "Unknown"; break;
        case WGPUErrorType_DeviceLost:  error_type_lbl = "Device lost"; break;
        default:                        error_type_lbl = "Unknown";
    }
    printf("%s error: %s\n", error_type_lbl, message);
}
#endif

ImGuiView::ImGuiView(
    ReactImgui* reactImgui,
    const char* windowId,
    const char* glWindowTitle,
    std::string rawFontDefs) {

    m_reactImgui = reactImgui;

    m_windowId = windowId;
    m_glWindowTitle = glWindowTitle;

    m_shouldLoadDefaultStyle = true;

    m_instance = wgpu::CreateInstance();

    m_imGuiCtx = ImGui::CreateContext();

    m_rawFontDefs = rawFontDefs;
}

void ImGuiView::LoadFontsFromDefs() {
    auto fontDefs = json::parse(m_rawFontDefs);
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    static constexpr ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    // static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_16_MDI, 0 };

    if (fontDefs.is_object() && fontDefs.contains("defs") && fontDefs["defs"].is_array()) {
        for (auto& [key, item] : fontDefs["defs"].items()) {
            if (item.is_object()) {
                if (item.contains("name") && item.contains("size") && item["name"].is_string() && item["size"].is_number_unsigned()) {
                    auto fontName = item["name"].template get<std::string>();
                    auto pathToFont = std::format("assets/fonts/{}.ttf", fontName.c_str());
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
                    auto pathToFaFontFile = std::format("assets/fonts/{}", FONT_ICON_FILE_NAME_FAS);
                    // auto pathToMdiFontFile = std::format("assets/fonts/{}", FONT_ICON_FILE_NAME_MDI);

                    io.Fonts->AddFontFromFileTTF(pathToFaFontFile.c_str(), iconFontSize, &icons_config, icons_ranges);
                    // io.Fonts->AddFontFromFileTTF(pathToMdiFontFile.c_str(), fontSize, &icons_config, icons_ranges);
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
    if (m_loadedFonts.size() == 0) {
        io.Fonts->AddFontDefault();
        float baseFontSize = 13.0f; // 13.0f is the size of the default font.
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;
        auto pathToFaFontFile = std::format("assets/fonts/{}", FONT_ICON_FILE_NAME_FAS);

        m_loadedFonts.push_back(
            io.Fonts->AddFontFromFileTTF(pathToFaFontFile.c_str(), iconFontSize, &icons_config, icons_ranges)
        );

        io.Fonts->Build();

        SetFontDefault(0);
    }
}

int ImGuiView::GetFontIndex(const std::string& fontName, const int fontSize) {
    if (m_fontDefMap.contains(fontName) && m_fontDefMap[fontName].contains(fontSize)) {
        return m_fontDefMap[fontName][fontSize];
    }

    return -1;
}

bool ImGuiView::IsFontIndexValid(const int fontIndex) const {
    return fontIndex >= 0 && fontIndex < m_loadedFonts.size();
}

void ImGuiView::SetFontDefault(const int fontIndex) const {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (IsFontIndexValid(fontIndex)) {
        io.FontDefault = m_loadedFonts[fontIndex];
    }
}

void ImGuiView::PushFont(const int fontIndex) const {
    ImGui::PushFont(m_loadedFonts[fontIndex]);
}

void ImGuiView::PopFont() {
    ImGui::PopFont();
}

ImGuiStyle& ImGuiView::GetStyle() {
    return ImGui::GetStyle();
}

void ImGuiView::InitGlfw() {
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    // Make sure GLFW does not initialize any graphics context.
    // This needs to be done explicitly later.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_glfwWindow = glfwCreateWindow(m_window_width, m_window_height, m_glWindowTitle, nullptr, nullptr);

    // Initialize the WebGPU environment
    if (!InitWGPU())
    {
        if (m_glfwWindow)
            glfwDestroyWindow(m_glfwWindow);
        glfwTerminate();
        return;
    }
    glfwShowWindow(m_glfwWindow);
}

#ifdef __EMSCRIPTEN__
bool ImGuiView::InitWGPU() {
    if (!m_device) {
        printf("device not set\n");
        return false;
    }

    wgpuDeviceSetUncapturedErrorCallback(m_device, wgpu_error_callback, nullptr);

    // Use C++ wrapper due to misbehavior in Emscripten.
    // Some offset computation for wgpuInstanceCreateSurface in JavaScript
    // seem to be inline with struct alignments in the C++ structure
    wgpu::SurfaceDescriptorFromCanvasHTMLSelector html_surface_desc = {};

    html_surface_desc.selector = m_canvasSelector.get();

    wgpu::SurfaceDescriptor surface_desc = {};
    surface_desc.nextInChain = &html_surface_desc;

    wgpu::Surface surface = m_instance.CreateSurface(&surface_desc);
    wgpu::Adapter adapter = {};
    m_wgpu_preferred_fmt = (WGPUTextureFormat)surface.GetPreferredFormat(adapter);
    m_wgpu_surface = surface.MoveToCHandle();

    return true;
}
#endif

#ifdef __EMSCRIPTEN__
void ImGuiView::SetUp() {
    InitGlfw();

    IMGUI_CHECKVERSION();

    ImGui_ImplWGPU_InitInfo init_info;
    init_info.Device = m_device;
    init_info.NumFramesInFlight = 3;
    init_info.RenderTargetFormat = m_wgpu_preferred_fmt;
    init_info.DepthStencilFormat = WGPUTextureFormat_Undefined;
    ImGui_ImplWGPU_Init(&init_info);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOther(m_glfwWindow, true);

    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback(m_canvasSelector.get());

    SetCurrentContext();
}
#else
void ImGuiView::SetUp(GLFWwindow* glfwWindow) {

}
#endif

#ifdef __EMSCRIPTEN__
void ImGuiView::CreateSwapChain(int width, int height) {
    if (m_wgpu_swap_chain)
        wgpuSwapChainRelease(m_wgpu_swap_chain);
    m_wgpu_swap_chain_width = width;
    m_wgpu_swap_chain_height = height;
    WGPUSwapChainDescriptor swap_chain_desc = {};
    swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
    swap_chain_desc.format = m_wgpu_preferred_fmt;
    swap_chain_desc.width = width;
    swap_chain_desc.height = height;
    swap_chain_desc.presentMode = WGPUPresentMode_Fifo;
    m_wgpu_swap_chain = wgpuDeviceCreateSwapChain(m_device, m_wgpu_surface, &swap_chain_desc);
}
#endif

void ImGuiView::HandleScreenSizeChanged() {
#ifdef __EMSCRIPTEN__
    int width, height;
    glfwGetFramebufferSize((GLFWwindow*)m_glfwWindow, &width, &height);
    if (width != m_wgpu_swap_chain_width && height != m_wgpu_swap_chain_height)
    {
        CreateSwapChain(width, height);

        ImGui_ImplWGPU_InvalidateDeviceObjects();
        ImGui_ImplWGPU_CreateDeviceObjects();
    }
#endif
}

#ifdef __EMSCRIPTEN__
void ImGuiView::RenderDrawData(WGPURenderPassEncoder pass) {
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
}
#else
void ImGuiView::RenderDrawData() {

}
#endif

void ImGuiView::CleanUp() {
    ImGui::DestroyContext(m_imGuiCtx);

#ifdef __EMSCRIPTEN__
    ImGui_ImplWGPU_Shutdown();
    ImGui_ImplGlfw_Shutdown();
#endif
}

#ifdef __EMSCRIPTEN__
void ImGuiView::PerformRendering() {
    WGPURenderPassColorAttachment color_attachments = {};
    color_attachments.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    color_attachments.loadOp = WGPULoadOp_Clear;
    color_attachments.storeOp = WGPUStoreOp_Store;
    color_attachments.clearValue = m_clearColor;
    color_attachments.view = wgpuSwapChainGetCurrentTextureView(m_wgpu_swap_chain);

    WGPURenderPassDescriptor render_pass_desc = {};
    render_pass_desc.colorAttachmentCount = 1;
    render_pass_desc.colorAttachments = &color_attachments;
    render_pass_desc.depthStencilAttachment = nullptr;

    WGPUCommandEncoderDescriptor enc_desc = {};
    WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(m_device, &enc_desc);

    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);

    RenderDrawData(pass);

    wgpuRenderPassEncoderEnd(pass);

    WGPUCommandBufferDescriptor cmd_buffer_desc = {};
    WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);
    m_queue = wgpuDeviceGetQueue(m_device);
    wgpuQueueSubmit(m_queue, 1, &cmd_buffer);
}
#else
void ImGuiView::PerformRendering() {}
#endif


void ImGuiView::BeginRenderLoop() {
    LoadFontsFromDefs();

    m_reactImgui->Init(this);

    SetUp();
    // Main loop
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(m_glfwWindow))
#endif
    {
        glfwPollEvents();

        HandleScreenSizeChanged();

        SetCurrentContext();

        m_reactImgui->Render(m_window_width, m_window_height);

        PerformRendering();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
    // Clean up
    CleanUp();

    // GLFW3Renderer::CleanUp();
}

void ImGuiView::SetWindowSize(int width, int height) {
    m_window_width = width;
    m_window_height = height;

    if (m_glfwWindow) {
        glfwSetWindowSize(m_glfwWindow, width, height);
    }
}

#ifdef __EMSCRIPTEN__
void ImGuiView::SetDeviceAndStart(WGPUDevice& cDevice) {
    m_device = cDevice;

    BeginRenderLoop();
}
#endif

#ifdef __EMSCRIPTEN__
void ImGuiView::RequestDevice(wgpu::Instance wgpuInstance, ImGuiView* glWasmInstance) {
    wgpuInstance.RequestAdapter(
        nullptr,
        [](WGPURequestAdapterStatus status, WGPUAdapter cAdapter,
            const char* message, void* userdata) {

            if (status != WGPURequestAdapterStatus_Success) {
                exit(0);
            }

            wgpu::Adapter adapter = wgpu::Adapter::Acquire(cAdapter);

            adapter.RequestDevice(
                nullptr,
                [](WGPURequestDeviceStatus status, WGPUDevice cDevice,
                const char* message, void* userdata) {
                    reinterpret_cast<ImGuiView*>(userdata)->SetDeviceAndStart(cDevice);
                },
                userdata);
        }, reinterpret_cast<void*>(glWasmInstance));
}
#endif

#ifdef __EMSCRIPTEN__
void ImGuiView::Init(std::string& cs) {
    m_canvasSelector = std::make_unique<char[]>(cs.length() + 1);
    strcpy(m_canvasSelector.get(), cs.c_str());

    RequestDevice(m_instance, this);
}
#else
void ImGuiView::Init() {
    BeginRenderLoop();
}
#endif

#ifdef __EMSCRIPTEN__
bool ImGuiView::LoadTexture(const void* data, const int numBytes, Texture* texture) {
    if (data == nullptr)
        return false;

    int width;
    int height;

    // TODO: figure out why we need the STB library to load image data for us, seems like I'm missing a step when using leptonica
    const auto stbiData = stbi_load_from_memory(static_cast<const stbi_uc*>(data), numBytes, &width, &height, nullptr, 4);

    WGPUTextureView view;
    {
        WGPUTextureDescriptor tex_desc = {};
        tex_desc.label = "texture";
        tex_desc.dimension = WGPUTextureDimension_2D;
        tex_desc.size.width = width;
        tex_desc.size.height = height;
        tex_desc.size.depthOrArrayLayers = 1;
        tex_desc.sampleCount = 1;
        tex_desc.format = WGPUTextureFormat_RGBA8Unorm;
        tex_desc.mipLevelCount = 1;
        tex_desc.viewFormatCount = 0;
        tex_desc.viewFormats = nullptr;
        tex_desc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;

        auto tex = wgpuDeviceCreateTexture(m_device, &tex_desc);

        WGPUTextureViewDescriptor tex_view_desc = {};
        tex_view_desc.format = WGPUTextureFormat_RGBA8Unorm;
        tex_view_desc.dimension = WGPUTextureViewDimension_2D;
        tex_view_desc.baseMipLevel = 0;
        tex_view_desc.mipLevelCount = 1;
        tex_view_desc.baseArrayLayer = 0;
        tex_view_desc.arrayLayerCount = 1;
        tex_view_desc.aspect = WGPUTextureAspect_All;
        view = wgpuTextureCreateView(tex, &tex_view_desc);

        WGPUImageCopyTexture dst_view = {};
        dst_view.texture = tex;
        dst_view.mipLevel = 0;
        dst_view.origin = { 0, 0, 0 };
        dst_view.aspect = WGPUTextureAspect_All;
        WGPUTextureDataLayout layout = {};
        layout.offset = 0;
        layout.bytesPerRow = width * 4;
        layout.rowsPerImage = height;
        const WGPUExtent3D size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

        const auto queue = wgpuDeviceGetQueue(m_device);

        wgpuQueueWriteTexture(queue, &dst_view, stbiData, static_cast<uint32_t>(width * 4 * height), &layout, &size);

        wgpuQueueRelease(queue);
    }

    texture->textureView = view;
    texture->width = width;
    texture->height = height;

    stbi_image_free(stbiData);

    return true;
}
#else
bool LoadTexture(const void* data, const int numBytes, Texture* texture) {

}
#endif

json ImGuiView::GetAvailableFonts() {
    SetCurrentContext();
    ImGuiIO& io = ImGui::GetIO();
    json fonts = json::array();

    for (ImFont* font : io.Fonts->Fonts) {
        fonts.push_back(font->GetDebugName());
    }

    return fonts;
};