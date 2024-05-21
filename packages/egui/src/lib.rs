#![warn(clippy::all, rust_2018_idioms)]

mod app;

use std::collections::HashMap;
use std::ops::Deref;
use std::sync::{Arc, Mutex};
use eframe::Frame;
use egui::Context;
use once_cell::sync::Lazy;
use serde::{Deserialize, Serialize};
use wasm_bindgen::prelude::*;
use serde_json::{Value};
use erased_serde::serialize_trait_object;
use std::borrow::Borrow;


pub static REACT_EGUI: Lazy<Arc<Mutex<HashMap<u64, Box<dyn Render + Send>>>>> = Lazy::new(|| {
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

pub struct App {
    
}

impl App {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> App {
        App{
            
        }
    }
}

impl eframe::App for App {
    fn update(&mut self, ctx: &Context, _frame: &mut Frame) {
        let m = REACT_EGUI.lock().unwrap_throw();

        egui::CentralPanel::default().show(ctx, |ui| {
            for (_k, v) in m.iter() {
                v.render(ui)
            }
        });
    }
}

#[wasm_bindgen]
pub fn get_content() -> String {
    let m = REACT_EGUI.lock().unwrap_throw();

    return serde_json::to_string(&m.deref()).unwrap();
}

#[wasm_bindgen]
pub fn set_widget(raw_widget_def: String) {
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
                    if widget_def["label"].is_string() {
                        // m.insert(widget_id, Box::new(Button::new(widget_id, widget_def["label"].as_str().unwrap().to_owned())));
                        m.insert(widget_id, Box::new(Button::new(widget_id, None)));
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
    fn render(&self, ui: &mut egui::Ui);
}

#[derive(Serialize, Deserialize)]
pub struct Button {
    pub id: u64,
    pub label: Option<String>,
}
//
impl Button {
    pub fn new(id: u64, label: Option<String>) -> Button {
        Button{
            id,
            label
        }
    }
}
//
impl Render for Button {
    fn render(&self, ui: &mut egui::Ui) {
        // if self.label.is_some() {
        //     let str = self.label.unwrap().as_str();
        //     let _ = ui.button(str.as_ref());
        // } else {
            let _ = ui.button("Test");
        // }
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
    fn render(&self, ui: &mut egui::Ui) {

    }
}

serialize_trait_object!(Render);
