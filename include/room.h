#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <message.h>
#include <user.h>
#include <vector>

class Room {
public:
  Room(const QString &id, const QString &name, const bool isDirectMsg,
       const std::vector<User> &users, std::vector<Message> &messages);

  QString id() const;
  QString name() const;
  bool isDirectMsg() const;
  std::vector<User> users() const;
  std::vector<Message> messages() const;

private:
  QString roomID;
  QString roomName;
  bool isDirectMsgFlag;
  std::vector<User> roomUsers;
  std::vector<Message> roomMsgs;
};

#endif // !ROOM_H
