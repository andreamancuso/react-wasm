#![warn(clippy::all, rust_2018_idioms)]

mod app;

use std::collections::HashMap;
use std::ops::{Deref};
use std::sync::{Arc, Mutex};
use eframe::Frame;
use eframe::web_sys::js_sys::Function;
use egui::{Context};
use once_cell::sync::Lazy;
use serde::{Deserialize, Serialize};
use wasm_bindgen::prelude::*;
use serde_json::{Value};
use erased_serde::serialize_trait_object;
use wasm_bindgen::prelude::*;

// type OnClickFn = &'static fn(u32) -> Result<JsValue, JsValue>;
type Widgets = HashMap<u32, Box<dyn Render + Send>>;
type Hierarchy = HashMap<u32, Vec<u32>>;

pub static WIDGETS: Lazy<Arc<Mutex<Widgets>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

pub static HIERARCHY: Lazy<Arc<Mutex<Hierarchy>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

// pub static EVENT_HANDLERS: Lazy<Arc<Mutex<EventHandlers>>> = Lazy::new(|| {
//     Arc::new(Mutex::new(HashMap::new()))
// });

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = console)]
    fn log(a: &str);
}

// pub fn with_stdout(f: &Function) {
//     let _ = f.call1(&JsValue::NULL, &JsValue::from_str("Hello world!"));
// }

// pub struct EventHandlers {
//     on_click: Box<dyn Fn(u32) -> Result<JsValue, JsValue>>,
//     on_change: Box<dyn Fn(u32) -> Result<JsValue, JsValue>>
// }
//
// impl EventHandlers {
//     pub fn new(on_click: Box<dyn Fn(u32) -> Result<JsValue, JsValue>>, on_change: Box<dyn Fn(u32) -> Result<JsValue, JsValue>>) -> EventHandlers {
//         EventHandlers{
//             on_click,
//             on_change
//         }
//     }
// }

// #[wasm_bindgen]
// pub struct EventHandlers {
//     on_click: js_sys::Function,
//     on_change: js_sys::Function,
// }
//
// impl EventHandlers {
//     #[wasm_bindgen(static_method_of = EventHandlers)]
//     pub fn new(on_click: js_sys::Function, on_change: js_sys::Function) -> EventHandlers {
//         EventHandlers {
//             on_click,
//             on_change
//         }
//     }
// }

#[wasm_bindgen]
pub struct VecU32 {
    xs: Vec<u32>,
}

#[wasm_bindgen]
impl VecU32 {
    pub fn each(&self, f: &js_sys::Function) {
        let this = JsValue::null();
        for &x in &self.xs {
            let x = JsValue::from(x);
            let _ = f.call1(&this, &x);
        }
    }
}

#[wasm_bindgen]
pub fn init_egui(event_handlers: VecU32) {
    // Redirect `log` message to `console.log` and friends:
    eframe::WebLogger::init(log::LevelFilter::Debug).ok();

    let web_options = eframe::WebOptions::default();

    // let a = OwningRef::new(Box::new(on_click));

    wasm_bindgen_futures::spawn_local(async {
        eframe::WebRunner::new()
            .start(
                "the_canvas_id", // hardcode it
                web_options,
                Box::new(|cc| Box::new(
                    crate::App::new(
                        cc))),
            )
            .await
            .expect("failed to start eframe");
    });
}

pub struct App {
    // event_handlers: EventHandlers
}

impl App {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> App {
        App{  }
    }

    pub fn render_widget_by_id(&mut self, widgets: &mut Widgets, hierarchy: &Hierarchy, ui: &mut egui::Ui, id: u32) {
        if widgets.get(&id).is_some() {
            let widget = widgets.get_mut(&id).unwrap();

            match widget.get_type().as_str() {
                "Horizontal" => {
                    ui.horizontal(|ui| {
                        self.render_children(widgets, hierarchy, ui, id);
                    });
                }
                &_ => {
                    widget.render(ui);
                }
            }
        }
    }

