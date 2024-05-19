#![warn(clippy::all, rust_2018_idioms)]

mod app;

use std::any::Any;
use std::collections::HashMap;
use std::sync::Mutex;
use std::sync::{RwLock};
pub use app::TemplateApp;
use wasm_bindgen::prelude::*;
use serde_json::{Value};

#[wasm_bindgen]
extern "C" {
    fn alert(s: &str);
}


static mut REACT_EGUI: Mutex<Option<ReactEgui>> = Mutex::new(None);

pub unsafe fn init_react_egui() {
    let mut m = REACT_EGUI.lock().unwrap();

    let _ = m.insert(ReactEgui::new());
}

#[wasm_bindgen]
pub fn greet(name: &str) {
    alert(&format!("Hello, {}!", name));
}


pub struct Widget<T> {
    id: u64,
    widget_type: String,
    props: Option<T>
}

impl Widget<()> {}

pub struct ReactEgui {
    pub widgets: RwLock<HashMap<u64, u64>>
}

impl ReactEgui {
    pub fn new() -> ReactEgui {
        ReactEgui{
            widgets: RwLock::new(HashMap::new())
        }
    }

    pub fn add_widget(&self, raw_widget_def: String) {
        let widget_def: Value = serde_json::from_str(&*raw_widget_def).unwrap();

        if widget_def.is_object() && widget_def["type"].is_string() {
            let maybe_widget_type = widget_def["type"].as_str();
            let maybe_widget_id = widget_def["id"].as_u64();

            if maybe_widget_id.is_some() && maybe_widget_type.is_some() {
                let widget_id = maybe_widget_id.unwrap();

                if maybe_widget_type == Option::from("Button") {
                    let label = widget_def["label"].as_str();

                    if label.is_some() {
                        // self.widgets.write().unwrap().insert(1, Button::new(widget_id, label.unwrap()));
                        self.widgets.write().unwrap().insert(widget_id, widget_id);
                    }
                }
            }
        }
    }
}


struct ButtonProps {
    label: String
}

trait Render {
    fn render(&self) -> ();
}

trait ButtonWidget: Render + Button {}

trait Button {
    fn render(self) -> ();
    fn new(widget_id: u64, label: &str) -> Widget<ButtonProps>;
}

impl Button for Widget<ButtonProps> {
    fn render(self) {}

    fn new(widget_id: u64, label: &str) -> Widget<ButtonProps> {
        Widget{
            id: widget_id,
            widget_type: "Unknown".parse().unwrap(),
            props: Option::from(ButtonProps {
                label: label.to_string()
            }),
        }
    }
}