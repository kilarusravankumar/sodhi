#include <friend.h>

Friend::Friend(const QString &username) : _username(username) {}

QString Friend::username() const { return _username; }
