#include "implot.h"
#include "implot_internal.h"

#include "./implotview.h"

void ImPlotView::SetCurrentContext() {
    ImGui::SetCurrentContext(imGuiCtx);
    ImPlot::SetCurrentContext(imPlotCtx);
};

void ImPlotView::CleanUp() {
    ImGuiView::CleanUp();

    ImPlot::DestroyContext(imPlotCtx);
};