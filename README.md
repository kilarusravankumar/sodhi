# Sodhi (Rust Desktop App)

A modern desktop application built with **Rust** and **Dioxus**, featuring a native WebView renderer.

## 🚧 Current Status: Prototype
This project is currently in the **prototyping phase**. We are building the authentication flows and core UI structure.

## 🛠 Tech Stack

- **Frontend Framework**: [Dioxus](https://dioxuslabs.com) (Rust)
- **Styling**: [Tailwind CSS](https://tailwindcss.com) (v3.4 via CDN)
- **HTTP Client**: `reqwest`
- **State Management**: Dioxus Signals & Context
- **Platform**: Desktop (via `dioxus-desktop` / WebView)

## ✨ Features implemented

### Authentication UI
- **Split-Screen Layout**: `AuthPage` component providing a side-by-side view of Login and Register forms.
- **Login Form**: User authentication (UI only currently).
- **Register Form**:
  - Full form validation (length, special characters, email format).
  - Real-time error feedback.
  - Submits to `http://localhost:8080/api/v1/register`.
- **Reusable Input Components**: Modular `Username`, `Password`, and `Email` components located in `src/components/input/`.

### 🎨 Styling & Dark Mode
- **Zero-Build Styling**: Tailwind CSS is currently **injected via CDN** in `main.rs` for rapid development.
- **Dark Mode**: Fully supported using Tailwind's `dark:` variant.
  - *Note*: The app respects the system theme preferences.

## 📂 Project Structure

```
src/
├── main.rs                 # Entry point, Router configuration, Window setup
├── state.rs                # Global AppState (User, Tokens)
└── components/
    ├── auth_page.rs        # Landing page with Login/Register split
    ├── login.rs            # Login form logic
    ├── register.rs         # Registration form with validation
    └── input/              # Reusable form fields
        ├── username.rs
        ├── password.rs
        └── email.rs
```

## 🚀 Getting Started

### Prerequisites
1.  **Rust**: [Install Rust](https://www.rust-lang.org/tools/install)
2.  **Dioxus CLI**:
    ```bash
    cargo install dioxus-cli
    ```
3.  **System Dependencies (Linux)**:
    ```bash
    sudo apt install libwebkit2gtk-4.0-dev build-essential libssl-dev libgtk-3-dev libayatana-appindicator3-dev librsvg2-dev
    ```

### Running the App
Start the development server with hot-reload:

```bash
dx serve --desktop
```

*Note: Since we use the Tailwind CDN, an internet connection is required for styling to appear correctly during development.*
