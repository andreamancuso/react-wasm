#![warn(clippy::all, rust_2018_idioms)]

mod app;
mod input_text;
mod radio_button;
mod collapsing_header;
mod horizontal;
mod button;
mod checkbox;
mod table;

use std::any::Any;
use std::collections::HashMap;
use std::ops::{Deref};
use std::sync::{Arc, Mutex};
use eframe::Frame;
use egui::{Context, TextBuffer};
use once_cell::sync::Lazy;
use wasm_bindgen::prelude::*;
use serde_json::{Value};
use js_sys::{Array, Function, Object};
use crate::button::Button;
use crate::checkbox::Checkbox;
use crate::collapsing_header::CollapsingHeader;
use crate::horizontal::Horizontal;
use crate::input_text::InputText;
use crate::radio_button::{RadioButton, RadioButtonGroup};
use crate::table::Table;

type Widgets = HashMap<u32, Box<dyn Render + Send>>;
type Hierarchy = HashMap<u32, Vec<u32>>;

pub static WIDGETS: Lazy<Arc<Mutex<Widgets>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

pub static HIERARCHY: Lazy<Arc<Mutex<Hierarchy>>> = Lazy::new(|| {
    Arc::new(Mutex::new(HashMap::new()))
});

#[wasm_bindgen]
extern "C" {
    #[wasm_bindgen(js_namespace = console)]
    fn log(a: &str);
}

pub struct EventHandlers {
    on_click: JsValue,
    on_text_change: JsValue,
    on_bool_value_change: JsValue
}

impl EventHandlers {
    pub fn new(on_click: JsValue, on_text_change: JsValue, on_bool_value_change: JsValue) -> EventHandlers {
        EventHandlers{
            on_click,
            on_text_change,
            on_bool_value_change
        }
    }

    pub fn on_click(&self, id: u32) {
        self.on_click.unchecked_ref::<Function>().call1(&JsValue::NULL, &JsValue::from(id)).unwrap();
    }

    pub fn on_text_change(&self, id: u32, value: String) {
        self.on_text_change.unchecked_ref::<Function>().call2(&JsValue::NULL, &JsValue::from(id), &JsValue::from(value)).unwrap();
    }

    pub fn on_bool_value_change(&self, id: u32, value: bool) {
        self.on_bool_value_change.unchecked_ref::<Function>().call2(&JsValue::NULL, &JsValue::from(id), &JsValue::from(value)).unwrap();
    }
}

#[wasm_bindgen]
pub fn init_egui(on_click: JsValue, on_text_change: JsValue, on_bool_value_change: JsValue) {
    // Redirect `log` message to `console.log` and friends:
    eframe::WebLogger::init(log::LevelFilter::Debug).ok();

    let web_options = eframe::WebOptions::default();

    wasm_bindgen_futures::spawn_local(async {
        eframe::WebRunner::new()
            .start(
                "the_canvas_id", // hardcode it
                web_options,
                Box::new(|cc| Box::new(
                    crate::App::new(
                        EventHandlers::new(on_click, on_text_change, on_bool_value_change),
                        cc
                        )
                    )
                ),
            )
            .await
            .expect("failed to start eframe");
    });
}

pub struct App {
    event_handlers: EventHandlers
}

impl App {
    pub fn new(event_handlers: EventHandlers, _cc: &eframe::CreationContext<'_>) -> App {
        App{ event_handlers }
    }

