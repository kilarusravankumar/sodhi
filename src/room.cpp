#include "room.h"
#include "message.h"

Room::Room(const QString &id, const QString &name, const bool isDirectMsg,
           const std::vector<User> &users, std::vector<Message> &messages)
    : roomID(id), roomName(name), isDirectMsgFlag(isDirectMsg),
      roomUsers(users), roomMsgs(messages) {}

QString Room::id() const { return roomID; }

QString Room::name() const { return roomName; }

bool Room::isDirectMsg() const { return isDirectMsgFlag; }

std::vector<User> Room::users() const { return roomUsers; }

std::vector<Message> Room::messages() const { return roomMsgs; }

void Room::appendMessage(Message &msg) { roomMsgs.push_back(msg); }
