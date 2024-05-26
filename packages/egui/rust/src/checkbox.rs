use serde_json::Value;
use crate::{App, Render};

pub struct Checkbox {
    pub id: u32,
    pub checked: bool,
    pub label: String,
    pub tooltip_text: Option<String>,
    pub widget_type: String
}

impl Checkbox {
    pub fn new(id: u32, label: &str, checked: bool, tooltip_text: Option<&str>) -> Checkbox {
        Checkbox {
            id,
            checked,
            label: String::from(label),
            widget_type: String::from("Checkbox"),
            tooltip_text: if tooltip_text.is_some() { Some(String::from(tooltip_text.unwrap())) } else { None }
        }
    }
    fn set_checked(mut self, checked: bool) -> () {
        self.checked = checked;
    }
}

impl Render for Checkbox {
    fn render(&mut self, ui: &mut egui::Ui, app: &App) {
        let response = ui.checkbox(&mut self.checked, self.label.as_str());

        if response.changed() {
            app.event_handlers.on_bool_value_change(self.id, self.checked);
        }

        if self.tooltip_text.is_some() {
            response.on_hover_text(self.tooltip_text.as_ref().unwrap());
        }
    }

    fn get_type(&self) -> &str {
        return self.widget_type.as_str();
    }
}

impl TryFrom<&Value> for Checkbox {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        let label = widget_def["label"].as_str();
        let tooltip_text = widget_def["tooltipText"].as_str();
        let default_checked = widget_def["defaultChecked"].as_bool();

        return if label.is_some() {
            Ok(Checkbox::new(
                widget_def["id"].as_u64().unwrap() as u32,
                label.unwrap(),
                default_checked.or(Some(false)).unwrap(),
                tooltip_text))
        } else {
            Err("Invalid Checkbox JSON data")
        }
    }
}