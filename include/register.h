#ifndef REGISTER_H
#define REGISTER_H

#include "config.h"
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class QNetworkAccessManager;

class Register : public QWidget {
  Q_OBJECT

public:
  explicit Register(Config *config, QWidget *parent = nullptr,
                    QNetworkAccessManager *manager = nullptr);

private slots:
  bool registerUser();
  void clearInputFields();
signals:
  void backClicked();
  void registrationSuccessful();

private:
  Config *m_config;
  QLineEdit *usernameEdit;
  QLineEdit *emailEdit;
  QLineEdit *passwordEdit;
  QLineEdit *repeatPasswordEdit;
  QPushButton *registerButton;
  QPushButton *backButton;
  QNetworkAccessManager *manager;
};

#endif // REGISTER_H
