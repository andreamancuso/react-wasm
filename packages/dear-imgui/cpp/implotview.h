#include "implot.h"
#include "implot_internal.h"

#include "./imguiview.h"

#pragma once

class ImPlotView : public ImGuiView {
    protected:
        ImPlotContext* m_imPlotCtx;

    public:
        ImPlotView(
            const char* newWindowId, 
            const char* newGlWindowTitle, 
            std::string& rawFontDefs
            ) : ImGuiView(newWindowId, newGlWindowTitle, rawFontDefs) {
            m_imPlotCtx = ImPlot::CreateContext();
        }

        void SetCurrentContext();

        void CleanUp();
};

