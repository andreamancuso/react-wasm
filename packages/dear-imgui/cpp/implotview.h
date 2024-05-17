#include "implot.h"
#include "implot_internal.h"

#include "./imguiview.h"

#pragma once

class ImPlotView : public ImGuiView {
    protected:
        ImPlotContext* m_imPlotCtx;

    public:
        ImPlotView(const char* newWindowId, const char* newGlWindowTitle) : ImGuiView(newWindowId, newGlWindowTitle) {
            m_imPlotCtx = ImPlot::CreateContext();
        }

        void SetCurrentContext();

        void CleanUp();
};

