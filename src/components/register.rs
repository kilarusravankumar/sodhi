use crate::components::input::Email;
use crate::components::input::Password;
use crate::components::input::Username;
use dioxus::prelude::*;
use reqwest::Client;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
struct RegisterRequest {
    username: String,
    password: String,
    email: String,
}

#[component]
pub fn Register() -> Element {
    let mut username = use_signal(|| String::new());
    let mut password = use_signal(|| String::new());
    let mut email = use_signal(|| String::new());
    let mut error_message = use_signal(|| String::new());
    let mut success_message = use_signal(|| String::new());
    let mut is_submitting = use_signal(|| false);

    let validate_form = move || -> Result<(), String> {
        let u = username();
        let p = password();
        let e = email();

        if u.len() <= 3 {
            return Err("Username must be greater than 3 characters.".to_string());
        }

        if p.len() <= 8 {
            return Err("Password must be greater than 8 characters.".to_string());
        }

        if !p.chars().any(|c| c.is_uppercase()) {
            return Err("Password must contain at least one capital letter.".to_string());
        }

        if !p.chars().any(|c| !c.is_alphanumeric()) {
            return Err("Password must contain at least one symbol.".to_string());
        }

        // Basic email validation
        if !e.contains('@') || !e.contains('.') || e.len() < 5 {
            return Err("Please enter a valid email address.".to_string());
        }

        Ok(())
    };

    let on_submit = move |evt: Event<FormData>| {
        evt.prevent_default();
        async move {
            error_message.set(String::new());
            success_message.set(String::new());

            if let Err(e) = validate_form() {
                error_message.set(e);
                return;
            }

            is_submitting.set(true);

            let request_body = RegisterRequest {
                username: username(),
                password: password(),
                email: email(),
            };

            let client = Client::new();
            match client
                .post("http://localhost:8080/api/v1/register")
                .json(&request_body)
                .send()
                .await
            {
                Ok(response) => {
                    if response.status().is_success() {
                        success_message.set("Registration successful!".to_string());
                        // Optional: Clear fields
                        username.set(String::new());
                        password.set(String::new());
                        email.set(String::new());
                    } else {
                        error_message
                            .set(format!("Registration failed: Status {}", response.status()));
                    }
                }
                Err(e) => {
                    error_message.set(format!("Network error: {}", e));
                }
            }
            is_submitting.set(false);
        }
    };

    rsx! {
        div {
            class: "p-8 bg-white rounded shadow-md w-96 dark:bg-gray-800",
            h2 {
                class: "mb-6 text-2xl font-bold text-center text-gray-800 dark:text-white",
                "Register"
            }

            if !error_message().is_empty() {
                div {
                    class: "mb-4 p-3 bg-red-100 border border-red-400 text-red-700 rounded dark:bg-red-900 dark:text-red-100 dark:border-red-700",
                    "{error_message}"
                }
            }

            if !success_message().is_empty() {
                div {
                    class: "mb-4 p-3 bg-green-100 border border-green-400 text-green-700 rounded dark:bg-green-900 dark:text-green-100 dark:border-green-700",
                    "{success_message}"
                }
            }

            form {
                onsubmit: on_submit,

                Username {
                    value: "{username}",
                oninput: move |e: FormEvent| username.set(e.value()) ,
                }

                Email{
                    value: "{email}",
                    oninput: move |e :FormEvent| email.set(e.value()),
                }

                Password {
                    value: "{password}",
                    oninput: move |e :FormEvent| password.set(e.value()),
                }
                 p { class: "text-xs text-gray-500 mb-6 -mt-4 dark:text-gray-400", "Min 8 chars, 1 capital, 1 symbol" }

                div {
                    class: "flex items-center justify-between",
                    button {
                        class: "w-full px-4 py-2 font-bold text-white bg-blue-500 rounded hover:bg-blue-700 focus:outline-none focus:shadow-outline disabled:opacity-50",
                        r#type: "submit",
                        disabled: "{is_submitting}",
                        if is_submitting() { "Registering..." } else { "Register" }
                    }
                }
            }
        }
    }
}
