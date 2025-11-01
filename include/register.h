#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);

signals:
    void backClicked();

private slots:
    void registerUser();

private:
    QLineEdit *usernameEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *repeatPasswordEdit;
    QPushButton *registerButton;
    QPushButton *backButton;
};

#endif // REGISTER_H
