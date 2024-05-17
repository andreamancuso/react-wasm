#include "implot.h"
#include "implot_internal.h"

#include "./implotview.h"

void ImPlotView::SetCurrentContext() {
    ImGui::SetCurrentContext(m_imGuiCtx);
    ImPlot::SetCurrentContext(m_imPlotCtx);
};

void ImPlotView::CleanUp() {
    ImGuiView::CleanUp();

    ImPlot::DestroyContext(m_imPlotCtx);
};