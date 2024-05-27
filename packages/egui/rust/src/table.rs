use std::collections::HashMap;
use eframe::emath::Align;
use egui::Layout;
use serde_json::Value;
use crate::{App, Render};
use egui_extras::{Column, TableBuilder};

#[derive(Clone, Copy, Debug, PartialEq)]
enum InitialColumnSize {
    /// Absolute size in points
    Absolute(f32),

    /// Base on content
    Automatic(f32),

    /// Take all available space
    Remainder,
}

#[derive(Debug)]
pub struct TableColumn {
    pub heading: String,
    pub column_size_type: Option<InitialColumnSize>, // todo: what's the default?
    pub column_size: Option<f32>,
    pub clip: Option<bool>,
    pub resizable: Option<bool>,
    pub field_id: Option<String>
}

#[derive(Debug)]
pub struct Table {
    pub id: u32,
    pub cell_layout: Layout,
    pub striped: bool,
    pub resizable: bool,
    pub clickable: bool,
    pub selection: std::collections::HashSet<usize>,
    pub columns: Vec<TableColumn>,
    pub widget_type: String,
    pub data: Vec<HashMap<String, String>>
}

impl Table {
    pub fn new(id: u32, cell_layout: Layout, columns: Vec<TableColumn>, striped: bool, resizable: bool, clickable: bool) -> Table {
        Table {
            id,
            cell_layout,
            columns,
            striped,
            resizable,
            clickable,
            selection: Default::default(),
            data: Vec::<HashMap::<String, String>>::new(),
            widget_type: String::from("Table"),
        }
    }

    pub fn set_data(&mut self, data: Vec<HashMap<String, String>>) {
        self.data = data;
    }

    pub fn append_data(&mut self, data: &mut Vec<HashMap<String, String>>) {
        self.data.append(data);
    }
}

fn expanding_content(ui: &mut egui::Ui) {
    let width = ui.available_width().clamp(20.0, 200.0);
    let height = ui.available_height();
    let (rect, _response) = ui.allocate_exact_size(egui::vec2(width, height), egui::Sense::hover());
    ui.painter().hline(
        rect.x_range(),
        rect.center().y,
        (1.0, ui.visuals().text_color()),
    );
}

fn long_text(row_index: usize) -> String {
    format!("Row {row_index} has some long text that you may want to clip, or it will take up too much horizontal space!")
}

fn thick_row(row_index: usize) -> bool {
    row_index % 6 == 0
}

impl Render for Table {
    fn render(&mut self, ui: &mut egui::Ui, app: &App) {

        ui.label(format!("Table data length: {}", self.data.len()).as_str());

        let text_height = egui::TextStyle::Body
            .resolve(ui.style())
            .size
            .max(ui.spacing().interact_size.y);

        let available_height = ui.available_height();
        let mut table = TableBuilder::new(ui)
            .striped(self.striped)
            .resizable(self.resizable)
            // .cell_layout(egui::Layout::left_to_right(egui::Align::Center))
            .cell_layout(self.cell_layout)
            .min_scrolled_height(0.0)
            .max_scroll_height(available_height);

        // todo: add support for rest of column sizes and ranges
        for column in self.columns.iter() {
            match column.column_size_type.unwrap_or(InitialColumnSize::Remainder) {
                InitialColumnSize::Absolute(_) => {
                    table = table.column(Column::exact(column.column_size.unwrap_or(100.0)));
                }
                InitialColumnSize::Automatic(_) => {
                    table = table.column(Column::initial(column.column_size.unwrap_or(100.0)));
                }
                InitialColumnSize::Remainder => {
                    table = table.column(Column::remainder());
                }
            }
        }

        if self.clickable {
            table = table.sense(egui::Sense::click());
        }

        // if let Some(row_index) = self.scroll_to_row.take() {
        //     table = table.scroll_to_row(row_index, None);
        // }


        table
            // todo: add support for more complex cases, styling, etc.
            .header(20.0, |mut header| {
                for column in self.columns.iter() {
                    header.col(|ui| {
                        ui.strong(column.heading.as_str());
                    });
                }
            })
            .body(|mut body| {
                let row_height = |i: usize| if thick_row(i) { 30.0 } else { 18.0 };
                body.heterogeneous_rows((0..self.data.len()).map(row_height), |mut row| {
                    let row_index = row.index();
                    let maybe_row_data = self.data.get(row_index);

                    if maybe_row_data.is_some() {
                        let row_data = maybe_row_data.unwrap();

                        for column in self.columns.iter() {
                            let maybe_field_id = &column.field_id;
                            if maybe_field_id.is_some() {
                                let field_id = maybe_field_id.as_ref().unwrap();

                                row.col(|ui| {
                                    let maybe_column_value = row_data.get(field_id.as_str());

                                    if maybe_column_value.is_some() {
                                        ui.label(maybe_column_value.unwrap().as_str());
                                    }

                                    // ui.style_mut().wrap = Some(false);
                                });
                            }
                        }
                    }

                    // row.set_selected(self.selection.contains(&row_index));

                    // self.toggle_row_selection(row_index, &row.response());
                });
            })
    }

    fn get_type(&self) -> &str {
        return self.widget_type.as_str();
    }

    fn as_table(&mut self) -> Option<&mut Table> {
        Some(self)
    }
}

impl TryFrom<&Value> for Table {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        let maybe_column_defs = widget_def["columns"].as_array();

        if maybe_column_defs.is_some() {
            let mut columns = Vec::<TableColumn>::new();
            let column_defs = maybe_column_defs.unwrap();

            for column_def in column_defs.iter() {
                if column_def.is_object() {
                    let maybe_heading = column_def["heading"].as_str();

                    if maybe_heading.is_some() {
                        let maybe_field_id = column_def["fieldId"].as_str();

                        columns.push(TableColumn {
                            heading: String::from(maybe_heading.unwrap()),
                            column_size_type: None,
                            column_size: None,
                            clip: None,
                            resizable: None,
                            field_id: if maybe_field_id.is_some() { Some(String::from(maybe_field_id.unwrap())) } else { None },
                        });
                    }
                }
            }

            return Ok(Table::new(
                widget_def["id"].as_u64().unwrap() as u32,
                Layout::left_to_right(Align::Center),
                columns,
                false,
                false,
                false
            ))
        }

        return Err("Invalid Table JSON data")
    }
}