#ifndef LOGIN_H
#define LOGIN_H

#include "user.h"
#include "config.h"
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <qjsonobject.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>

class Login : public QWidget {
  Q_OBJECT

public:
  Login(Config *config, QWidget *parent = nullptr, QNetworkAccessManager *manager = nullptr);
  void processSuccessfulReply(const QByteArray &replyData);
signals:
  void loginSuccessful(const QString &activeToken, const QString &refreshToken,
                     const QJsonObject &data);
  void backClicked();
private slots:
  void loginUser();

private:
  Config *m_config;
  QLineEdit *usernameEdit;
  QLineEdit *passwordEdit;
  QPushButton *loginButton;
  QPushButton *backButton;
  QNetworkAccessManager *manager;
  QString activeToken;
  QString refreshToken;
  User user;
};

#endif // LOGIN_H
