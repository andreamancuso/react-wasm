#ifndef IMPLOT_VIEW
#define IMPLOT_VIEW

#include "implot.h"

#include "./imguiview.h"

class ReactImgui;

class ImPlotView : public ImGuiView {
    protected:
        ImPlotContext* m_imPlotCtx;

    public:
        ImPlotView(ReactImgui* reactImgui, const char* newWindowId, const char* newGlWindowTitle, std::string& rawFontDefs);

        void SetCurrentContext();

        void CleanUp();
};

#endif