#include "dashboard.h"
#include "commandpalette.h"
#include "messagebubble.h"
#include "room.h"
#include "roomslayout.h"
#include "user.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QScrollArea>
#include <QShortcut>
#include <QSplitter>
#include <QUrl>
#include <QVBoxLayout>
#include <QtWebSockets/QWebSocket>
#include <qdatetime.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qnetworkrequest.h>
#include <qsplitter.h>
#include <qurl.h>
#include <qvariant.h>
#include <qwebsocket.h>
#include <qwidget.h>

Dashboard::Dashboard(Config *config, const QString &token, const QJsonObject &data,
                     QWidget *parent)
    : QWidget(parent), m_config(config), activeRoom(nullptr), m_webSocket(new QWebSocket),
      m_chatInput(nullptr), m_chatLayout(nullptr), m_chatPlaceholder(nullptr),
      m_activeToken(token), m_networkManager(new QNetworkAccessManager(this)) {

  m_commandPaletteShortcut = new QShortcut(QKeySequence("Ctrl+P"), this);
  connect(m_commandPaletteShortcut, &QShortcut::activated, this,
          &Dashboard::openCommandPalette);

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

  qDebug() << "user logged in as" << user->username();

  layout(data);

  QUrl url(m_config->wsBaseUrl() + "/chat");
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
  if (m_chatLayout && activeRoom) {
    Message msg = Message::fromJson(message);

    if (m_chatPlaceholder) {
      m_chatPlaceholder->hide();
      m_chatLayout->removeWidget(m_chatPlaceholder);
      m_chatPlaceholder->deleteLater();
      m_chatPlaceholder = nullptr;
    }

    activeRoom->appendMessage(msg);

    bool isSelf = (msg.sentBy().id() == user->id());
    MessageBubble *bubble = new MessageBubble(msg, isSelf, this);
    m_chatLayout->addWidget(bubble);
  }
}

void Dashboard::onSendMessage() {
  if (m_chatInput && !m_chatInput->text().isEmpty() && activeRoom) {
    QJsonObject jsonObj;
    jsonObj["message"] = m_chatInput->text();

    QDateTime now = QDateTime::currentDateTimeUtc();

    QString sentTimestamp = now.toString(Qt::ISODate);
    jsonObj["sent_timestamp"] = sentTimestamp;
    jsonObj["room_id"] = activeRoom->id();
    jsonObj["sent_by"] = user->id();
    jsonObj["sender"] = user->toJson();
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
  m_splitter = new QSplitter(Qt::Horizontal, this);

  QWidget *leftPanel = roomListPanel(data);
  QWidget *rightPanel = chatWindow(data);

  // Add panels to splitter and set initial sizes
  m_splitter->addWidget(leftPanel);
  m_splitter->addWidget(rightPanel);
  m_splitter->setSizes({200, 600}); // Initial size ratio

  mainLayout->addWidget(m_splitter);
  setLayout(mainLayout);
}

QWidget *Dashboard::roomListPanel(const QJsonObject &data) {

  // --- Left Panel: Rooms List ---
  QWidget *leftPanel = new QWidget(this);
  QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
  leftLayout->setContentsMargins(10, 10, 10, 10);
  leftLayout->setSpacing(10);
  RoomsLayout *rooms = new RoomsLayout(data, activeRoom, leftPanel);
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

  m_chatPlaceholder = new QLabel("Messages will appear here...", chatContent);
  m_chatPlaceholder->setStyleSheet("color: #888;");
  m_chatLayout->addWidget(m_chatPlaceholder);

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

Room *Dashboard::getActiveRoom() const { return activeRoom; }

std::vector<Room> Dashboard::allRooms() { return rooms; }

void Dashboard::openCommandPalette() {
  CommandPalette *palette = CommandPalette::instance(this);
  connect(palette, &CommandPalette::createRoomRequested, this,
          &Dashboard::createRoom, Qt::UniqueConnection);
  palette->exec();
}

void Dashboard::createRoom(const QString &roomName) {
  QUrl url(m_config->apiBaseUrl() + "/room");
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QString bearerToken = "Bearer " + m_activeToken;
  request.setRawHeader("Authorization", bearerToken.toUtf8());
  request.setRawHeader("user-id", user->id().toLocal8Bit());

  qDebug() << "user id  i am passing is " << user->id();
  qDebug() << "user id belongs to user " << user->username();
  QJsonObject jsonBody;
  jsonBody["room_name"] = roomName;
  jsonBody["user_id"] = user->id();
  jsonBody["active_token"] = m_activeToken;
  jsonBody["is_dm"] = false;

  QJsonDocument doc(jsonBody);
  QByteArray data = doc.toJson();

  QNetworkReply *reply = m_networkManager->post(request, data);
  connect(reply, &QNetworkReply::finished, [this, reply]() {
    if (reply->error() == QNetworkReply::NoError) {
      QByteArray responseBytes = reply->readAll();
      qDebug() << "Room created successfully:" << responseBytes;

      QJsonDocument doc = QJsonDocument::fromJson(responseBytes);
      QJsonObject responseObj = doc.object();
      if (responseObj.contains("dashboard_data")) {
        refreshDashboard(responseObj["dashboard_data"].toObject());
      }
    } else {
      qDebug() << "Error creating room:" << reply->errorString();
    }
    reply->deleteLater();
  });
}

void Dashboard::refreshDashboard(const QJsonObject &data) {
  rooms.clear();
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
  } else {
    activeRoom = nullptr;
  }

  // Refresh the UI
  if (m_splitter) {
    QWidget *oldLeftPanel = m_splitter->widget(0);
    if (oldLeftPanel) {
      oldLeftPanel->hide();
      oldLeftPanel->deleteLater();
    }

    QWidget *newLeftPanel = roomListPanel(data);
    m_splitter->insertWidget(0, newLeftPanel);

    QList<int> currentSizes = m_splitter->sizes();
    if (currentSizes.size() >= 2) {
      m_splitter->setSizes(currentSizes);
    }
  }

  loadChatWindow();
}

void Dashboard::loadChatWindow() {
    if (!m_chatLayout) return;

    // Clear existing items
    QLayoutItem *child;
    while ((child = m_chatLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    if (!activeRoom) {
         m_chatPlaceholder = new QLabel("No room selected", this);
         m_chatPlaceholder->setStyleSheet("color: #888;");
         m_chatLayout->addWidget(m_chatPlaceholder);
         return;
    }

    std::vector<Message> msgs = activeRoom->messages();
    if (msgs.empty()) {
         m_chatPlaceholder = new QLabel("Messages will appear here...", this);
         m_chatPlaceholder->setStyleSheet("color: #888;");
         m_chatLayout->addWidget(m_chatPlaceholder);
    } else {
        m_chatPlaceholder = nullptr;
        for (const Message &msg : msgs) {
            bool isSelf = (msg.sentBy().id() == user->id());
            MessageBubble *bubble = new MessageBubble(msg, isSelf, this);
            m_chatLayout->addWidget(bubble);
        }
    }
}