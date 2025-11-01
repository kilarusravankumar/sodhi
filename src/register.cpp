#include "register.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

Register::Register(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Register");

    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Username");

    emailEdit = new QLineEdit;
    emailEdit->setPlaceholderText("Email");

    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    repeatPasswordEdit = new QLineEdit;
    repeatPasswordEdit->setPlaceholderText("Repeat Password");
    repeatPasswordEdit->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("Register");
    backButton = new QPushButton("Back");

    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::Box);

    QVBoxLayout *vLayout = new QVBoxLayout(frame);
    vLayout->addWidget(usernameEdit);
    vLayout->addWidget(emailEdit);
    vLayout->addWidget(passwordEdit);
    vLayout->addWidget(repeatPasswordEdit);
    vLayout->addWidget(registerButton);
    vLayout->addWidget(backButton);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addWidget(frame);
    hLayout->addStretch();

    setLayout(hLayout);

    connect(registerButton, &QPushButton::clicked, this, &Register::registerUser);
    connect(backButton, &QPushButton::clicked, this, &Register::backClicked);
}

void Register::registerUser()
{
    if (passwordEdit->text() != repeatPasswordEdit->text()) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:8080/api/v1/register"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = usernameEdit->text();
    json["password"] = passwordEdit->text();
    json["email"] = emailEdit->text();

    QByteArray data = QJsonDocument(json).toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error()) {
            QMessageBox::critical(this, "Error", reply->errorString());
        } else {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            QString msg = obj["msg"].toString();
            QMessageBox::information(this, "Success", msg);
        }
        reply->deleteLater();
    });
}
