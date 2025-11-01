#include "mainwindow.h"
#include "register.h"
#include "login.h"
#include "landingpage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout> 
#include <QStackedWidget> 
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 1. Set window properties
    setWindowTitle("Sodhi");
    setMinimumSize(300, 200);

    // 2. Create the QStackedWidget
    //    This widget will hold all our pages.
    stackedWidget = new QStackedWidget(this);

    // 3. Create our pages by calling our helper functions
    //    Note: We're not using private member variables for these
    //    pages, because the stackedWidget will "own" them and
    //    we can just refer to them by their index (0, 1, 2).
    QWidget *landingPage = createLandingPage();
    QWidget *loginPage = createLoginPage();
    QWidget *registerPage = createRegisterPage();

    // 4. Add the pages to the stacked widget
    //    The order matters!
    stackedWidget->addWidget(landingPage);  // Index 0
    stackedWidget->addWidget(loginPage);    // Index 1
    stackedWidget->addWidget(registerPage); // Index 2

    // 5. Create a main layout for the *entire window*
    //    This layout will just contain our stackedWidget,
    //    making sure it fills the whole window.
    QHBoxLayout *windowLayout = new QHBoxLayout(this);
    windowLayout->addWidget(stackedWidget);

    // 6. Set the window's layout
    setLayout(windowLayout);
}

// Destructor remains empty
MainWindow::~MainWindow()
{
}

// --- Our new private slots (the function implementations) ---

void MainWindow::showLoginPage()
{
    qDebug() << "Switching to Login Page (Index 1)";
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showRegisterPage()
{
    qDebug() << "Switching to Register Page (Index 2)";
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showLandingPage()
{
    qDebug() << "Switching to Landing Page (Index 0)";
    stackedWidget->setCurrentIndex(0);
}

// --- Our new helper functions to create the pages ---

QWidget* MainWindow::createLandingPage()
{
    LandingPage *landingPage = new LandingPage(this);
    connect(landingPage, &LandingPage::loginClicked, this, &MainWindow::showLoginPage);
    connect(landingPage, &LandingPage::registerClicked, this, &MainWindow::showRegisterPage);
    return landingPage;
}

QWidget* MainWindow::createLoginPage()
{
    Login *loginPage = new Login(this);
    connect(loginPage, &Login::backClicked, this, &MainWindow::showLandingPage);
    return loginPage;
}

QWidget* MainWindow::createRegisterPage()
{
    Register *registerPage = new Register(this);
    connect(registerPage, &Register::backClicked, this, &MainWindow::showLandingPage);
    return registerPage;
}
