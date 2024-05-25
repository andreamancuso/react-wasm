use serde_json::Value;
use crate::{App, Render};

pub struct InputText {
    pub id: u32,
    pub value: String,
    pub widget_type: String
}

impl InputText {
    pub fn new(id: u32, value: Option<&str>) -> InputText {
        InputText{
            id,
            value: String::from(value.unwrap_or_default()),
            widget_type: String::from("InputText")
        }
    }
    fn set_value(mut self, value: &str) -> () {
        self.value = String::from(value);
    }
}

impl Render for InputText {
    fn render(&mut self, ui: &mut egui::Ui, app: &App) {
        if ui.text_edit_singleline(&mut self.value).changed() {
            app.event_handlers.on_text_change(self.id, self.value.clone());
        }
    }

    fn get_type(&self) -> &str {
        return self.widget_type.as_str();
    }

    fn get_label(&self) -> &str {
        return "";
    }
}

impl TryFrom<&Value> for InputText {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        let default_value = widget_def["defaultValue"].as_str();

        return if default_value.is_some() {
            Ok(InputText::new(
                widget_def["id"].as_u64().unwrap() as u32,
                default_value
            ))
        } else {
            Err("Invalid InputText JSON data")
        }
    }
}