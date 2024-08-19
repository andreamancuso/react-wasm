#include "styled_widget.h"



class PlotView final : public StyledWidget {
private:
    std::vector<double> m_xValues;
    std::vector<double> m_yValues;

    int m_xAxisDecimalDigits;
    int m_yAxisDecimalDigits;

    int m_dataPointsLimit = 6000;

    bool m_axisAutoFit;

public:
    static std::unique_ptr<PlotView> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        auto id = widgetDef["id"].template get<int>();
        int xAxisDecimalDigits = 0;
        int yAxisDecimalDigits = 0;
        bool axisAutoFit = false;

        if (widgetDef.contains("xAxisDecimalDigits") && widgetDef.contains("yAxisDecimalDigits")) {
            xAxisDecimalDigits = widgetDef["xAxisDecimalDigits"].template get<int>();
            yAxisDecimalDigits = widgetDef["yAxisDecimalDigits"].template get<int>();
        }

        if (widgetDef.contains("axisAutoFit")) {
            axisAutoFit = widgetDef["axisAutoFit"].template get<bool>();
        }

        return std::make_unique<PlotView>(view, id, xAxisDecimalDigits, yAxisDecimalDigits, axisAutoFit, maybeStyle);

        // throw std::invalid_argument("Invalid JSON data");
    }

    static int axisValueFormatter(double value, char* buff, int size, void* decimalPlaces) {
        if (value == 0) {
            return snprintf(buff, size,"0");
        }

        return snprintf(buff, size, "%.*f", reinterpret_cast<int>(decimalPlaces), value);
    };

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;

    PlotView(ReactImgui* view, const int id, const int xAxisDecimalDigits, const int yAxisDecimalDigits, const bool axisAutoFit, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style) {
        m_type = "plot-view";
        m_xAxisDecimalDigits = xAxisDecimalDigits;
        m_yAxisDecimalDigits = yAxisDecimalDigits;
        m_axisAutoFit = axisAutoFit;

        m_xValues.reserve(m_dataPointsLimit);
        m_yValues.reserve(m_dataPointsLimit);
    }

    void Render(ReactImgui* view, const std::optional<ImRect>& viewport) override;

    void Patch(const json& widgetPatchDef, ReactImgui* view) override;

    bool HasInternalOps();

    void HandleInternalOp(const json& opDef);

    void AppendData(const double x, const double y) {
        if (m_xValues.size() >= m_dataPointsLimit) {
            m_xValues.erase(m_xValues.begin());
            m_yValues.erase(m_yValues.begin());
        }

        m_xValues.push_back(x);
        m_yValues.push_back(y);
    }

    void SetAxesDecimalDigits(const int x, const int y) {
        m_xAxisDecimalDigits = x;
        m_yAxisDecimalDigits = y;
    }

    void SetAxesAutoFit(const bool enabled) {
        m_axisAutoFit = enabled;
    }

    void ResetData() {
        m_xValues.clear();
        m_yValues.clear();
    }
};
