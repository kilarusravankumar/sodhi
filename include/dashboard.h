#pragma once

#include "room.h"
#include <QJsonObject>
#include <QWidget>
#include <qwidget.h>

class QLabel;
class QVBoxLayout;
class QScrollArea;
class QPushButton;
class QLineEdit;
class QSplitter;
class QWebSocket;
class User;
class Room;

class Dashboard : public QWidget {
  Q_OBJECT

public:
  explicit Dashboard(const QString &token, const QJsonObject &data,
                     QWidget *parent = nullptr);
  User getUser() const;
  Room getActiveRoom() const;
  std::vector<Room> allRooms();

private slots:
  void onConnected();
  void onTextMessageReceived(const QString &message);
  void onBinaryMessageReceived(const QByteArray &message);
  void onSendMessage();

private:
  void layout(const QJsonObject &data);

  User *user;
  Room *activeRoom;
  std::vector<Room> rooms;
  QWidget *roomListPanel(const QJsonObject &data);
  QWidget *chatWindow(const QJsonObject &data);

  QWebSocket *m_webSocket;
  QLineEdit *m_chatInput;
  QVBoxLayout *m_chatLayout;
};
