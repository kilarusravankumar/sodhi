#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QUuid>

class Message {
public:
  Message() = default;
  Message(const QString &msgContent, const QString &sentTimestamp,
          const QString &sentBy);

  static Message fromJson(const QByteArray &jsonBytes);
  static Message fromJson(const QJsonObject &jsonObj);

  QString msgContent() const;
  QString sentTimestamp() const;
  QString sentBy() const;

  qint64 id() const;
  QUuid roomId() const;
  QUuid sentByUuid() const;
  QDateTime timestamp() const;

private:
  qint64 m_id;
  QString m_content;
  QDateTime m_sentTimestamp;
  QUuid m_roomId;
  QUuid m_sentByUuid;
  QString m_senderName;
};
#endif
