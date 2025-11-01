/*
 * mainwindow.h - Header file for our MainWindow class.
 *
 * We're updating this to handle multiple pages using a QStackedWidget.
 */

#pragma once

#include <QWidget>

// Forward declarations for all the Qt classes we'll use as pointers.
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget; // <-- The new class for page navigation

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// We've added private "slots". These are functions that
// signals (like a button click) can connect to.
private slots:
    void showLoginPage();
    void showRegisterPage();
    // We'll add a slot to go "back" to the landing page
    void showLandingPage();

private:
    // Helper function to create the landing page
    QWidget* createLandingPage();

    // Helper function to create the (simple) login page
    QWidget* createLoginPage();

    // Helper function to create the (simple) register page
    QWidget* createRegisterPage();

    // The main widget that holds all our "pages"
    QStackedWidget *stackedWidget;

    // We'll keep the buttons here so we can add a "back" button
    // to the other pages, but the rest of the landing page
    // widgets can be local to the createLandingPage() function.
    QPushButton *loginButton;
    QPushButton *registerButton;
};

