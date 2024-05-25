use serde_json::Value;
use crate::{App, Render};

pub struct Button {
    pub id: u32,
    pub label: String,
    pub widget_type: String
}
//
impl Button {
    pub fn new(id: u32, label: &str) -> Button {
        Button{
            id,
            label: String::from(label),
            widget_type: String::from("Button")
        }
    }
}
//
impl Render for Button {
    fn render(&mut self, ui: &mut egui::Ui, app: &App) {
        if ui.button(self.label.as_str()).clicked() {
            app.event_handlers.on_click(self.id);
        }
    }

    fn get_type(&self) -> &str {
        return self.widget_type.as_str();
    }

    fn get_label(&self) -> &str {
        return "";
    }
}

impl TryFrom<&Value> for Button {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        return if widget_def["label"].is_string() {
            Ok(Button::new(
                widget_def["id"].as_u64().unwrap() as u32,
                widget_def["label"].as_str().unwrap()
            ))
        } else {
            Err("Invalid CollapsingHeader JSON data")
        }
    }
}