    pub fn render_widgets(&mut self, widgets: &mut Widgets, hierarchy: &Hierarchy, ui: &mut egui::Ui, id: Option<u32>) {
        let normalized_id = id.or(Some(0)).unwrap();

        if widgets.get(&normalized_id).is_some() {
            self.render_widget_by_id(widgets, hierarchy, ui, normalized_id);
        }

        if widgets.get(&normalized_id).is_none() {
            // render_children?
            self.render_children(widgets, hierarchy, ui, normalized_id);
        }
    }

    pub fn render_children(&mut self, w: &mut Widgets, hierarchy: &Hierarchy, ui: &mut egui::Ui, id: u32) {
        if hierarchy.get(&id).is_some() {
            let children_ids = hierarchy.get(&id).unwrap();

            if !children_ids.is_empty() {
                for val in children_ids.iter() {
                    self.render_widgets(w, hierarchy, ui, Some(*val));
                }
            }
        }
    }
}

impl eframe::App for App {
    fn update(&mut self, ctx: &Context, _frame: &mut Frame) {
        let mut widgets = WIDGETS.lock().unwrap_throw();
        let hierarchy = HIERARCHY.lock().unwrap_throw();

        egui::CentralPanel::default().show(ctx, |ui| {
            self.render_widgets(&mut widgets, hierarchy.deref(), ui, None);
        });
    }
}

#[wasm_bindgen]
pub fn append_child(parent_id: u32, child_id: u32) -> () {
    let mut m = HIERARCHY.lock().unwrap_throw();

    let children = m.get_mut(&parent_id);

    if children.is_some() {
        let vec = children.unwrap();

        if !vec.contains(&child_id) {
            vec.push(child_id);
        }
    } else {
        m.insert(parent_id, vec![child_id]);
    }
}

#[wasm_bindgen]
pub fn set_children(parent_id: u32, raw_children_ids: String) -> () {
    let mut m = HIERARCHY.lock().unwrap_throw();

    let children_ids: Value = serde_json::from_str(&*raw_children_ids).unwrap();

    if children_ids.is_array() {
        // Convert array of JSON Value instances into Vec<u32>
        m.insert(parent_id, children_ids.as_array().unwrap().iter().map(|x| x.as_u64().unwrap() as u32).collect());
    }
}

#[wasm_bindgen]
pub fn get_widgets() -> String {
    let m = WIDGETS.lock().unwrap_throw();

    return serde_json::to_string(&m.deref()).unwrap();
}

#[wasm_bindgen]
pub fn get_hierarchy() -> String {
    let h = HIERARCHY.lock().unwrap_throw();

    return serde_json::to_string(&h.deref()).unwrap();
}

#[wasm_bindgen]
pub fn set_widget(raw_widget_def: String) {
    let mut m = WIDGETS.lock().unwrap_throw();

    let widget_def: Value = serde_json::from_str(&*raw_widget_def).unwrap();

    if widget_def.is_object() && widget_def["type"].is_string() {
        let maybe_widget_type = widget_def["type"].as_str();
        let maybe_widget_id = widget_def["id"].as_u64();

        if maybe_widget_id.is_some() && maybe_widget_type.is_some() {
            let widget_id = maybe_widget_id.unwrap() as u32;
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
                "Horizontal" => {
                    m.insert(widget_id, Box::new(Horizontal::new(widget_id)));
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

    fn get_type(&self) -> String;
}

#[derive(Serialize, Deserialize)]
pub struct Button {
    pub id: u32,
    pub label: String,
}
//
impl Button {
    pub fn new(id: u32, label: &str) -> Button {
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

    fn get_type(&self) -> String {
        return "Button".parse().unwrap();
    }
}

#[derive(Serialize, Deserialize)]
pub struct InputText {
    pub id: u32,
    pub value: String,
}

impl InputText {
    fn new(id: u32, value: Option<&str>) -> InputText {
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

    fn get_type(&self) -> String {
        return "InputText".parse().unwrap();
    }
}

#[derive(Serialize, Deserialize)]
struct Horizontal {
    pub id: u32
}

impl Horizontal {
    fn new(id: u32) -> Horizontal {
        Horizontal{
           id
        }
    }
}

impl Render for Horizontal {
    fn render(&mut self, _ui: &mut egui::Ui) {}

    fn get_type(&self) -> String {
        return "Horizontal".parse().unwrap();
    }
}

serialize_trait_object!(Render);
