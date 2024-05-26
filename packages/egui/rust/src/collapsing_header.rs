use serde_json::Value;
use crate::{App, Render};

pub struct CollapsingHeader {
    pub id: u32,
    pub label: String,
    pub widget_type: String
}

impl CollapsingHeader {
    pub fn new(id: u32, label: &str) -> CollapsingHeader {
        CollapsingHeader{
            id,
            label: String::from(label),
            widget_type: String::from("CollapsingHeader")
        }
    }
}

impl Render for CollapsingHeader {
    fn render(&mut self, _ui: &mut egui::Ui, app: &App) {}

    fn get_type(&self) -> &str {
        return self.widget_type.as_str();
    }

    fn as_collapsing_header(&mut self) -> Option<&mut CollapsingHeader> { Some(self) }
}

impl TryFrom<&Value> for CollapsingHeader {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        return if widget_def["label"].is_string() {
            Ok(CollapsingHeader::new(
                widget_def["id"].as_u64().unwrap() as u32,
                widget_def["label"].as_str().unwrap()
            ))
        } else {
            Err("Invalid Checkbox JSON data")
        }
    }
}
