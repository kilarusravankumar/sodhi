use dioxus::prelude::*;
use serde::{Deserialize, Serialize};

#[derive(Clone, Debug, PartialEq, Deserialize, Serialize, Default)]
pub struct User {
    pub id: String,
    pub username: String,
    pub email: String,
}

#[derive(Clone, Debug)]
pub struct AppState {
    pub user: Option<User>,
    pub access_token: String,
    pub refresh_token: String,
}

impl Default for AppState {
    fn default() -> Self {
        Self {
            user: None,
            access_token: String::new(),
            refresh_token: String::new(),
        }
    }
}

impl AppState {
    pub fn is_logged_in(&self) -> bool {
        self.user.is_some()
    }

    pub fn username(&self) -> String {
        self.user
            .as_ref()
            .map(|u| u.username.clone())
            .unwrap_or_else(|| "Guest".to_string())
    }

    pub fn user_id(&self) -> String {
        self.user.as_ref().map(|u| u.id.clone()).unwrap_or_default()
    }
}

pub type GlobalState = Signal<AppState>;

pub fn use_app_state() -> GlobalState {
    use_context::<Signal<AppState>>()
}
