use serde_json::Value;
use crate::{App, Render};

pub fn get_radio_button_value_from_json_value(value: &Value) -> Option<String> {
    if value.is_string() {
        Some(String::from(value.as_str().unwrap()))
    } else { None }
}

pub struct RadioButton {
    pub id: u32,
    pub checked: bool,
    pub label: String,
    pub tooltip_text: Option<String>,
    pub value: Option<String>,
    pub widget_type: String
}

impl RadioButton {
    pub fn new(id: u32, label: &str, checked: bool, value: Option<String>, tooltip_text: Option<&str>) -> RadioButton {
        RadioButton {
            id,
            checked,
            label: String::from(label),
            widget_type: String::from("RadioButton"),
            value,
            tooltip_text: if tooltip_text.is_some() { Some(String::from(tooltip_text.unwrap())) } else { None }
        }
    }
    fn set_checked(mut self, checked: bool) -> () {
        self.checked = checked;
    }
}

impl TryFrom<&Value> for RadioButton {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        let label = widget_def["label"].as_str();

        if label.is_some() {
            return Ok(RadioButton::new(
                widget_def["id"].as_u64().unwrap() as u32,
                label.unwrap(),
                widget_def["defaultChecked"].as_bool().or(Some(false)).unwrap(),
                get_radio_button_value_from_json_value(&widget_def["value"]),
                widget_def["tooltipText"].as_str()
            ));
        } else {
            return Err("Invalid RadioButton JSON data");
        }
    }
}

// todo: not sure whether it makes sense to render standalone instances of RadioButton
impl Render for RadioButton {
    fn render(&mut self, ui: &mut egui::Ui, app: &App) {
        let response = ui.add(egui::RadioButton::new(self.checked, self.label.as_str()));
        if response.clicked() {
            self.checked = !self.checked;

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

pub struct RadioButtonGroupOption {
    pub label: String,
    pub tooltip_text: Option<String>,
    pub value: String,
}

impl TryFrom<&Value> for RadioButtonGroupOption {
    type Error = &'static str;

    fn try_from(option_def: &Value) -> Result<Self, Self::Error> {
        let label = option_def["label"].as_str();
        let tooltip_text = option_def["tooltipText"].as_str();

        if label.is_some() {
            return Ok(RadioButtonGroupOption {
                label: String::from(label.unwrap()),
                tooltip_text: if tooltip_text.is_some() { Some(String::from(tooltip_text.unwrap())) } else { None },
                value: get_radio_button_value_from_json_value(&option_def["value"]).unwrap()
            });
        } else {
            return Err("Invalid RadioButtonGroupOption JSON data");
        }
    }
}

pub struct RadioButtonGroup {
    pub id: u32,
    pub value: String,
    pub widget_type: String,
    pub options: Vec<RadioButtonGroupOption>
}

impl RadioButtonGroup {
    pub fn new(id: u32, value: Option<String>, options: Vec<RadioButtonGroupOption>) -> RadioButtonGroup {
        RadioButtonGroup {
            id,
            value: value.unwrap_or_default(),
            options,
            widget_type: String::from("RadioButtonGroup"),
        }
    }
}

impl Render for RadioButtonGroup {
    fn render(&mut self, ui: &mut egui::Ui, app: &App) {
        for option in self.options.iter() {
            if ui.add(egui::RadioButton::new(self.value == option.value, option.label.as_str())).clicked() {
                // todo: must we really check whether the value has changed ourselves?
                if self.value != option.value {
                    self.value = String::from(option.value.as_str());
                    // todo: must we make a second copy here? #fml
                    app.event_handlers.on_text_change(self.id, String::from(option.value.as_str()));
                }
            }
        }
    }

    fn get_type(&self) -> &str {
        return self.widget_type.as_str();
    }
}

impl TryFrom<&Value> for RadioButtonGroup {
    type Error = &'static str;

    fn try_from(widget_def: &Value) -> Result<Self, Self::Error> {
        return if widget_def["options"].is_array() {
            let mut options = Vec::<RadioButtonGroupOption>::new();

            for option_def in widget_def["options"].as_array().unwrap().iter() {
                let option_result = RadioButtonGroupOption::try_from(option_def);

                if option_result.is_ok() {
                    options.push(option_result.unwrap());
                }
            }

            Ok(RadioButtonGroup::new(
                widget_def["id"].as_u64().unwrap() as u32,
                get_radio_button_value_from_json_value(&widget_def["defaultValue"]),
                options
            ))
        } else {
            Err("Invalid RadioButtonGroup JSON data")
        }
    }
}
