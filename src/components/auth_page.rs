use dioxus::prelude::*;
use crate::components::{Login, Register};

#[component]
pub fn AuthPage() -> Element {
    rsx! {
        div {
            class: "flex flex-row h-screen items-center justify-center bg-gray-100 dark:bg-gray-900",
            div {
                class: "flex flex-row items-center justify-center space-x-12",
                
                div {
                    Login {}
                }

                div {
                    class: "w-px h-96 bg-gray-300 dark:bg-gray-700",
                }

                div {
                    Register {}
                }
            }
        }
    }
}
