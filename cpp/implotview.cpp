#include "implot.h"
#include "implot_internal.h"

#include "./imguiview.cpp"

class ImPlotView : public ImGuiView {
    protected:
        ImPlotContext* imPlotCtx;

    public:
        ImPlotView(const char* newWindowId, const char* newGlWindowTitle) : ImGuiView(newWindowId, newGlWindowTitle) {
            imPlotCtx = ImPlot::CreateContext();
        }

        void SetCurrentContext() {
            ImGui::SetCurrentContext(imGuiCtx);
            ImPlot::SetCurrentContext(imPlotCtx);
        }

        void CleanUp() {
            ImGuiView::CleanUp();

            ImPlot::DestroyContext(imPlotCtx);
        }
};

