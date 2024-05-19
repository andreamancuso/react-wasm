#![warn(clippy::all, rust_2018_idioms)]

mod app;

use serde::{Serialize, Deserialize};
use std::any::Any;
use std::collections::HashMap;
use std::sync::Mutex;
use std::sync::{RwLock};
pub use app::TemplateApp;
use wasm_bindgen::prelude::*;
use serde_json::{Value};

static mut REACT_EGUI: Mutex<Option<ReactEgui>> = Mutex::new(None);

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = console)]
    fn log(a: &str);
}

#[wasm_bindgen]
pub unsafe fn init_react_egui() {
    let mut m = REACT_EGUI.lock().unwrap();

    let _ = m.insert(ReactEgui::new());
}

#[wasm_bindgen]
pub unsafe fn add_widget(raw_widget_def: String) {
    let mut m = REACT_EGUI.lock().unwrap_throw();

    if (m.is_some()) {
        m.as_ref().unwrap_throw().add_widget(raw_widget_def);
    }
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
                        // self.widgets.write().unwrap().insert(1, Button::new(widget_id, label.unwrap()));
                        self.widgets.write().unwrap().insert(widget_id, widget_id);

                        log("f\n");
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