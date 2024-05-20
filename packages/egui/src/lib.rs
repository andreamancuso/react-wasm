#![warn(clippy::all, rust_2018_idioms)]

mod app;

use std::collections::HashMap;
use std::sync::Mutex;
use once_cell::sync::Lazy;
pub use app::TemplateApp;
use wasm_bindgen::prelude::*;
use serde_json::{Value};

static REACT_EGUI: Lazy<Mutex<Box<ReactEgui<dyn Render + Send>>>> = Lazy::new(|| {
    Mutex::new(Box::new(ReactEgui::new()))
});

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = console)]
    fn log(a: &str);
}

#[wasm_bindgen]
pub unsafe fn add_widget(raw_widget_def: String) {
    let mut m = REACT_EGUI.lock().unwrap_throw();

    m.add_widget(raw_widget_def);
}

// ----------------

pub trait Render {
    fn render(&self);
}

pub struct ReactEgui<T: Render + Send + ?Sized> {
    pub widgets: HashMap<&'static u64, Box<T>>,
}

impl<T> ReactEgui<T>
    where T: Render + Send {

    pub fn new() -> ReactEgui<T> {
        ReactEgui{
            widgets: HashMap::new()
        }
    }

    pub fn add_widget(&mut self, raw_widget_def: String) {
        log("a\n");

        let widget_def: Value = serde_json::from_str(&*raw_widget_def).unwrap();

        if widget_def.is_object() && widget_def["type"].is_string() {
            log("b\n");
            let maybe_widget_type = widget_def["type"].as_str();
            let maybe_widget_id = widget_def["id"].as_u64();

            if maybe_widget_id.is_some() && maybe_widget_type.is_some() {
                log("c\n");
                let widget_id = maybe_widget_id.unwrap();

                if maybe_widget_type == Option::from("Button") {
                    log("d\n");
                    let label = widget_def["label"].as_str();

                    if label.is_some() {
                        log("e\n");
                        self.widgets.insert(widget_id, Box::new(Button::new(widget_id, label.unwrap())));

                        log("f\n");
                    }
                }
            }
        }
    }

    pub fn render(&self) {

    }
}

pub struct Button {
    pub id: u64,
    pub label: String,
}

impl Button {
    pub fn new(id: u64, label: &str) -> Button {
        Button{
            id,
            label: label.parse().unwrap()
        }
    }
}

impl Render for Button {
    fn render(&self) {

    }
}

pub struct InputText {
    pub id: u64,
    pub value: String,
}

impl InputText {
    fn new(id: u64, value: &str) -> InputText {
        InputText{
            id,
            value: value.parse().unwrap()
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
