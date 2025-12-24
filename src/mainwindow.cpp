#include "mainwindow.h"
#include "dashboard.h"
#include "landingpage.h"
#include "login.h"
#include "register.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <qnetworkaccessmanager.h>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  manager = new QNetworkAccessManager(parent);
  setWindowTitle("Sodhi");
  setMinimumSize(300, 200);

  stackedWidget = new QStackedWidget(this);

  QWidget *landingPage = createLandingPage();
  QWidget *loginPage = createLoginPage(manager);
  QWidget *registerPage = createRegisterPage();

  stackedWidget->addWidget(landingPage);  // Index 0
  stackedWidget->addWidget(loginPage);    // Index 1
  stackedWidget->addWidget(registerPage); // Index 2

  QHBoxLayout *windowLayout = new QHBoxLayout(this);
  windowLayout->addWidget(stackedWidget);

  setLayout(windowLayout);
}

// Destructor remains empty
MainWindow::~MainWindow() {}

QString MainWindow::getActiveToken() { return this->activeToken; }

QString MainWindow::getRefreshToken() { return this->refreshToken; }
// --- Our new private slots (the function implementations) ---

void MainWindow::showLoginPage() {
  qDebug() << "Switching to Login Page (Index 1)";
  stackedWidget->setCurrentIndex(1);
}

void MainWindow::showRegisterPage() {
  qDebug() << "Switching to Register Page (Index 2)";
  stackedWidget->setCurrentIndex(2);
}

void MainWindow::showLandingPage() {
  qDebug() << "Switching to Landing Page (Index 0)";
  stackedWidget->setCurrentIndex(0);
}

void MainWindow::handleLoginSuccessful(const QString &activeToken,
                                       const QString &refreshToken,
                                       const QJsonObject &data) {
  this->activeToken = activeToken;
  this->refreshToken = refreshToken;

  qDebug() << "Login successful! Switching to dashboard.";

  Dashboard *dashboardPage = new Dashboard(this->activeToken, data, this);

  int dashboardIndex = stackedWidget->addWidget(dashboardPage);
  stackedWidget->setCurrentIndex(dashboardIndex);
}

// --- Our new helper functions to create the pages ---

QWidget *MainWindow::createLandingPage() {
  LandingPage *landingPage = new LandingPage(this);
  connect(landingPage, &LandingPage::loginClicked, this,
          &MainWindow::showLoginPage);
  connect(landingPage, &LandingPage::registerClicked, this,
          &MainWindow::showRegisterPage);
  return landingPage;
}

QWidget *MainWindow::createLoginPage(QNetworkAccessManager *manager) {
  Login *loginPage = new Login(this, manager);
  connect(loginPage, &Login::backClicked, this, &MainWindow::showLandingPage);
  connect(loginPage, &Login::loginSuccessful, this,
          &MainWindow::handleLoginSuccessful);
  return loginPage;
}

QWidget *MainWindow::createRegisterPage() {
  Register *registerPage = new Register(this, manager);
  connect(registerPage, &Register::backClicked, this,
          &MainWindow::showLandingPage);
  connect(registerPage, &Register::registrationSuccessful, this,
          &MainWindow::showLoginPage);
  return registerPage;
}
