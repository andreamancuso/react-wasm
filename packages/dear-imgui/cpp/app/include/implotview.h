#include "implot.h"

#include "./imguiview.h"

#pragma once

class ImPlotView : public ImGuiView {
    protected:
        ImPlotContext* m_imPlotCtx;

    public:
        ImPlotView(const char* newWindowId, const char* newGlWindowTitle, std::string& rawFontDefs);

        void SetCurrentContext();

        void CleanUp();
};

