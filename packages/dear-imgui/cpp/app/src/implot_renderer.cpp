#include "implot.h"
#include "implot_internal.h"

#include "implot_renderer.h"

ImPlotRenderer::ImPlotRenderer(
    ReactImgui* reactImgui,
    const char* newWindowId,
    const char* newGlWindowTitle,
    std::string& rawFontDefs) : ImGuiRenderer(reactImgui, newWindowId, newGlWindowTitle, rawFontDefs) {

    m_imPlotCtx = ImPlot::CreateContext();
}

void ImPlotRenderer::SetCurrentContext() {
    ImGui::SetCurrentContext(m_imGuiCtx);
    ImPlot::SetCurrentContext(m_imPlotCtx);
};

void ImPlotRenderer::CleanUp() {
    ImGuiRenderer::CleanUp();

    ImPlot::DestroyContext(m_imPlotCtx);
};