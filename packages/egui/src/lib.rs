#![warn(clippy::all, rust_2018_idioms)]

mod app;

use std::collections::HashMap;
use std::ops::Deref;
use std::sync::{Arc, Mutex};
use eframe::Frame;
use egui::Context;
use once_cell::sync::Lazy;
use serde::{Deserialize, Serialize};
pub use app::TemplateApp;
use wasm_bindgen::prelude::*;
use serde_json::{Value};
use std::borrow::Borrow;
use erased_serde::serialize_trait_object;


pub static WIDGETS: Lazy<Arc<Mutex<HashMap<u64, Box<dyn Render + Send>>>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});
pub static HIERARCHY: Lazy<Arc<Mutex<HashMap<u64, Vec<u64>>>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = console)]
    fn log(a: &str);
}

#[wasm_bindgen]
pub fn init_egui() {
    // Redirect `log` message to `console.log` and friends:
    eframe::WebLogger::init(log::LevelFilter::Debug).ok();

    let web_options = eframe::WebOptions::default();

    wasm_bindgen_futures::spawn_local(async {
        eframe::WebRunner::new()
            .start(
                "the_canvas_id", // hardcode it
                web_options,
                Box::new(|cc| Box::new(crate::App::new(cc))),
            )
            .await
            .expect("failed to start eframe");
    });
}

pub struct App {}

impl App {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> App {
        App{}
    }

    pub fn render_widget_by_id(&mut self, w: &mut HashMap<u64, Box<dyn Render + Send>>, id: u64, ui: &mut egui::Ui) {
        if w.get(&id).is_some() {
            w.get_mut(&id).unwrap().render(ui);
        }
    }

    pub fn render_widgets(&mut self, w: &mut HashMap<u64, Box<dyn Render + Send>>, h: &HashMap<u64, Vec<u64>>, id: Option<u64>, ui: &mut egui::Ui) {
        let normalized_id = id.or(Some(0)).unwrap();

        if w.get(&normalized_id).is_some() {
            self.render_widget_by_id(w, normalized_id, ui);
        }

        if w.get(&normalized_id).is_none() {
            // render_children?
            self.render_children(w, h, normalized_id, ui);
        }
    }

    pub fn render_children(&mut self, w: &mut HashMap<u64, Box<dyn Render + Send>>, h: &HashMap<u64, Vec<u64>>, id: u64, ui: &mut egui::Ui) {
        if h.get(&id).is_some() {
            if !h.get(&id).unwrap().is_empty() {
                for val in h.get(&id).unwrap().iter() {
                    self.render_widgets(w, h, Some(*val), ui);
                }
            }
        }
    }
}

impl eframe::App for App {
    fn update(&mut self, ctx: &Context, _frame: &mut Frame) {
        let mut w = WIDGETS.lock().unwrap_throw();
        let h = HIERARCHY.lock().unwrap_throw();

        egui::CentralPanel::default().show(ctx, |ui| {
            self.render_widgets(&mut *w, h.deref(), None, ui);
        });
    }
}

#[wasm_bindgen]
pub fn append_child(parent_id: u64, child_id: u64) -> () {
    let mut m = HIERARCHY.lock().unwrap_throw();

    let children = m.get_mut(&parent_id);

    if children.is_some() {
        // todo: ensure child_id isn't already present...
        children.unwrap().push(child_id);
    }
}

#[wasm_bindgen]
pub fn set_children(parent_id: u64, raw_children_ids: String) -> () {
    let mut m = HIERARCHY.lock().unwrap_throw();

    let children_ids: Value = serde_json::from_str(&*raw_children_ids).unwrap();

    if children_ids.is_array() {
        // Convert array of JSON Value instances into Vec<u64>
        m.insert(parent_id, children_ids.as_array().unwrap().iter().map(|x| x.as_u64().unwrap()).collect());
    }
}

#[wasm_bindgen]
pub fn get_content() -> String {
    let m = WIDGETS.lock().unwrap_throw();

    return serde_json::to_string(&m.deref()).unwrap();
}

#[wasm_bindgen]
pub fn set_widget(raw_widget_def: String) {
    let mut m = WIDGETS.lock().unwrap_throw();

    let widget_def: Value = serde_json::from_str(&*raw_widget_def).unwrap();

    if widget_def.is_object() && widget_def["type"].is_string() {
        let maybe_widget_type = widget_def["type"].as_str();
        let maybe_widget_id = widget_def["id"].as_u64();

        if maybe_widget_id.is_some() && maybe_widget_type.is_some() {
            let widget_id = maybe_widget_id.unwrap();
            let widget_type = maybe_widget_type.unwrap();

            match widget_type {
                "Button" => {
                    if widget_def["label"].is_string() {
                        m.insert(widget_id, Box::new(Button::new(widget_id, widget_def["label"].as_str().unwrap())));
                    }
                }
                "InputText" => {
                    let default_value = widget_def["defaultValue"].as_str();

                    if default_value.is_some() {
                        m.insert(widget_id, Box::new(InputText::new(widget_id, default_value)));
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
    fn render(&mut self, ui: &mut egui::Ui);
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
    fn render(&mut self, ui: &mut egui::Ui) {
        let _ = ui.button(self.label.as_str());
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
    fn render(&mut self, ui: &mut egui::Ui) {
        ui.text_edit_singleline(&mut self.value);
    }
}

serialize_trait_object!(Render);
