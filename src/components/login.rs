use crate::components::input::{Password, Username};
use dioxus::prelude::*;

#[component]
pub fn Login() -> Element {
    let mut username = use_signal(|| String::new());
    let mut password = use_signal(|| String::new());

    let on_submit = move |evt: Event<FormData>| {
        evt.prevent_default();
        // Login logic will go here
        println!("Login attempt for: {}", username());
    };

    rsx! {
        div {
            class: "p-8 bg-white rounded shadow-md w-96 dark:bg-gray-800",
            h2 {
                class: "mb-6 text-2xl font-bold text-center text-gray-800 dark:text-white",
                "Login"
            }
            form {
                onsubmit: on_submit,

                Username {
                    value: username(),
                    oninput: move |e:FormEvent| username.set(e.value())
                }

                Password {
                    value: password(),
                    oninput: move |e:FormEvent| password.set(e.value())
                }

                div {
                    class: "flex items-center justify-between mt-6",
                    button {
                        class: "w-full px-4 py-2 font-bold text-white bg-blue-500 rounded hover:bg-blue-700 focus:outline-none focus:shadow-outline",
                        r#type: "submit",
                        "Login"
                    }
                }
            }
        }
    }
}
