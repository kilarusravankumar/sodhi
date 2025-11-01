#include "login.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Login");

    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login");
    backButton = new QPushButton("Back");

    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::Box);

    QVBoxLayout *vLayout = new QVBoxLayout(frame);
    vLayout->addWidget(usernameEdit);
    vLayout->addWidget(passwordEdit);
    vLayout->addWidget(loginButton);
    vLayout->addWidget(backButton);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addWidget(frame);
    hLayout->addStretch();

    setLayout(hLayout);

    connect(loginButton, &QPushButton::clicked, this, &Login::loginUser);
    connect(backButton, &QPushButton::clicked, this, &Login::backClicked);
}

void Login::loginUser()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:8080/api/v1/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = usernameEdit->text();
    json["password"] = passwordEdit->text();

    QByteArray data = QJsonDocument(json).toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error()) {
            QMessageBox::critical(this, "Error", reply->errorString());
        } else {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            
            if (obj.contains("data")) {
                QJsonObject dataObj = obj["data"].toObject();
                activeToken = dataObj["active_token"].toString();
                refreshToken = dataObj["refresh_token"].toString();

                QJsonObject userObj = dataObj["user"].toObject();
                user = User(userObj["id"].toString(),
                            userObj["username"].toString(),
                            userObj["email"].toString(),
                            userObj["friends"].toVariant());

                QMessageBox::information(this, "Success", obj["msg"].toString());
            } else {
                QMessageBox::warning(this, "Error", obj["msg"].toString());
            }
        }
        reply->deleteLater();
    });
}