    pub fn render_widget_by_id(&mut self, widgets: &mut Widgets, hierarchy: &Hierarchy, ui: &mut egui::Ui, id: u32) {
        if widgets.contains_key(&id) {
            ui.push_id(id, |ui| {
                match widgets.get(&id).unwrap_throw().get_type() {
                    "Horizontal" => {
                        ui.horizontal(|ui| {
                            self.render_children(widgets, hierarchy, ui, id);
                        });
                    }
                    "CollapsingHeader" => {
                        // todo: not sure which of the two approaches to take, bearing in mind that CollapsingState is the most flexible of the two
                        // egui::CollapsingHeader::new(widgets.get(&id).unwrap_throw().get_label())
                        //     .enabled(false)
                        //     .show(ui, |ui| {
                        //         self.render_children(widgets, hierarchy, ui, id);
                        //     });

                        let collapsing_header_id = ui.make_persistent_id(id);
                        let collapsing_state = egui::collapsing_header::CollapsingState::load_with_default_open(ui.ctx(), collapsing_header_id, true);

                        collapsing_state.show_header(ui, |ui| {
                                ui.label(widgets.get(&id).unwrap_throw().get_label()).clicked(); // you can put checkboxes or whatever here
                            })
                            .body(|ui| {
                                self.render_children(widgets, hierarchy, ui, id);
                            })
                        ;
                    }
                    &_ => {
                        widgets.get_mut(&id).unwrap().render(ui, self);
                    }
                }
            });
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
        let try_lock_result = WIDGETS.try_lock();
        let mut widgets = try_lock_result.unwrap();

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
pub fn get_hierarchy() -> String {
    let h = HIERARCHY.lock().unwrap_throw();

    return serde_json::to_string(&h.deref()).unwrap();
}

#[wasm_bindgen]
pub fn set_widget(raw_widget_def: String) {
    let try_lock_result = WIDGETS.try_lock();
    let mut widgets = try_lock_result.unwrap();

    let widget_def: Value = serde_json::from_str(&*raw_widget_def).unwrap();

    if widget_def.is_object() && widget_def["type"].is_string() {
        let maybe_widget_type = widget_def["type"].as_str();
        let maybe_widget_id = widget_def["id"].as_u64();

        if maybe_widget_id.is_some() && maybe_widget_type.is_some() {
            let widget_id = maybe_widget_id.unwrap() as u32;
            let widget_type = maybe_widget_type.unwrap();

            match widget_type {
                "Button" => {
                    let widget_result = Button::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "InputText" => {
                    let widget_result = InputText::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "Checkbox" => {
                    let widget_result = Checkbox::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "RadioButton" => {
                    let widget_result = RadioButton::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "RadioButtonGroup" => {
                    let widget_result = RadioButtonGroup::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "Horizontal" => {
                    let widget_result = Horizontal::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "CollapsingHeader" => {
                    let widget_result = CollapsingHeader::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    }
                }
                "Table" => {
                    let widget_result = Table::try_from(&widget_def);

                    if widget_result.is_ok() {
                        widgets.insert(widget_id, Box::new(widget_result.unwrap()));
                    } else {
                        log("Could not add Table widget");
                    }
                }
                &_ => {
                    log(format!("Unrecognised type: {}", widget_type).as_str());
                }
            }
        }
    }
}

#[wasm_bindgen]
pub fn append_data_to_table(widget_id: u32, data: Array) {
    let try_lock_result = WIDGETS.try_lock();
    let mut widgets = try_lock_result.unwrap();
    let mut maybe_widget = widgets.get_mut(&widget_id);

    if maybe_widget.is_some() {
        let mut unknown_widget = maybe_widget.unwrap();

        if unknown_widget.get_type().as_str() == "Table" {
            // let a: &mut Box<dyn Any> = unknown_widget;

            let a = unknown_widget.as_any();
            // let b = a.downcast_ref::<Table>();

            // if let Some(table) = any_mut.downcast_mut::<Table>() {
            //     let mut new_data = Vec::<HashMap<String, String>>::new();
            //
            //     for item in data.iter() {
            //         if item.is_object() {
            //             log("is_object returned true");
            //
            //             let obj: &Object = item.unchecked_ref();
            //             let mut row = HashMap::<String, String>::new();
            //
            //             for object_entry_as_js_value in Object::entries(&obj).iter() {
            //                 log("inside Object::entries");
            //
            //                 let object_entry: &Array = object_entry_as_js_value.unchecked_ref();
            //
            //                 let key = object_entry.get(0).as_string().unwrap();
            //                 let value = object_entry.get(1).as_string().unwrap();
            //
            //                 row.insert(key, value);
            //             }
            //
            //             new_data.push(row);
            //         } else {
            //             log("item in array not an object");
            //         }
            //     }
            //
            //     // table.append_data(&mut new_data);
            // } else  {
            //     log("downcasting unsuccessful");
            // }
        } else {
            log("widget not of the right type");
        }
    } else {
        log(format!("widget not found: {}", widget_id).as_str());
    }
}

// ----------------

pub trait AToAny: 'static {
    fn as_any(&mut self) -> &mut dyn Any;
}

impl<T: 'static> AToAny for T {
    fn as_any(&mut self) ->&mut (dyn Any + 'static) {
        self
    }
}

pub trait Render: AToAny {
    fn render(&mut self, ui: &mut egui::Ui, app: &App);

    fn get_type(&self) -> &str;

    fn get_label(&self) -> &str;
}

