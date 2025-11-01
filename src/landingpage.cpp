#include "landingpage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

LandingPage::LandingPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Welcome to Sodhi");
    QPushButton *loginButton = new QPushButton("Login");
    QPushButton *registerButton = new QPushButton("Register");

    layout->addWidget(label);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LandingPage::loginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LandingPage::registerClicked);
}
