#include "user.h"

User::User(const QString &id, const QString &username, const QString &email, const QVariant &friends)
    : m_id(id), m_username(username), m_email(email), m_friends(friends)
{
}

QString User::id() const
{
    return m_id;
}

QString User::username() const
{
    return m_username;
}

QString User::email() const
{
    return m_email;
}

QVariant User::friends() const
{
    return m_friends;
}
