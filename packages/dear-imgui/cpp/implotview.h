#include "implot.h"
#include "implot_internal.h"

#include "./imguiview.h"

#pragma once

class ImPlotView : public ImGuiView {
    protected:
        ImPlotContext* imPlotCtx;

    public:
        ImPlotView(const char* newWindowId, const char* newGlWindowTitle) : ImGuiView(newWindowId, newGlWindowTitle) {
            imPlotCtx = ImPlot::CreateContext();
        }

        void SetCurrentContext();

        void CleanUp();
};

