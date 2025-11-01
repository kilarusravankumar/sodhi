#ifndef USER_H
#define USER_H

#include <QString>
#include <QVariant>

class User
{
public:
    User() = default;
    User(const QString &id, const QString &username, const QString &email, const QVariant &friends);

    QString id() const;
    QString username() const;
    QString email() const;
    QVariant friends() const;

private:
    QString m_id;
    QString m_username;
    QString m_email;
    QVariant m_friends;
};

#endif // USER_H
