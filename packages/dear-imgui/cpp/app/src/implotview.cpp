#include "implot.h"
#include "implot_internal.h"

#include "./implotview.h"

ImPlotView::ImPlotView(
    ReactImgui* reactImgui,
    const char* newWindowId,
    const char* newGlWindowTitle,
    std::string& rawFontDefs) : ImGuiView(reactImgui, newWindowId, newGlWindowTitle, rawFontDefs) {

    m_imPlotCtx = ImPlot::CreateContext();
}

void ImPlotView::SetCurrentContext() {
    ImGui::SetCurrentContext(m_imGuiCtx);
    ImPlot::SetCurrentContext(m_imPlotCtx);
};

void ImPlotView::CleanUp() {
    ImGuiView::CleanUp();

    ImPlot::DestroyContext(m_imPlotCtx);
};