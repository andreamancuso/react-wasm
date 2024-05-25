use serde_json::Value;
use crate::{App, Render};

pub struct Horizontal {
    pub id: u32,
    pub widget_type: String
}

impl Horizontal {
    pub fn new(id: u32) -> Horizontal {
        Horizontal{
            id,
            widget_type: String::from("Horizontal")
        }
    }
}

impl Render for Horizontal {
    fn render(&mut self, _ui: &mut egui::Ui, app: &App) {}

    fn get_type(&self) -> &str { return self.widget_type.as_str(); }

    fn get_label(&self) -> &str {
        return "";
    }
}

impl TryFrom<&Value> for Horizontal {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        return Ok(Horizontal::new(
            widget_def["id"].as_u64().unwrap() as u32
        ));
    }
}