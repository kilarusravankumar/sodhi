#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QPushButton>

#include "login.h"
#include "register.h"

class TestLoginRegister : public QObject
{
    Q_OBJECT

private slots:
    void testRegister_passwordsMismatch();
    void testLogin_successfulSignal();
};

void TestLoginRegister::testRegister_passwordsMismatch()
{
    // Arrange
    Register registerWidget;
    QLineEdit *passEdit = registerWidget.findChild<QLineEdit*>("passwordEdit");
    QLineEdit *repeatPassEdit = registerWidget.findChild<QLineEdit*>("repeatPasswordEdit");

    QVERIFY(passEdit);
    QVERIFY(repeatPassEdit);

    passEdit->setText("password123");
    repeatPassEdit->setText("password456");

    // Act
    // We can't easily call the private slot directly, so we click the button.
    bool result = false;
    QMetaObject::invokeMethod(&registerWidget, "registerUser", Q_RETURN_ARG(bool, result));

    // Assert
    QCOMPARE(result, false);
}

void TestLoginRegister::testLogin_successfulSignal()
{
    // Arrange
    // We need a QNetworkAccessManager for the constructor, but we won't use it.
    QNetworkAccessManager manager;
    Login loginWidget(nullptr, &manager);
    QSignalSpy spy(&loginWidget, &Login::loginSuccessful);

    // A sample successful JSON response, taken from login.cpp comments
    QByteArray successJson = R"({
        "data": {
            "active_token": "test_active_token",
            "dashboard_data": {
                "rooms": []
            },
            "refresh_token": "test_refresh_token"
        },
        "msg": "Test User logged In"
    })";

    // Act
    loginWidget.processSuccessfulReply(successJson);

    // Assert
    QCOMPARE(spy.count(), 1); // Check that the signal was emitted exactly once

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "test_active_token");
    QCOMPARE(arguments.at(1).toString(), "test_refresh_token");
    QVERIFY(arguments.at(2).toJsonObject().contains("rooms"));
}

QTEST_MAIN(TestLoginRegister)
#include "test_login_register.moc"
