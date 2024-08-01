#include <imgui.h>

#include "widget/plot_view.h"
#include "reactimgui.h"

bool PlotView::HasCustomWidth() {
    return false;
}

bool PlotView::HasCustomHeight() {
    return false;
}

void PlotView::Render(ReactImgui* view) {
    // static int e = 0;
    // ImGui::RadioButton("line plot", &e, 0);
    // ImGui::SameLine();
    // ImGui::RadioButton("scatter plot", &e, 1);

    auto size = ImVec2(YGNodeLayoutGetWidth(m_layoutNode->m_node), YGNodeLayoutGetHeight(m_layoutNode->m_node));

    if (ImPlot::BeginPlot("Line Plots", size, ImPlotFlags_NoMenus | ImPlotFlags_NoMouseText | ImPlotFlags_NoLegend | ImPlotFlags_NoTitle)) {
        if (m_axisAutoFit) {
            ImPlot::SetupAxes("x","y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        } else {
            ImPlot::SetupAxes("x","y");
            // TODO: compute axes limits?
            // ImPlot::SetupAxesLimits(0,15000,0,1000);
        }

        if (m_xAxisDecimalDigits > 0) {
            ImPlot::SetupAxisFormat(ImAxis_X1, axisValueFormatter, (void*)m_xAxisDecimalDigits);
        }

        if (m_yAxisDecimalDigits > 0) {
            ImPlot::SetupAxisFormat(ImAxis_Y1, axisValueFormatter, (void*)m_yAxisDecimalDigits);
        }

        double* x_valuesPtr = m_xValues.data();
        double* y_valuesPtr = m_yValues.data();

        // if (e == 0) {
            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
            ImPlot::PlotLine("line-plot", x_valuesPtr, y_valuesPtr, m_xValues.size());
        // } else {
            // ImPlot::PlotScatter("scatter-plot", x_valuesPtr, y_valuesPtr, m_xValues.size());
        // }
        ImPlot::EndPlot();
    }
};

void PlotView::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("xAxisDecimalDigits") && widgetPatchDef.contains("yAxisDecimalDigits")) {
            const auto xAxisDecimalDigits = widgetPatchDef["xAxisDecimalDigits"].template get<int>();
            const auto yAxisDecimalDigits = widgetPatchDef["yAxisDecimalDigits"].template get<int>();

            SetAxesDecimalDigits(xAxisDecimalDigits, yAxisDecimalDigits);
        }

        if (widgetPatchDef.contains("axisAutoFit")) {
            const auto axisAutoFit = widgetPatchDef["axisAutoFit"].template get<bool>();
            SetAxesAutoFit(axisAutoFit);
        }
    }
};

bool PlotView::HasInternalOps() {
    return true;
}

// void ReactImgui::RenderMap(int id, double centerX, double centerY, int zoom)
void PlotView::HandleInternalOp(const json& opDef) {
    if (opDef.contains("op") && opDef["op"].is_string()) {
        const auto op = opDef["op"].template get<std::string>();

        if (op == "appendData" && opDef.contains("x") && opDef.contains("y")) {
            const auto x = opDef["x"].template get<double>();
            const auto y = opDef["y"].template get<double>();

            AppendData(x, y);
        } else if (op == "setAxesDecimalDigits" && opDef.contains("x") && opDef.contains("y")) {
            const auto x = opDef["x"].template get<int>();
            const auto y = opDef["y"].template get<int>();

            SetAxesDecimalDigits(x, y);
        } else if (op == "setAxesAutoFit" && opDef.contains("enabled")) {
            const auto enabled = opDef["enabled"].template get<bool>();

            SetAxesAutoFit(enabled);
        } else if (op == "resetData") {
            ResetData();
        }
    }
};