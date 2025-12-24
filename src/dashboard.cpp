#include "dashboard.h"
#include "room.h"
#include "roomslayout.h"
#include "user.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QUrl>
#include <QVBoxLayout>
#include <QtWebSockets/QWebSocket>
#include <qdatetime.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qnamespace.h>
#include <qnetworkrequest.h>
#include <qsplitter.h>
#include <qurl.h>
#include <qvariant.h>
#include <qwebsocket.h>
#include <qwidget.h>

Dashboard::Dashboard(const QString &token, const QJsonObject &data,
                     QWidget *parent)
    : QWidget(parent), activeRoom(nullptr), m_webSocket(new QWebSocket),
      m_chatInput(nullptr), m_chatLayout(nullptr) {

  const QJsonObject userJsonObj = data["user"].toObject();
  user = new User(
      userJsonObj["id"].toString(), userJsonObj["username"].toString(),
      userJsonObj["email"].toString(), userJsonObj["friends"].toVariant());
  QJsonArray roomsJsonArr = data["rooms"].toArray();

  for (const QJsonValue &r : roomsJsonArr) {
    QJsonObject roomObj = r.toObject();
    QString id = roomObj["id"].toString();
    QString name = roomObj["name"].toString();
    bool isDm = roomObj["is_dm"].toBool();

    std::vector<User> roomMembers;
    QJsonArray membersArr = roomObj["members"].toArray();
    for (const QJsonValue &m : membersArr) {
      QJsonObject memberObj = m.toObject();
      roomMembers.emplace_back(memberObj["id"].toString(),
                               memberObj["username"].toString(),
                               memberObj["email"].toString(), QVariant());
    }

    std::vector<Message> messages;
    rooms.emplace_back(id, name, isDm, roomMembers, messages);
  }

  if (!rooms.empty()) {
    activeRoom = &rooms[0];
  }

  layout(data);

  QUrl url("ws://localhost:8080/api/v1/chat");
  QNetworkRequest request(url);
  QString bearerToken = "Bearer " + token;
  request.setRawHeader("Authorization", bearerToken.toLocal8Bit());
  request.setRawHeader("user-id", user->id().toLocal8Bit());
  connect(m_webSocket, &QWebSocket::connected, this, &Dashboard::onConnected);
  connect(m_webSocket, &QWebSocket::textMessageReceived, this,
          &Dashboard::onTextMessageReceived);
  connect(m_webSocket, &QWebSocket::binaryMessageReceived, this,
          &Dashboard::onBinaryMessageReceived);

  m_webSocket->open(request);
}

void Dashboard::onConnected() { qDebug() << "WebSocket connected"; }

void Dashboard::onTextMessageReceived(const QString &message) {
  if (m_chatLayout) {
    QLabel *messageLabel = new QLabel(message, this);
    messageLabel->setWordWrap(true);
    m_chatLayout->addWidget(messageLabel);
  }
}

void Dashboard::onBinaryMessageReceived(const QByteArray &message) {
  if (m_chatLayout) {
    qDebug() << "Binary message received: " << message;
    Message msg = Message::fromJson(message);
    QLabel *messageLabel = new QLabel(msg.msgContent(), this);
    messageLabel->setWordWrap(true);
    m_chatLayout->addWidget(messageLabel);
  }
}

void Dashboard::onSendMessage() {
  if (m_chatInput && !m_chatInput->text().isEmpty()) {
    QJsonObject jsonObj;
    jsonObj["message"] = m_chatInput->text();

    QDateTime now = QDateTime::currentDateTimeUtc();

    QString sentTimestamp = now.toString(Qt::ISODate);
    jsonObj["sent_timestamp"] = sentTimestamp;
    jsonObj["room_id"] = activeRoom->id();
    jsonObj["sent_by"] = user->id();
    QJsonDocument jsonDoc(jsonObj);
    m_webSocket->sendBinaryMessage(jsonDoc.toJson(QJsonDocument::Compact));
    m_chatInput->clear();
  }
}

void Dashboard::layout(const QJsonObject &data) {
  // Main horizontal layout for the entire dashboard
  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  // Create a splitter to divide rooms list from the chat view
  QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

  QWidget *leftPanel = roomListPanel(data);
  QWidget *rightPanel = chatWindow(data);

  // Add panels to splitter and set initial sizes
  splitter->addWidget(leftPanel);
  splitter->addWidget(rightPanel);
  splitter->setSizes({200, 600}); // Initial size ratio

  mainLayout->addWidget(splitter);
  setLayout(mainLayout);
}

QWidget *Dashboard::roomListPanel(const QJsonObject &data) {

  // --- Left Panel: Rooms List ---
  QWidget *leftPanel = new QWidget(this);
  QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
  leftLayout->setContentsMargins(10, 10, 10, 10);
  leftLayout->setSpacing(10);
  RoomsLayout *rooms = new RoomsLayout(data, leftPanel);
  leftPanel->setLayout(leftLayout);
  return leftPanel;
}

// TODO:  make chat messages appear and create websocket connection alive for
// this room

QWidget *Dashboard::chatWindow(const QJsonObject &data) {
  // --- Right Panel: Chat View ---
  QWidget *rightPanel = new QWidget();
  QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
  rightLayout->setContentsMargins(10, 10, 10, 10);
  rightLayout->setSpacing(5);

  // This will be the area where chat messages appear
  QScrollArea *chatArea = new QScrollArea(rightPanel);
  chatArea->setWidgetResizable(true);
  QWidget *chatContent = new QWidget(chatArea);
  m_chatLayout = new QVBoxLayout(chatContent);
  m_chatLayout->setAlignment(Qt::AlignTop);
  QLabel *chatPlaceholder =
      new QLabel("Messages will appear here...", chatContent);
  chatPlaceholder->setStyleSheet("color: #888;");
  m_chatLayout->addWidget(chatPlaceholder);
  chatContent->setLayout(m_chatLayout);
  chatArea->setWidget(chatContent);

  // --- Chat Input Bar ---
  QWidget *inputBarWidget = new QWidget(rightPanel);
  QHBoxLayout *inputLayout = new QHBoxLayout(inputBarWidget);
  inputLayout->setContentsMargins(0, 5, 0, 0);

  m_chatInput = new QLineEdit(inputBarWidget);
  m_chatInput->setPlaceholderText("Type a message...");
  m_chatInput->setStyleSheet("font-size: 14px; padding: 8px;");

  QPushButton *sendButton = new QPushButton("Send", inputBarWidget);
  sendButton->setStyleSheet("font-size: 14px; padding: 8px;");
  connect(m_chatInput, &QLineEdit::returnPressed, sendButton,
          &QPushButton::animateClick);
  connect(sendButton, &QPushButton::clicked, this, &Dashboard::onSendMessage);

  inputLayout->addWidget(m_chatInput);
  inputLayout->addWidget(sendButton);
  inputBarWidget->setLayout(inputLayout);

  rightLayout->addWidget(chatArea);
  rightLayout->addWidget(inputBarWidget);
  rightPanel->setLayout(rightLayout);

  return rightPanel;
}

User Dashboard::getUser() const { return *user; }

Room Dashboard::getActiveRoom() const { return *activeRoom; }

std::vector<Room> Dashboard::allRooms() { return rooms; }
