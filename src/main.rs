#![allow(non_snake_case)]

use dioxus::prelude::*;
mod components;
use components::AuthPage;
mod state;
use state::{use_app_state, AppState};

const FAVICON: Asset = asset!("/assets/favicon.ico");
const MAIN_CSS: Asset = asset!("/assets/main.css");
const HEADER_SVG: Asset = asset!("/assets/header.svg");
const TAILWIND_CSS: Asset = asset!("/assets/tailwind.css");

fn main() {
    let config = dioxus::desktop::Config::new()
        .with_window(dioxus::desktop::WindowBuilder::new().with_title("Sodhi"))
        .with_custom_head(r#"<script src="https://cdn.tailwindcss.com"></script>"#.to_string());

    LaunchBuilder::new().with_cfg(config).launch(App);
}

#[derive(Clone, Routable, Debug, PartialEq)]
enum Route {
    #[route("/")]
    Home {},
}

#[component]
fn App() -> Element {
    use_context_provider(|| Signal::new(AppState::default()));
    rsx! {
        Router::<Route> {}
    }
}

#[component]
fn Home() -> Element {
    let app_state = use_app_state();
    let is_logged_in = app_state.read().is_logged_in();

    if is_logged_in {
        rsx! {
            div { "We will implement this later" }
        }
    } else {
        rsx! {
            div{
                AuthPage {}
            }
        }
    }
}
