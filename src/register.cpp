#include "register.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVBoxLayout>

Register::Register(QWidget *parent, QNetworkAccessManager *manager)
    : QWidget(parent), manager(manager) {
  setWindowTitle("Register");

  usernameEdit = new QLineEdit;
  usernameEdit->setObjectName("usernameEdit");
  usernameEdit->setPlaceholderText("Username");

  emailEdit = new QLineEdit;
  emailEdit->setObjectName("emailEdit");
  emailEdit->setPlaceholderText("Email");

  passwordEdit = new QLineEdit;
  passwordEdit->setObjectName("passwordEdit");
  passwordEdit->setPlaceholderText("Password");
  passwordEdit->setEchoMode(QLineEdit::Password);

  repeatPasswordEdit = new QLineEdit;
  repeatPasswordEdit->setObjectName("repeatPasswordEdit");
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

void Register::clearInputFields() {
  usernameEdit->clear();
  emailEdit->clear();
  passwordEdit->clear();
  repeatPasswordEdit->clear();
}

bool Register::registerUser() {
  if (passwordEdit->text() != repeatPasswordEdit->text()) {
    QMessageBox::warning(this, "Error", "Passwords do not match.");
    return false;
  }

  registerButton->setEnabled(false);

  QNetworkRequest request(QUrl("http://localhost:8080/api/v1/register"));
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QJsonObject json;
  json["username"] = usernameEdit->text();
  json["password"] = passwordEdit->text();
  json["email"] = emailEdit->text();

  QByteArray data = QJsonDocument(json).toJson();

  QNetworkReply *reply = manager->post(request, data);

  connect(reply, &QNetworkReply::finished, [=]() {
    registerButton->setEnabled(true);

    if (reply->error()) {
      QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
      QJsonObject obj = doc.object();
      QString errorMessage = obj["error"].toString();
      if (!errorMessage.isEmpty()) {
        QMessageBox::critical(this, "Error", errorMessage);
      } else {
        QMessageBox::critical(this, "Error", reply->errorString());
      }
    } else {
      QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
      QJsonObject obj = doc.object();
      QString msg = obj["msg"].toString();
      QMessageBox::information(this, "Success", msg);

      clearInputFields();
      emit registrationSuccessful();
    }
    reply->deleteLater();
  });

  return true;
}
