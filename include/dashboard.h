#pragma once

#include "room.h"
#include "config.h"
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
class QNetworkAccessManager;
class QShortcut;
class User;
class Room;

class Dashboard : public QWidget {
  Q_OBJECT

public:
  explicit Dashboard(Config *config, const QString &token, const QJsonObject &data,
                     QWidget *parent = nullptr);
  User getUser() const;
  Room *getActiveRoom() const;
  std::vector<Room> allRooms();

private slots:
  void onConnected();
  void onTextMessageReceived(const QString &message);
  void onBinaryMessageReceived(const QByteArray &message);
  void onSendMessage();
  void openCommandPalette();
  void createRoom(const QString &roomName);

private:
  void layout(const QJsonObject &data);

  Config *m_config;
  User *user;
  Room *activeRoom;
  std::vector<Room> rooms;
  QWidget *roomListPanel(const QJsonObject &data);
  QWidget *chatWindow(const QJsonObject &data);

  QWebSocket *m_webSocket;
  QLineEdit *m_chatInput;
  QVBoxLayout *m_chatLayout;
  QLabel *m_chatPlaceholder;

  QString m_activeToken;
  QNetworkAccessManager *m_networkManager;
  QShortcut *m_commandPaletteShortcut;
  QSplitter *m_splitter;
  void refreshDashboard(const QJsonObject &data);
  void loadChatWindow();
};
