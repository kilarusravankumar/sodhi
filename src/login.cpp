#include "login.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVBoxLayout>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>

Login::Login(Config *config, QWidget *parent, QNetworkAccessManager *manager)
    : QWidget(parent), m_config(config), manager(manager) {
  setWindowTitle("Login");

  usernameEdit = new QLineEdit;
  usernameEdit->setObjectName("usernameEdit");
  usernameEdit->setPlaceholderText("Username");

  passwordEdit = new QLineEdit;
  passwordEdit->setObjectName("passwordEdit");
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

// {
//     "data": {
//         "active_token": "crzMKnwAUg45Hu6tGehzMSuEbFFzfpDbYO1oFJpEXew",
//         "dashboard_data": {
//             "friends": [],
//             "rooms": [
//                 {
//                     "created_at": "2025-11-11T10:11:13.296593-06:00",
//                     "created_by": "f537f24c-f59f-4ce5-a462-33f14df0b41c",
//                     "id": "63f5a143-bc55-4bd3-9050-cc7594adbc4e",
//                     "is_dm": false,
//                     "members": [
//                         {
//                             "email": "",
//                             "id": "f537f24c-f59f-4ce5-a462-33f14df0b41c",
//                             "username": "kreten"
//                         },
//                         {
//                             "email": "",
//                             "id": "183abbf4-20e0-46a1-a361-ab726d66d0c5",
//                             "username": "batman"
//                         },
//                         {
//                             "email": "",
//                             "id": "353899d5-30af-4f1b-8aff-783f7ce568c1",
//                             "username": "alfred"
//                         }
//                     ],
//                     "name": "batcave"
//                 }
//             ],
//             "user": {
//                 "email": "kreten@gmail.com",
//                 "id": "f537f24c-f59f-4ce5-a462-33f14df0b41c",
//                 "username": "kreten"
//             }
//         },
//         "refresh_token": "nDK5p79YC86S01ogMjj3s9j77dWjxY15L0lY9cMBxkE"
//     },
//     "msg": "kreten logged In"
// }

void Login::loginUser() {
  QNetworkRequest request(QUrl(m_config->apiBaseUrl() + "/login"));
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
      processSuccessfulReply(reply->readAll());
    }
    reply->deleteLater();
  });
}

void Login::processSuccessfulReply(const QByteArray &replyData) {
  QJsonDocument doc = QJsonDocument::fromJson(replyData);
  QJsonObject obj = doc.object();

  if (obj.contains("data")) {
    QJsonObject dataObj = obj["data"].toObject();
    activeToken = dataObj["active_token"].toString();
    refreshToken = dataObj["refresh_token"].toString();

    loginSuccessful(activeToken, refreshToken,
                    dataObj["dashboard_data"].toObject());
    QMessageBox::information(this, "Success", obj["msg"].toString());
  } else {
    QMessageBox::warning(this, "Error", obj["msg"].toString());
  }
}
