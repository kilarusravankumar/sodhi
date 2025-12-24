#ifndef FRIEND_H
#define FRIEND_H

#include <QString>

class Friend {
public:
  Friend();
  Friend(const QString &username);
  QString username() const;

private:
  QString _username;
};

#endif // !FRIEND_H
