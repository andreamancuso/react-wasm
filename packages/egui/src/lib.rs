#![warn(clippy::all, rust_2018_idioms)]

mod app;

use std::collections::HashMap;
use std::ops::Deref;
use std::sync::{Arc, Mutex};
use once_cell::sync::Lazy;
use serde::{Deserialize, Serialize};
pub use app::TemplateApp;
use wasm_bindgen::prelude::*;
use serde_json::{Value};
use erased_serde::serialize_trait_object;


static REACT_EGUI: Lazy<Arc<Mutex<HashMap<u64, Box<dyn Render + Send>>>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = console)]
    fn log(a: &str);
}

#[wasm_bindgen]
pub fn get_content() -> String {
    let mut m = REACT_EGUI.lock().unwrap_throw();

    return serde_json::to_string(&m.deref()).unwrap();
}

#[wasm_bindgen]
pub fn add_widget(raw_widget_def: String) {
    let mut m = REACT_EGUI.lock().unwrap_throw();

    let widget_def: Value = serde_json::from_str(&*raw_widget_def).unwrap();

    if widget_def.is_object() && widget_def["type"].is_string() {
        let maybe_widget_type = widget_def["type"].as_str();
        let maybe_widget_id = widget_def["id"].as_u64();

        if maybe_widget_id.is_some() && maybe_widget_type.is_some() {
            let widget_id = maybe_widget_id.unwrap();
            let widget_type = maybe_widget_type.unwrap();

            match widget_type {
                "Button" => {
                    let label = widget_def["label"].as_str();

                    if label.is_some() {
                        m.insert(widget_id, Box::new(Button::new(widget_id, label.unwrap())));
                    }
                }
                "InputText" => {
                    let value = widget_def["value"].as_str();

                    if value.is_some() {
                        m.insert(widget_id, Box::new(InputText::new(widget_id, value)));
                    }
                }
                &_ => {
                    log(format!("Unrecognised type: {}", widget_type).as_str());
                }
            }
        }
    }
}

// ----------------

pub trait Render: erased_serde::Serialize {
    fn render(&self);
}

#[derive(Serialize, Deserialize)]
pub struct Button {
    pub id: u64,
    pub label: String,
}
//
impl Button {
    pub fn new(id: u64, label: &str) -> Button {
        Button{
            id,
            label: label.parse().unwrap()
        }
    }
}
//
impl Render for Button {
    fn render(&self) {

    }
}

#[derive(Serialize, Deserialize)]
pub struct InputText {
    pub id: u64,
    pub value: String,
}

impl InputText {
    fn new(id: u64, value: Option<&str>) -> InputText {
        InputText{
            id,
            value: value.unwrap_or_default().parse().unwrap()
        }
    }
    fn set_value(mut self, value: &str) -> () {
        self.value = value.parse().unwrap();
    }
}

impl Render for InputText {
    fn render(&self) {

    }
}

serialize_trait_object!(Render);
