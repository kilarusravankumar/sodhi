#include "message.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <qlogging.h>

Message::Message(const QString &msgContent, const QString &sentTimestamp,
                 const QString &sentBy)
    : m_id(0), m_content(msgContent),
      m_sentTimestamp(QDateTime::fromString(sentTimestamp, Qt::ISODate)),
      m_sentByUuid(QUuid::fromString(sentBy)) {}

Message Message::fromJson(const QByteArray &jsonBytes) {
  QJsonDocument doc = QJsonDocument::fromJson(jsonBytes);
  if (doc.isNull() || !doc.isObject()) {
    return Message();
  }
  return fromJson(doc.object());
}

Message Message::fromJson(const QJsonObject &jsonObj) {
  Message msg;
  // Use toVariant().toLongLong() for broader compatibility, though toInteger()
  // works in Qt6
  msg.m_id = jsonObj["id"].toVariant().toLongLong();
  msg.m_content = jsonObj["message"].toString();

  QString timeStr = jsonObj["sent_timestamp"].toString();
  msg.m_sentTimestamp = QDateTime::fromString(timeStr, Qt::ISODate);

  msg.m_roomId = QUuid::fromString(jsonObj["room_id"].toString());
  msg.m_sentByUuid = QUuid::fromString(jsonObj["sent_by"].toString());

  msg.m_sender = User(jsonObj["sender"]["id"].toString(),
                      jsonObj["sender"]["username"].toString(),
                      jsonObj["sender"]["email"].toString(), QVariant());
  ;

  return msg;
}

QString Message::msgContent() const { return m_content; }

QString Message::sentTimestamp() const {
  return m_sentTimestamp.toString(Qt::ISODate);
}

User Message::sentBy() const { return m_sender; }

qint64 Message::id() const { return m_id; }

QUuid Message::roomId() const { return m_roomId; }

QUuid Message::sentByUuid() const { return m_sentByUuid; }

QDateTime Message::timestamp() const { return m_sentTimestamp; }
