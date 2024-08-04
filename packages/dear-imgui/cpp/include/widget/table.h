#include "styled_widget.h"

// todo: for those use cases where we expect large quantities of data, should we preallocate?
class Table final : public StyledWidget {
    typedef struct {
        std::optional<std::string> fieldId;
        std::string heading;
    } TableColumn;

    protected:
        ImGuiTableFlags m_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        Table(ReactImgui* view, const int id, const std::vector<TableColumn>& columns, const std::optional<int> clipRows, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style) {
            m_type = "Table";
            m_columns = columns;
            m_clipRows = 0;

            if (clipRows.has_value() && clipRows.value() > 0) {
                m_clipRows = clipRows.value();
            }
        }

    public:
        TableData m_data;
        std::vector<TableColumn> m_columns;
        int m_clipRows; // todo: potentially redundant?

        static std::vector<TableColumn> extractColumns(const json& columnsDef) {
            std::vector<TableColumn> columns;

            if (columnsDef.is_array()) {
                for (auto& [key, item] : columnsDef.items()) {
                    columns.push_back({
                        std::make_optional(item["fieldId"].template get<std::string>()),
                        item["heading"].template get<std::string>()
                    });
                }
            }

            return columns;
        }

        static std::unique_ptr<Table> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();

                if (widgetDef.contains("columns") && widgetDef["columns"].is_array()) {
                    std::optional<int> clipRows;

                    if (widgetDef.contains("clipRows") && widgetDef["clipRows"].is_number_integer()) {
                        clipRows.emplace(widgetDef["clipRows"].template get<int>());
                    }

                    return makeWidget(view, id, extractColumns(widgetDef["columns"]), clipRows, maybeStyle);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Table> makeWidget(ReactImgui* view, const int id, const std::vector<TableColumn>& columns, std::optional<int> clipRows, std::optional<WidgetStyle>& style) {
            Table instance(view, id, columns, clipRows, style);

            return std::make_unique<Table>(std::move(instance));
        }

        static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        bool HasInternalOps();

        void HandleInternalOp(const json& opDef);

        void SetColumns(const json& columnsDef) {
            m_data.clear();
            m_columns.clear();

            auto newColumns = extractColumns(columnsDef);
            m_columns.insert(m_columns.end(), newColumns.begin(), newColumns.end());
        }

        void SetClipRows(int clipRows) {
            if (clipRows > 0) {
                m_clipRows = clipRows;
            }
        }

        void SetData(TableData& data) {
            m_data.clear();
            AppendData(data);
        }

        void AppendData(TableData& data) {
            m_data.insert(m_data.end(), data.begin(), data.end());
        }

        // TODO: this is repeated a million times
        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};