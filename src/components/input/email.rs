use dioxus::prelude::*;

#[component]
pub fn Email(value: String, oninput: EventHandler<FormEvent>) -> Element {
    rsx! {
        div {
            class: "mb-4",
            label { class: "block mb-2 text-sm font-bold text-gray-700 dark:text-gray-300", "Email" }
            input {
                class: "w-full px-3 py-2 leading-tight text-gray-700 border rounded shadow appearance-none focus:outline-none focus:shadow-outline dark:bg-gray-700 dark:text-white dark:border-gray-600",
                r#type: "email",
                placeholder: "enter your email",
                value: "{value}",
                oninput: move |e:FormEvent| oninput.call(e),
            }
        }
    }
}
